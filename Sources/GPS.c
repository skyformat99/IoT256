/*
 * GPS.c
 *
 *  Created on: Apr 14, 2017
 *      Author: hflw
 */


#include "uart_comm.h"
#include "util.h"
#include "PE_Types.h"
#include "PE_Error.h"

//#include "WIFI.h"
//#include "GPRS.h"
#include "uart0.h"
#include "GPS.h"

//char wifi_cmd_recv_buf[1024];

extern uart0_TDeviceDataPtr uart0DeviceDataPrv;
#define MAX_GPS_RECV_BUF_SIZE	512
char gps_cmd_recv_buf[MAX_GPS_RECV_BUF_SIZE];
char *g_gps_result_last_valid_pointer_in_recv_buf=gps_cmd_recv_buf , *g_gps_result_recv_end_pointer=gps_cmd_recv_buf;
char gps_cmd_result_checking_buf[MAX_GPS_RECV_BUF_SIZE];

#define GPS_DATA_COMING_HEADER "GPGLL,"
uint16_t g_new_gps_data_length;
float g_la,g_lo;
#define MIN_GPS_DATA_COMING_DATA_LEN	10
uint16_t	g_is_new_gps_data_received = 0;//this is indicate new gps data arrived from server
char * g_new_gps_data_start=NULL;
#define MAX_GPS_SINGLE_MSG_BUF_SIZE	128
char recv_gps_data_buf[MAX_GPS_SINGLE_MSG_BUF_SIZE];
char *g_gps_result_last_valid_pointer_in_checking_buf, *g_gps_result_last_valid_pointer_in_checking_buf_tail;
#define GPS_SKIP_DATA_TIMES	10
int g_gps_skip_data_times=0;

int g_gps_connected=0;

uint16_t GPSStartRecv_internal()
{
	uint16_t result;
	result = uart_read_async(WIFIGPS_COM_PORT, gps_cmd_recv_buf, MAX_GPS_RECV_BUF_SIZE);
	return result;
}

//we need to copy data from at recv buffer to othe place before analyze because at recv buffer may wrapped.
//and it's hard to use check function like strstr and strcmp 
uint16_t GPSCopyRecvData2ResultCheckingBuf()
{
	uint16_t result = ERR_OK;
	int len,len2;
	uint16_t i;
	if(!g_gps_result_last_valid_pointer_in_recv_buf)
	{
		log_printf("ERROR:g_gps_result_last_valid_pointer_in_recv_buf is NULL!\n");
		return ERR_VALUE;
	}
	if(!g_gps_result_recv_end_pointer)
	{
		log_printf("ERROR:g_gps_result_recv_end_pointer is NULL!\n");
		return ERR_VALUE;
	}

	if(g_gps_result_recv_end_pointer == g_gps_result_last_valid_pointer_in_recv_buf)//no new data recved
		return ERR_FAULT;
	//status 1: recv data not wrapped
	if(g_gps_result_recv_end_pointer > g_gps_result_last_valid_pointer_in_recv_buf)
	{	
		len = g_gps_result_recv_end_pointer - g_gps_result_last_valid_pointer_in_recv_buf ;
		if(len > MAX_GPS_RECV_BUF_SIZE)
		{
			log_printf("ERROR:too much data need copy or calculate error!len:%d\n", len);
			len = MAX_GPS_RECV_BUF_SIZE;
		}
		memcpy(gps_cmd_result_checking_buf, g_gps_result_last_valid_pointer_in_recv_buf, len );
#ifdef TIMER_DEBUG
		log_printf("cpchk cped.len:%d\n", len);
#endif
	}
	else
	//status 2: recv data wrapped
	{
		//part 1: data before buffer end
		//len = &gps_cmd_recv_buf[MAX_GPS_RECV_BUF_SIZE -1] - g_gps_result_last_valid_pointer_in_recv_buf + 1;
		len = MAX_GPS_RECV_BUF_SIZE - (g_gps_result_last_valid_pointer_in_recv_buf - gps_cmd_recv_buf);
#ifdef TIMER_DEBUG
		log_printf("cpchk cp1.len:%d\n", len);
#endif
		if(len > MAX_GPS_RECV_BUF_SIZE)
		{
			log_printf("ERROR:too much data need copy or calculate error!len:%d\n", len);
			len = MAX_GPS_RECV_BUF_SIZE;
		}
		memcpy(gps_cmd_result_checking_buf, g_gps_result_last_valid_pointer_in_recv_buf, len);
		//part 2: data before recv pointer
		len2 = g_gps_result_recv_end_pointer - gps_cmd_recv_buf;
		if(len + len2> MAX_GPS_RECV_BUF_SIZE)
		{
			log_printf("ERROR:too much data need copy or calculate error!len:%d\n", len);
			len2 = MAX_GPS_RECV_BUF_SIZE - len;
			if(len2 < 0)
				len2 = 0;
		}
		memcpy(gps_cmd_result_checking_buf + len, gps_cmd_recv_buf, len2);
		len += len2;
#ifdef TIMER_DEBUG
		log_printf("cpchk wrap cped.len:%d\n", len);
#endif
	}
	
	//clear useless '\0'
	for(i=0;i<len;i++)
			if(gps_cmd_result_checking_buf[i]=='\0')
				gps_cmd_result_checking_buf[i] = ' ';
		gps_cmd_result_checking_buf[len] = '\0';
/*#ifdef TIMER_DEBUG
	if(len > MAX_OUTPUT_BUF_SIZE)
		log_printf("chkbuf:%s len:%d\n", (gps_cmd_result_checking_buf + len - MAX_OUTPUT_BUF_SIZE), len);
	else
		log_printf("checking buf:%s len:%d\n", gps_cmd_result_checking_buf, len);
#endif*/

	//reset valid result pointer before we start analyze
	g_gps_result_last_valid_pointer_in_checking_buf = gps_cmd_result_checking_buf;
	g_gps_result_last_valid_pointer_in_checking_buf_tail = g_gps_result_last_valid_pointer_in_checking_buf + len;
	
	return result;
}

