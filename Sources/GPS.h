/*
 * GPS.h
 *
 *  Created on: Apr 14, 2017
 *      Author: hflw
 */

#ifndef GPS_H_
#define GPS_H_

//#define FAKE_GPS_DATA

#define MAX_WIFI_CMD_RECV_BUF_SIZE 1500
#define AT_CMD_WIFI_HEAD_FAST_SEND "AT+CIPSEND"

byte WIFIInit();
uint16_t WIFISendDataFromLineBuffer(char * buf, uint16_t length);
uint16_t WIFIStartAsyncRecv();


#endif /* GPS_H_ */
