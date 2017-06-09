/*
 * GPRS.h
 *
 *  Created on: Aug 11, 2016
 *      Author: hflw
 */

#ifndef GPRS_H_
#define GPRS_H_

byte GPRSInit();
uint16_t GPRSMIPCallSetup();
uint16_t SendDataToServerFromLineBuffer(char * buf, uint16_t length);
uint16_t SendDataFromLineBufferAllMode(char * buf, uint16_t length, char sendType);
uint16_t GPRSSendDataFromWrapBuffer(char * buf, uint16_t bufsize, char * start, uint16_t length);
uint16_t GPRSSendDataCommon(uint16_t *sentlen, char * sendMode, char sendType);
uint16_t GPRSConnectToServer(char * socketid, char * ipaddr, char * serverport, char * localport);
uint16_t GPRSPickoutDataHexmode(char * buf, uint16_t len, uint16_t  *pay_load_len);
uint16_t GPRSAnalyzeRecvData(char * bufp, uint16_t len);
void GPRSCheckRecvResult_internal();

typedef void (* GPRS_READ_CALLBACK_FUNC_PTR) (char * buffer, int nread, void * privdata);
void GPRSSetReadDataCallback(GPRS_READ_CALLBACK_FUNC_PTR func, void * privData);
uint16_t CheckContainSubStr(char *result, char *expect, int * startfrom, int *length);
uint16_t GPRSIsConnectToServer(char * socketid, int runtimes);
uint16_t GPRSDisconnectToServer(char * socketid, int runtimes);

uint16_t ATSendDataByHEX(short comnum, char * buf, uint16_t length, uint16_t *sentlen);

#endif /* GPRS_H_ */
