/*
 * GPRS.c
 *
 *  Created on: Aug 11, 2016
 *      Author: hflw
 */

#include "uart_comm.h"
#include "util.h"
#include "PE_Types.h"
#include "PE_Error.h"
#include "ATcommand.h"
#include "mqttc\mqtt.h"
#include "uart2.h"
#include "GPRS.h"
#include "WIFI.h"
#include "GPTE.h"

extern uart2_TDeviceDataPtr uart2DeviceDataPrv;
extern Mqtt g_mqtt;
extern char at_cmd_recv_buf[MAX_CMD_RECV_BUF_SIZE];
extern char *g_at_result_last_valid_pointer_in_recv_buf , *g_at_result_recv_end_pointer;
extern char at_cmd_result_checking_buf[MAX_CMD_RECV_BUF_SIZE];
extern struct at_expected_result_and_condition g_current_at_expected_result;

//transfer convert HEX buffer and recv result's available part buffer
char recv_cmd_hex_buf[MAX_CMD_HEX_BUF_SIZE];
//identifer's buffer, i.e. +MIPDATA
char recv_server_data_buf[MAX_CMD_HEX_BUF_SIZE];
extern int g_wifi_connected;
extern char wifi_cmd_recv_buf[1024];

//following global flag work as event signal
//it should be set during checking AT result
// and these flag should be clear after it have been processed.
#define GPRS_DATA_COMING_HEADER "+MIPDATA:"
#define MIN_GPRS_DATA_COMING_DATA_LEN	12
uint16_t	g_is_new_gprs_data_received = 0;//this is indicate new gprs data arrived from server
uint16_t	g_new_gprs_data_length=0;
char * g_new_gprs_data_start=NULL;

#define GPRS_RESET_HEADER "LONGSUNG READY"
#define GPRS_CLOSE_MIPLINK_HEADER "+MIPCLOSE:"
uint16_t g_get_gprs_tcp_closed_error = 0;
#define GPRS_CLOSE_ERROR_HEADER "ERROR: 201"
uint16_t g_is_gprs_error_detected = 0;//this is indicate unexpected error occured
#define MQTT_CONNECTED_HEADER "mqttc"
//>>>flags end


extern char *g_at_result_last_valid_pointer_in_checking_buf_tail;
uint16_t GPRSStartRecv_internal()
{
	uint16_t result;
	result = uart_read_async(GPRS_COM_PORT, at_cmd_recv_buf, MAX_CMD_RECV_BUF_SIZE);
	return result;
}

void GPRSOtherCommonExpectedResultChecking()
{
	char *bufp;
	int checkstrlen=strlen(at_cmd_result_checking_buf);
	
	//we should check other common expect result at same time
	if((bufp= strstr(at_cmd_result_checking_buf, GPRS_DATA_COMING_HEADER)) >0)
	{
		bufp += strlen(GPRS_DATA_COMING_HEADER) + 1;//skip space

		g_new_gprs_data_length=0;
		while(bufp[g_new_gprs_data_length]!='\0' &&
				bufp[g_new_gprs_data_length]!='\n' &&
				bufp[g_new_gprs_data_length]!='\r' &&
				bufp+g_new_gprs_data_length < at_cmd_result_checking_buf + checkstrlen )
			g_new_gprs_data_length++;
		
		//if( g_new_gprs_data_length >= MIN_GPRS_DATA_COMING_DATA_LEN)
		if(bufp[g_new_gprs_data_length] == '\r' ||bufp[g_new_gprs_data_length] == '\n')
		{
			if( g_new_gprs_data_length > MAX_CMD_HEX_BUF_SIZE - 1)
			{
				log_printf("---too many data from server via gprs!\n");
				g_new_gprs_data_length = MAX_CMD_HEX_BUF_SIZE -1;
			}
			//move out data from checking buf because it may be flush later
			memcpy(recv_server_data_buf,bufp, g_new_gprs_data_length);
			recv_server_data_buf[g_new_gprs_data_length] = '\0';
			g_new_gprs_data_start = recv_server_data_buf;
			log_printf("<--- get gprs data:%s\n", recv_server_data_buf);
			
			g_is_new_gprs_data_received = 1;//set flag
			ATUpdateLastValidResulPointerInCheckingBuf(bufp+g_new_gprs_data_length);
		}

		//uart2_CancelBlockTransmission(uart2DeviceDataPrv);
	}
	if((bufp= strstr(at_cmd_result_checking_buf, GPRS_CLOSE_MIPLINK_HEADER)) >0
			|| (bufp= strstr(at_cmd_result_checking_buf, GPRS_RESET_HEADER)) >0 )
	{
		//g_get_gprs_tcp_closed_error = 1;//set flag
		ATUpdateLastValidResulPointerInCheckingBuf(bufp + strlen(GPRS_CLOSE_MIPLINK_HEADER));

		//uart2_CancelBlockTransmission(uart2DeviceDataPrv);
	}
	if((bufp= strstr(at_cmd_result_checking_buf, MQTT_CONNECTED_HEADER)) >0)
	{
		//g_mqtt_alreadd_connected_when_try_conn = 1;//g_mqtt.state = MQTT_STATE_CONNECTED;//set flag
		ATUpdateLastValidResulPointerInCheckingBuf(bufp + strlen(MQTT_CONNECTED_HEADER));

		//uart2_CancelBlockTransmission(uart2DeviceDataPrv);
	}
	
	if((bufp= strstr(at_cmd_result_checking_buf, GPRS_CLOSE_ERROR_HEADER)) >0)
	{
		g_is_gprs_error_detected = 1;//set flag
		ATUpdateLastValidResulPointerInCheckingBuf(g_at_result_last_valid_pointer_in_checking_buf_tail);//we will reset, let's ignore all currrent recv data

		//uart2_CancelBlockTransmission(uart2DeviceDataPrv);
	}

}

