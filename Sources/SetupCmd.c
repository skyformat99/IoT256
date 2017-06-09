/*
 * SetupCmd.c
 *
 *  Created on: Apr 23, 2017
 *      Author: hflw
 */
#include "FlashDrv.h"
#include "SetupCmd.h"
#include "mqttc\mqtt.h"

#define SPLIT_CMD_CHAR	'='
uint16_t SplitCmd(char *str, char ** cmd, char ** data)
{
	uint16_t result = ERR_OK;
	char * ch = str;
	while(*ch != SPLIT_CMD_CHAR && *ch)
		ch ++;
	if(*ch == SPLIT_CMD_CHAR)
	{
		*cmd=str;
		*ch ='\0';
		*data= ch + 1;
		result = ERR_OK;
	}
	else
	{
		result = ERR_VALUE;
	}
	return result;
}

uint16_t ProcessSetupCmd(char * str)
{
	uint16_t result = ERR_OK;
	char * cmd, * data;
	
	if(!str)
	{
		log_printf("In ProcessSetupCmd: cmd is null!\n");
		result = ERR_VALUE;
	}
	result = SplitCmd(str, &cmd, &data);
	if(result!=ERR_OK)
		log_printf("Split cmd failed!\n");
	else
	{
		log_printf("Start process command:%s data:%s\n",cmd, data);
		if(!strcmp(cmd, USER_DATA_CMD_MQTT_SERVER_ADDR))
		{
			if(strlen(data) > IP_ADDR_SIZE)
				log_printf("data is too long.\n");
			else
			{
				WriteMqttServerAddr(data);
				//strcpy(data, "FAKEDATA");
				//_delay_sometime(1000);
				ReadMqttServerAddr(data);
			}
		}
		else if(!strcmp(cmd, USER_DATA_CMD_DISABLE_WIFI))
		{
			WriteDisableWifi(data);
			ReadDisableWifi(data);
		}
		else if(!strcmp(cmd, USER_DATA_MQTT_TOPIC_ONLINE_CMD))
		{
			WriteMqttTopicOnline(data);
			ReadMqttTopicOnline(data);
		}
		else if(!strcmp(cmd, USER_DATA_MQTT_TOPIC_STRING_CMD))
		{
			WriteMqttTopicString(data);
			ReadMqttTopicString(data);
		}
		else if(!strcmp(cmd, USER_DATA_MQTT_TOPIC_GPS_CMD))
		{
			WriteMqttTopicGps(data);
			ReadMqttTopicGps(data);
		}
		else if(!strcmp(cmd, USER_DATA_WIFI_SSID_CMD))
		{
			WriteWifiSSID(data);
			ReadWifiSSID(data);
		}
		else if(!strcmp(cmd, USER_DATA_WIFI_PWD_CMD))
		{
			WriteWifiPWD(data);
			ReadWifiPWD(data);
		}
		if(result==ERR_OK)
			log_printf("Process command success:%s\n",str);
		else
			log_printf("Process command failed:%s\n",str);
	}
	return result;
}

uint16_t WriteStringToFlash(char *feature_name, int offset, char * data, int size)
{
	uint16_t result = ERR_OK;
	result = EraseFlashHAL(offset, FLASH_BLOCK_SIZE);
	if(result == ERR_OK)
	{
		result = WriteToFlashHAL(offset, data, size);
		if(result == ERR_OK)
			log_printf("Write %s success: %s\n",feature_name, data);
		else
			log_printf("Write %s failed: %s\n",feature_name, data);
	}
	else
		log_printf("Write %s failed: %s\n",feature_name, data);

	return result;
}

uint16_t ReadStringFromFlash(char *feature_name, int offset, char *addr, int size)
{
	uint16_t result = ERR_OK;
	result = ReadFromFlashHAL(offset, addr, size);
	if(result == ERR_OK)
		log_printf("Read % success: %s\n",feature_name, addr);
	else
		log_printf("Read %s failed: %s\n",feature_name, addr);
	return result;
}


uint16_t WriteMqttServerAddr(char *addr)
{
	return WriteStringToFlash("MQTT Server addr", USER_DATA_OFFSET_MQTT_SERVER_ADDR, addr, IP_ADDR_SIZE);
}

uint16_t ReadMqttServerAddr(char *addr)
{
	return ReadStringFromFlash("MQTT Server addr", USER_DATA_OFFSET_MQTT_SERVER_ADDR, addr, IP_ADDR_SIZE);
}

uint16_t WriteDisableWifi(char *value)
{
	return WriteStringToFlash("disable WIFI", USER_DATA_OFFSET_DISABLE_WIFI, value, USER_DATA_LENGTH_DISABLE_WIFI);
}

uint16_t ReadDisableWifi(char *value)
{
	return ReadStringFromFlash("disable WIFI", USER_DATA_OFFSET_DISABLE_WIFI, value, USER_DATA_LENGTH_DISABLE_WIFI);
}

uint16_t WriteMqttTopicOnline(char *value)
{
	return WriteStringToFlash("mqtt topic online", USER_DATA_MQTT_TOPIC_ONLINE_OFFSET, value, USER_DATA_MQTT_TOPIC_ONLINE_LENGTH);
}

uint16_t ReadMqttTopicOnline(char *value)
{
	return ReadStringFromFlash("mqtt topic online", USER_DATA_MQTT_TOPIC_ONLINE_OFFSET, value, USER_DATA_MQTT_TOPIC_ONLINE_LENGTH);
}

uint16_t WriteMqttTopicString(char *value)
{
	return WriteStringToFlash("mqtt topic string", USER_DATA_MQTT_TOPIC_STRING_OFFSET, value, USER_DATA_MQTT_TOPIC_STRING_LENGTH);
}

uint16_t ReadMqttTopicString(char *value)
{
	return ReadStringFromFlash("mqtt topic string", USER_DATA_MQTT_TOPIC_STRING_OFFSET, value, USER_DATA_MQTT_TOPIC_STRING_LENGTH);
}

uint16_t WriteMqttTopicGps(char *value)
{
	return WriteStringToFlash("mqtt topic gps", USER_DATA_MQTT_TOPIC_GPS_OFFSET, value, USER_DATA_MQTT_TOPIC_GPS_LENGTH);
}

uint16_t ReadMqttTopicGps(char *value)
{
	return ReadStringFromFlash("mqtt topic gps", USER_DATA_MQTT_TOPIC_GPS_OFFSET, value, USER_DATA_MQTT_TOPIC_GPS_LENGTH);
}


uint16_t WriteWifiSSID(char *value)
{
	return WriteStringToFlash("WiFi SSID", USER_DATA_WIFI_SSID_OFFSET, value, USER_DATA_WIFI_SSID_LENGTH);
}

uint16_t ReadWifiSSID(char *value)
{
	return ReadStringFromFlash("WiFi SSID", USER_DATA_WIFI_SSID_OFFSET, value, USER_DATA_WIFI_SSID_LENGTH);
}

uint16_t WriteWifiPWD(char *value)
{
	return WriteStringToFlash("WiFi PWD", USER_DATA_WIFI_PWD_OFFSET, value, USER_DATA_WIFI_PWD_LENGTH);
}

uint16_t ReadWifiPWD(char *value)
{
	return ReadStringFromFlash("WiFi PWD", USER_DATA_WIFI_PWD_OFFSET, value, USER_DATA_WIFI_PWD_LENGTH);
}


