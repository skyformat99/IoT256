/*
 * FlashDrv.h
 *
 *  Created on: Apr 22, 2017
 *      Author: hflw
 */

#ifndef FLASHDRV_H_
#define FLASHDRV_H_

#include "PE_Types.h"
#include "PE_Error.h"
#include "PE_Const.h"

#define USER_DATA_START_ADDR	0x00020000LU


uint16_t InitInnerFlash();
uint16_t EraseFlashHAL(int offset, int size);
uint16_t WriteToFlashHAL(int offset, char *data, int size);
uint16_t ReadFromFlashHAL(int offset, char *data, int size);


#endif /* FLASHDRV_H_ */
