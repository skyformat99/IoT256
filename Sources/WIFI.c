/*
 * WIFI.c
 *
 *  Created on: Oct 20, 2016
 *      Author: hflw
 */

#include "uart_comm.h"
#include "util.h"
#include "PE_Types.h"
#include "PE_Error.h"
#include "ATcommand.h"

#include "WIFI.h"
#include "GPRS.h"
#include "uart0.h"
#include "mqttc\mqtt.h"
#include "SetupCmd.h"

char wifi_cmd_recv_buf[1024];

extern uart0_TDeviceDataPtr uart0DeviceDataPrv;
extern char at_cmd_recv_buf[MAX_CMD_RECV_BUF_SIZE];
extern char *g_at_result_last_valid_pointer_in_recv_buf , *g_at_result_recv_end_pointer;
extern char at_cmd_result_checking_buf[MAX_CMD_RECV_BUF_SIZE];
extern struct at_expected_result_and_condition g_current_at_expected_result;

int g_wifi_connected=0;

char g_wifi_disable[USER_DATA_LENGTH_DISABLE_WIFI];
char g_wifi_ssid[USER_DATA_WIFI_SSID_LENGTH];
char g_wifi_pwd[USER_DATA_WIFI_PWD_LENGTH];
uint16_t WiFiStartRecv_internal()
{
	uint16_t result;
	result = uart_read_async(WIFIGPS_COM_PORT, at_cmd_recv_buf, MAX_CMD_RECV_BUF_SIZE);
	return result;
}

uint16_t WIFIATExecute(char *cmd, char *expect_result)
{
	return ATExecuteOnce(WIFIGPS_COM_PORT, cmd, wifi_cmd_recv_buf, MAX_WIFI_CMD_RECV_BUF_SIZE, expect_result);
}

uint16_t WIFIATExecuteComplex(char *cmd, CHECK_RESULT_FUNC_PTR checkfunc, char *expect_result, char shortmode, char * value, int * param1, int * param2)
{
	return ATExecuteComplex(WIFIGPS_COM_PORT, cmd, wifi_cmd_recv_buf, MAX_WIFI_CMD_RECV_BUF_SIZE, checkfunc, expect_result, shortmode, value, param1, param2);

}