void GPRSCheckRecvResult_internal()
{
	char *tmpp=at_cmd_recv_buf;
	if(!uart2DeviceDataPrv)
		return ERR_PARAM_VALUE;

	if(uart2DeviceDataPrv->InpRecvDataNum > MIN_RESULT_STRING_SIZE)
	{
		if(uart2DeviceDataPrv->InpRecvDataNum < MAX_CMD_RECV_BUF_SIZE)
			tmpp = at_cmd_recv_buf + uart2DeviceDataPrv->InpRecvDataNum; 
		else
		{
			log_printf("+++++++++We should not be here. we are exceed the recv buffer.\n");
			tmpp = &at_cmd_recv_buf[MAX_CMD_RECV_BUF_SIZE];//NOTE: this is out of the memory
		}
		if(tmpp != g_at_result_recv_end_pointer)//new data came
		{
			g_at_result_recv_end_pointer = tmpp;
#ifdef TIMER_DEBUG
			log_printf("recv len:%d recvend:%x\n", 
				uart2DeviceDataPrv->InpRecvDataNum, g_at_result_recv_end_pointer);
#endif
			//copy data from recv buffer to result checking buffer
			if(ATCopyATRecvData2ResultCheckingBuf()== ERR_OK)//copy success
			{
				//check AT result from result checking buffer
				g_current_at_expected_result.analyze_result = ATCheckResultFromResultCheckingBuf();
			
				//check other common expected result from checking buffer
				//the result will be marked in following function
				GPRSOtherCommonExpectedResultChecking();
			
				//update last valide result pointer
				ATUpdateLastValidResulPointerInRecvBuf();
			}
		}
	}

}


/*uint16_t ATStartRecv_internal(short comnum)
{
	uint16_t result;
	if(!g_wifi_connected)
	{
		memset(at_cmd_recv_buf, 0, MAX_CMD_RECV_BUF_SIZE);
		result = uart_read_async(comnum, at_cmd_recv_buf, MAX_CMD_RECV_BUF_SIZE);
	}
	else
	{
		memset(wifi_cmd_recv_buf, 0, MAX_WIFI_CMD_RECV_BUF_SIZE);
		result = uart_read_async(comnum, wifi_cmd_recv_buf, MAX_WIFI_CMD_RECV_BUF_SIZE);
	}
	return result;
}*/



uint16_t GPRSATExecuteOnce(char *cmd, char *expect_result)
{
	return ATExecuteOnce(GPRS_COM_PORT, cmd, at_cmd_recv_buf, MAX_CMD_RECV_BUF_SIZE, expect_result);
}

uint16_t GPRSATExecute(char *cmd, char *expect_result, int runtimes)
{
	return ATExecute(GPRS_COM_PORT, cmd, at_cmd_recv_buf, MAX_CMD_RECV_BUF_SIZE, expect_result, runtimes);
}

