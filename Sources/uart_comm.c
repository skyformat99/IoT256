/*
 * uart_comm.c
 *
 *  Created on: Aug 4, 2016
 *      Author: hflw
 */


#include <PE_Types.h>
#include <PE_Error.h>
#include "uart_comm.h"
#include "cstring"
#include "uart1.h"
#include "uart0.h"
#include "uart2.h"
#include "util.h"
#include "ATcommand.h"

typedef uart2_TDeviceDataPtr uart_TDeviceDataPtr;
extern uart0_TDeviceDataPtr uart0DeviceDataPrv;
extern uart1_TDeviceDataPtr uart1DeviceDataPrv;
extern uart2_TDeviceDataPtr uart2DeviceDataPrv;

char logbuf[MAX_PRINTF_LENGTH];
uint16_t log_printf(char *fmt,...)
{
	unsigned int n;
	uint16_t sent;
 
	//return 0;
	
	va_list ptr;
	va_start(ptr,fmt);
	n = vsprintf(logbuf,fmt,ptr);
	va_end(ptr);
	
	if(n>0)
		return uart_write_sync(HOST_COM_PORT, logbuf, strlen((const char*)logbuf), &sent);
	else
		return ERR_VALUE;
}

uint16_t uart_write_async(short comnum, char *buf, uint16_t bufsize)
{
	char *pbuf = buf;
	uint16_t result=ERR_OK;
	
	LDD_TError (* uart_send_func)(LDD_TDeviceData *DeviceDataPtr, LDD_TData *BufferPtr, uint16_t Size);
	uart_TDeviceDataPtr pDevData;

	switch(comnum)
	{
			case WIFIGPS_COM_PORT: uart_send_func = uart0_SendBlock;
				pDevData = (uart_TDeviceDataPtr)uart0DeviceDataPrv;
				break;
			case HOST_COM_PORT: uart_send_func = uart1_SendBlock;
				pDevData = (uart_TDeviceDataPtr)uart1DeviceDataPrv;
				break;
			case GPRS_COM_PORT: uart_send_func = uart2_SendBlock;
				pDevData = uart2DeviceDataPrv;
				break;
			default: 
				return ERR_VALUE;
	}
		
	result = uart_send_func(pDevData, pbuf,bufsize);
	
	return result;
}

#define MAX_WAIT_SEND_UART_COUNT	100000
uint16_t uart_write_sync(short comnum, char *buf, uint16_t bufsize, uint16_t *sent)
{
	uint16_t result=ERR_OK;
	long wait_complete=0;
	int needCancelIfFailed = 1;
	
	LDD_TError ( * uart_cancel_func)(LDD_TDeviceData *DeviceDataPtr);
	uart_TDeviceDataPtr pDevData;

	switch(comnum)
	{
			case WIFIGPS_COM_PORT: uart_cancel_func = uart0_CancelBlockTransmission;
				pDevData = (uart_TDeviceDataPtr)uart0DeviceDataPrv;
				needCancelIfFailed = 1;
				break;
			case HOST_COM_PORT: uart_cancel_func = uart1_CancelBlockTransmission;
				pDevData = (uart_TDeviceDataPtr)uart1DeviceDataPrv;
				needCancelIfFailed = 0;
				break;
			case GPRS_COM_PORT: uart_cancel_func = uart2_CancelBlockTransmission;
				pDevData = uart2DeviceDataPrv;
				needCancelIfFailed = 1;
				break;
			default: 
				return ERR_VALUE;
	}

	result = uart_write_async(comnum, buf, bufsize);
	if(result == ERR_OK || result == ERR_BUSY)
		while(pDevData->OutDataNumReq && wait_complete++ < MAX_WAIT_SEND_UART_COUNT);
	
	* sent = pDevData->OutSentDataNum;
	if(pDevData->OutDataNumReq == 0)
		return ERR_OK;
	else
	{
		if(needCancelIfFailed)
			uart_cancel_func(pDevData);
		return result;
	}
}


uint16_t uart_read_async(short comnum, char * buf, uint16_t bufsize)
{
	uint16_t result=ERR_OK;
	LDD_TError (* uart_receive_func)(LDD_TDeviceData *DeviceDataPtr, LDD_TData *BufferPtr, uint16_t Size);
	uart_TDeviceDataPtr pDevData;

	switch(comnum)
	{
		case WIFIGPS_COM_PORT: uart_receive_func = uart0_ReceiveBlock;
			pDevData = (uart_TDeviceDataPtr)uart0DeviceDataPrv;
			break;
		case HOST_COM_PORT: uart_receive_func = uart1_ReceiveBlock;
			pDevData = (uart_TDeviceDataPtr)uart1DeviceDataPrv;
			break;
		case GPRS_COM_PORT: uart_receive_func = uart2_ReceiveBlock;
			pDevData = uart2DeviceDataPrv;
			break;
		default:
			return ERR_VALUE;
	}
	
	result = (*uart_receive_func)(pDevData, buf, bufsize);
	return result;
}

#define MAX_RETRY_RECV_UART_COUNT_GPRS	5
#define MAX_RETRY_RECV_UART_COUNT_HOST	10
#define MAX_RETRY_RECV_UART_COUNT_GPS	5

uint16_t uart_read_sync(short comnum, char * buf, uint16_t bufsize, uint16_t * recved)
{
	return uart_read_sync_complex(comnum, buf, bufsize, recved, 1);
}

uint16_t uart_read_sync_complex(short comnum, char * buf, uint16_t bufsize, uint16_t * recved, int needCancel)
{
	uint16_t result=ERR_OK;
	LDD_TError ( * uart_cancel_func)(LDD_TDeviceData *DeviceDataPtr);
	uart_TDeviceDataPtr pDevData;
	int retry =0;
	int needCancelIfFailed = 0;
	int max_retry_recv_count;

	switch(comnum)
	{
		case WIFIGPS_COM_PORT: uart_cancel_func = uart0_CancelBlockReception;
			pDevData = (uart_TDeviceDataPtr)uart0DeviceDataPrv;
			needCancelIfFailed =needCancel;
			max_retry_recv_count = MAX_RETRY_RECV_UART_COUNT_GPS;
			break;
		case HOST_COM_PORT: uart_cancel_func = uart1_CancelBlockReception;
			pDevData = (uart_TDeviceDataPtr)uart1DeviceDataPrv;
			needCancelIfFailed = needCancel;
			max_retry_recv_count = MAX_RETRY_RECV_UART_COUNT_HOST;
			break;
		case GPRS_COM_PORT: uart_cancel_func = uart2_CancelBlockReception;
			pDevData = uart2DeviceDataPrv;
			needCancelIfFailed = needCancel;
			max_retry_recv_count = MAX_RETRY_RECV_UART_COUNT_GPRS;
			break;
		default:
			return ERR_VALUE;
	}

	result = uart_read_async(comnum, buf, bufsize);
	if(result == ERR_OK || result == ERR_BUSY)
		while(pDevData->InpDataNumReq && retry++ < max_retry_recv_count && pDevData->InpRecvDataNum < MIN_RESULT_STRING_SIZE)
			_delay_sometime(1);

	*recved = pDevData->InpRecvDataNum;
	if(pDevData->InpDataNumReq == 0)
		return ERR_OK;
	else
	{
		if(needCancelIfFailed)//we failed to read all data, cancel this time, so that we can recieve again in next time.
			uart_cancel_func(pDevData);
		return result;
	}
}
