/*
 * HOST.c
 *
 *  Created on: Aug 23, 2016
 *      Author: hflw
 */
#include "PE_Types.h"
#include "PE_Error.h"
#include "uart_comm.h"
#include "util.h"
#include "stdlib.h"
#include "cstring"
#include "HOST.h"
#include "uart1.h"
#include "CQueue.h"

char comm_buf[COMM_BUF_SIZE + HOST_MSG_SIZE];//we order more just for tmp buffer to save wrapped data before send out
char *cur_bufp=comm_buf, * bufhead=comm_buf, * buftail=comm_buf, *lastmsg_tail=comm_buf;
char * g_buflastbyte= comm_buf + COMM_BUF_SIZE -1;
char * g_buffirstbyte = comm_buf;
static int g_wrap_recv_comm_buf = 0;//if we have already receive data wrap from buffer ending

extern uart1_TDeviceDataPtr uart1DeviceDataPrv;

uint16_t HOSTStartRecv_internal(int isWrapped)
{
	uint16_t result;
	g_wrap_recv_comm_buf = isWrapped;
	cur_bufp=comm_buf;
	if(isWrapped == 0)//we are not been wrapped, we read data from buffer head, reset lastmsg tail.
		lastmsg_tail = comm_buf;
	result = uart_read_async(HOST_COM_PORT, comm_buf, COMM_BUF_SIZE);
	return result;
}

inline uint16_t HOSTPickOutMsg()
{
	uint16_t result= ERR_FAULT;
	char * RecvLength;
	uint16_t len;
	int tmplen;
	char * tmpchp;
	
	/////////////////check full msg////////////////////////////
	
	//get length from msg
	if(g_buflastbyte - bufhead >= 2)//we have enough space in the tail to save header and length bytes
	{
		RecvLength = bufhead +1;
		len = * RecvLength + (*(RecvLength + 1)<< 8);
	}
	else
	{
		if(g_buflastbyte -  bufhead == 1)//not enough space in the tail to save length, another byte should in the head
		{
			RecvLength = bufhead +1;
			len = * RecvLength + (comm_buf[0]<< 8);
		}
		else//bufhead should be at last byte in the tail of com buf
		{
			//this means header A5 is the last byte of buffer
			//length should be in the head of buffer
			RecvLength = comm_buf;
			len = * RecvLength + (*(RecvLength + 1)<< 8);
		}
	}
	if(len <=0 || len > HOST_MSG_SIZE)
		return result;
	
	//calculate buftail address
	buftail = bufhead + 1 + 2 + len + 1;//skip 1 byte for header and skip 2 bytes for length and then 1 byte for checksum
	if(buftail > g_buflastbyte)//we are exceed the buffer
	{
		tmplen = buftail - g_buflastbyte;
		buftail = comm_buf + tmplen -1;
	}
	
	if(buftail >= cur_bufp)
		return result;
	
	/*if(!g_wrap_recv_comm_buf)
	{
		if(buftail >= cur_bufp)
			return result;
	}
	else
	{
		if(buftail < comm_buf + COMM_BUF_SIZE)
		{
			//we have enough data in the tails
			//buftail = bufhead + 1 + 2 + len + 1;//skip 1 byte for header and skip 2 bytes for length and then 1 byte for checksum
			//if(buftail > cur_bufp)
			//	return result;
		}
		else
		{
			//we have data wrapped from head
			buftail = comm_buf + (buftail - (comm_buf + COMM_BUF_SIZE));
			if(buftail >= cur_bufp)
				return result;
		}
	}*/
	//following code is useless,just for set debug breakpoint
	//if(g_wrap_recv_comm_buf)
	//	buftail= buftail;
	
	if(*buftail == TAIL_ID)//FIXME: we should checksum here
	{
#ifdef DUMP_DATA
		//following code for debug only
			log_printf("pick out msg in to queue: head:%x:tail:%x:len:%d ---->\n",bufhead, buftail, len);
			if(bufhead <= buftail)
			{
				for(tmpchp = bufhead;tmpchp <= buftail; tmpchp ++)
					log_printf("%x",*tmpchp);
				log_printf("\n");
			}
			else
			{
				for(tmpchp = bufhead;tmpchp <= g_buflastbyte ; tmpchp ++)
					log_printf("%x",*tmpchp);
				for(tmpchp = comm_buf; tmpchp <= buftail; tmpchp ++)
					log_printf("%x",*tmpchp);
				log_printf("\n");
			}
		//end of debug code
#endif
		if(!TestFullBufferQueue(g_host_msg_queue_p))
		{
			if(_send_msg_to_queue(g_host_msg_queue_p, NEW_HOST_DATA_READ_TO_SEND, bufhead+3, buftail-1, len) == ERR_OK)
			{
				result = ERR_OK;
				//set header after tail
				lastmsg_tail = buftail + 1;//record tail of last msg
				*buftail = 0xFF;//clear tailer flag
				*bufhead = 0xFF;//clear header flag
			}
		}
		else
			log_printf("HOST msg Q full\n");
	}
	//else
	//{
		//here is useless,just for set debug breakpoint
	//	if(g_wrap_recv_comm_buf)
	//		return result;
	//}
	
	return result;
}