uint16_t GPRSATExecuteComplex(char *cmd, CHECK_RESULT_FUNC_PTR checkfunc, char *expect_result, int runtimes, char * value, int * param1, int * param2)
{
	return ATExecuteComplex(GPRS_COM_PORT, cmd, at_cmd_recv_buf, MAX_CMD_RECV_BUF_SIZE, checkfunc, expect_result, runtimes, value, param1, param2);

}

//uint16_t GPRSATExecuteComplexTryAndWait(char *cmd, CHECK_RESULT_FUNC_PTR checkfunc, char *expect_result, char * value, int * param1, int * param2)
//{
//	return ATExecuteComplexTryAndWait(GPRS_COM_PORT, cmd, at_cmd_recv_buf, MAX_CMD_RECV_BUF_SIZE, checkfunc, expect_result, value, param1, param2);
//}

byte GPRSSoftReset()
{
	uint16_t result=ERR_OK;
	log_printf("soft reset GPRS......\n\n");
	result = GPRSATExecuteOnce( "AT+CFUN=1,1\r\n", "OK\r\n");
	if(result == ERR_OK)
		result = GPRSInit();
	return result;
}

#define MAX_RETRY_INIT_GPRS	3
byte GPRSInit()
{
	int startfrom, length;
	int retrytimes = MAX_RETRY_INIT_GPRS;//we retry to init gprs serveral times

	uint16_t result;
	
	GPRSStartRecv_internal();

	while(retrytimes-- > 0)
	{
		log_printf("start init GPRS......\n\n");
		if(retrytimes <= 0)//last time
		{
			//GSM POWER SUPPLY OFF
			//GPTE_SetFieldValue(NULL, PTE20_GSM_POW_EN, 0);
			log_printf("---------reset GPRS power......\n\n");
			gsm_pow_on(0);
			_delay_sometime(400);
		}
		//GSM POWER SUPPLY ON
		//GPTE_SetFieldValue(NULL, PTE20_GSM_POW_EN, 1);
		gsm_pow_on(1);
		_delay_sometime(100);

		result = GPRSATExecute( "AT\r\n", "OK\r\n", 5);
		if(result != ERR_OK)
		{
			log_printf("modem not response AT, reset modem\n");
			//GSM POWER SUPPLY OFF
			//GPTE_SetFieldValue(NULL, PTE20_GSM_POW_EN, 0);
			//_delay_sometime(400);
			//GSM POWER SUPPLY ON
			//GPTE_SetFieldValue(NULL, PTE20_GSM_POW_EN, 1);
			//_delay_sometime(100);
			gsm_pow_on(1);
		}
		if(result == ERR_OK)
			result = GPRSATExecuteOnce( "AT+S32K=0\r\n", "OK\r\n");
		//if(result == ERR_OK)
		//	result = GPRSATExecuteOnce( "AT+CFUN=1\r\n", "OK\r\n");
		if(result == ERR_OK)
		{
			startfrom = 12;
			length = 1;
			result = GPRSATExecuteComplex("AT+CIND?\r\n", CheckContainSubStr, "+CIND:", 40, "1", &startfrom, &length);
		}
		if(result==ERR_OK)
			result = GPRSATExecuteOnce( "AT+CGATT=1\r\n", "OK\r\n");
		if(result==ERR_OK)
		{
			startfrom = 8;
			length = 1;
			result = GPRSATExecuteComplex( "AT+CGATT?\r\n", CheckContainSubStr, "+CGATT:", 20,  "1", &startfrom, &length);
		}
	
		//if(result == ERR_OK && GPRSIsConnectToServer(MQTT_SERVER_SOCKET_ID, 1) != ERR_OK)
		if(result == ERR_OK)
			result = GPRSConnectToServer(MQTT_SERVER_SOCKET_ID, g_mqtt_server_ip, MQTT_SERVER_PORT_STRING, MQTT_CLIENT_PORT);
		
		if(result == ERR_OK)
			result = GPRSATExecuteOnce("AT+MIPMODE=1,0,0\r\n", "OK\r\n");
#ifdef ENALBE_GPRS_TRANSPORT_MODE
		if(result == ERR_OK)
			result = GPRSATExecuteOnce( "AT+MIPTCFG=1,1\r\n", "OK\r\n");//result = ATExecute("AT+MIPTCFG=1,3,1200\r\n", "OK\r\n");
#endif
		if(result == ERR_OK)
			break;
	}
		
	if(result != ERR_OK)
	{
		log_printf("Init GPRS failed!\n");
		return result;
	}
	else
	{
		log_printf("Init GPRS success!\n");
		return ERR_OK;
	}
}