#define MAX_WAIT_GET_IP_TIMES	5
byte WIFIInit()
{
	int startfrom, length;
	int wait_ip_retry_times = MAX_WAIT_GET_IP_TIMES;
	char cmdATtmp[128];

	uint16_t result;
	
	WiFiStartRecv_internal();
	log_printf("start init WIFI......\n\n");
	result = WIFIATExecute( "AT\r\n", "OK\r\n");
	if(result == ERR_OK)
		result = WIFIATExecute( "AT+CWMODE=3\r\n", "OK\r\n");
	if(result == ERR_OK)
	{
		log_printf("Wait WIFI to get ip......\n\n");
		while (wait_ip_retry_times--)
		{
			_delay_sometime(200);
			//result = WIFIATExecute( "AT\r\n", "CONNECTED\r\n");
			//if(result == ERR_OK)
				//break;
			//if(result == ERR_OK)
			strcpy(cmdATtmp,"AT+CIPSTART=\"TCP\",\"");
			strcat(cmdATtmp,g_mqtt_server_ip);
			strcat(cmdATtmp,"\",3881\r\n");
				result = WIFIATExecute( cmdATtmp, "CONNECT\r\n");
			if(result == ERR_OK)
				break;
			if(result != ERR_OK)//try again
				result = WIFIATExecute( cmdATtmp, "ALREADY");
			if(result == ERR_OK)
				break;
		}
	}
	
	//if(result == ERR_OK)
		//result = WIFIATExecute( "AT+CIFSR\r\n", "OK\r\n");
	if(result != ERR_OK)
	{
		strcpy(cmdATtmp,"AT+CWJAP=\"");
		strcat(cmdATtmp,g_wifi_ssid);
		strcat(cmdATtmp,"\",\"");
		strcat(cmdATtmp,g_wifi_pwd);
		strcat(cmdATtmp,"\"\r\n");
		result = WIFIATExecute( cmdATtmp, "CONNECTED\r\n");
		//result = WIFIATExecute( "AT+CWJAP=\"aWiFi-410\",\"12345678\"\r\n", "CONNECTED\r\n");
		//result = WIFIATExecute( "AT+CWJAP=\"huiyishi\",\"yanfaxd10\"\r\n", "CONNECTED\r\n");
		//result = WIFIATExecute( "AT+CWJAP=\"jack_field\",\"sweethome\"\r\n", "CONNECTED\r\n");
		//result = WIFIATExecute( "AT+CWJAP=\"NETGEAR49\",\"13029932\"\r\n", "CONNECTED\r\n");
		//result = WIFIATExecute( "AT+CWJAP=\"dlink123\",\"yanfa999\"\r\n", "CONNECTED\r\n");
		//WIFIATExecute( "AT+CWJAP=\"HUAWEI P9\",\"88316057\"\r\n", "CONNECTED\r\n");
	}
	
	//if(result == ERR_OK)
		//result = WIFIATExecute( "AT+CIPSTART=\"TCP\",\"218.3.215.228\",3881\r\n", "CONNECT\r\n");
	//if(result != ERR_OK)//try again
		//result = WIFIATExecute( "AT+CIPSTART=\"TCP\",\"218.3.215.228\",3881\r\n", "ALREADY");
	
	if(result != ERR_OK)
	{
		log_printf("Init WIFI failed!\n");
		g_wifi_connected = 0;
		return result;
	}
	else
	{
		log_printf("Init WIFI success!\n");
		g_wifi_connected = 1;
		return ERR_OK;
	}
}


uint16_t WIFISendDataCommon( uint16_t *sentlen)
{
	uint16_t result=ERR_OK;
	log_printf("send data to WIFI...\n");

	/*if(GPRSIsConnectToServer(MQTT_SERVER_SOCKET_ID) != ERR_OK)
	{
		//result = ATConnectToServer(MQTT_SERVER_SOCKET_ID, MQTT_SERVER_IP_ADDR, MQTT_SEVER_PORT, MQTT_CLIENT_PORT);
		result = GPRSInit();
		//if(result == ERR_OK)
		//	mqtt_init_and_connect();
	}
	if(result != ERR_OK)
	{
		log_printf("connect to server failed!");
		return result;
	}*/

	char cmdhead[32];
	//result = WIFIATExecute( "AT+CIPSTART=\"TCP\",\"218.3.215.228\",3881\r\n", "ALREADY");
	
	//if(sendType == GPRS_SEND_FAST)
	//{
		strcpy(cmdhead, AT_CMD_WIFI_HEAD_FAST_SEND);
		strcat(cmdhead, "=");
		sprintf(cmdhead, "%s%d", cmdhead, *sentlen);
		strcat(cmdhead, "\r\n");
	/*}
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
	}*/
	//if(result == ERR_OK)
		result = uart_write_sync(WIFIGPS_COM_PORT, cmdhead, strlen(cmdhead), sentlen);
	//if(sendType == GPRS_SEND_NORMAL || sendType == GPRS_SEND_TRANSPORT)
	//	_delay_sometime(1);
	return result;
}

