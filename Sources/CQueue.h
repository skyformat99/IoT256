/*
 * CQueue.h
 *
 *  Created on: Nov 18, 2016
 *      Author: hflw
 */

#ifndef CQUEUE_H_
#define CQUEUE_H_

#include "include\psptypes.h"
#include "HOST.h"

#define MAX_QUEUE_LEN	(HOST_MAX_MSG)

#define ERR_MASK	0xff
#define INIT_COMPLETE 0xff00

/*************************************************************************************************************************
结构体声明
*************************************************************************************************************************/

typedef struct {
	uint_32 status;
	char * head, * tail;
	uint_16 len;
}COMM_MSG;

typedef struct {
	int state;
	int length;
	COMM_MSG *init_address,*rear, *front;
} BUFFER_QUEUE;



/*BUFFER_QUEUE_CONTROL_STRUCT buffer_queue_control =
{
.pInit = InitBufferQueue,
.pTestEmpty = TestEmptyBufferQueue,
.pAdd = AddBufferQueue,
.pOut = OutBufferQueue,
.pCheckLength = CheckBufferQueueLength,
.pCheckState = CheckBufferQueueState,
.pClear = ClearBufferQueue,
};*/

#define SetReg(reg, value) ((reg) = (value))
#define SetRegMask(reg, mask, value) ((reg) =  value & mask)

extern BUFFER_QUEUE g_host_msg_queue_head;
extern BUFFER_QUEUE *g_host_msg_queue_p;
extern BUFFER_QUEUE g_gprs_msg_queue_head;
extern BUFFER_QUEUE *g_gprs_msg_queue_p;

#endif /* CQUEUE_H_ */
