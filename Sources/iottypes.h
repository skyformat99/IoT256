/*
 * iottypes.h
 *
 *  Created on: Aug 13, 2016
 *      Author: hflw
 */

#ifndef IOTTYPES_H_
#define IOTTYPES_H_


#define true	1
#define false	0

#define AE_SETSIZE (1024*10)    /* Max number of fd supported */

#define AE_OK 0
#define AE_ERR -1

#define AE_NONE 0
#define AE_READABLE 1
#define AE_WRITABLE 2

#define AE_FILE_EVENTS 1
#define AE_TIME_EVENTS 2
#define AE_ALL_EVENTS (AE_FILE_EVENTS|AE_TIME_EVENTS)
#define AE_DONT_WAIT 4

#define AE_NOMORE -1

/* Macros */
#define AE_NOTUSED(V) ((void) V)

#endif /* IOTTYPES_H_ */