uint16_t HOSTCheckRecvNewMsg_internal()
{
	uint16_t result= ERR_FAULT;
	char * buflastbyte= comm_buf + COMM_BUF_SIZE -1;
	int RecvLength;
	
	if(!uart1DeviceDataPrv)
		return ERR_PARAM_VALUE;
	
	//set current recv data ending pointer
	cur_bufp = comm_buf + uart1DeviceDataPrv->InpRecvDataNum;
	
	//we should check if we are wrapped before start checking msg
	if(g_wrap_recv_comm_buf == 0)//we are not wrapped comm_buf during recv
	{
		bufhead = lastmsg_tail;
		while(bufhead < cur_bufp)
		{
			if(*bufhead != HEAD_ID)
			{
				bufhead ++;
				continue;
			}
			if((result = HOSTPickOutMsg()) == ERR_OK)//found new message from HOST, add new msg to msg queue
			{
				bufhead = lastmsg_tail;
			}
			else
				bufhead ++;
		}
	}
	else
	{
		//log_printf("comm buf wrapped\n");
		//g_wrap_recv_comm_buf = 0;
		//part1: old data
		if(lastmsg_tail > cur_bufp)//we have old data
		{
			bufhead = lastmsg_tail;
			while(bufhead < comm_buf + COMM_BUF_SIZE)
			{
				if(*bufhead != HEAD_ID)
				{
					bufhead ++;
					continue;
				}
				if((result = HOSTPickOutMsg()) == ERR_OK)//found new message from HOST, add new msg to msg queue
				{
					bufhead = lastmsg_tail;
					g_wrap_recv_comm_buf = 0;
					break;
				}
				else
					bufhead ++;
			}
			//lastmsg_tail = comm_buf;//wrap from buffer head
		}
		//part2: new data
		bufhead = lastmsg_tail;
		while(bufhead < cur_bufp)
		{
			if(*bufhead != HEAD_ID)
			{
				bufhead ++;
				continue;
			}
			if((result = HOSTPickOutMsg()) == ERR_OK)//found new message from HOST, add new msg to msg queue
			{
				bufhead = lastmsg_tail;
				g_wrap_recv_comm_buf = 0;
				break;
			}
			else
				bufhead ++;
		}
	}
	
	//check if we already get enough data but we still haven't pick out right msg from it.
	//this mean some data is wrong, maybe length, maybe tail,maybe other.
	//so we should go ahead to skip current msg and try from next one
	if(result != ERR_OK)
	{
		if(cur_bufp >= lastmsg_tail)
			RecvLength = cur_bufp - lastmsg_tail;
		else
		{
			RecvLength = buflastbyte - lastmsg_tail +1;
			RecvLength += cur_bufp - comm_buf;
		}
		if(RecvLength > HOST_MSG_SIZE)//we already get enough data of one msg
		{
			log_printf("Wrong data. recv len:%d\n",RecvLength);
			//move ahead. this step should skip a new msg's header.
			if(lastmsg_tail < buflastbyte)
				lastmsg_tail ++;
			else
				lastmsg_tail = comm_buf;
		}
	}
	
	return result;
}