GPRS_READ_CALLBACK_FUNC_PTR g_gprs_callback;
void * g_gprs_callback_privdata;
void GPRSSetReadDataCallback(GPRS_READ_CALLBACK_FUNC_PTR func, void * privData)
{
	g_gprs_callback = func;
	g_gprs_callback_privdata = privData;
}


uint16_t GPRSMIPCallSetup()
{
	uint16_t result;
	int startfrom, length;
	result = GPRSATExecuteOnce( "AT+MIPCALL=1,\"PPP\"\r\n", "OK\r\n");
	if(result==ERR_OK)
	{
		startfrom = 10;
		length = 1;
		result = GPRSATExecuteComplex( "AT+MIPCALL?\r\n", CheckContainSubStr, "+MIPCALL:", 3, "1", &startfrom, &length);
	}
	
	return result;
}


/*uint16_t GPRSStartAsyncRecv()
{
	uint16_t result;
	result = ATStartRecv_internal(GPRS_COM_PORT);
	g_at_async_read_timeout = MAX_AT_ASYNC_READ_TICKS;
	return result;
}
*/


uint16_t GPRSSendDataFromLineBufferAllMode(char * buf, uint16_t length, char sendType)
{
	uint16_t result=ERR_OK;
	uint16_t uartsentlen=length, uartsentpayloadlen;

	//check connection and send head
	result = GPRSSendDataCommon(&uartsentlen, SEMD_MODE_HEX, sendType);
	if(result != ERR_OK)
		return result;
	//send payload
#ifdef DUMP_DATA
		//following code for debug only
		char *tmpchp;
		log_printf("try to send data to GPRS:buf:%x:len:%d ---->\n",buf,length);
		for(tmpchp = buf;tmpchp - buf < length; tmpchp ++)
			log_printf("%x",*tmpchp);
		log_printf("\n");
		//end of debug 
#endif
	if(sendType != GPRS_SEND_TRANSPORT)
		result = ATSendDataByHEX(GPRS_COM_PORT, buf, length, &uartsentpayloadlen);
	else
		result = uart_write_sync(GPRS_COM_PORT, buf, length, &uartsentpayloadlen);
	if(result != ERR_OK)
		return result;
	//send tail and start read result
	char * cmdtail;
	char ctrlz = CTRLZ;
	if(sendType == GPRS_SEND_FAST)
		cmdtail = "\"\r\n";
	else if(sendType == GPRS_SEND_NORMAL)
		cmdtail = &ctrlz;
	else if(sendType == GPRS_SEND_TRANSPORT)
	{
		_delay_sometime(5);
		cmdtail = AT_CMD_TAIL_TRANSPORT_SEND;
	}
	//GPRSStartAsyncRecv();
	log_printf("sending...\n");
	result = uart_write_sync(GPRS_COM_PORT, cmdtail, strlen(cmdtail), &uartsentlen);
	if(result != ERR_OK)
		return result;
	if(sendType == GPRS_SEND_TRANSPORT)
	{
		_delay_sometime(10);
		//_delay_sometime(10);
		//_delay_sometime(10);
	}
		
	log_printf("send data to GPRS with line buffer complete!sent payload: %d\n", uartsentpayloadlen);
	
	return result;
}


uint16_t GPRSSendDataFromLineBuffer(char * buf, uint16_t length)
{
#ifdef ENALBE_GPRS_TRANSPORT_MODE
	char sendType = GPRS_SEND_TRANSPORT;
#else
	char sendType = GPRS_SEND_FAST;
#endif
	return  SendDataFromLineBufferAllMode(buf, length, sendType);
}

