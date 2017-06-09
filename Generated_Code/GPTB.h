/* ###################################################################
**     THIS COMPONENT MODULE IS GENERATED BY THE TOOL. DO NOT MODIFY IT.
**     Filename    : GPTB.h
**     Project     : ProcessorExpert
**     Processor   : MKL26Z256VLH4
**     Component   : GPIO_LDD
**     Version     : Component 01.128, Driver 01.06, CPU db: 3.00.000
**     Compiler    : GNU C Compiler
**     Date/Time   : 2017-04-13, 23:37, # CodeGen: 55
**     Abstract    :
**         The HAL GPIO component will provide a low level API for unified
**         access to general purpose digital input/output pins across
**         various device designs.
**
**         RTOS drivers using HAL GPIO API will be simpler and more
**         portable to various microprocessors.
**     Settings    :
**          Component name                                 : GPTB
**          Port                                           : PTB
**          Port width                                     : 32 bits
**          Mask of allocated pins                         : F000F
**          Interrupt service/event                        : Disabled
**          Bit fields                                     : 8
**            Bit field                                    : 
**              Field name                                 : PTB0_GSM_ASSEMBLY
**              Pins                                       : 1
**                Pin                                      : 
**                  Pin                                    : ADC0_SE8/TSI0_CH0/PTB0/LLWU_P5/I2C0_SCL/TPM1_CH0
**                  Pin signal                             : 
**                  Initial pin direction                  : Input
**                  Initial pin event                      : Disabled
**            Bit field                                    : 
**              Field name                                 : PTB1_GPS_ASSEMBLY
**              Pins                                       : 1
**                Pin                                      : 
**                  Pin                                    : ADC0_SE9/TSI0_CH6/PTB1/I2C0_SDA/TPM1_CH1
**                  Pin signal                             : 
**                  Initial pin direction                  : Input
**                  Initial pin event                      : Disabled
**            Bit field                                    : 
**              Field name                                 : PTB2_BT_ASSEMBLY
**              Pins                                       : 1
**                Pin                                      : 
**                  Pin                                    : ADC0_SE12/TSI0_CH7/PTB2/I2C0_SCL/TPM2_CH0
**                  Pin signal                             : 
**                  Initial pin direction                  : Input
**                  Initial pin event                      : Disabled
**            Bit field                                    : 
**              Field name                                 : PTB3_GSM_PWR_EN
**              Pins                                       : 1
**                Pin                                      : 
**                  Pin                                    : ADC0_SE13/TSI0_CH8/PTB3/I2C0_SDA/TPM2_CH1
**                  Pin signal                             : 
**                  Initial pin direction                  : Output
**                    Initial output state                 : 0
**                  Initial pin event                      : Disabled
**            Bit field                                    : 
**              Field name                                 : PTB16_GSM_PWRKEY
**              Pins                                       : 1
**                Pin                                      : 
**                  Pin                                    : TSI0_CH9/PTB16/SPI1_MOSI/UART0_RX/TPM_CLKIN0/SPI1_MISO
**                  Pin signal                             : 
**                  Initial pin direction                  : Output
**                    Initial output state                 : 0
**                  Initial pin event                      : Disabled
**            Bit field                                    : 
**              Field name                                 : PTB17_GPS_WAKE_UP
**              Pins                                       : 1
**                Pin                                      : 
**                  Pin                                    : TSI0_CH10/PTB17/SPI1_MISO/UART0_TX/TPM_CLKIN1/SPI1_MOSI
**                  Pin signal                             : 
**                  Initial pin direction                  : Output
**                    Initial output state                 : 0
**                  Initial pin event                      : Disabled
**            Bit field                                    : 
**              Field name                                 : PTB18_GPS_TIMEPULSE
**              Pins                                       : 1
**                Pin                                      : 
**                  Pin                                    : TSI0_CH11/PTB18/TPM2_CH0/I2S0_TX_BCLK
**                  Pin signal                             : 
**                  Initial pin direction                  : Input
**                  Initial pin event                      : Disabled
**            Bit field                                    : 
**              Field name                                 : PTB19_GPS_PWR_EN
**              Pins                                       : 1
**                Pin                                      : 
**                  Pin                                    : TSI0_CH12/PTB19/TPM2_CH1/I2S0_TX_FS
**                  Pin signal                             : 
**                  Initial pin direction                  : Output
**                    Initial output state                 : 0
**                  Initial pin event                      : Disabled
**          Initialization                                 : 
**            Auto initialization                          : yes
**            Event mask                                   : 
**              OnPortEvent                                : Disabled
**     Contents    :
**         Init            - LDD_TDeviceData* GPTB_Init(LDD_TUserData *UserDataPtr);
**         SetFieldValue   - void GPTB_SetFieldValue(LDD_TDeviceData *DeviceDataPtr, LDD_GPIO_TBitField...
**         GetFieldValue   - GPTB_TFieldValue GPTB_GetFieldValue(LDD_TDeviceData *DeviceDataPtr,...
**         ClearFieldBits  - void GPTB_ClearFieldBits(LDD_TDeviceData *DeviceDataPtr, LDD_GPIO_TBitField...
**         SetFieldBits    - void GPTB_SetFieldBits(LDD_TDeviceData *DeviceDataPtr, LDD_GPIO_TBitField...
**         ToggleFieldBits - void GPTB_ToggleFieldBits(LDD_TDeviceData *DeviceDataPtr, LDD_GPIO_TBitField...
**
**     Copyright : 1997 - 2013 Freescale Semiconductor, Inc. All Rights Reserved.
**     SOURCE DISTRIBUTION PERMISSIBLE as directed in End User License Agreement.
**     
**     http      : www.freescale.com
**     mail      : support@freescale.com
** ###################################################################*/
/*!
** @file GPTB.h
** @version 01.06
** @brief
**         The HAL GPIO component will provide a low level API for unified
**         access to general purpose digital input/output pins across
**         various device designs.
**
**         RTOS drivers using HAL GPIO API will be simpler and more
**         portable to various microprocessors.
*/         
/*!
**  @addtogroup GPTB_module GPTB module documentation
**  @{
*/         