uint16_t WIFISendDataFromLineBuffer(char * buf, uint16_t length)
{
	uint16_t result=ERR_OK;
	uint16_t uartsentlen=length, uartsentpayloadlen;
	int i;

	//check connection and send head
	result = WIFISendDataCommon(&uartsentlen);//, SEMD_MODE_HEX, sendType);
	if(result != ERR_OK)
		return result;
	//send payload
	//if(sendType != GPRS_SEND_TRANSPORT)
	//	result = ATSendDataByHEX(GPRS_COM_PORT, buf, length, &uartsentpayloadlen);
	//else
	
	log_printf("sending...\n");
	//WIFIStartAsyncRecv();
	for(i=0;i<length;i++)
		log_printf("%0x",buf[i]);
	log_printf("\n");
	result = uart_write_sync(WIFIGPS_COM_PORT, buf, length, &uartsentpayloadlen);
	if(result != ERR_OK)
		return result;
	
	
	//send tail and start read result
	/*char * cmdtail;
	char ctrlz = CTRLZ;
	if(sendType == GPRS_SEND_FAST)
		cmdtail = "\"\r\n";
	else if(sendType == GPRS_SEND_NORMAL)
		cmdtail = &ctrlz;
	else if(sendType == GPRS_SEND_TRANSPORT)
	{
		_delay_sometime(5);
		cmdtail = AT_CMD_TAIL_TRANSPORT_SEND;
	}*/
	//GPRSStartAsyncRecv();
	//log_printf("sending...\n");
	//result = uart_write_sync(GPRS_COM_PORT, cmdtail, strlen(cmdtail), &uartsentlen);
	//if(result != ERR_OK)
		//return result;
	/*if(sendType == GPRS_SEND_TRANSPORT)
	{
		_delay_sometime(10);
		//_delay_sometime(10);
		//_delay_sometime(10);
	}*/
		
	log_printf("send data to WIFI with line buffer complete!sent payload: %d\n", uartsentpayloadlen);
	
	return result;
}

#define MAX_WIFI_ASYNC_READ_TICKS	30
static int g_wifi_async_read_ticks = 0;
/*uint16_t WIFIStartAsyncRecv()
{
	uint16_t result;
	result = ATStartRecv_internal(WIFIGPS_COM_PORT);
	g_wifi_async_read_ticks = MAX_WIFI_ASYNC_READ_TICKS;
	return result;
}*/


/*uint16_t	g_is_new_wifi_data_received = 0;
uint16_t	g_new_wifi_data_length=0;
char * g_new_wifi_data_start=NULL;
uint16_t WIFICheckRecvResult_internal()
{
	uint16_t result= ERR_FAULT;
	char *bufp;
	char buf_len[5],*bufplentail;
	int i;
	if(!uart0DeviceDataPrv)
		return ERR_PARAM_VALUE;
	if(!g_wifi_async_read_ticks)
		return ERR_FAULT;
	else
	{
		g_wifi_async_read_ticks --;
		if(!g_wifi_async_read_ticks)
		{
			uart0_CancelBlockTransmission(uart0DeviceDataPrv);//this is last time we are here, cancel read.
		}
	}

	if(uart0DeviceDataPrv->InpRecvDataNum > 0)
	{
		//we receive message we should analyze them here
		if((bufp= strstr(wifi_cmd_recv_buf, WIFI_DATA_COMING_HEADER)) >0)
		{
			g_is_new_wifi_data_received = 1;//set flag
			
			bufp += strlen(WIFI_DATA_COMING_HEADER) + 1;//skip ,
			g_new_wifi_data_start = bufp;
			bufplentail = bufp;
			for(i=0;i<5;i++)
				if(*bufplentail == ':')
					break;
				else
					bufplentail++;
			*bufplentail = 0;
			g_new_wifi_data_length = atoi(g_new_wifi_data_start);
			g_new_wifi_data_start = bufplentail+1;

			g_wifi_async_read_ticks = 0;//get message no need to check read result again.
			uart0_CancelBlockTransmission(uart0DeviceDataPrv);
			result = ERR_OK;
		}*/
		/*if((bufp= strstr(gprs_cmd_recv_buf, GPRS_CLOSE_MIPLINK_HEADER)) >0)
		{
			g_is_gprs_tcp_closed = 1;//set flag

			g_gprs_async_read_ticks = 0;//get message no need to check read result again.
			uart2_CancelBlockTransmission(uart2DeviceDataPrv);
			result = ERR_OK;
		}
		if((bufp= strstr(gprs_cmd_recv_buf, GPRS_CLOSE_ERROR_HEADER)) >0)
		{
			g_is_gprs_error_detected = 1;//set flag

			g_gprs_async_read_ticks = 0;//get message no need to check read result again.
			uart2_CancelBlockTransmission(uart2DeviceDataPrv);
			result = ERR_OK;
		}*/
	/*}
	
	return result;
}*/

