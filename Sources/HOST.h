/*
 * HOST.h
 *
 *  Created on: Aug 23, 2016
 *      Author: hflw
 */

#ifndef HOST_H_
#define HOST_H_

#define HOST_MAX_MSG	9
#define HOST_MSG_SIZE	800
#define COMM_BUF_SIZE	HOST_MSG_SIZE * HOST_MAX_MSG

#define HEAD_ID	0xa5
#define TAIL_ID	0x5a

uint16_t HOSTStartRecv_internal(int isWrapped);

#endif /* HOST_H_ */