#ifndef __GPTB_H
#define __GPTB_H

/* MODULE GPTB. */

/* Include shared modules, which are used for whole project */
#include "PE_Types.h"
#include "PE_Error.h"
#include "PE_Const.h"
#include "IO_Map.h"
/* Include inherited beans */
#include "IO_Map.h"
#include "GPIO_PDD.h"
#include "PORT_PDD.h"

#include "Cpu.h"

#ifdef __cplusplus
extern "C" {
#endif 


/*! Peripheral base address of a device allocated by the component. This constant can be used directly in PDD macros. */
#define GPTB_PRPH_BASE_ADDRESS  0x400FF040U
  
/*! Device data structure pointer used when auto initialization property is enabled. This constant can be passed as a first parameter to all component's methods. */
#define GPTB_DeviceData  ((LDD_TDeviceData *)PE_LDD_GetDeviceStructure(PE_LDD_COMPONENT_GPTB_ID))

/* Methods configuration constants - generated for all enabled component's methods */
#define GPTB_Init_METHOD_ENABLED       /*!< Init method of the component GPTB is enabled (generated) */
#define GPTB_SetFieldValue_METHOD_ENABLED /*!< SetFieldValue method of the component GPTB is enabled (generated) */
#define GPTB_GetFieldValue_METHOD_ENABLED /*!< GetFieldValue method of the component GPTB is enabled (generated) */
#define GPTB_ClearFieldBits_METHOD_ENABLED /*!< ClearFieldBits method of the component GPTB is enabled (generated) */
#define GPTB_SetFieldBits_METHOD_ENABLED /*!< SetFieldBits method of the component GPTB is enabled (generated) */
#define GPTB_ToggleFieldBits_METHOD_ENABLED /*!< ToggleFieldBits method of the component GPTB is enabled (generated) */

/* Events configuration constants - generated for all enabled component's events */

/* Definition of bit field constants */
#define PTB0_GSM_ASSEMBLY ((LDD_GPIO_TBitField)0)
#define PTB1_GPS_ASSEMBLY ((LDD_GPIO_TBitField)1)
#define PTB2_BT_ASSEMBLY ((LDD_GPIO_TBitField)2)
#define PTB3_GSM_PWR_EN ((LDD_GPIO_TBitField)3)
#define PTB16_GSM_PWRKEY ((LDD_GPIO_TBitField)4)
#define PTB17_GPS_WAKE_UP ((LDD_GPIO_TBitField)5)
#define PTB18_GPS_TIMEPULSE ((LDD_GPIO_TBitField)6)
#define PTB19_GPS_PWR_EN ((LDD_GPIO_TBitField)7)

/* Definition of implementation constants */
#define GPTB_ALLOCATED_PINS_MASK 0x000F000FU /*!< Mask of all allocated pins from the port */
#define GPTB_MODULE_BASE_ADDRESS FPTB_BASE_PTR /*!< Name of macro used as the base address */
#define GPTB_PORTCONTROL_BASE_ADDRESS PORTB_BASE_PTR /*!< Name of macro used as the base address */
#define GPTB_AVAILABLE_EVENTS_MASK 0x00U /*!< Mask of all available events */
#define GPTB_FIELD_0_PIN_0 LDD_GPIO_PIN_0 /*!< Constant for the pin in the field used in the method ConnectPin */
#define GPTB_FIELD_1_PIN_0 LDD_GPIO_PIN_1 /*!< Constant for the pin in the field used in the method ConnectPin */
#define GPTB_FIELD_2_PIN_0 LDD_GPIO_PIN_2 /*!< Constant for the pin in the field used in the method ConnectPin */
#define GPTB_FIELD_3_PIN_0 LDD_GPIO_PIN_3 /*!< Constant for the pin in the field used in the method ConnectPin */
#define GPTB_FIELD_4_PIN_0 LDD_GPIO_PIN_16 /*!< Constant for the pin in the field used in the method ConnectPin */
#define GPTB_FIELD_5_PIN_0 LDD_GPIO_PIN_17 /*!< Constant for the pin in the field used in the method ConnectPin */
#define GPTB_FIELD_6_PIN_0 LDD_GPIO_PIN_18 /*!< Constant for the pin in the field used in the method ConnectPin */
#define GPTB_FIELD_7_PIN_0 LDD_GPIO_PIN_19 /*!< Constant for the pin in the field used in the method ConnectPin */
#define GPTB_PTB0_GSM_ASSEMBLY_START_BIT 0u /*!< Index of the starting bit of the bit field (0 represents LSB) */
#define GPTB_PTB0_GSM_ASSEMBLY_MASK 0x01u /*!< Mask of the bits allocated by the bit field (within the port) */
#define GPTB_PTB1_GPS_ASSEMBLY_START_BIT 1u /*!< Index of the starting bit of the bit field (0 represents LSB) */
#define GPTB_PTB1_GPS_ASSEMBLY_MASK 0x02u /*!< Mask of the bits allocated by the bit field (within the port) */
#define GPTB_PTB2_BT_ASSEMBLY_START_BIT 2u /*!< Index of the starting bit of the bit field (0 represents LSB) */
#define GPTB_PTB2_BT_ASSEMBLY_MASK 0x04u /*!< Mask of the bits allocated by the bit field (within the port) */
#define GPTB_PTB3_GSM_PWR_EN_START_BIT 3u /*!< Index of the starting bit of the bit field (0 represents LSB) */
#define GPTB_PTB3_GSM_PWR_EN_MASK 0x08u /*!< Mask of the bits allocated by the bit field (within the port) */
#define GPTB_PTB16_GSM_PWRKEY_START_BIT 16u /*!< Index of the starting bit of the bit field (0 represents LSB) */
#define GPTB_PTB16_GSM_PWRKEY_MASK 0x00010000u /*!< Mask of the bits allocated by the bit field (within the port) */
#define GPTB_PTB17_GPS_WAKE_UP_START_BIT 17u /*!< Index of the starting bit of the bit field (0 represents LSB) */
#define GPTB_PTB17_GPS_WAKE_UP_MASK 0x00020000u /*!< Mask of the bits allocated by the bit field (within the port) */
#define GPTB_PTB18_GPS_TIMEPULSE_START_BIT 18u /*!< Index of the starting bit of the bit field (0 represents LSB) */
#define GPTB_PTB18_GPS_TIMEPULSE_MASK 0x00040000u /*!< Mask of the bits allocated by the bit field (within the port) */
#define GPTB_PTB19_GPS_PWR_EN_START_BIT 19u /*!< Index of the starting bit of the bit field (0 represents LSB) */
#define GPTB_PTB19_GPS_PWR_EN_MASK 0x00080000u /*!< Mask of the bits allocated by the bit field (within the port) */
/* Representation of unaligned data value of the port.
   Unsigned integer of proper width depending on the size of the GPIO port allocated.
   Typically the value of n-th bit represents the data for the n-th pin within the port.
   Such value is not abstracted from the physical position of the bits within the port. */
typedef uint32_t GPTB_TPortValue;

/* Representation of right-aligned data value of the bit field.
   Typically the value of n-th bit represents the data of the n-th bit within the bit field (not within the port).
   There are used only so many lowest bits, as it is denoted by the width of the bit field.
   Such value is abstracted from the physical position of the bit field pins within the port.
   Equals to the type <compId>_TPortValue. */
typedef GPTB_TPortValue GPTB_TFieldValue;



/*
** ===================================================================
**     Method      :  GPTB_Init (component GPIO_LDD)
*/
/*!
**     @brief
**         This method initializes the associated peripheral(s) and the
**         component internal variables. The method is called
**         automatically as a part of the application initialization
**         code.
**     @param
**         UserDataPtr     - Pointer to the RTOS device
**                           structure. This pointer will be passed to
**                           all events as parameter.
**     @return
**                         - Pointer to the dynamically allocated private
**                           structure or NULL if there was an error.
*/
/* ===================================================================*/
LDD_TDeviceData* GPTB_Init(LDD_TUserData *UserDataPtr);

/*
** ===================================================================
**     Method      :  GPTB_SetFieldValue (component GPIO_LDD)
*/
/*!
**     @brief
**         This method sets the output data value of the specified bit
**         field.
**     @param
**         DeviceDataPtr   - Device data structure
**                           pointer returned by <Init> method.
**     @param
**         Field           - Bit field to write. Bit fields are
**                           defined during design time and for each bit
**                           field there is a generated constant.
**     @param
**         Value           - Aligned data value to writting to the
**                           specified bit field. The bit 0 corresponds
**                           with the pin which has index 0 within the
**                           given bit field, the bit 1 corresponds with
**                           the pin which has index 1 within the given
**                           bit field, etc.
*/
/* ===================================================================*/
void GPTB_SetFieldValue(LDD_TDeviceData *DeviceDataPtr, LDD_GPIO_TBitField Field, GPTB_TFieldValue Value);

/*
** ===================================================================
**     Method      :  GPTB_GetFieldValue (component GPIO_LDD)
*/
/*!
**     @brief
**         This method returns the current input data of the specified
**         field.
**     @param
**         DeviceDataPtr   - Device data structure
**                           pointer returned by <Init> method.
**     @param
**         Field           - Bit field to reading. Bit fields are
**                           defined during design time and for each bit
**                           field there is a generated constant.
**     @return
**                         - Aligned current port input value masked for
**                           allocated pins of the field. The bit 0
**                           corresponds with the pin which has index 0
**                           within the given bit field, the bit 1
**                           corresponds with the pin which has index 1
**                           within the given bit field, etc.
*/
/* ===================================================================*/
GPTB_TFieldValue GPTB_GetFieldValue(LDD_TDeviceData *DeviceDataPtr, LDD_GPIO_TBitField Field);

/*
** ===================================================================
**     Method      :  GPTB_ClearFieldBits (component GPIO_LDD)
*/
/*!
**     @brief
**         This method drives the specified bits of the specified bit
**         field to the inactive level.
**     @param
**         DeviceDataPtr   - Device data structure
**                           pointer returned by <Init> method.
**     @param
**         Field           - Bit field to write. Bit fields are
**                           defined during design time and for each bit
**                           field there is a generated constant.
**     @param
**         Mask            - Aligned mask of bits to setting the
**                           inactive level. Each field pin has
**                           corresponding bit in the mask. Bit value 0
**                           means not selected bit, bit value 1 means
**                           selected bit. The bit 0 corresponds with
**                           the pin which has index 0 within the given
**                           bit field, the bit 1 corresponds with the
**                           pin which has index 1 within the given bit
**                           field, etc.
*/
/* ===================================================================*/
void GPTB_ClearFieldBits(LDD_TDeviceData *DeviceDataPtr, LDD_GPIO_TBitField Field, GPTB_TFieldValue Mask);

/*
** ===================================================================
**     Method      :  GPTB_SetFieldBits (component GPIO_LDD)
*/
/*!
**     @brief
**         This method drives the specified bits of the specified bit
**         field to the active level.
**     @param
**         DeviceDataPtr   - Device data structure
**                           pointer returned by <Init> method.
**     @param
**         Field           - Bit field to write. Bit fields are
**                           defined during design time and for each bit
**                           field there is a generated constant.
**     @param
**         Mask            - Aligned mask of bits to setting the
**                           active level. Each field pin has
**                           corresponding bit in the mask. Bit value 0
**                           means not selected bit, bit value 1 means
**                           selected bit. The bit 0 corresponds with
**                           the pin which has index 0 within the given
**                           bit field, the bit 1 corresponds with the
**                           pin which has index 1 within the given bit
**                           field, etc.
*/
/* ===================================================================*/
void GPTB_SetFieldBits(LDD_TDeviceData *DeviceDataPtr, LDD_GPIO_TBitField Field, GPTB_TFieldValue Mask);

/*
** ===================================================================
**     Method      :  GPTB_ToggleFieldBits (component GPIO_LDD)
*/
/*!
**     @brief
**         This method inverts the specified bits of the specified bit
**         field to other level.
**     @param
**         DeviceDataPtr   - Device data structure
**                           pointer returned by <Init> method.
**     @param
**         Field           - Bit field to write. Bit fields are
**                           defined during design time and for each bit
**                           field there is a generated constant.
**     @param
**         Mask            - Aligned mask of bits to inverting the
**                           current level. Each field pin has
**                           corresponding bit in the mask. Bit value 0
**                           means not selected bit, bit value 1 means
**                           selected bit. The bit 0 corresponds with
**                           the pin which has index 0 within the given
**                           bit field, the bit 1 corresponds with the
**                           pin which has index 1 within the given bit
**                           field, etc.
*/
/* ===================================================================*/
void GPTB_ToggleFieldBits(LDD_TDeviceData *DeviceDataPtr, LDD_GPIO_TBitField Field, GPTB_TFieldValue Mask);

/* END GPTB. */

#ifdef __cplusplus
}  /* extern "C" */
#endif 

#endif
/* ifndef __GPTB_H */
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