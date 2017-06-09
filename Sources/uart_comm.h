/*
 * uart_comm.h
 *
 *  Created on: Aug 4, 2016
 *      Author: hflw
 */

#ifndef UART_COMM_H_
#define UART_COMM_H_

#include <stdarg.h>
#include <stdio.h>
#include "PE_Types.h"
#include "PE_Error.h"

#define MAX_PRINTF_LENGTH	256
#define GPRS_COM_PORT	2
#define HOST_COM_PORT	1
#define WIFIGPS_COM_PORT	0

uint16_t log_printf(char *fmt,...);
uint16_t uart_write_sync(short comnum, char *buf, uint16_t bufsize, uint16_t *sent);
uint16_t uart_write_async(short comnum, char *buf, uint16_t bufsize);
uint16_t uart_read_sync(short comnum, char * buf, uint16_t bufsize, uint16_t * recved);
uint16_t uart_read_sync_complex(short comnum, char * buf, uint16_t bufsize, uint16_t * recved, int needCancel);
uint16_t uart_read_async(short comnum, char * buf, uint16_t bufsize);

#endif /* UART_COMM_H_ */
