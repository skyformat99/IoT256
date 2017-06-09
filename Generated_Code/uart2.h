/* ###################################################################
**     THIS COMPONENT MODULE IS GENERATED BY THE TOOL. DO NOT MODIFY IT.
**     Filename    : uart2.h
**     Project     : ProcessorExpert
**     Processor   : MKL26Z256VLH4
**     Component   : Serial_LDD
**     Version     : Component 01.168, Driver 01.10, CPU db: 3.00.000
**     Compiler    : GNU C Compiler
**     Date/Time   : 2016-11-18, 13:28, # CodeGen: 49
**     Abstract    :
**         This component "Serial_LDD" implements an asynchronous serial
**         communication. The component supports different settings of
**         parity, word width, stop-bit and communication speed,
**         user can select interrupt or polling handler.
**         Communication speed can be changed also in runtime.
**         The component requires one on-chip asynchronous serial communication channel.
**     Settings    :
**          Component name                                 : uart2
**          Device                                         : UART2
**          Interrupt service/event                        : Enabled
**            Interrupt RxD                                : INT_UART2
**            Interrupt RxD priority                       : medium priority
**            Interrupt TxD                                : INT_UART2
**            Interrupt TxD priority                       : medium priority
**            Interrupt Error                              : INT_UART2
**            Interrupt Error priority                     : medium priority
**          Settings                                       : 
**            Data width                                   : 8 bits
**            Parity                                       : None
**            Stop bits                                    : 1
**            Loop mode                                    : Normal
**            Baud rate                                    : 115200 baud
**            Wakeup condition                             : Idle line wakeup
**            Stop in wait mode                            : no
**            Idle line mode                               : Starts after start bit
**            Transmitter output                           : Not inverted
**            Receiver input                               : Not inverted
**            Break generation length                      : 10/11 bits
**            Receiver                                     : Enabled
**              RxD                                        : ADC0_DM3/ADC0_SE7a/PTE23/TPM2_CH1/UART2_RX
**              RxD pin signal                             : 
**            Transmitter                                  : Enabled
**              TxD                                        : ADC0_DP3/ADC0_SE3/PTE22/TPM2_CH0/UART2_TX
**              TxD pin signal                             : 
**            Flow control                                 : None
**          Initialization                                 : 
**            Enabled in init. code                        : yes
**            Auto initialization                          : no
**            Event mask                                   : 
**              OnBlockSent                                : Enabled
**              OnBlockReceived                            : Enabled
**              OnTxComplete                               : Disabled
**              OnError                                    : Disabled
**              OnBreak                                    : Disabled
**          CPU clock/configuration selection              : 
**            Clock configuration 0                        : This component enabled
**            Clock configuration 1                        : This component disabled
**            Clock configuration 2                        : This component disabled
**            Clock configuration 3                        : This component disabled
**            Clock configuration 4                        : This component disabled
**            Clock configuration 5                        : This component disabled
**            Clock configuration 6                        : This component disabled
**            Clock configuration 7                        : This component disabled
**     Contents    :
**         Init                    - LDD_TDeviceData* uart2_Init(LDD_TUserData *UserDataPtr);
**         SendBlock               - LDD_TError uart2_SendBlock(LDD_TDeviceData *DeviceDataPtr, LDD_TData...
**         ReceiveBlock            - LDD_TError uart2_ReceiveBlock(LDD_TDeviceData *DeviceDataPtr, LDD_TData...
**         CancelBlockTransmission - LDD_TError uart2_CancelBlockTransmission(LDD_TDeviceData *DeviceDataPtr);
**         CancelBlockReception    - LDD_TError uart2_CancelBlockReception(LDD_TDeviceData *DeviceDataPtr);
**
**     Copyright : 1997 - 2013 Freescale Semiconductor, Inc. All Rights Reserved.
**     SOURCE DISTRIBUTION PERMISSIBLE as directed in End User License Agreement.
**     
**     http      : www.freescale.com
**     mail      : support@freescale.com
** ###################################################################*/
/*!
** @file uart2.h
** @version 01.10
** @brief
**         This component "Serial_LDD" implements an asynchronous serial
**         communication. The component supports different settings of
**         parity, word width, stop-bit and communication speed,
**         user can select interrupt or polling handler.
**         Communication speed can be changed also in runtime.
**         The component requires one on-chip asynchronous serial communication channel.
*/         
/*!
**  @addtogroup uart2_module uart2 module documentation
**  @{
*/         