void GPSUpdateLastValidResulPointerInCheckingBuf(char *bufp)
{
	if(bufp > g_gps_result_last_valid_pointer_in_checking_buf)
	{
		g_gps_result_last_valid_pointer_in_checking_buf = bufp;
#ifdef TIMER_DEBUG
		log_printf("chg check p:%x\n", g_gps_result_last_valid_pointer_in_checking_buf);
#endif
	}
}

void GPSUpdateLastValidResulPointerInRecvBuf()
{
	int len = g_gps_result_last_valid_pointer_in_checking_buf - gps_cmd_result_checking_buf;
#ifdef TIMER_DEBUG
	log_printf("chkmvlen:%d\n", len);
#endif
	if(g_gps_result_last_valid_pointer_in_recv_buf + len > gps_cmd_recv_buf + MAX_GPS_RECV_BUF_SIZE - 1)
	{
		//wrap to head
		len -= MAX_GPS_RECV_BUF_SIZE - (g_gps_result_last_valid_pointer_in_recv_buf - gps_cmd_recv_buf);
		g_gps_result_last_valid_pointer_in_recv_buf = gps_cmd_recv_buf + len;
#ifdef TIMER_DEBUG
		log_printf("wraped\n");
#endif
	}
	else
	{
		//no wrap
		g_gps_result_last_valid_pointer_in_recv_buf += len;
	}

#ifdef TIMER_DEBUG
	log_printf("validmvlen %d recvbuf:%x valp:%x\n", 
			len, (&gps_cmd_recv_buf[0]), g_gps_result_last_valid_pointer_in_recv_buf);
#endif
}


byte GPSInit()
{
	int startfrom, length;

	uint16_t result=ERR_OK;
	
	WiFiStartRecv_internal();
	log_printf("start init GPS......\n\n");
	g_gps_connected=1;//this is HACK, we should check something
	return result;
}