#define WIFI_DATA_COMING_HEADER "+IPD"
uint16_t g_new_wifi_data_length;
#define MIN_WIFI_DATA_COMING_DATA_LEN	7
uint16_t	g_is_new_wifi_data_received = 0;//this is indicate new wifi data arrived from server
char * g_new_wifi_data_start=NULL;
extern char recv_server_data_buf[MAX_CMD_HEX_BUF_SIZE];
extern char *g_at_result_last_valid_pointer_in_checking_buf_tail;
void WIFIOtherCommonExpectedResultChecking()
{
	char *bufp;
	char buf_len[5],*bufplentail;
	int i;
	int checkstrlen=strlen(at_cmd_result_checking_buf);
	
	//we should check other common expect result at same time
	if((bufp= strstr(at_cmd_result_checking_buf, WIFI_DATA_COMING_HEADER)) >0)
	{
		//bufp += strlen(WIFI_DATA_COMING_HEADER) + 1;//skip space
		if(g_at_result_last_valid_pointer_in_checking_buf_tail - at_cmd_result_checking_buf < MIN_WIFI_DATA_COMING_DATA_LEN)
			return;
		
		bufp += strlen(WIFI_DATA_COMING_HEADER) + 1;//skip ,
					g_new_wifi_data_start = bufp;
					bufplentail = bufp;
					for(i=0;i<5;i++)
						if(*bufplentail == ':')
							break;
						else
							buf_len[i]=*bufplentail++;
					//*bufplentail = 0;
					buf_len[i]=0;
					g_new_wifi_data_length = atoi(buf_len);
		if(*bufplentail == ':' && g_new_wifi_data_length)
			if(bufp+g_new_wifi_data_length + 2 <= g_at_result_last_valid_pointer_in_checking_buf_tail)
				g_new_wifi_data_start = bufplentail+1;
			else
				return;
		else
			return;
					

		/*g_new_wifi_data_length=0;
		while(bufp[g_new_wifi_data_length]!='\0' &&
				//bufp[g_new_wifi_data_length]!='\n' &&
				//bufp[g_new_wifi_data_length]!='\r' &&
				bufp+g_new_wifi_data_length < at_cmd_result_checking_buf + checkstrlen )
			g_new_wifi_data_length++;*/
		
		//if( g_new_wifi_data_length < MIN_WIFI_DATA_COMING_DATA_LEN)
			//return;
		//if(bufp[g_new_wifi_data_length] == '\r' || bufp[g_new_wifi_data_length] == '\n')
		{
			if( g_new_wifi_data_length > MAX_CMD_HEX_BUF_SIZE - 1)
			{
				log_printf("---too many data from server via wifi!\n");
				g_new_wifi_data_length = MAX_CMD_HEX_BUF_SIZE -1;
			}
			//move out data from checking buf because it may be flush later
			memcpy(recv_server_data_buf,g_new_wifi_data_start, g_new_wifi_data_length);
			recv_server_data_buf[g_new_wifi_data_length] = '\0';
			//g_new_wifi_data_start = recv_server_data_buf;
			log_printf("<--- get wifi data:len:%d\n", g_new_wifi_data_length);
			for(i=0;i<g_new_wifi_data_length;i++)
				log_printf("%0x ",recv_server_data_buf[i]);
			log_printf("\n");
			
			g_is_new_wifi_data_received = 1;//set flag
			//ATUpdateLastValidResulPointerInCheckingBuf(bufp);
			ATUpdateLastValidResulPointerInCheckingBuf(g_new_wifi_data_start + g_new_wifi_data_length);
		}

		//uart2_CancelBlockTransmission(uart2DeviceDataPrv);
	}
	/*if((bufp= strstr(at_cmd_result_checking_buf, GPRS_CLOSE_MIPLINK_HEADER)) >0
			|| (bufp= strstr(at_cmd_result_checking_buf, GPRS_RESET_HEADER)) >0 )
	{
		g_is_gprs_tcp_closed = 1;//set flag
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
		ATUpdateLastValidResulPointerInCheckingBuf(g_at_result_last_valid_pointer_in_checking_buf_tail);

		//uart2_CancelBlockTransmission(uart2DeviceDataPrv);
	}*/

}