#ifndef __uart2_H
#define __uart2_H

/* MODULE uart2. */


#include "Cpu.h"

#ifdef __cplusplus
extern "C" {
#endif 


/*! Peripheral base address of a device allocated by the component. This constant can be used directly in PDD macros. */
#define uart2_PRPH_BASE_ADDRESS  0x4006C000U
  
/* Methods configuration constants - generated for all enabled component's methods */
#define uart2_Init_METHOD_ENABLED      /*!< Init method of the component uart2 is enabled (generated) */
#define uart2_SendBlock_METHOD_ENABLED /*!< SendBlock method of the component uart2 is enabled (generated) */
#define uart2_ReceiveBlock_METHOD_ENABLED /*!< ReceiveBlock method of the component uart2 is enabled (generated) */
#define uart2_CancelBlockTransmission_METHOD_ENABLED /*!< CancelBlockTransmission method of the component uart2 is enabled (generated) */
#define uart2_CancelBlockReception_METHOD_ENABLED /*!< CancelBlockReception method of the component uart2 is enabled (generated) */

/* Events configuration constants - generated for all enabled component's events */
#define uart2_OnBlockReceived_EVENT_ENABLED /*!< OnBlockReceived event of the component uart2 is enabled (generated) */
#define uart2_OnBlockSent_EVENT_ENABLED /*!< OnBlockSent event of the component uart2 is enabled (generated) */

#define ENABLED_TX_INT        0x01U    /*!< TX interrupt enabled      */
#define BREAK_DETECTED        0x02U    /*!< Break detected            */
#define TX_COMPLETED          0x04U    /*!< Transmission completed    */
#define ENABLE_TX_COMPLETE    0x10U    /*!< Enable/Disable of TX complete detection. Used in the polling mode only */

/*! Device data structure type */
typedef struct {
  uint16_t SerFlag;                    /*!< Flags for serial communication */
  uint16_t InpRecvDataNum;             /*!< The counter of received characters */
  uint8_t *InpDataPtr;                 /*!< The buffer pointer for received characters */
  uint16_t InpDataNumReq;              /*!< The counter of characters to receive by ReceiveBlock() */
  uint16_t OutSentDataNum;             /*!< The counter of sent characters */
  uint8_t *OutDataPtr;                 /*!< The buffer pointer for data to be transmitted */
  uint16_t OutDataNumReq;              /*!< The counter of characters to be send by SendBlock() */
  LDD_TUserData *UserDataPtr;          /*!< Pointer to user data */
} uart2_TDeviceData;

typedef uart2_TDeviceData *uart2_TDeviceDataPtr ; /*!< Pointer to the device data structure. */

/*
** ===================================================================
**     Method      :  uart2_Init (component Serial_LDD)
*/
/*!
**     @brief
**         Initializes the device. Allocates memory for the device data
**         structure, allocates interrupt vectors and sets interrupt
**         priority, sets pin routing, sets timing, etc. If the "Enable
**         in init. code" is set to "yes" value then the device is also
**         enabled(see the description of the Enable() method). In this
**         case the Enable() method is not necessary and needn't to be
**         generated. 
**     @param
**         UserDataPtr     - Pointer to the user or
**                           RTOS specific data. This pointer will be
**                           passed as an event or callback parameter.
**     @return
**                         - Device data structure pointer.
*/
/* ===================================================================*/
LDD_TDeviceData* uart2_Init(LDD_TUserData *UserDataPtr);

/*
** ===================================================================
**     Method      :  uart2_ReceiveBlock (component Serial_LDD)
*/
/*!
**     @brief
**         Specifies the number of data to receive. The method returns
**         ERR_BUSY until the specified number of characters is
**         received. Method <CancelBlockReception> can be used to
**         cancel a running receive operation.
**     @param
**         DeviceDataPtr   - Device data structure
**                           pointer returned by <Init> method.
**     @param
**         BufferPtr       - Pointer to a buffer where
**                           received characters will be stored.
**     @param
**         Size            - Number of characters to receive
**     @return
**                         - Error code, possible codes:
**                           ERR_OK - OK
**                           ERR_SPEED - The component does not work in
**                           the active clock configuration.
**                           ERR_PARAM_SIZE - Parameter Size is out of
**                           expected range.
**                           ERR_DISABLED - The component or device is
**                           disabled.
**                           ERR_BUSY - The previous receive request is
**                           pending.
*/
/* ===================================================================*/
LDD_TError uart2_ReceiveBlock(LDD_TDeviceData *DeviceDataPtr, LDD_TData *BufferPtr, uint16_t Size);

/*
** ===================================================================
**     Method      :  uart2_SendBlock (component Serial_LDD)
*/
/*!
**     @brief
**         Sends a block of characters. The method returns ERR_BUSY
**         when the previous block transmission is not completed.
**         Method <CancelBlockTransmission> can be used to cancel a
**         transmit operation. This method is available only if the
**         transmitter property is enabled.
**     @param
**         DeviceDataPtr   - Device data structure
**                           pointer returned by <Init> method.
**     @param
**         BufferPtr       - Pointer to a buffer from where
**                           data will be sent.
**     @param
**         Size            - Number of characters in the buffer.
**     @return
**                         - Error code, possible codes:
**                           ERR_OK - OK
**                           ERR_SPEED - The component does not work in
**                           the active clock configuration.
**                           ERR_PARAM_SIZE - Parameter Size is out of
**                           expected range.
**                           ERR_DISABLED - The component or device is
**                           disabled.
**                           ERR_BUSY - The previous transmit request is
**                           pending.
*/
/* ===================================================================*/
LDD_TError uart2_SendBlock(LDD_TDeviceData *DeviceDataPtr, LDD_TData *BufferPtr, uint16_t Size);

/*
** ===================================================================
**     Method      :  uart2_CancelBlockTransmission (component Serial_LDD)
*/
/*!
**     @brief
**         Immediately cancels the running transmit process started by
**         method <SendBlock>. Characters already stored in the
**         transmit shift register will be sent.
**     @param
**         DeviceDataPtr   - Device data structure
**                           pointer returned by <Init> method.
**     @return
**                         - Error code, possible codes:
**                           ERR_OK - OK
**                           ERR_SPEED - The component does not work in
**                           the active clock configuration.
**                           ERR_DISABLED - The component or device is
**                           disabled.
*/
/* ===================================================================*/
LDD_TError uart2_CancelBlockTransmission(LDD_TDeviceData *DeviceDataPtr);

/*
** ===================================================================
**     Method      :  uart2_CancelBlockReception (component Serial_LDD)
*/
/*!
**     @brief
**         Immediately cancels the running receive process started by
**         method <ReceiveBlock>. Characters already stored in the HW
**         FIFO will be lost.
**     @param
**         DeviceDataPtr   - Device data structure
**                           pointer returned by <Init> method.
**     @return
**                         - Error code, possible codes:
**                           ERR_OK - OK
**                           ERR_SPEED - The component does not work in
**                           the active clock configuration.
**                           ERR_DISABLED - The component or device is
**                           disabled.
*/
/* ===================================================================*/
LDD_TError uart2_CancelBlockReception(LDD_TDeviceData *DeviceDataPtr);

/*
** ===================================================================
**     Method      :  uart2_Interrupt (component Serial_LDD)
**
**     Description :
**         The ISR function handling the device receive/transmit 
**         interrupt. Calls InterruptTX/InterruptRX methods.
**         This method is internal. It is used by Processor Expert only.
** ===================================================================
*/
/* {Default RTOS Adapter} ISR function prototype */
PE_ISR(uart2_Interrupt);

/* END uart2. */

#ifdef __cplusplus
}  /* extern "C" */
#endif 

#endif
/* ifndef __uart2_H */
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