void GPSOtherCommonExpectedResultChecking()
{
	char *bufp;
	char buf_len[5],*bufplentail;
	int i;
	int checkstrlen=strlen(gps_cmd_result_checking_buf);
	
	//we should check other common expect result at same time
	if((bufp= strstr(gps_cmd_result_checking_buf, GPS_DATA_COMING_HEADER)) >0)
	{
		//bufp += strlen(GPS_DATA_COMING_HEADER) + 1;//skip space
		if(g_gps_result_last_valid_pointer_in_checking_buf_tail - gps_cmd_result_checking_buf < MIN_GPS_DATA_COMING_DATA_LEN)
			return;
		
		bufp += strlen(GPS_DATA_COMING_HEADER);//skip ,
		g_new_gps_data_start = bufp;

		g_new_gps_data_length=0;
		while(bufp[g_new_gps_data_length]!='\0' &&
				bufp[g_new_gps_data_length]!='\n' &&
				bufp[g_new_gps_data_length]!='\r' &&
				bufp+g_new_gps_data_length < gps_cmd_result_checking_buf + checkstrlen )
			g_new_gps_data_length++;
		
		if( g_new_gps_data_length < MIN_GPS_DATA_COMING_DATA_LEN -1)
			return;
		if(bufp[g_new_gps_data_length] == '\r' || bufp[g_new_gps_data_length] == '\n')
		{
			if( g_new_gps_data_length > MAX_GPS_SINGLE_MSG_BUF_SIZE - 1)
			{
				log_printf("---too many data from server via gps!\n");
				g_new_gps_data_length = MAX_GPS_SINGLE_MSG_BUF_SIZE -1;
			}
			//move out data from checking buf because it may be flush later
			memcpy(recv_gps_data_buf,g_new_gps_data_start, g_new_gps_data_length);
			recv_gps_data_buf[g_new_gps_data_length] = '\0';
			if(g_gps_skip_data_times == 0)
			{
				g_is_new_gps_data_received = 1;//set flag
				log_printf("<--- get gps data:len:%d data:%s\n", g_new_gps_data_length,gps_cmd_result_checking_buf);
			}
			if(g_gps_skip_data_times ++ == GPS_SKIP_DATA_TIMES)
				g_gps_skip_data_times = 0;
			GPSUpdateLastValidResulPointerInCheckingBuf(g_new_gps_data_start + g_new_gps_data_length);
		}
	}
	/*if((bufp= strstr(gps_cmd_result_checking_buf, GPRS_CLOSE_MIPLINK_HEADER)) >0
			|| (bufp= strstr(gps_cmd_result_checking_buf, GPRS_RESET_HEADER)) >0 )
	{
		g_is_gprs_tcp_closed = 1;//set flag
		ATUpdateLastValidResulPointerInCheckingBuf(bufp + strlen(GPRS_CLOSE_MIPLINK_HEADER));

		//uart2_CancelBlockTransmission(uart2DeviceDataPrv);
	}
	if((bufp= strstr(gps_cmd_result_checking_buf, MQTT_CONNECTED_HEADER)) >0)
	{
		//g_mqtt_alreadd_connected_when_try_conn = 1;//g_mqtt.state = MQTT_STATE_CONNECTED;//set flag
		ATUpdateLastValidResulPointerInCheckingBuf(bufp + strlen(MQTT_CONNECTED_HEADER));

		//uart2_CancelBlockTransmission(uart2DeviceDataPrv);
	}
	
	if((bufp= strstr(gps_cmd_result_checking_buf, GPRS_CLOSE_ERROR_HEADER)) >0)
	{
		g_is_gprs_error_detected = 1;//set flag
		ATUpdateLastValidResulPointerInCheckingBuf(bufp);

		//uart2_CancelBlockTransmission(uart2DeviceDataPrv);
	}*/

}

extern int g_pin_retry_times;
extern int g_mqtt_connecting_timeout;

#ifdef FAKE_GPS_DATA
int g_gps_fake_timeout = MAX_FACK_GPS_TIMEOUT;
#endif

void GPSCheckRecvResult_internal_async()
{
	char *tmpp=gps_cmd_recv_buf;
	if(!uart0DeviceDataPrv)
		return ERR_PARAM_VALUE;
#ifdef FAKE_GPS_DATA
	char *fakegpsdata = "4717.11364,N,00833.91565,E,092321.00,A,A*60";
	if(g_pin_retry_times == 1 && g_gps_fake_timeout == 0 && g_is_new_gps_data_received ==0)
	{
		g_new_gps_data_start = fakegpsdata;
		g_new_gps_data_length = strlen(fakegpsdata);
		memcpy(recv_gps_data_buf,g_new_gps_data_start, g_new_gps_data_length);
		g_is_new_gps_data_received =1;
		g_gps_fake_timeout = MAX_FACK_GPS_TIMEOUT;
		return;
	}
#endif
	
	if(uart0DeviceDataPrv->InpRecvDataNum > MIN_GPS_DATA_COMING_DATA_LEN)
	{
		if(uart0DeviceDataPrv->InpRecvDataNum < MAX_GPS_RECV_BUF_SIZE)
			tmpp = gps_cmd_recv_buf + uart0DeviceDataPrv->InpRecvDataNum; 
		else
			tmpp = &gps_cmd_recv_buf[0];//NOTE: this is out of the memory
		if(tmpp != g_gps_result_recv_end_pointer)//new data came
		{
			g_gps_result_recv_end_pointer = tmpp;
#ifdef TIMER_DEBUG
			log_printf("gps recv len:%d recvend:%x\n", 
				uart0DeviceDataPrv->InpRecvDataNum, g_gps_result_recv_end_pointer);
#endif
			//copy data from recv buffer to result checking buffer
			if(GPSCopyRecvData2ResultCheckingBuf()== ERR_OK)//copy success
			{
				//check other common expected result from checking buffer
				//the result will be marked in following function
				GPSOtherCommonExpectedResultChecking();
			
				//update last valide result pointer
				GPSUpdateLastValidResulPointerInRecvBuf();
			}
		}
	}

}



