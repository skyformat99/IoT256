/*
 * ATcommand.c
 *
 *  Created on: Aug 10, 2016
 *      Author: hflw
 */
#include "PE_Types.h"
#include "PE_Error.h"
#include "uart_comm.h"
#include "util.h"
#include "stdlib.h"
#include "cstring"
#include "ATcommand.h"
#include "mqttc\mqtt.h"
#include "HOST.h"

#define MAX_OUTPUT_BUF_SIZE	64
int g_gprs_or_wifi_mode = GPRS_MODE;
int g_at_async_read_timeout = 0;//how long time we wait for a AT response
extern char at_cmd_recv_buf[MAX_CMD_RECV_BUF_SIZE];//circle buffer for result AT result
extern char at_cmd_result_checking_buf[MAX_CMD_RECV_BUF_SIZE];//the buffer for analyze AT result, we copy from recv buffer to make a line buffer 
extern char g_substring_result_check_buf[RESULT_EXPECT_LENGTH];//when we checking complex string condition, we using this buffer as temp buff
char *g_at_result_last_valid_pointer_in_recv_buf = at_cmd_recv_buf;
char *g_at_result_last_valid_pointer_in_checking_buf = at_cmd_result_checking_buf;
char *g_at_result_last_valid_pointer_in_checking_buf_tail;
//following value will be set at GPRS.c or Wifi.c
char *g_at_result_recv_end_pointer = at_cmd_recv_buf;//the pointer for where the last byte have been recv in recv buf
struct at_expected_result_and_condition g_current_at_expected_result;

extern int g_wifi_connected;

void ATUpdateLastValidResulPointerInCheckingBuf(char *bufp)
{
	if(bufp > g_at_result_last_valid_pointer_in_checking_buf)
	{
		g_at_result_last_valid_pointer_in_checking_buf = bufp;
#ifdef TIMER_DEBUG
		log_printf("chg check p:%x\n", g_at_result_last_valid_pointer_in_checking_buf);
#endif
	}
}


void ATUpdateLastValidResulPointerInRecvBuf()
{
	int len = g_at_result_last_valid_pointer_in_checking_buf - at_cmd_result_checking_buf;
#ifdef TIMER_DEBUG
	log_printf("chkmvlen:%d\n", len);
#endif
	if(g_at_result_last_valid_pointer_in_recv_buf + len > at_cmd_recv_buf + MAX_CMD_RECV_BUF_SIZE - 1)
	{
		//wrap to head
		len -= MAX_CMD_RECV_BUF_SIZE - (g_at_result_last_valid_pointer_in_recv_buf - at_cmd_recv_buf);
		g_at_result_last_valid_pointer_in_recv_buf = at_cmd_recv_buf + len;
#ifdef TIMER_DEBUG
		log_printf("wraped\n");
#endif
	}
	else
	{
		//no wrap
		g_at_result_last_valid_pointer_in_recv_buf += len;
	}

#ifdef TIMER_DEBUG
	log_printf("validmvlen %d recvbuf:%x valp:%x\n", 
			len, (&at_cmd_recv_buf[0]), g_at_result_last_valid_pointer_in_recv_buf);
#endif
}


uint16_t CheckEqualStr(char *checkbuf, char *expect, char * value, int * param1, int *param2)
{
	char * bufp;
	uint16_t result = ERR_FAULT;
	if(!expect || !checkbuf || !value)
	{
		log_printf("ERROR: chkequ NULL pointer!\n");
		return result;
	}

	if((bufp = strstr(checkbuf, expect )) == 0)
	{	//isn't match
		//log_printf("AT result isn't match expectation!\n");
		result = ERR_FAULT;
	}
	else
	{
		//log_printf("AT result match!\n");
		ATUpdateLastValidResulPointerInCheckingBuf(bufp + strlen(expect));
		result = ERR_OK;
	}
#ifdef TIMER_DEBUG
	log_printf("checkequ exp:%s result:%d\n", expect, result);
#endif
	return result;
}

