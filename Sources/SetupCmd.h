/*
 * SetupCmd.h
 *
 *  Created on: Apr 23, 2017
 *      Author: hflw
 */

#ifndef SETUPCMD_H_
#define SETUPCMD_H_

#define FLASH_BLOCK_SIZE	1024

#define USER_DATA_OFFSET_MQTT_SERVER_ADDR	0
#define USER_DATA_CMD_MQTT_SERVER_ADDR "MQTTSERVERIP"

#define USER_DATA_OFFSET_DISABLE_WIFI	1024
#define USER_DATA_CMD_DISABLE_WIFI	"DISABLEWIFI"
#define USER_DATA_LENGTH_DISABLE_WIFI 2

#define USER_DATA_MQTT_TOPIC_ONLINE_OFFSET	2048
#define USER_DATA_MQTT_TOPIC_ONLINE_CMD	"MQTTONLINE"
#define USER_DATA_MQTT_TOPIC_ONLINE_LENGTH MQTT_TOPIC_DATA_LEN

#define USER_DATA_MQTT_TOPIC_STRING_OFFSET	3072
#define USER_DATA_MQTT_TOPIC_STRING_CMD	"MQTTSTRING"
#define USER_DATA_MQTT_TOPIC_STRING_LENGTH MQTT_TOPIC_DATA_LEN

#define USER_DATA_MQTT_TOPIC_GPS_OFFSET	4096
#define USER_DATA_MQTT_TOPIC_GPS_CMD	"MQTTGPS"
#define USER_DATA_MQTT_TOPIC_GPS_LENGTH MQTT_TOPIC_DATA_LEN

#define USER_DATA_WIFI_SSID_OFFSET	5120
#define USER_DATA_WIFI_SSID_CMD	"WIFISSID"
#define USER_DATA_WIFI_SSID_LENGTH 64

#define USER_DATA_WIFI_PWD_OFFSET	6144
#define USER_DATA_WIFI_PWD_CMD	"WIFIPWD"
#define USER_DATA_WIFI_PWD_LENGTH 64

uint16_t WriteMqttServerAddr(char *addr);
uint16_t ReadMqttServerAddr(char *addr);
uint16_t WriteDisableWifi(char *value);
uint16_t ReadDisableWifi(char *value);
uint16_t WriteMqttTopicOnline(char *value);
uint16_t ReadMqttTopicOnline(char *value);
uint16_t WriteMqttTopicString(char *value);
uint16_t ReadMqttTopicString(char *value);
uint16_t WriteMqttTopicGps(char *value);
uint16_t ReadMqttTopicGps(char *value);
uint16_t WriteWifiSSID(char *value);
uint16_t ReadWifiSSID(char *value);
uint16_t WriteWifiPWD(char *value);
uint16_t ReadWifiPWD(char *value);


#endif /* SETUPCMD_H_ */
