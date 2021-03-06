/* ###################################################################
**     THIS COMPONENT MODULE IS GENERATED BY THE TOOL. DO NOT MODIFY IT.
**     Filename    : InnerFlash.h
**     Project     : ProcessorExpert
**     Processor   : MKL26Z256VLH4
**     Component   : FLASH_LDD
**     Version     : Component 01.099, Driver 01.12, CPU db: 3.00.000
**     Compiler    : GNU C Compiler
**     Date/Time   : 2017-04-23, 00:49, # CodeGen: 65
**     Abstract    :
**          This embedded component implements an access to an on-chip flash memory.
**          Using this component the flash memory could be written to, erased,
**          read from. Some other services would be provided, if they are supported
**          by hardware, e.g. changes of the flash memory protection state.
**     Settings    :
**          Component name                                 : InnerFlash
**          Device                                         : FTFA
**          Use user memory areas                          : no
**          Interrupt service                              : Disabled
**            Write batch size                             : Minimal
**            Erase batch size                             : Minimal
**            Read batch size                              : Unlimited
**          Safe launch and wait                           : yes
**            Safe routine location                        : Defined statically by the component
**            Interruptable wait loop                      : no
**          Initialization                                 : 
**            Enabled in init. code                        : yes
**            Auto initialization                          : no
**            Event mask                                   : 
**              OnOperationComplete                        : Enabled
**              OnError                                    : Disabled
**            CPU clock/configuration selection            : 
**              Clock configuration 0                      : This component enabled
**              Clock configuration 1                      : This component disabled
**              Clock configuration 2                      : This component disabled
**              Clock configuration 3                      : This component disabled
**              Clock configuration 4                      : This component disabled
**              Clock configuration 5                      : This component disabled
**              Clock configuration 6                      : This component disabled
**              Clock configuration 7                      : This component disabled
**     Contents    :
**         Init  - LDD_TDeviceData* InnerFlash_Init(LDD_TUserData *UserDataPtr);
**         Read  - LDD_TError InnerFlash_Read(LDD_TDeviceData *DeviceDataPtr, LDD_FLASH_TAddress...
**         Write - LDD_TError InnerFlash_Write(LDD_TDeviceData *DeviceDataPtr, LDD_TData...
**         Erase - LDD_TError InnerFlash_Erase(LDD_TDeviceData *DeviceDataPtr,...
**         Main  - void InnerFlash_Main(LDD_TDeviceData *DeviceDataPtr);
**
**     Copyright : 1997 - 2013 Freescale Semiconductor, Inc. All Rights Reserved.
**     SOURCE DISTRIBUTION PERMISSIBLE as directed in End User License Agreement.
**     
**     http      : www.freescale.com
**     mail      : support@freescale.com
** ###################################################################*/
/*!
** @file InnerFlash.h
** @version 01.12
** @brief
**          This embedded component implements an access to an on-chip flash memory.
**          Using this component the flash memory could be written to, erased,
**          read from. Some other services would be provided, if they are supported
**          by hardware, e.g. changes of the flash memory protection state.
*/         
/*!
**  @addtogroup InnerFlash_module InnerFlash module documentation
**  @{
*/         

#ifndef __InnerFlash_H
#define __InnerFlash_H

/* MODULE InnerFlash. */

/* Include shared modules, which are used for whole project */
#include "PE_Types.h"
#include "PE_Error.h"
#include "PE_Const.h"
#include "IO_Map.h"
/* Include inherited beans */
#include "FTFA_PDD.h"
#include "MCM_PDD.h"

#include "Cpu.h"

