/*
 * util.h
 *
 *  Created on: Aug 4, 2016
 *      Author: hflw
 */

#ifndef UTIL_H_
#define UTIL_H_

#include <PE_Types.h>
#include <PE_Error.h>
#include "include\psptypes.h"
#include "HOST.h"
#include "CQueue.h"

//#define TIMER_DEBUG
//#define DUMP_DATA

#define MAX_FACK_GPS_TIMEOUT	10

//msgq
enum send_status{
	NEW_HOST_DATA_READ_TO_SEND,
	NEW_GPRS_DATA,
	SEND_OUT,
	SEND_FAILED,
	NEW_GPS_DATA
};

#define MAX_MQTT_CONNECTING_TIMEOUT	50

#define MAX_COMM_MSG	HOST_MAX_MSG
#define MAX_GPRS_MSG	3

int _delay_sometime(int j);;
uint16_t _send_msg_to_queue(BUFFER_QUEUE * handle, uint_32 status, char * head, char * tail, uint_16 len);
void ATChar2HEX(char ch, char *hex);
char ATHEX2Char(char *hex);
void block_delay(int m);


#endif /* UTIL_H_ */
