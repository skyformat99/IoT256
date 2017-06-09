/*
 * util.c
 *
 *  Created on: Aug 4, 2016
 *      Author: hflw
 */

#include "util.h"
#include "CQueue.h"

int _delay_sometime(int j)
{
	int i,val;
	while(j>0){
		for(i=0;i<10000;i++) val=i*1;
		j--;
	}
	return val;
}

uint16_t _send_msg_to_queue(BUFFER_QUEUE * handle, uint_32 status, char * head, char * tail, uint_16 len)
{
	COMM_MSG msg;
	_mqx_uint result;
	msg.status = status;//counter % 2;
	msg.head = head;
	msg.tail = tail;
	msg.len = len;
	result = AddBufferQueue(handle, &msg);//_lwmsgq_send(handle, (uint_32 *)&msg, LWMSGQ_SEND_BLOCK_ON_FULL);

	return result;
	
}

void ATChar2HEX(char ch, char *hex)
{
	if((ch & 0x0f) > 9)
		hex[1] = (ch & 0x0f) -10 + 'A';
	else
		hex[1] = (ch & 0x0f) + '0';
	
	if(((ch >> 4) & 0x0f) >9)
		hex[0] = ((ch >> 4) & 0x0f) -10 + 'A';
	else
		hex[0] = ((ch >> 4) & 0x0f) + '0';
}

char ATHEX2Char(char *hex)
{
	char h,l,result;
	if(hex[0] >= 'A' && hex[0] <='F')
		h=hex[0]-'A'+10;
	else
		h=hex[0] - '0';
	if(hex[1] >= 'A' && hex[1] <='F')
		l=hex[1]-'A'+10;
	else
		l=hex[1] - '0';
	result = (h << 4) | l;
	return result;
}





