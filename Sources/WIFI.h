/*
 * WIFI.h
 *
 *  Created on: Oct 20, 2016
 *      Author: hflw
 */

#ifndef WIFI_H_
#define WIFI_H_

#define MAX_WIFI_CMD_RECV_BUF_SIZE 1500
#define AT_CMD_WIFI_HEAD_FAST_SEND "AT+CIPSEND"
#define WIFI_DEFAULT_SSID	"huiyishi"
#define WIFI_DEFAULT_PWD	"yanfaxd10"

byte WIFIInit();
uint16_t WIFISendDataFromLineBuffer(char * buf, uint16_t length);
uint16_t WIFIStartAsyncRecv();

#endif /* WIFI_H_ */