uint16_t CheckContainSubStr(char *checkbuf, char *expect, char * value, int * startfrom, int *length)
{
	char *bufp;
	char *bufpend = at_cmd_result_checking_buf+MAX_CMD_RECV_BUF_SIZE -1;//this is point to the array last byte
	uint16_t result = ERR_FAULT;
	
	if(!expect || !checkbuf || !value)
	{
		log_printf("ERROR:NULL pointer!\n");
		return ERR_PARAM_VALUE;
	}
	if((bufp = strstr(checkbuf, expect )) == 0)//no match expected result in the check buf
	{
#ifdef TIMER_DEBUG
		log_printf("chkstr notmatch\n");
#endif
		return ERR_PARAM_VALUE;
	}
	if(*length > RESULT_EXPECT_LENGTH - 1)
	{
#ifdef TIMER_DEBUG
		log_printf("chkstr len:%d too big\n", *length);
#endif	
		return ERR_PARAM_SIZE;
	}
	if(bufp + (*startfrom) + *length + 1<= bufpend )//check if we have enough source to copy
	{
		strncpy(g_substring_result_check_buf, (bufp + (*startfrom)), *length);
		g_substring_result_check_buf[*length] = '\0';
		if(strcmp(g_substring_result_check_buf, value) == 0)
		{
			ATUpdateLastValidResulPointerInCheckingBuf(bufp + (*startfrom) +*length);
#ifdef TIMER_DEBUG
		log_printf("chkstr match\n");
#endif	
			return ERR_OK;
		}
		else
		{
#ifdef TIMER_DEBUG
		log_printf("chkstr notmatch\n");
#endif
			return ERR_FAULT;
		}

	}
	else
	{
		log_printf("ERROR:chkstr big than chkbuf!\n");
		return ERR_PARAM_SIZE;
	}
}

//we need to copy data from at recv buffer to othe place before analyze because at recv buffer may wrapped.
//and it's hard to use check function like strstr and strcmp 
uint16_t ATCopyATRecvData2ResultCheckingBuf()
{
	uint16_t result = ERR_OK;
	int len;
	uint16_t i;
	if(!g_at_result_last_valid_pointer_in_recv_buf)
	{
		log_printf("ERROR:g_at_result_last_valid_pointer_in_recv_buf is NULL!\n");
		return ERR_VALUE;
	}
	if(!g_at_result_recv_end_pointer)
	{
		log_printf("ERROR:g_at_result_recv_end_pointer is NULL!\n");
		return ERR_VALUE;
	}

	if(g_at_result_recv_end_pointer == g_at_result_last_valid_pointer_in_recv_buf)//no new data recved
		return ERR_FAULT;
	//status 1: recv data not wrapped
	if(g_at_result_recv_end_pointer > g_at_result_last_valid_pointer_in_recv_buf)
	{	
		len = g_at_result_recv_end_pointer - g_at_result_last_valid_pointer_in_recv_buf ;
		if(len > MAX_CMD_RECV_BUF_SIZE)
		{
			log_printf("ERROR:too much data need copy!len:%d\n", len);
			len = MAX_CMD_RECV_BUF_SIZE;
		}
		memcpy(at_cmd_result_checking_buf, g_at_result_last_valid_pointer_in_recv_buf, len );
#ifdef TIMER_DEBUG
		log_printf("cpchk cped.len:%d\n", len);
#endif
	}
	else
	//status 2: recv data wrapped
	{
		//part 1: data before buffer end
		//len = &at_cmd_recv_buf[MAX_CMD_RECV_BUF_SIZE -1] - g_at_result_last_valid_pointer_in_recv_buf + 1;
		len = MAX_CMD_RECV_BUF_SIZE - (g_at_result_last_valid_pointer_in_recv_buf - at_cmd_recv_buf);
#ifdef TIMER_DEBUG
		log_printf("cpchk cp1.len:%d\n", len);
#endif
		memcpy(at_cmd_result_checking_buf, g_at_result_last_valid_pointer_in_recv_buf, len);
		//part 2: data before recv pointer
		memcpy(at_cmd_result_checking_buf + len, at_cmd_recv_buf, 
				g_at_result_recv_end_pointer - at_cmd_recv_buf);
		len += g_at_result_recv_end_pointer - at_cmd_recv_buf;
#ifdef TIMER_DEBUG
		log_printf("cpchk wrap cped.len:%d\n", len);
#endif
	}
	
	//clear useless '\0'
	if(!g_wifi_connected)
	{
		for(i=0;i<len;i++)
			if(at_cmd_result_checking_buf[i]=='\0')
				at_cmd_result_checking_buf[i] = ' ';
		at_cmd_result_checking_buf[len] = '\0';
	}
/*#ifdef TIMER_DEBUG
	if(len > MAX_OUTPUT_BUF_SIZE)
		log_printf("chkbuf:%s len:%d\n", (at_cmd_result_checking_buf + len - MAX_OUTPUT_BUF_SIZE), len);
	else
		log_printf("checking buf:%s len:%d\n", at_cmd_result_checking_buf, len);
#endif*/

	//reset valid result pointer before we start analyze
	g_at_result_last_valid_pointer_in_checking_buf = at_cmd_result_checking_buf;
	g_at_result_last_valid_pointer_in_checking_buf_tail = g_at_result_last_valid_pointer_in_checking_buf + len;
	
	return result;
}

