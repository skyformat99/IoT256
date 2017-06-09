/*
 * FlashDrv.c
 *
 *  Created on: Apr 22, 2017
 *      Author: hflw
 */

#include "FlashDrv.h"
#include "InnerFlash.h"

LDD_TDeviceData * InnerFlashDev =NULL;
int g_flash_complete = 0;
uint16_t InitInnerFlash()
{
	uint16_t result = ERR_OK;
	InnerFlashDev = InnerFlash_Init(NULL);
	if(InnerFlashDev)
		result = ERR_OK;
	else
		result = ERR_FAULT;
	return result;
}

uint16_t EraseFlashHAL(int offset, int size)
{
	uint16_t result = ERR_OK;
	
	log_printf("Try to erase flash offset:%x size:%d\n", offset, size);
	if(0 ==size)
	{
		result = ERR_VALUE;
		log_printf("Erase failed! invalid data!\n");
	}
	else
	{
		g_flash_complete = 0;
		log_printf("start erase offset:%x size %d\n", offset, size);
		result = InnerFlash_Erase(InnerFlashDev, USER_DATA_START_ADDR + offset, size);
		if(result != ERR_OK)
			log_printf("Erase failed!code:%d\n", result);
		else
		{
			while(!g_flash_complete)
				InnerFlash_Main(InnerFlashDev);
		}
		log_printf("Erase success!\n");
	}
	
	return result;
}

uint16_t WriteToFlashHAL(int offset, char *data, int size)
{
	uint16_t result = ERR_OK;
	log_printf("Try to write flash offset:%x size:%d data:%s\n", offset, size,data);
	if(0 ==size || 0 == data)
	{
		result = ERR_VALUE;
		log_printf("Write failed! invalid data!\n");
	}
	else
	{
		g_flash_complete = 0;
		log_printf("start write offset:%x size %d\n", offset, size);
		result = InnerFlash_Write(InnerFlashDev, data, USER_DATA_START_ADDR + offset, size);
		if(result != ERR_OK)
			log_printf("Write failed!code:%d\n", result);
		else
		{
			while(!g_flash_complete)
				InnerFlash_Main(InnerFlashDev);
		}
		log_printf("Write success!\n");
		g_flash_complete = 0;
	}
	
	return result;
}

uint16_t ReadFromFlashHAL(int offset, char *data, int size)
{
	uint16_t result = ERR_OK;
	log_printf("Try to read flash offset:%x size:%d\n", offset, size);
	if(0 ==size || 0 == data)
	{
		result = ERR_VALUE;
		log_printf("Read failed! invalid data!\n");
	}
	else
	{
		g_flash_complete = 0;
		log_printf("start read offset:%x size %d\n", offset, size);
		result = InnerFlash_Read(InnerFlashDev, USER_DATA_START_ADDR + offset, data, size);
		if(result == ERR_OK)
		{
			while(!g_flash_complete)
				InnerFlash_Main(InnerFlashDev);
			log_printf("Read success!\n");
		}
		else
			log_printf("Read failed!code:%d\n", result);
	}
	return result;
}