uint16_t GPRSSendDataFromWrapBuffer(char * buf, uint16_t bufsize, char * start, uint16_t length)
{
	uint16_t result=ERR_OK;
	uint16_t uartsentlen, uartsentpayloadlen=0;
	//check connection and send head
	result = GPRSSendDataCommon(&uartsentlen, SEND_MODE_ASCII, GPRS_SEND_FAST);
	if(result != ERR_OK)
		return result;
	//send payload
	if(start + length > buf + bufsize)//wrap data
	{
		result = uart_write_sync(GPRS_COM_PORT, start, buf + bufsize - start, &uartsentlen);
		if(result != ERR_OK)
			return result;
		uartsentpayloadlen +=uartsentlen;
		result = uart_write_sync(GPRS_COM_PORT, buf, length- (buf + bufsize - start), &uartsentlen);
		if(result != ERR_OK)
			return result;
		uartsentpayloadlen +=uartsentlen;
	}
	else
	{
		result = uart_write_sync(GPRS_COM_PORT, start, length, &uartsentlen);
		if(result != ERR_OK)
			return result;
		uartsentpayloadlen +=uartsentlen;
	}
	//send tail and start read result
	char * cmdtail = "\"\r\n";
	GPRSStartAsyncRecv();
	log_printf("sending...\n");
	result = uart_write_sync(GPRS_COM_PORT, cmdtail, strlen(cmdtail), &uartsentlen);
	if(result != ERR_OK)
		return result;
	
	log_printf("send data to GPRS complete!sent payload: %d\n", uartsentpayloadlen);
	return result;
}


uint16_t GPRSSendDataCommon( uint16_t *sentlen, char * sendMode, char sendType)
{
	uint16_t result=ERR_OK;
	log_printf("send data to GPRS...\n");

	//if(GPRSIsConnectToServer(MQTT_SERVER_SOCKET_ID, 2) != ERR_OK)
	{
		//result = ATConnectToServer(MQTT_SERVER_SOCKET_ID, MQTT_SERVER_IP_ADDR, MQTT_SEVER_PORT, MQTT_CLIENT_PORT);
		//result = GPRSConnectToServer(MQTT_SERVER_SOCKET_ID, MQTT_SERVER_ADDR, MQTT_SERVER_PORT_STRING, MQTT_CLIENT_PORT);
		//if(result != ERR_OK)
		//	result = GPRSInit();
		//if(result == ERR_OK)
		//	mqtt_init_and_connect();
	}
	//if(result != ERR_OK)
	//{
	//	log_printf("connect to server failed!");
	//	return result;
	//}

	char cmdhead[32];
	
	if(sendType == GPRS_SEND_FAST)
	{
		strcpy(cmdhead, AT_CMD_HEAD_FAST_SEND);
		strcat(cmdhead, "=");
		strcat(cmdhead, MQTT_SERVER_SOCKET_ID);
		strcat(cmdhead, ",");
		strcat(cmdhead, sendMode);
		strcat(cmdhead, ",\"");
	}
	else if(sendType == GPRS_SEND_NORMAL)
	{
			strcpy(cmdhead, AT_CMD_HEAD_NORMAL_SEND);
			strcat(cmdhead, "=");
			strcat(cmdhead, MQTT_SERVER_SOCKET_ID);
			strcat(cmdhead, ",");
			sprintf(cmdhead, "%s%d", cmdhead, *sentlen);
			strcat(cmdhead, "\r\n");
	}
	else if(sendType == GPRS_SEND_TRANSPORT)
	{
		strcpy(cmdhead, AT_CMD_HEAD_TRANSPORT_SEND);
		strcat(cmdhead, "=");
		strcat(cmdhead, MQTT_SERVER_SOCKET_ID);
		strcat(cmdhead, "\r\n");
	}
	if(result == ERR_OK)
		result = uart_write_sync(GPRS_COM_PORT, cmdhead, strlen(cmdhead), sentlen);
	if(sendType == GPRS_SEND_NORMAL || sendType == GPRS_SEND_TRANSPORT)
		_delay_sometime(30);
	return result;
}

#define MAX_RECONNECT_SERVER_TIMES	3
uint16_t GPRSConnectToServer(char * socketid, char * ipaddr, char * serverport, char * localport)
{
	
	char cmd[64]="AT+MIPOPEN=";
	int reconnect=0;
	uint16_t result=ERR_OK;
	strcat(cmd, socketid);
	strcat(cmd, ",\"TCP\",\"");
	strcat(cmd, ipaddr);
	strcat(cmd, "\",");
	strcat(cmd, serverport);
	//strcat(cmd, ",");
	//strcat(cmd, localport);
	strcat(cmd, "\r\n");
	
	if((result = GPRSMIPCallSetup()) != ERR_OK)
		return result;

	reconnect =0;
	while(reconnect++ < MAX_RECONNECT_SERVER_TIMES)
	{
		result = GPRSATExecute(cmd, "OK\r\n", 1);
		if(result != ERR_OK)
		{
			result = GPRSDisconnectToServer(MQTT_SERVER_SOCKET_ID, 1);//disconnect 
			if(result == ERR_OK)
				result = GPRSATExecute(cmd, "OK\r\n", 1);//connected again
			else
				result = GPRSATExecute(cmd, "+CME ERROR: 4\r\n", 1);//ignore already connected
		}
		if(result == ERR_OK){
			result = GPRSIsConnectToServer(MQTT_SERVER_SOCKET_ID, 40);
			if(result == ERR_OK)
			{
				break;//connected
			}
		}
	}
	return result;
		
}