void WIFICheckRecvResult_internal_async()
{
	char *tmpp=at_cmd_recv_buf;
	if(!uart0DeviceDataPrv)
		return ERR_PARAM_VALUE;

	if(uart0DeviceDataPrv->InpRecvDataNum > MIN_RESULT_STRING_SIZE)
	{
		if(uart0DeviceDataPrv->InpRecvDataNum < MAX_CMD_RECV_BUF_SIZE)
			tmpp = at_cmd_recv_buf + uart0DeviceDataPrv->InpRecvDataNum; 
		else
			tmpp = &at_cmd_recv_buf[MAX_CMD_RECV_BUF_SIZE];//NOTE: this is out of the memory
		if(tmpp != g_at_result_recv_end_pointer)//new data came
		{
			g_at_result_recv_end_pointer = tmpp;
#ifdef TIMER_DEBUG
			log_printf("wifi recv len:%d recvend:%x\n", 
				uart0DeviceDataPrv->InpRecvDataNum, g_at_result_recv_end_pointer);
#endif
			//copy data from recv buffer to result checking buffer
			if(ATCopyATRecvData2ResultCheckingBuf()== ERR_OK)//copy success
			{
				//check AT result from result checking buffer
				g_current_at_expected_result.analyze_result = ATCheckResultFromResultCheckingBuf();
			
				//check other common expected result from checking buffer
				//the result will be marked in following function
				WIFIOtherCommonExpectedResultChecking();
			
				//update last valide result pointer
				ATUpdateLastValidResulPointerInRecvBuf();
			}
		}
	}

}

GPRS_READ_CALLBACK_FUNC_PTR g_wifi_callback;
void * g_wifi_callback_privdata;
void WIFISetReadDataCallback(GPRS_READ_CALLBACK_FUNC_PTR func, void * privData)
{
	g_wifi_callback = func;
	g_wifi_callback_privdata = privData;
}

uint16_t WIFIAnalyzeRecvData(char * bufp, uint16_t len)
{
	uint16_t result = ERR_OK;
	uint16_t recv_data_len = len, pay_load_len;
	//result = GPRSPickoutDataHexmode(bufp, recv_data_len, &pay_load_len);
	//if(result != ERR_OK)
		//return result;

	g_wifi_callback(bufp, recv_data_len, g_wifi_callback_privdata);
	return result;
}

uint16_t WIFISendDataFromLineBufferAllMode(char * buf, uint16_t length, char sendType)
{
	uint16_t result=ERR_OK;
	uint16_t uartsentlen=length, uartsentpayloadlen;

	//check connection and send head
	result = WIFISendDataCommon(&uartsentlen);
	if(result != ERR_OK)
		return result;
	//send payload
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
		//_delay_sometime(5);
		cmdtail = AT_CMD_TAIL_TRANSPORT_SEND;
	}
	GPRSStartAsyncRecv();
	log_printf("sending...\n");
	result = uart_write_sync(GPRS_COM_PORT, cmdtail, strlen(cmdtail), &uartsentlen);
	if(result != ERR_OK)
		return result;
	if(sendType == GPRS_SEND_TRANSPORT)
	{
		//_delay_sometime(1);
		//_delay_sometime(10);
		//_delay_sometime(10);
	}
		
	log_printf("send data to GPRS with line buffer complete!sent payload: %d\n", uartsentpayloadlen);
	
	return result;
}