//we check result according to currrent at command's expected result and its condition.
//we may check this many time from result analyze buffer and mark the result
//the analyze result will be polling by at execute and decide next step accordingly
uint16_t ATCheckResultFromResultCheckingBuf()
{
	if(g_current_at_expected_result.checkfunc != NULL)
		return g_current_at_expected_result.checkfunc(at_cmd_result_checking_buf, g_current_at_expected_result.expect_result, 
			g_current_at_expected_result.value, g_current_at_expected_result.param1, g_current_at_expected_result.param2 );
	else
	{
		log_printf("g_current_at_expected_result.checkfunc is NULL!\n");
		return ERR_FAULT;
	}
}


//the function is the core function to execute AT command once
//including send AT command and get it's result
uint16_t ATExecuteCmd_internal(short comnum, char *cmd, char *recv, uint16_t * recv_len)
{
	uint16_t sent;
	uint16_t bufsize = *recv_len, recv_once_len;
	char * bufp = recv;
	int read_count;
	uint16_t result=ERR_OK;
	int chkbufsize=0;
	
	g_at_async_read_timeout = MAX_AT_ASYNC_READ_TICKS;
	
	if(!cmd)
	{
		log_printf("cmd is NULL!\n");
		return ERR_VALUE;
	}

	log_printf("send AT:%s", cmd);
	if((result = uart_write_sync(comnum, cmd, strlen(cmd), &sent))!= ERR_OK)
	{
		//log_printf("...failed! result:%d\n", result);
		return result;
	}
	else
	{
		//log_printf("...success!\n");
	} 
	
	//wait for result checking, polling expect
	while(1)
	{
		if(WIFIGPS_COM_PORT == comnum)
			WIFICheckRecvResult_internal_async();
		else
			GPRSCheckRecvResult_internal();//HACK: we should check in main thread

		if(!g_at_async_read_timeout)
		{
			log_printf("...timeout!\n");
			//reset valid pointer. timeout, the old result is useless.
			g_at_result_last_valid_pointer_in_recv_buf = g_at_result_recv_end_pointer;
			//print check buf
			chkbufsize = strlen(at_cmd_result_checking_buf);
			if(chkbufsize > MAX_OUTPUT_BUF_SIZE)
			{
				log_printf("ATresp:%s\n", (at_cmd_result_checking_buf + chkbufsize - MAX_OUTPUT_BUF_SIZE));
			}
			else
				log_printf("ATresp:%s\n", at_cmd_result_checking_buf);
			result = ERR_OVERFLOW;
			break;
		}
		if(g_current_at_expected_result.analyze_result == ERR_OK )
		{
			log_printf("...success!\n");
			log_printf("ATresp:%s\n", at_cmd_result_checking_buf);
			result = g_current_at_expected_result.analyze_result;
			break;
		}
		_delay_sometime(1);
	}
	return result;
	
	//receive result. NOTE:we should add MUTEX here
	//we need read many times, due to we may only read part data
	/*read_count = 0;
	*recv_len = 0;
	//log_printf("start recv AT result:\n");
	while(read_count++ < MAX_RE_READ_TIMES )
	{
		//log_printf("read %d times\n", read_count);
		result = uart_read_sync(comnum, bufp, bufsize, &recv_once_len);
		
		if(result !=ERR_OK)
		{
			//log_printf("...read failed:\n");
			continue;
		}
		if(recv_once_len <= 0)
		{
			//log_printf("...read nothing\n");
			continue;
		}
		else
		{
			//log_printf("...read %d data\n", recv_once_len);
			*recv_len += recv_once_len;
			bufp += recv_once_len;
		}
	}
	
	if(*recv_len <=0)
	{
		log_printf("no AT result captured!\n");
		return ERR_RXEMPTY;
	}
	else if(*recv_len > bufsize)
	{
		log_printf("AT result is too long!Ignore these data!\n");
		return ERR_RANGE;
	}
	else
	{
		recv[*recv_len] = '\0';
		log_printf("AT command result is:\n%s\n", recv);
		return ERR_OK;
	}*/
}

uint16_t ATExecuteOnce(short comnum, char *cmd, char * recv_buf, uint16_t recv_len, char *expect_result)
{
	return ATExecuteComplex(comnum, cmd, recv_buf, recv_len, CheckEqualStr, expect_result, 1, NULL, NULL, NULL);
}

