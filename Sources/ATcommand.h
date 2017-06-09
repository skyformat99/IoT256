/*
 * ATcommand.h
 *
 *  Created on: Aug 10, 2016
 *      Author: hflw
 */

#ifndef ATCOMMAND_H_
#define ATCOMMAND_H_

#define ENALBE_GPRS_TRANSPORT_MODE

typedef uint16_t (* CHECK_RESULT_FUNC_PTR)(char *result, char *expect, char * value, int * param1, int *param2);

#define AT_CMD_HEAD_FAST_SEND	"AT+MIPSTRS"
#define MQTT_SERVER_SOCKET_ID	"1"
#define HTTP_SERVER_SOCKET_ID	"0"
#define SEND_MODE_ASCII			"0"
#define SEMD_MODE_HEX			"1"
#define AT_CMD_HEAD_NORMAL_SEND	"AT+MIPSEND"
#define CTRLZ					0x1A
#define AT_CMD_HEAD_TRANSPORT_SEND	"AT+MIPTRSP"
#define AT_CMD_TAIL_TRANSPORT_SEND	"+++"

enum gprs_send_type
{
	GPRS_SEND_NORMAL,
	GPRS_SEND_FAST,
	GPRS_SEND_TRANSPORT,
};

enum gprs_or_wifi_mode_type
{
	GPRS_MODE,
	WIFI_MODE,
};

#define MAX_EXPECT_RESULT_DESCRIPTION_STRING_LENGTH	128
struct at_expected_result_and_condition
{
	CHECK_RESULT_FUNC_PTR checkfunc;
	char expect_result[MAX_EXPECT_RESULT_DESCRIPTION_STRING_LENGTH];
	char value[MAX_EXPECT_RESULT_DESCRIPTION_STRING_LENGTH];//key value of string type in expected result
	int * param1;//other condition for checkfunc param1
	int * param2;//other condition for checkfunc param2
	uint16_t analyze_result;//this is the result after checking above condition
};

#define MAX_CMD_HEX_BUF_SIZE	(HOST_MSG_SIZE * 2)
#define MAX_CMD_RECV_BUF_SIZE	(HOST_MSG_SIZE * 3)
#define RESULT_EXPECT_LENGTH	(HOST_MSG_SIZE)

#define MIN_RESULT_STRING_SIZE			10

//how long time we wait for a AT response
#define MAX_AT_ASYNC_READ_TICKS	10

uint16_t CheckEqualStr(char *result, char *expect, char *value, int * param1, int *param2);

uint16_t ATExecuteCmd_internal(short comnum, char *cmd, char *recv, uint16_t * recv_len);
uint16_t ATExecuteOnce(short comnum, char *cmd, char * recv_buf, uint16_t recv_len, char *expect_result);
uint16_t ATExecuteComplex(short comnum, char *cmd, char *recv_buf, uint16_t recv_len, CHECK_RESULT_FUNC_PTR checkfunc, char *expect_result, int runtimes, char * value, int * param1, int * param2);
uint16_t ATExecuteComplexTryAndWait(short comnum, char *cmd, char *recv_buf, uint16_t recv_len, CHECK_RESULT_FUNC_PTR checkfunc, char *expect_result, char *value, int * param1, int * param2);

uint16_t SendDataToServerFromLineBuffer(char * buf, uint16_t length);
#endif /* ATCOMMAND_H_ */
