/* ###################################################################
**     Filename    : mqx_tasks.c
**     Project     : ProcessorExpert
**     Processor   : MKL26Z256VLH4
**     Component   : Events
**     Version     : Driver 01.00
**     Compiler    : GNU C Compiler
**     Date/Time   : 2016-07-29, 10:14, # CodeGen: 3
**     Abstract    :
**         This is user's event module.
**         Put your event handler code here.
**     Settings    :
**     Contents    :
**         Task1_task - void Task1_task(uint32_t task_init_data);
**
** ###################################################################*/
/*!
** @file mqx_tasks.c
** @version 01.00
** @brief
**         This is user's event module.
**         Put your event handler code here.
*/         
/*!
**  @addtogroup mqx_tasks_module mqx_tasks module documentation
**  @{
*/         
/* MODULE mqx_tasks */

#include "Cpu.h"
#include "Events.h"
#include "mqx_tasks.h"

#ifdef __cplusplus
extern "C" {
#endif 


/* User includes (#include below this line is not maintained by Processor Expert) */
#include "uart_comm.h"
#include "util.h"
#include "GPRS.h"
#include "ATcommand.h"
#include "HOST.h"
#include "mqttc\mqtt.h"
#include "WIFI.h"
#include "CQueue.h"

#include "HAL\bsp.h"
#include "HAL\pins.h"
#include "uart_comm.h"
#include "SetupCmd.h"

int mqtt_init_and_connect();



#define EVENT_MASK1	0x01
#define EVENT_MASK2 0x02
//LWSEM_STRUCT task1sem, task2sem;
//LWEVENT_STRUCT commEvent;
//msgq
//extern uint_32 g_host_msg_queue_p[sizeof(LWMSGQ_STRUCT)/sizeof(uint_32)+sizeof(COMM_MSG)/sizeof(uint_32)*MAX_COMM_MSG];
//extern uint_32 g_gprs_msg_queue_p[sizeof(LWMSGQ_STRUCT)/sizeof(uint_32)+sizeof(COMM_MSG)/sizeof(uint_32)*MAX_GPRS_MSG];
char at_cmd_recv_buf[MAX_CMD_RECV_BUF_SIZE];//circle buffer for result AT result
char at_cmd_result_checking_buf[MAX_CMD_RECV_BUF_SIZE];//the buffer for analyze AT result, we copy from recv buffer to make a line buffer 
char g_substring_result_check_buf[RESULT_EXPECT_LENGTH];//when we checking complex string condition, we using this buffer as temp buff

COMM_MSG HostQueueBuffer[MAX_QUEUE_LEN + 1];
COMM_MSG GprsQueueBuffer[MAX_QUEUE_LEN + 1];
BUFFER_QUEUE g_host_msg_queue_head;
BUFFER_QUEUE *g_host_msg_queue_p = &g_host_msg_queue_head;
BUFFER_QUEUE g_gprs_msg_queue_head;
BUFFER_QUEUE *g_gprs_msg_queue_p = &g_gprs_msg_queue_head;

//timer
//LWTIMER_PERIOD_STRUCT commTimerQ;
//LWTIMER_STRUCT commTimer;
int g_timer_tick=0;
#define COMM_TIMER_TICKS	100
#define TIMER_TICK_OFFSET	0

//GPRS data global variable
extern uint16_t	g_is_new_gprs_data_received;
extern uint16_t	g_new_gprs_data_length;
extern char * g_new_gprs_data_start;
extern uint16_t g_get_gprs_tcp_closed_error;
extern uint16_t g_is_gprs_error_detected;
//if we found comm msg haven't been processed for too long time, we should reset GPRS module
#define MAX_WAIT_COMM_MSG_PROCESS_RETRY_TIMES	1000
uint16_t g_wait_comm_msg_precess_retry_times=MAX_WAIT_COMM_MSG_PROCESS_RETRY_TIMES;

extern Mqtt g_mqtt;
short g_need_ping_mqtt = 0;//this is for timer, timer will trigger new ping action by this flag
#define SKIP_TICK_FOR_EACH_PING	50

extern int g_wifi_connected;
extern uint16_t	g_is_new_wifi_data_received;
extern uint16_t	g_new_wifi_data_length;
extern char * g_new_wifi_data_start;
extern g_gprs_or_wifi_mode;
extern int g_at_async_read_timeout;
extern char at_cmd_result_checking_buf[MAX_CMD_RECV_BUF_SIZE];
extern char * g_buffirstbyte;
extern char * g_buflastbyte;

extern int g_gps_connected;
extern uint16_t g_is_new_gps_data_received;
extern uint16_t	g_new_gps_data_length;
extern char * g_new_gps_data_start;

extern uart1_TDeviceDataPtr uartDebugDeviceDataPrv;
//following flags identify the network status
int g_tcp_connected=0;
int g_mqtt_connected=0;
int g_pin_retry_times=0;
#define MAX_PIN_RETRY_TIMES	5
int g_mqtt_connecting_timeout = -1;// -1: disable timeout checking.     0:timeout.    >0:not timeout 
int g_mqtt_ping_counter = 0;
//>>>>end of net state

#ifdef FAKE_GPS_DATA
extern int g_gps_fake_timeout;
#endif

extern uart0_TDeviceDataPtr uartGPSAndWIFIDeviceDataPrv;

byte commTimerFunc(void * param)
{
	uint16_t * pcounter = param;
	byte result = ERR_OK;

	//flash status led
	led_status_on((*pcounter) % 2);
	*pcounter = *pcounter + 1;
	
	if(g_at_async_read_timeout) 
		g_at_async_read_timeout--;
#ifdef FAKE_GPS_DATA
	if(g_gps_fake_timeout)
		g_gps_fake_timeout --;
#endif
	
	if(g_mqtt_connecting_timeout >0)
		g_mqtt_connecting_timeout --;

	if(g_mqtt.state == MQTT_STATE_CONNECTED)
	{
		g_mqtt_ping_counter ++;
		if(g_mqtt_ping_counter % SKIP_TICK_FOR_EACH_PING == 0)
		{
			g_need_ping_mqtt = 1;
			g_mqtt_ping_counter = 0;
		}
	}

	return result;
}

/* this is connection maintain functions
 * including mqtt connection and tcp connection
 */
extern char at_cmd_recv_buf[MAX_CMD_RECV_BUF_SIZE];
extern char gprs_cmd_proc_buf[MAX_CMD_RECV_BUF_SIZE];
#define MAX_WIFI_INIT_RETRY_TIMES	5
#define MAX_GPRS_INIT_RETRY_TIMES	5
extern char g_wifi_disable[USER_DATA_LENGTH_DISABLE_WIFI];
extern char g_wifi_ssid[USER_DATA_WIFI_SSID_LENGTH];
extern char g_wifi_pwd[USER_DATA_WIFI_PWD_LENGTH];
void TryToConnectGPRSorWIFI()
{
	uint16_t result = ERR_NOTAVAIL;
	int wifi_init_retry = 0;
	int gprs_init_retry = 0;
	led_task1_on(0);
	led_task2_on(0);
	while(result != ERR_OK)
	{
		if(strcmp(g_wifi_disable,"1") != 0)
		{
			wifi_init_retry =0;
			gps_pow_on(0);
			wifi_enable_on(1);
			gps_wifi_uart_select(SEL_WIFI);
			uart0_SelectBaudRate(uartGPSAndWIFIDeviceDataPrv, uart0_BM_115200BAUD);
			while(result != ERR_OK && wifi_init_retry++ < MAX_WIFI_INIT_RETRY_TIMES)
				result = WIFIInit();
		}
		//if(result != ERR_OK)
		else	//we only can access wifi or gprs,
		{
			wifi_enable_on(0);
			gprs_init_retry=0;
			while(result != ERR_OK && gprs_init_retry++ < MAX_GPRS_INIT_RETRY_TIMES)
				result = GPRSInit();
		}
	}
	g_tcp_connected=1;
	g_mqtt_connected=0;
	g_wait_comm_msg_precess_retry_times = MAX_WAIT_COMM_MSG_PROCESS_RETRY_TIMES;
	//clear queue
	ClearBufferQueue(g_host_msg_queue_p);

	return;
}

uint16_t TryToPingMqttServer()
{
	uint16_t result = ERR_OK;

	//we use ping to check network status
	//and we ignore several no ping response
	//this is because the network of GPRS is very poor
	//until ping failed reach maxim times we regard it as network disconnect
	if(g_need_ping_mqtt && g_mqtt_connected && g_tcp_connected)
	{
		if(g_pin_retry_times > MAX_PIN_RETRY_TIMES)
		{
			g_mqtt_connected = 0;//we should not be here due to we will reset g_pin_retry_times when we get ping ack
			g_pin_retry_times = 0;//reset counter
			g_tcp_connected = 0;
			log_printf("+++++++++++++++too long time no response from server. No pingack back. set tcp and mqtt disconnect!");
			result = ERR_NOTAVAIL;
		}
		else
		{
			mqtt_ping(&g_mqtt);//only send mqtt ping, in the onack of connect will set flag of mqtt connected
			//mark retry
			g_pin_retry_times ++;
			g_need_ping_mqtt = 0;
			log_printf("sent ping. tconn %d mconn %d retry %d\n", g_tcp_connected, g_mqtt_connected, g_pin_retry_times);
			result = ERR_OK;
		}
	}
	return result;
}

void CheckMqttStateAndTryToConnect()
{
	if(!g_tcp_connected)
		return;
	if(!g_mqtt_connected)
	{
		if(g_mqtt.state == MQTT_STATE_CONNECTED )
		{
			//this means our device state machine isn't same as server.
			//we need disconnect before reconnect
			mqtt_disconnect(&g_mqtt);
		}
	}
	if(g_mqtt.state == MQTT_STATE_INIT || g_mqtt.state == MQTT_STATE_DISCONNECTED )
		mqtt_init_and_connect();
	else if(g_mqtt.state == MQTT_STATE_CONNECTING )
	{
		//timeout checking
		if(!g_mqtt_connecting_timeout)//timeout
		{
			g_mqtt_connecting_timeout = -1;
			g_mqtt_connected =0;
			if(g_tcp_connected && !g_wifi_connected)
				GPRSDisconnectToServer(MQTT_SERVER_SOCKET_ID, 1);
			g_tcp_connected = 0;
			log_printf("+++++++++++++++++++++connecting mqtt timeout. reset module!....\n");
			TryToConnectGPRSorWIFI();
			mqtt_init_and_connect();
		}
	}
	
	led_net_on(g_mqtt_connected && g_tcp_connected);
	
}

//we should handle GPRS or WIFI error message here
uint16_t TcpErrorHandle()
{
	uint16_t result = ERR_OK;
	if(!g_wifi_connected)
	{
		//check if tcp connection is closed
		if(g_get_gprs_tcp_closed_error)
		{
			log_printf("get TCP closed error!\n %s\n",at_cmd_result_checking_buf);
			g_get_gprs_tcp_closed_error = 0;
			g_tcp_connected = 0;
			result = ERR_NOTAVAIL;
		}
		//check if unkown error occurred
		if(g_is_gprs_error_detected)
		{
		  log_printf("new unkown error detected!\n recv %s\n", at_cmd_result_checking_buf);
		  g_is_gprs_error_detected = 0;
		  g_tcp_connected = 0;//ignore unknown error temporary.
		  //_delay_sometime(100);
		  if(GPRSSoftReset()== ERR_OK)
			  g_tcp_connected = 1;
		  result = ERR_NOTAVAIL;
		}
	}
	else
	{
		
	}
	return result;
}

//when we get new data, we need to push them into queue in task1 and check them in task2
uint16_t CreateMsgInQueue()
{
	uint16_t result = ERR_OK;

	if(g_is_new_gprs_data_received)
	  result = _send_msg_to_queue(g_gprs_msg_queue_p, NEW_GPRS_DATA, g_new_gprs_data_start, NULL, g_new_gprs_data_length);
	else if(g_is_new_wifi_data_received)
	  result = _send_msg_to_queue(g_gprs_msg_queue_p, NEW_GPRS_DATA, g_new_wifi_data_start, NULL, g_new_wifi_data_length);
	else
	  result = _send_msg_to_queue(g_host_msg_queue_p, NEW_GPS_DATA, g_new_gps_data_start,
			  g_new_gps_data_start+g_new_gps_data_length-1, g_new_gps_data_length);

	if(result == ERR_OK)
	{
	  if(g_is_new_gprs_data_received)
		  g_is_new_gprs_data_received =0;//clear flag
	  else if(g_is_new_wifi_data_received)
		  g_is_new_wifi_data_received = 0;
	  else
	  {
		  g_is_new_gps_data_received = 0;
#ifdef FAKE_GPS_DATA
		  g_gps_fake_timeout = MAX_FACK_GPS_TIMEOUT;
#endif
	  }
	}
	else//send msg failed
	{
	  if(g_is_new_gprs_data_received)
		  log_printf("create message failed. we can't trigger task2 for GPRS recv data anaylze and sent.\n");
	  else if(g_is_new_wifi_data_received)
		  log_printf("create message failed. we can't trigger task2 for WIFI recv data anaylze and sent.\n");
	  else if(g_is_new_gps_data_received)
		  log_printf("create message failed. we can't trigger task2 for GPS recv data anaylze and sent.\n");
	}
	return result;
}

uint16_t InitHostMessageQueue()
{
	uint16_t result = ERR_OK;
	result = InitBufferQueue(g_host_msg_queue_p, HostQueueBuffer, MAX_COMM_MSG);//_lwmsgq_init((pointer)g_host_msg_queue_p, MAX_COMM_MSG, sizeof(COMM_MSG)/sizeof(uint_32));
	if(result != ERR_OK)
		log_printf("create common message queue failed.\n");
	else
		log_printf("create common message queue success.\n");
	return result;
}

uint16_t InitCommunicationMessageQueue()
{
	uint16_t result = ERR_OK;
	result = InitBufferQueue(g_gprs_msg_queue_p, GprsQueueBuffer, MAX_COMM_MSG);//_lwmsgq_init((pointer)g_gprs_msg_queue_p, MAX_GPRS_MSG, sizeof(COMM_MSG)/sizeof(uint_32));
	if(result != ERR_OK)
		log_printf("create gprs message queue failed.\n");
	else
		log_printf("create gprs message queue success.\n");
	return result;
}

uint16_t IoTModuleBasicInit()
{
	uint16_t result = ERR_OK;
	//
	//part 0: uart init
	//
	//uart1 ---> HOST and debug
	//uart0----> GPS and wifi
	//uart2----> GSM/GPRS
	if((initDebugUartPort()) == NULL)
	{
		//log_printf("HOST uart init failed!\n");//can't output log due to port init failed
		result = ERR_NOTAVAIL;
		while(1)//log port failed we blink the LED to show status
		{
			led_task1_flash();
		}
		//HOST is log port, if we failed, nothing for next step.
	}
	else
		log_printf("HOST uart init success! Ver. %s\n", VER_NUM);
	
	if(result == ERR_OK && (initGPSAndWIFIUartPort()) == NULL)
	{
		log_printf("GPS uart init failed!\n");
		result =ERR_NOTAVAIL;
	}
	else
		log_printf("GPS uart init success!\n");

	if(result == ERR_OK && (initGSMUartPort()) == NULL)
	{
		log_printf("GPRS uart init failed!\n");
		result =ERR_NOTAVAIL;
	}
	else
		log_printf("GPRS uart init success!\n");

	//part1: message queue
	result = InitHostMessageQueue();
	if(result == ERR_OK)
		result = InitCommunicationMessageQueue();
		
	//part 2: init inner flash
	if(result == ERR_OK)
	{
		result = InitInnerFlash();
		if(result == ERR_OK)
			log_printf("Init inner flash success!\n");
		else
			log_printf("Init inner flash failed!\n");
	}

	return result;
}

#define MAX_START_SET_RETRY	20
#define MAX_SETTING_BUF_SIZE	1024
uint16_t ReadSetupString(char *settingbuf)
{
	char *chsetp=settingbuf;
	int settingrecvlen;
	uint16_t result=ERR_FAULT;
	uint16_t comResult=ERR_OK;

	//uart_read_sync(HOST_COM_PORT,settingbuf,MAX_SETTING_BUF_SIZE,&settingrecvlen);
	//_delay_sometime(100);
	comResult = uart_read_async(HOST_COM_PORT, settingbuf, MAX_SETTING_BUF_SIZE);
	_delay_sometime(100);//wait a while to get data from uart
	if(comResult == ERR_OK || comResult == ERR_BUSY)
	{
		while(uartDebugDeviceDataPrv->InpDataNumReq 
				&& uartDebugDeviceDataPrv->InpRecvDataNum > 0
				&& uartDebugDeviceDataPrv->InpRecvDataNum < MAX_SETTING_BUF_SIZE -1)
		{
			chsetp=settingbuf;
			while(*chsetp!='\n' && *chsetp!='\r' && chsetp - settingbuf < MAX_SETTING_BUF_SIZE -1)
				chsetp++;
			if(*chsetp=='\n' || *chsetp=='\r')
				break;
			_delay_sometime(1);
		}
		if(uartDebugDeviceDataPrv->InpRecvDataNum > 0 && (*chsetp=='\n' || *chsetp=='\r'))
		{
			settingrecvlen = chsetp - settingbuf;
			if(settingrecvlen < MAX_SETTING_BUF_SIZE)
				settingbuf[settingrecvlen] = '\0';
			else
				settingbuf[MAX_SETTING_BUF_SIZE]='\0';
			log_printf("Get string: %s\n",settingbuf);
			result=ERR_OK;
		}
		uart1_CancelBlockReception(uartDebugDeviceDataPrv);
		_delay_sometime(1);
	}
	
	return result;
}

/*
** ===================================================================
**     Event       :  Task1_task (module mqx_tasks)
**
**     Component   :  Task1 [MQXLite_task]
**     Description :
**         MQX task routine. The routine is generated into mqx_tasks.c
**         file.
**     Parameters  :
**         NAME            - DESCRIPTION
**         task_init_data  - 
**     Returns     : Nothing
** ===================================================================
*/

#define START_SETTING_KEY	"StartSetE"
int g_start_setting_mode=0;
void TaskInit()
{
	uint16_t result=ERR_OK;
	char settingbuf[MAX_SETTING_BUF_SIZE + 1];
	int startsetretry = MAX_START_SET_RETRY;
	  
	result = IoTModuleBasicInit();
	if(result != ERR_OK)
	{
		log_printf("IoTModule basic init failed! Dead lock!\n");
		while(1);
	}
	else
	{
		log_printf("IoTModule basic init success!\n");
		//wait sometime if user want to set param
		while(startsetretry --)
		{
			if(ERR_OK==ReadSetupString(settingbuf))
			{
				if(strstr(settingbuf,START_SETTING_KEY))
				{
					g_start_setting_mode=1;
					log_printf("IoTModule enter setup mode...\n");
					break;
				}
			}
		}
		//go into setup mode
		while(g_start_setting_mode)
		{
			if(ERR_OK==ReadSetupString(settingbuf))
			{
				log_printf("Get command success!cmd:%s\n",settingbuf);
				ProcessSetupCmd(settingbuf);
			}
		}
		
		//read config from flash
		if(ReadMqttServerAddr(g_mqtt_server_ip) != ERR_OK)
			strcpy(g_mqtt_server_ip,MQTT_SERVER_ADDR);
		else if(strlen(g_mqtt_server_ip) <=0)
			strcpy(g_mqtt_server_ip,MQTT_SERVER_ADDR);
		if(ReadDisableWifi(g_wifi_disable) != ERR_OK)
			strcpy(g_wifi_disable,"1");
		else if(strlen(g_mqtt_server_ip) <=0)
			strcpy(g_wifi_disable,"1");
		if(ReadMqttTopicOnline(g_mqtt_topic_online) != ERR_OK)
			strcpy(g_mqtt_topic_online, MQTT_TOPIC_ONLINE);
		else if(strlen(g_mqtt_topic_online)<=0)
			strcpy(g_mqtt_topic_online, MQTT_TOPIC_ONLINE);
		if(ReadMqttTopicString(g_mqtt_topic_string) != ERR_OK)
			strcpy(g_mqtt_topic_string, MQTT_TOPIC_STRING);
		else if(strlen(g_mqtt_topic_string)<=0)
			strcpy(g_mqtt_topic_string, MQTT_TOPIC_STRING);
		if(ReadMqttTopicGps(g_mqtt_topic_gps) != ERR_OK)
			strcpy(g_mqtt_topic_gps, MQTT_TOPIC_GPS);
		else if(strlen(g_mqtt_topic_gps)<=0)
			strcpy(g_mqtt_topic_gps, MQTT_TOPIC_GPS);
		if(ReadWifiSSID(g_wifi_ssid) != ERR_OK)
			strcpy(g_wifi_ssid, WIFI_DEFAULT_SSID);
		else if(strlen(g_wifi_ssid)<=0)
			strcpy(g_wifi_ssid, WIFI_DEFAULT_SSID);
		if(ReadWifiPWD(g_wifi_pwd) != ERR_OK)
			strcpy(g_wifi_pwd, WIFI_DEFAULT_PWD);
		else if(strlen(g_wifi_pwd)<=0)
			strcpy(g_wifi_pwd, WIFI_DEFAULT_PWD);
		
		//go into normal setup process
		log_printf("GSM module power on!\n");
		gsm_pow_on(1);
		log_printf("Init communication module......");
		TryToConnectGPRSorWIFI();

		if(!g_wifi_connected)//GPS only work with GPRS
		{
			gps_pow_on(1);
			wifi_enable_on(0);
			uart0_SelectBaudRate(uartGPSAndWIFIDeviceDataPrv, uart0_BM_9600BAUD);
			gps_wifi_uart_select(SEL_GPS);
			result = GPSInit();
		}

		if((result = HOSTStartRecv_internal(0)) != ERR_OK)
		{
			log_printf("start read HOST failed! result:%x\nGoing to dead lock.\n", result);
			while(1);
		}
		else
			log_printf("start read data from HOST......\n");

		mqtt_init_and_connect();
	}
}

void Task1_task(uint32_t task_init_data)
{
	  uint16_t result=ERR_OK;

	  log_printf("Enter task 1 loop.....\n");
	  while(1)
	  {
		  TryToPingMqttServer();
		  //reconnect GPRS according to tcp status
		  //the module is useless if tcp isn't connected
		  if(!g_tcp_connected)
		  {
			  log_printf("+++++++++++++++TCP is disconnected for some reason! reset communication module!\n");
			  TryToConnectGPRSorWIFI();
		  }

		  CheckMqttStateAndTryToConnect();
		  
		  //Check recved data and analyze them, we may get error message or new data from wifi/gprs
		  if(!g_wifi_connected)
			  GPRSCheckRecvResult_internal();//checking result buffer
		  else 
			  WIFICheckRecvResult_internal_async();//checking wifi result buffer
		  result = TcpErrorHandle();
		  if(result != ERR_OK)//we get some error
			  continue;

		  //check GPS recved data
		  if(g_gps_connected)
			  GPSCheckRecvResult_internal_async();
		  
		  
		  //check if we receive new data from GPRS/wifi/GPS, normally this data is come from server
		  if( g_is_new_gprs_data_received|| g_is_new_wifi_data_received ||g_is_new_gps_data_received)
		  {
			  led_task1_on(1);
			  //we should create a message here to trigger task2 to analyze
			  if(!TestFullBufferQueue(g_gprs_msg_queue_p))
			  {
				  result=CreateMsgInQueue();
				  led_task1_on(0);
				  if(result == ERR_OK)
					  return;//release task1 and goto task2
				  else
					  log_printf("push into msg queue failed. we may lost data!\n");
			  }
			  else
				  log_printf("GPRS msg Q is full. we may lost data!\n");
			  led_task1_on(0);
		  }

		  led_task1_on(1);
		  HOSTCheckRecvNewMsg_internal();//try to check host buffer, if we have valid msg from HOST, we will get msg in g_host_msg_queue_p
		  led_task1_on(0);
		  if(!TestEmptyBufferQueue(g_host_msg_queue_p))
		  {
			  //we will not handle message queue of host data here
			  //host message queue will be handle in task2
			  //but we try to check some issue here before we handle them
			  led_task1_on(1);
			  //we have data in msg queue, we check if host queue if full and alert
			  static int hostqsize = 0;
			  if(hostqsize != CheckBufferQueueLength(g_host_msg_queue_p))
			  {
				  hostqsize = CheckBufferQueueLength(g_host_msg_queue_p);
				  log_printf("----queue size:%d\n", hostqsize);
				  if(hostqsize > MAX_QUEUE_LEN)
					  ClearBufferQueue(g_host_msg_queue_p);//normally we should be here until we have some issue
			  }
			  //reset GPRS module if we have retry too many times
			  if(g_wait_comm_msg_precess_retry_times == 0 && g_tcp_connected && g_mqtt_connected)
			  {
				  log_printf("too long time to process data in queue! reset GPRS or WiFi module!\n");
				  TryToConnectGPRSorWIFI();//this is not necessary, but we need make sure communication is workable.
				  if(g_tcp_connected)
				  {
					  mqtt_init_and_connect();
					  g_wait_comm_msg_precess_retry_times = MAX_WAIT_COMM_MSG_PROCESS_RETRY_TIMES;
					  //HACK: we lost some data for more stable when connect again
					  ClearBufferQueue(g_host_msg_queue_p);
				  }
			  }
			  else
				  g_wait_comm_msg_precess_retry_times--;
			  led_task1_on(0);
			  return;//release task1 and goto task2
		  }
	  }//end of while(1)
}

/*
** ===================================================================
**     Event       :  Task2_task (module mqx_tasks)
**
**     Component   :  Task2 [MQXLite_task]
**     Description :
**         MQX task routine. The routine is generated into mqx_tasks.c
**         file.
**     Parameters  :
**         NAME            - DESCRIPTION
**         task_init_data  - 
**     Returns     : Nothing
** ===================================================================
*/
void Task2_task(uint32_t task_init_data)
{
	  uint16_t result;
	  COMM_MSG msg;
	  
	  //this task charge of send data relate task
	  log_printf("Enter task 2!\n");
	  while(1)
	  {
		  led_task2_on(1);
		  
		  //analyze GPRS/wifi async msg and return result
		  if(!TestEmptyBufferQueue(g_gprs_msg_queue_p))
		  {
			  result = OutBufferQueue(g_gprs_msg_queue_p, &msg);
			  if(result != ERR_OK)
			  {
					led_task2_on(0);
					log_printf("It is strange that we pick out message from gprs wifi queue failure. reset queue.\n");
					result = InitCommunicationMessageQueue();
					if(result != ERR_OK)
						TaskInit();//it will dead lock here if failed
					return;//release task2 goto task1
			  }
			  else//pick out msg success
			  {
				  if(msg.status == NEW_GPRS_DATA)
				  {
					  if(!g_wifi_connected)
						  GPRSAnalyzeRecvData(msg.head, msg.len);
					  else
						  WIFIAnalyzeRecvData(msg.head, msg.len);
				  }
				  else
					  log_printf("Unkown msg type:%d\n",msg.status);
			  }
		  }

		  if(!TestEmptyBufferQueue(g_host_msg_queue_p))
		  {
			  if(g_tcp_connected && g_mqtt_connected)
			  {
				  result = OutBufferQueue(g_host_msg_queue_p, &msg);
				  if(result != ERR_OK)
				  {
						led_task2_on(0);
						log_printf("It is strange that we pick out message from host msg queue failure. reset queue.\n");
						result = InitHostMessageQueue();
						if(result != ERR_OK)
							TaskInit();//it will dead lock here if failed
						return;//release task2 goto task1
				  }
				  else//pick out host msg from queue
				  {
					  led_task2_on(1);
					  log_printf("get msg from comm msg queue!status:%d head:%x tail:%x len:%d\n",
							  msg.status, msg.head, msg.tail, msg.len);
					  if(msg.status == NEW_HOST_DATA_READ_TO_SEND)
					  {
						  //move data to line buffer
						  if(msg.tail < msg.head)
						  {
							  int taillen = g_buflastbyte - msg.head + 1;
							  char * tmphead = g_buflastbyte + 1;
							  memcpy(tmphead, msg.head, taillen);
							  memcpy(tmphead + taillen, g_buffirstbyte, msg.len - taillen);
							  client_publish_msg(g_mqtt_topic_string, msg.len, tmphead);
						  }
						  else
							  client_publish_msg(g_mqtt_topic_string, msg.len, msg.head);
						  g_wait_comm_msg_precess_retry_times = MAX_WAIT_COMM_MSG_PROCESS_RETRY_TIMES;//reset counter of comm msg process
						  g_timer_tick = 0;//just publish msg, don't send ping, reset timer counter
					  }
					  else if(msg.status == NEW_GPS_DATA)//GPS is another input source, it share the host msg queue now
					  {
						  //move data to line buffer
						  if(msg.tail < msg.head)
						  {
							  int taillen = g_buflastbyte - msg.head + 1;
							  char * tmphead = g_buflastbyte + 1;
							  memcpy(tmphead, msg.head, taillen);
							  memcpy(tmphead + taillen, g_buffirstbyte, msg.len - taillen);
							  client_publish_msg(g_mqtt_topic_gps, msg.len, tmphead);
						  }
						  else
							  client_publish_msg(g_mqtt_topic_gps, msg.len, msg.head);
						  g_wait_comm_msg_precess_retry_times = MAX_WAIT_COMM_MSG_PROCESS_RETRY_TIMES;//reset counter of comm msg process
						  g_timer_tick = 0;//just publish msg, don't send ping, reset timer counter
					  }
				  }
			  }
			  //else
				  //log_printf("We got host or gps msg in queue, but tcp or mqtt is not connected!\n");
		  }

		  led_task2_on(0);
		  return;
	  }
}

/* END mqx_tasks */

#ifdef __cplusplus
}  /* extern "C" */
#endif 

/*!
** @}
*/
/*
** ###################################################################
**
**     This file was created by Processor Expert 10.2 [05.06]
**     for the Freescale Kinetis series of microcontrollers.
**
** ###################################################################
*/