uint16_t ATExecute(short comnum, char *cmd, char * recv_buf, uint16_t recv_len, char *expect_result, int runtimes)
{
	return ATExecuteComplex(comnum, cmd, recv_buf, recv_len, CheckEqualStr, expect_result, runtimes, NULL, NULL, NULL);
}

#define MAX_CMD_RETRY_TIMES	20
uint16_t ATExecuteComplex(short comnum, char *cmd, char * recv_buf, uint16_t recv_len, CHECK_RESULT_FUNC_PTR checkfunc, char *expect_result, int runtimes, char *value, int * param1, int * param2)
{
	uint16_t step_result;
	int cmd_retry_times=0;
	int max_retry_times = MAX_CMD_RETRY_TIMES;
	
	//log_printf("Start execute AT cmd:\n%s expect result:\n%s\n\n", cmd, expect_result);
	if(runtimes > 0)
		max_retry_times = runtimes;
	while(++cmd_retry_times <= max_retry_times)
	{
		//log_printf("try execute AT command %d times\n", cmd_retry_times);
		
		//prepare check conditions
		g_current_at_expected_result.checkfunc = checkfunc;
		if(expect_result)
		{
			if(strlen(expect_result) < MAX_EXPECT_RESULT_DESCRIPTION_STRING_LENGTH - 1)
				strcpy(g_current_at_expected_result.expect_result , expect_result);
			else
				strncpy(g_current_at_expected_result.expect_result , expect_result, MAX_EXPECT_RESULT_DESCRIPTION_STRING_LENGTH - 1);
		}
		else
			memset(g_current_at_expected_result.expect_result, 0, MAX_EXPECT_RESULT_DESCRIPTION_STRING_LENGTH);
		
		if(value)
		{
			if(strlen(value) < MAX_EXPECT_RESULT_DESCRIPTION_STRING_LENGTH - 1)
				strcpy(g_current_at_expected_result.value , value);
			else
				strncpy(g_current_at_expected_result.value , value, MAX_EXPECT_RESULT_DESCRIPTION_STRING_LENGTH - 1);
		}
		else
			memset(g_current_at_expected_result.value, 0, MAX_EXPECT_RESULT_DESCRIPTION_STRING_LENGTH);

		g_current_at_expected_result.param1 = param1;
		g_current_at_expected_result.param2 = param2;
		g_current_at_expected_result.analyze_result = ERR_NOTAVAIL;
		
		//start execute the AT command
		if((step_result = ATExecuteCmd_internal(comnum, cmd, recv_buf, &recv_len)) != ERR_OK)
		{
			//log_printf("execute failed, try again!\n");
			continue;
		}
		else
		{
			/*if(checkfunc(recv_buf, expect_result, param1, param2 ) != ERR_OK)
			{
				continue;//try again
			}
			else
			{
				break;
			}*/
			break;//success!
		}
	}
	if(cmd_retry_times <= max_retry_times )
	{
		log_printf("Execute AT:%s success!\n\n\n", cmd);
		return ERR_OK;
	}
	else
	{
		log_printf("Execute AT:%s failed!\n\n\n", cmd);
		g_at_result_last_valid_pointer_in_recv_buf = g_at_result_recv_end_pointer;//already failed, will start new cmd or resend , the old result is useless.
		return ERR_FAULT;
	}
}


//#define MAX_WAIT_EXPECT_TIMES	20
//uint16_t ATExecuteComplexTryAndWait(short comnum, char *cmd, char *recv_buf, uint16_t recv_len, CHECK_RESULT_FUNC_PTR checkfunc, char *expect_result, char *value, int * param1, int * param2)
//{
//	uint16_t result=ERR_OK;
/*	int wait_and_retry_expect_times = 0;
	while((result = ATExecuteComplex(comnum, cmd, recv_buf, recv_len, checkfunc, expect_result, 0, value, param1, param2))!= ERR_OK 
				&& wait_and_retry_expect_times++ < MAX_WAIT_EXPECT_TIMES);
	if(wait_and_retry_expect_times < MAX_WAIT_EXPECT_TIMES)	//get expect result
		return ERR_OK;
	else
		return ERR_FAULT;
}*/

uint16_t SendDataToServerFromLineBuffer(char * buf, uint16_t length)
{	
	if(!g_wifi_connected)
		return GPRSSendDataFromLineBuffer(buf, length);
	else
		return WIFISendDataFromLineBuffer(buf, length);
}