uint16_t GPRSIsConnectToServer(char * socketid, int runtimes)
{
	char * cmd="AT+MIPOPEN?\r\n";
	uint16_t result;
	int startfrom, length;

	startfrom = 10;
	length = 1;
	result = GPRSATExecuteComplex(cmd, CheckContainSubStr, "+MIPOPEN:" , runtimes, MQTT_SERVER_SOCKET_ID, &startfrom, &length);
	
	return result;
}

uint16_t GPRSDisconnectToServer(char * socketid, int runtimes)
{
	char cmd[64];
	uint16_t result;

	strcpy(cmd, "AT+MIPCLOSE=");
	strcat(cmd, socketid);
	strcat(cmd, "\r\n");
	result = GPRSATExecute(cmd, "OK\r\n", runtimes);
	_delay_sometime(100);
	return result;
}


uint16_t GPRSPickoutDataHexmode(char * buf, uint16_t len, uint16_t  *pay_load_len)
{
	uint16_t result = ERR_OK;
	int i;
	int socketid, payloadlen;
	char *bufp= buf;
	//find first ','
	while(*bufp != ',' && bufp -buf < len) bufp++;
	if(*bufp != ',')
		return ERR_MATH;
	*bufp = 0;//create a string tail
	socketid= atoi(buf);//convert socketid in buf into int
	if(socketid != atoi(MQTT_SERVER_SOCKET_ID))
		return ERR_VALUE;

	//find second ','
	bufp++;
	buf = bufp;
	while(*bufp != ',' && bufp -buf < len) bufp++;
	if(*bufp != ',')
		return ERR_MATH;
	*bufp = 0;//create a string tail
	payloadlen = atoi(buf);//convert length in buf into int
	bufp++;
	
	if(payloadlen > MAX_CMD_HEX_BUF_SIZE)
		return ERR_RANGE;
	*pay_load_len = payloadlen;
	for(i=0;i<payloadlen*2;i+=2)
	{
		recv_cmd_hex_buf[i/2] =  ATHEX2Char(&bufp[i]);
	}
	recv_cmd_hex_buf[i/2] =0;

	return result;
}

uint16_t GPRSAnalyzeRecvData(char * bufp, uint16_t len)
{
	uint16_t result = ERR_OK;
	uint16_t recv_data_len = len, pay_load_len;
	result = GPRSPickoutDataHexmode(bufp, recv_data_len, &pay_load_len);
	if(result != ERR_OK)
		return result;

	g_gprs_callback(recv_cmd_hex_buf, pay_load_len, g_gprs_callback_privdata);
	return result;
}




uint16_t ATSendDataByHEX(short comnum, char * buf, uint16_t length, uint16_t *sentlen)
{
	uint16_t result=ERR_OK;
	uint16_t len=0;
	int i;
	
	while(length > 0)
	{
		if(length < MAX_CMD_HEX_BUF_SIZE)
		{	//we could process in one times
			len = length;
			length = 0;
		}
		else
		{	//we should process in several times
			len = MAX_CMD_HEX_BUF_SIZE;
			length -= MAX_CMD_HEX_BUF_SIZE;
		}
		for(i=0;i<len*2;i+=2)
			ATChar2HEX(buf[i/2], &recv_cmd_hex_buf[i]);
		recv_cmd_hex_buf[len * 2] =0;
		result = uart_write_sync(comnum, recv_cmd_hex_buf, len*2, sentlen);
	}
	return result;
}


uint16_t SendDataFromLineBufferAllMode(char * buf, uint16_t length, char sendType)
{
	if(!g_wifi_connected)
		GPRSSendDataFromLineBufferAllMode(buf, length, sendType);
	else
		WIFISendDataFromLineBuffer(buf, length);
	
}