#ifdef __cplusplus
extern "C" {
#endif 


/* Parameters of the flash memory blocks used by the component - general format */
#define InnerFlash_BLOCK0_ADDRESS            0x00LU
#define InnerFlash_BLOCK0_SIZE               0x00020000LU
#define InnerFlash_BLOCK0_ERASABLE_UNIT_SIZE 0x0400LU
#define InnerFlash_BLOCK0_PROT_UNIT_SIZE     0x2000LU

#define InnerFlash_BLOCK1_ADDRESS            0x00020000LU
#define InnerFlash_BLOCK1_SIZE               0x00020000LU
#define InnerFlash_BLOCK1_ERASABLE_UNIT_SIZE 0x0400LU
#define InnerFlash_BLOCK1_PROT_UNIT_SIZE     0x2000LU

/* Parameters of the flash memory blocks used by the component - CPU specific format */
#define InnerFlash_PFLASH_ADDRESS            0x00LU
#define InnerFlash_PFLASH_SIZE               0x00040000LU
#define InnerFlash_PFLASH_ERASABLE_UNIT_SIZE 0x0400LU
#define InnerFlash_PFLASH_PROT_UNIT_SIZE     0x2000LU
#define InnerFlash_ERASABLE_UNIT_SIZE        0x0400LU
#define InnerFlash_ERASABLE_UNIT_MASK        0x03FFLU
#define InnerFlash_WRITE_UNIT_SIZE           0x04LU
#define InnerFlash_WRITE_UNIT_MASK           0x03LU

/*! Peripheral base address of a device allocated by the component. This constant can be used directly in PDD macros. */
#define InnerFlash_PRPH_BASE_ADDRESS  0x40020000U
  
/* Methods configuration constants - generated for all enabled component's methods */
#define InnerFlash_Init_METHOD_ENABLED /*!< Init method of the component InnerFlash is enabled (generated) */
#define InnerFlash_Read_METHOD_ENABLED /*!< Read method of the component InnerFlash is enabled (generated) */
#define InnerFlash_Write_METHOD_ENABLED /*!< Write method of the component InnerFlash is enabled (generated) */
#define InnerFlash_Erase_METHOD_ENABLED /*!< Erase method of the component InnerFlash is enabled (generated) */
#define InnerFlash_Main_METHOD_ENABLED /*!< Main method of the component InnerFlash is enabled (generated) */

/* Events configuration constants - generated for all enabled component's events */
#define InnerFlash_OnOperationComplete_EVENT_ENABLED /*!< OnOperationComplete event of the component InnerFlash is enabled (generated) */

/*
** ===================================================================
**     Method      :  InnerFlash_Init (component FLASH_LDD)
*/
/*!
**     @brief
**         Initializes the device. Allocates memory for the device data
**         structure, allocates interrupt vectors and sets interrupt
**         priority, sets pin routing, sets timing, etc. If the "Enable
**         in init. code" property is set to "yes" (<Enabled in init.
**         code>) value then the device is also enabled(see the
**         description of the Enable() method). In this case the
**         Enable() method is not necessary and needn't to be generated.
**         This method can be called only once. Before the second call
**         of Init() the Deinit() must be called first.
**     @param
**         UserDataPtr     - Pointer to the user or
**                           RTOS specific data. This pointer will be
**                           passed as an event or callback parameter.
**     @return
**                         - Device data structure pointer.
*/
/* ===================================================================*/
LDD_TDeviceData* InnerFlash_Init(LDD_TUserData *UserDataPtr);

/*
** ===================================================================
**     Method      :  InnerFlash_Write (component FLASH_LDD)
*/
/*!
**     @brief
**         This method sets up a flash memory write operation. The
**         operation itself is performing by defined batches (tproperty
**         <Write batch size>) by periodical calling the componentís
**         Main method in the user application (higher level OS service)
**         or by the componentís ISR, if an componentís interrupt
**         service is enabled.
**     @param
**         DeviceDataPtr   - Device data structure
**                           pointer returned by <Init> method.
**     @param
**         FromPtr         - Pointer to data to be written to
**                           the flash memory.
**     @param
**         ToAddress       - Address to the flash memory
**                           the input data would be written to.
**     @param
**         Size            - Size of the input data (in bytes) to be
**                           written to the flash memory.
**     @return
**                         - Error code
**                           ERR_OK - OK
**                           ERR_DISABLED - Component is disabled
**                           ERR_SPEED - This device does not work in
**                           the active clock configuration
**                           ERR_BUSY - Some flash memory operation is
**                           already in progress 
**                           ERR_PARAM_ADDRESS - Desired flash memory
**                           area is out of allowed range
**                           ERR_NOTAVAIL - When Safe launch and wait
**                           mode is enabled (property Safe launch and
**                           wait) and safe routine location is defined
**                           in runtime (property Safe routine location)
**                           and the safe routine location has not been
**                           specified yet (the SetSafeRoutineLocation
**                           method has not been used to define the
**                           location the safe routine will be copied to).
*/
/* ===================================================================*/
LDD_TError InnerFlash_Write(LDD_TDeviceData *DeviceDataPtr, LDD_TData *FromPtr, LDD_FLASH_TAddress ToAddress, LDD_FLASH_TDataSize Size);

/*
** ===================================================================
**     Method      :  InnerFlash_Read (component FLASH_LDD)
*/
/*!
**     @brief
**         This method sets up a flash memory read operation. The
**         operation itself is performing by defined batches (property
**         <Read batch size>) by periodical calling the componentís
**         Main method in the user application (higher level OS service).
**     @param
**         DeviceDataPtr   - Device data structure
**                           pointer returned by <Init> method.
**     @param
**         FromAddress     - Address to the flash
**                           memory the output data would be read from.
**     @param
**         ToPtr           - Pointer to a location the data would
**                           be written to.
**     @param
**         Size            - Size of the data (in bytes)  to be read
**                           from the flash memory.
**     @return
**                         - Error code
**                           ERR_OK - OK
**                           ERR_DISABLED - Component is disabled
**                           ERR_SPEED - This device does not work in
**                           the active clock configuration
**                           ERR_BUSY - Some flash memory operation is
**                           already in progress 
**                           ERR_PARAM_ADDRESS - Desired flash memory
**                           area is out of allowed range
*/
/* ===================================================================*/
LDD_TError InnerFlash_Read(LDD_TDeviceData *DeviceDataPtr, LDD_FLASH_TAddress FromAddress, LDD_TData *ToPtr, LDD_FLASH_TDataSize Size);

/*
** ===================================================================
**     Method      :  InnerFlash_Erase (component FLASH_LDD)
*/
/*!
**     @brief
**         This method sets up a flash memory erase operation. The
**         operation itself is performing by defined batches (property
**         <Erase batch size>) by periodical calling the componentís
**         Main method in the user application (higher level OS service)
**         or by the componentís ISR, if an componentís interrupt
**         service is enabled.
**     @param
**         DeviceDataPtr   - Device data structure
**                           pointer returned by <Init> method.
**     @param
**         FromAddress     - Address of the flash
**                           memory area (the first erase sector is the
**                           sector the given address belongs to) to be
**                           erased.
**     @param
**         Size            - Size of the flash memory area (in bytes)
**                           to be erased. The flash memory is erased by
**                           the erase sectors. The first erased sector
**                           is a sector the address specified by the
**                           input parameter Address belongs to. The
**                           last erased sector is a sector the address
**                           calculated like an addition of the address
**                           specified by the input parameter Address
**                           and the size specified by the input
**                           parameter Size belongs to.
**     @return
**                         - Error code
**                           ERR_OK - OK
**                           ERR_DISABLED - Component is disabled
**                           ERR_SPEED - This device does not work in
**                           the active clock configuration
**                           ERR_BUSY - Some flash memory operation is
**                           already in progress 
**                           ERR_PARAM_ADDRESS - Desired flash memory
**                           area is out of allowed range or is not
**                           aligned to erasable units' bounderies
**                           ERR_NOTAVAIL - When Safe launch and wait
**                           mode is enabled (property Safe launch and
**                           wait) and safe routine location is defined
**                           in runtime (property Safe routine location)
**                           and the safe routine location has not been
**                           specified yet (the SetSafeRoutineLocation
**                           method has not been used to define the
**                           location the safe routine will be copied to).
*/
/* ===================================================================*/
LDD_TError InnerFlash_Erase(LDD_TDeviceData *DeviceDataPtr, LDD_FLASH_TAddress FromAddress, LDD_FLASH_TDataSize Size);

/*
** ===================================================================
**     Method      :  InnerFlash_Main (component FLASH_LDD)
*/
/*!
**     @brief
**         This method is used to perform one batch of a flash memory
**         operation operation. This method is used to perform batches
**         of all flash memory operations (Write, Read, Erase,
**         EraseBlock, VerifyErasedBlock) when the component works in
**         the polled mode (interrupt service is disabled - property
**         <Interrupt service>). This method performs batches of the
**         read flash memory operation, when the component works in the
**         interrupt triggered mode (interrupt service is enabled).
**         This method is enabled only if the component works in the
**         polled mode or if the Read method is enabled.
**     @param
**         DeviceDataPtr   - Device data structure
**                           pointer returned by <Init> method.
*/
/* ===================================================================*/
void InnerFlash_Main(LDD_TDeviceData *DeviceDataPtr);

/* END InnerFlash. */

#ifdef __cplusplus
}  /* extern "C" */
#endif 

#endif
/* ifndef __InnerFlash_H */
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
