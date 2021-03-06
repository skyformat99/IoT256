/* ###################################################################
**     THIS COMPONENT MODULE IS GENERATED BY THE TOOL. DO NOT MODIFY IT.
**     Filename    : GPTB.c
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
** @file GPTB.c
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

/* MODULE GPTB. */

#include "GPTB.h"
/* {Default RTOS Adapter} No RTOS includes */
#include "IO_Map.h"

#ifdef __cplusplus
extern "C" {
#endif 

typedef struct {
  LDD_TUserData *UserData;             /* RTOS device data structure */
} GPTB_TDeviceData, *GPTB_TDeviceDataPtr; /* Device data structure type */
/* {Default RTOS Adapter} Static object used for simulation of dynamic driver memory allocation */
static GPTB_TDeviceData DeviceDataPrv__DEFAULT_RTOS_ALLOC;
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
LDD_TDeviceData* GPTB_Init(LDD_TUserData *UserDataPtr)
{
  /* Allocate LDD device structure */
  GPTB_TDeviceData *DeviceDataPrv;

  /* {Default RTOS Adapter} Driver memory allocation: Dynamic allocation is simulated by a pointer to the static object */
  DeviceDataPrv = &DeviceDataPrv__DEFAULT_RTOS_ALLOC;
  /* Save RTOS Device structure */
  DeviceDataPrv->UserData = UserDataPtr; /* Store the RTOS device structure */
  /* GPIOB_PDOR: PDO&=~0x000B0008 */
  GPIOB_PDOR &= (uint32_t)~(uint32_t)(GPIO_PDOR_PDO(0x000B0008));                                   
  /* GPIOB_PDDR: PDD&=~0x00040007,PDD|=0x000B0008 */
  GPIOB_PDDR = (uint32_t)((GPIOB_PDDR & (uint32_t)~(uint32_t)(
                GPIO_PDDR_PDD(0x00040007)
               )) | (uint32_t)(
                GPIO_PDDR_PDD(0x000B0008)
               ));                                  
  /* Initialization of Port Control registers */
  /* PORTB_PCR0: ISF=0,MUX=1 */
  PORTB_PCR0 = (uint32_t)((PORTB_PCR0 & (uint32_t)~(uint32_t)(
                PORT_PCR_ISF_MASK |
                PORT_PCR_MUX(0x06)
               )) | (uint32_t)(
                PORT_PCR_MUX(0x01)
               ));                                  
  /* PORTB_PCR1: ISF=0,MUX=1 */
  PORTB_PCR1 = (uint32_t)((PORTB_PCR1 & (uint32_t)~(uint32_t)(
                PORT_PCR_ISF_MASK |
                PORT_PCR_MUX(0x06)
               )) | (uint32_t)(
                PORT_PCR_MUX(0x01)
               ));                                  
  /* PORTB_PCR2: ISF=0,MUX=1 */
  PORTB_PCR2 = (uint32_t)((PORTB_PCR2 & (uint32_t)~(uint32_t)(
                PORT_PCR_ISF_MASK |
                PORT_PCR_MUX(0x06)
               )) | (uint32_t)(
                PORT_PCR_MUX(0x01)
               ));                                  
  /* PORTB_PCR3: ISF=0,MUX=1 */
  PORTB_PCR3 = (uint32_t)((PORTB_PCR3 & (uint32_t)~(uint32_t)(
                PORT_PCR_ISF_MASK |
                PORT_PCR_MUX(0x06)
               )) | (uint32_t)(
                PORT_PCR_MUX(0x01)
               ));                                  
  /* PORTB_PCR16: ISF=0,MUX=1 */
  PORTB_PCR16 = (uint32_t)((PORTB_PCR16 & (uint32_t)~(uint32_t)(
                 PORT_PCR_ISF_MASK |
                 PORT_PCR_MUX(0x06)
                )) | (uint32_t)(
                 PORT_PCR_MUX(0x01)
                ));                                  
  /* PORTB_PCR17: ISF=0,MUX=1 */
  PORTB_PCR17 = (uint32_t)((PORTB_PCR17 & (uint32_t)~(uint32_t)(
                 PORT_PCR_ISF_MASK |
                 PORT_PCR_MUX(0x06)
                )) | (uint32_t)(
                 PORT_PCR_MUX(0x01)
                ));                                  
  /* PORTB_PCR18: ISF=0,MUX=1 */
  PORTB_PCR18 = (uint32_t)((PORTB_PCR18 & (uint32_t)~(uint32_t)(
                 PORT_PCR_ISF_MASK |
                 PORT_PCR_MUX(0x06)
                )) | (uint32_t)(
                 PORT_PCR_MUX(0x01)
                ));                                  
  /* PORTB_PCR19: ISF=0,MUX=1 */
  PORTB_PCR19 = (uint32_t)((PORTB_PCR19 & (uint32_t)~(uint32_t)(
                 PORT_PCR_ISF_MASK |
                 PORT_PCR_MUX(0x06)
                )) | (uint32_t)(
                 PORT_PCR_MUX(0x01)
                ));                                  
  /* Registration of the device structure */
  PE_LDD_RegisterDeviceStructure(PE_LDD_COMPONENT_GPTB_ID,DeviceDataPrv);
  return ((LDD_TDeviceData *)DeviceDataPrv);
}

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
void GPTB_SetFieldValue(LDD_TDeviceData *DeviceDataPtr, LDD_GPIO_TBitField Field, GPTB_TFieldValue Value)
{
  (void)DeviceDataPtr;                 /* Parameter is not used, suppress unused argument warning */
  switch (Field) {                     /* no break */
    case PTB0_GSM_ASSEMBLY: {          /* bit field #0 */
      GPIO_PDD_SetPortDataOutput(GPTB_MODULE_BASE_ADDRESS,
        (
          GPIO_PDD_GetPortDataOutput(GPTB_MODULE_BASE_ADDRESS)
          & ((GPTB_TPortValue)(~((GPTB_TPortValue)GPTB_PTB0_GSM_ASSEMBLY_MASK)))
        )
        | (
          Value
          & ((GPTB_TPortValue)GPTB_PTB0_GSM_ASSEMBLY_MASK)
        )
      );
      break;
    }
    case PTB1_GPS_ASSEMBLY: {          /* bit field #1 */
      GPIO_PDD_SetPortDataOutput(GPTB_MODULE_BASE_ADDRESS,
        (
          GPIO_PDD_GetPortDataOutput(GPTB_MODULE_BASE_ADDRESS)
          & ((GPTB_TPortValue)(~((GPTB_TPortValue)GPTB_PTB1_GPS_ASSEMBLY_MASK)))
        )
        | (
          ((GPTB_TPortValue)(Value << GPTB_PTB1_GPS_ASSEMBLY_START_BIT))
          & ((GPTB_TPortValue)GPTB_PTB1_GPS_ASSEMBLY_MASK)
        )
      );
      break;
    }
    case PTB2_BT_ASSEMBLY: {           /* bit field #2 */
      GPIO_PDD_SetPortDataOutput(GPTB_MODULE_BASE_ADDRESS,
        (
          GPIO_PDD_GetPortDataOutput(GPTB_MODULE_BASE_ADDRESS)
          & ((GPTB_TPortValue)(~((GPTB_TPortValue)GPTB_PTB2_BT_ASSEMBLY_MASK)))
        )
        | (
          ((GPTB_TPortValue)(Value << GPTB_PTB2_BT_ASSEMBLY_START_BIT))
          & ((GPTB_TPortValue)GPTB_PTB2_BT_ASSEMBLY_MASK)
        )
      );
      break;
    }
    case PTB3_GSM_PWR_EN: {            /* bit field #3 */
      GPIO_PDD_SetPortDataOutput(GPTB_MODULE_BASE_ADDRESS,
        (
          GPIO_PDD_GetPortDataOutput(GPTB_MODULE_BASE_ADDRESS)
          & ((GPTB_TPortValue)(~((GPTB_TPortValue)GPTB_PTB3_GSM_PWR_EN_MASK)))
        )
        | (
          ((GPTB_TPortValue)(Value << GPTB_PTB3_GSM_PWR_EN_START_BIT))
          & ((GPTB_TPortValue)GPTB_PTB3_GSM_PWR_EN_MASK)
        )
      );
      break;
    }
    case PTB16_GSM_PWRKEY: {           /* bit field #4 */
      GPIO_PDD_SetPortDataOutput(GPTB_MODULE_BASE_ADDRESS,
        (
          GPIO_PDD_GetPortDataOutput(GPTB_MODULE_BASE_ADDRESS)
          & ((GPTB_TPortValue)(~((GPTB_TPortValue)GPTB_PTB16_GSM_PWRKEY_MASK)))
        )
        | (
          ((GPTB_TPortValue)(Value << GPTB_PTB16_GSM_PWRKEY_START_BIT))
          & ((GPTB_TPortValue)GPTB_PTB16_GSM_PWRKEY_MASK)
        )
      );
      break;
    }
    case PTB17_GPS_WAKE_UP: {          /* bit field #5 */
      GPIO_PDD_SetPortDataOutput(GPTB_MODULE_BASE_ADDRESS,
        (
          GPIO_PDD_GetPortDataOutput(GPTB_MODULE_BASE_ADDRESS)
          & ((GPTB_TPortValue)(~((GPTB_TPortValue)GPTB_PTB17_GPS_WAKE_UP_MASK)))
        )
        | (
          ((GPTB_TPortValue)(Value << GPTB_PTB17_GPS_WAKE_UP_START_BIT))
          & ((GPTB_TPortValue)GPTB_PTB17_GPS_WAKE_UP_MASK)
        )
      );
      break;
    }
    case PTB18_GPS_TIMEPULSE: {        /* bit field #6 */
      GPIO_PDD_SetPortDataOutput(GPTB_MODULE_BASE_ADDRESS,
        (
          GPIO_PDD_GetPortDataOutput(GPTB_MODULE_BASE_ADDRESS)
          & ((GPTB_TPortValue)(~((GPTB_TPortValue)GPTB_PTB18_GPS_TIMEPULSE_MASK)))
        )
        | (
          ((GPTB_TPortValue)(Value << GPTB_PTB18_GPS_TIMEPULSE_START_BIT))
          & ((GPTB_TPortValue)GPTB_PTB18_GPS_TIMEPULSE_MASK)
        )
      );
      break;
    }
    case PTB19_GPS_PWR_EN: {           /* bit field #7 */
      GPIO_PDD_SetPortDataOutput(GPTB_MODULE_BASE_ADDRESS,
        (
          GPIO_PDD_GetPortDataOutput(GPTB_MODULE_BASE_ADDRESS)
          & ((GPTB_TPortValue)(~((GPTB_TPortValue)GPTB_PTB19_GPS_PWR_EN_MASK)))
        )
        | (
          ((GPTB_TPortValue)(Value << GPTB_PTB19_GPS_PWR_EN_START_BIT))
          & ((GPTB_TPortValue)GPTB_PTB19_GPS_PWR_EN_MASK)
        )
      );
      break;
    }
    default:
      break;                           /* Invalid Field is not treated, result is undefined */
  } /* switch (Field) */
}

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
GPTB_TFieldValue GPTB_GetFieldValue(LDD_TDeviceData *DeviceDataPtr, LDD_GPIO_TBitField Field)
{
  (void)DeviceDataPtr;                 /* Parameter is not used, suppress unused argument warning */
  switch (Field) {                     /* no break */
    case PTB0_GSM_ASSEMBLY: {          /* bit field #0 */
      return
        (GPTB_TFieldValue)(
          GPIO_PDD_GetPortDataInput(GPTB_MODULE_BASE_ADDRESS)
          & (GPTB_TPortValue)GPTB_PTB0_GSM_ASSEMBLY_MASK
        );
    }
    case PTB1_GPS_ASSEMBLY: {          /* bit field #1 */
      return
        (GPTB_TFieldValue)(
          (
            GPIO_PDD_GetPortDataInput(GPTB_MODULE_BASE_ADDRESS)
            & (GPTB_TPortValue)GPTB_PTB1_GPS_ASSEMBLY_MASK
          )
          >> GPTB_PTB1_GPS_ASSEMBLY_START_BIT
        );
    }
    case PTB2_BT_ASSEMBLY: {           /* bit field #2 */
      return
        (GPTB_TFieldValue)(
          (
            GPIO_PDD_GetPortDataInput(GPTB_MODULE_BASE_ADDRESS)
            & (GPTB_TPortValue)GPTB_PTB2_BT_ASSEMBLY_MASK
          )
          >> GPTB_PTB2_BT_ASSEMBLY_START_BIT
        );
    }
    case PTB3_GSM_PWR_EN: {            /* bit field #3 */
      return
        (GPTB_TFieldValue)(
          (
            GPIO_PDD_GetPortDataInput(GPTB_MODULE_BASE_ADDRESS)
            & (GPTB_TPortValue)GPTB_PTB3_GSM_PWR_EN_MASK
          )
          >> GPTB_PTB3_GSM_PWR_EN_START_BIT
        );
    }
    case PTB16_GSM_PWRKEY: {           /* bit field #4 */
      return
        (GPTB_TFieldValue)(
          (
            GPIO_PDD_GetPortDataInput(GPTB_MODULE_BASE_ADDRESS)
            & (GPTB_TPortValue)GPTB_PTB16_GSM_PWRKEY_MASK
          )
          >> GPTB_PTB16_GSM_PWRKEY_START_BIT
        );
    }
    case PTB17_GPS_WAKE_UP: {          /* bit field #5 */
      return
        (GPTB_TFieldValue)(
          (
            GPIO_PDD_GetPortDataInput(GPTB_MODULE_BASE_ADDRESS)
            & (GPTB_TPortValue)GPTB_PTB17_GPS_WAKE_UP_MASK
          )
          >> GPTB_PTB17_GPS_WAKE_UP_START_BIT
        );
    }
    case PTB18_GPS_TIMEPULSE: {        /* bit field #6 */
      return
        (GPTB_TFieldValue)(
          (
            GPIO_PDD_GetPortDataInput(GPTB_MODULE_BASE_ADDRESS)
            & (GPTB_TPortValue)GPTB_PTB18_GPS_TIMEPULSE_MASK
          )
          >> GPTB_PTB18_GPS_TIMEPULSE_START_BIT
        );
    }
    case PTB19_GPS_PWR_EN: {           /* bit field #7 */
      return
        (GPTB_TFieldValue)(
          (
            GPIO_PDD_GetPortDataInput(GPTB_MODULE_BASE_ADDRESS)
            & (GPTB_TPortValue)GPTB_PTB19_GPS_PWR_EN_MASK
          )
          >> GPTB_PTB19_GPS_PWR_EN_START_BIT
        );
    }
    default:
      break;                           /* Invalid BitField is not treated, result is undefined */
  } /* switch (Field) */
  return (GPTB_TFieldValue)0U;
}

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
void GPTB_ClearFieldBits(LDD_TDeviceData *DeviceDataPtr, LDD_GPIO_TBitField Field, GPTB_TFieldValue Mask)
{
  (void)DeviceDataPtr;                 /* Parameter is not used, suppress unused argument warning */
  switch (Field) {                     /* no break */
    case PTB0_GSM_ASSEMBLY: {          /* bit field #0 */
      GPIO_PDD_ClearPortDataOutputMask(GPTB_MODULE_BASE_ADDRESS,
        ((GPTB_TPortValue)GPTB_PTB0_GSM_ASSEMBLY_MASK)
        & Mask
      );
      break;
    }
    case PTB1_GPS_ASSEMBLY: {          /* bit field #1 */
      GPIO_PDD_ClearPortDataOutputMask(GPTB_MODULE_BASE_ADDRESS,
        ((GPTB_TPortValue)GPTB_PTB1_GPS_ASSEMBLY_MASK)
        & ((GPTB_TPortValue)(Mask << GPTB_PTB1_GPS_ASSEMBLY_START_BIT))
      );
      break;
    }
    case PTB2_BT_ASSEMBLY: {           /* bit field #2 */
      GPIO_PDD_ClearPortDataOutputMask(GPTB_MODULE_BASE_ADDRESS,
        ((GPTB_TPortValue)GPTB_PTB2_BT_ASSEMBLY_MASK)
        & ((GPTB_TPortValue)(Mask << GPTB_PTB2_BT_ASSEMBLY_START_BIT))
      );
      break;
    }
    case PTB3_GSM_PWR_EN: {            /* bit field #3 */
      GPIO_PDD_ClearPortDataOutputMask(GPTB_MODULE_BASE_ADDRESS,
        ((GPTB_TPortValue)GPTB_PTB3_GSM_PWR_EN_MASK)
        & ((GPTB_TPortValue)(Mask << GPTB_PTB3_GSM_PWR_EN_START_BIT))
      );
      break;
    }
    case PTB16_GSM_PWRKEY: {           /* bit field #4 */
      GPIO_PDD_ClearPortDataOutputMask(GPTB_MODULE_BASE_ADDRESS,
        ((GPTB_TPortValue)GPTB_PTB16_GSM_PWRKEY_MASK)
        & ((GPTB_TPortValue)(Mask << GPTB_PTB16_GSM_PWRKEY_START_BIT))
      );
      break;
    }
    case PTB17_GPS_WAKE_UP: {          /* bit field #5 */
      GPIO_PDD_ClearPortDataOutputMask(GPTB_MODULE_BASE_ADDRESS,
        ((GPTB_TPortValue)GPTB_PTB17_GPS_WAKE_UP_MASK)
        & ((GPTB_TPortValue)(Mask << GPTB_PTB17_GPS_WAKE_UP_START_BIT))
      );
      break;
    }
    case PTB18_GPS_TIMEPULSE: {        /* bit field #6 */
      GPIO_PDD_ClearPortDataOutputMask(GPTB_MODULE_BASE_ADDRESS,
        ((GPTB_TPortValue)GPTB_PTB18_GPS_TIMEPULSE_MASK)
        & ((GPTB_TPortValue)(Mask << GPTB_PTB18_GPS_TIMEPULSE_START_BIT))
      );
      break;
    }
    case PTB19_GPS_PWR_EN: {           /* bit field #7 */
      GPIO_PDD_ClearPortDataOutputMask(GPTB_MODULE_BASE_ADDRESS,
        ((GPTB_TPortValue)GPTB_PTB19_GPS_PWR_EN_MASK)
        & ((GPTB_TPortValue)(Mask << GPTB_PTB19_GPS_PWR_EN_START_BIT))
      );
      break;
    }
    default:
      break;                           /* Invalid Field is not treated, result is undefined */
  } /* switch (Field) */
}

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
void GPTB_SetFieldBits(LDD_TDeviceData *DeviceDataPtr, LDD_GPIO_TBitField Field, GPTB_TFieldValue Mask)
{
  (void)DeviceDataPtr;                 /* Parameter is not used, suppress unused argument warning */
  switch (Field) {                     /* no break */
    case PTB0_GSM_ASSEMBLY: {          /* bit field #0 */
      GPIO_PDD_SetPortDataOutputMask(GPTB_MODULE_BASE_ADDRESS,
        ((GPTB_TPortValue)GPTB_PTB0_GSM_ASSEMBLY_MASK)
        & Mask
      );
      break;
    }
    case PTB1_GPS_ASSEMBLY: {          /* bit field #1 */
      GPIO_PDD_SetPortDataOutputMask(GPTB_MODULE_BASE_ADDRESS,
        ((GPTB_TPortValue)GPTB_PTB1_GPS_ASSEMBLY_MASK)
        & ((GPTB_TPortValue)(Mask << GPTB_PTB1_GPS_ASSEMBLY_START_BIT))
      );
      break;
    }
    case PTB2_BT_ASSEMBLY: {           /* bit field #2 */
      GPIO_PDD_SetPortDataOutputMask(GPTB_MODULE_BASE_ADDRESS,
        ((GPTB_TPortValue)GPTB_PTB2_BT_ASSEMBLY_MASK)
        & ((GPTB_TPortValue)(Mask << GPTB_PTB2_BT_ASSEMBLY_START_BIT))
      );
      break;
    }
    case PTB3_GSM_PWR_EN: {            /* bit field #3 */
      GPIO_PDD_SetPortDataOutputMask(GPTB_MODULE_BASE_ADDRESS,
        ((GPTB_TPortValue)GPTB_PTB3_GSM_PWR_EN_MASK)
        & ((GPTB_TPortValue)(Mask << GPTB_PTB3_GSM_PWR_EN_START_BIT))
      );
      break;
    }
    case PTB16_GSM_PWRKEY: {           /* bit field #4 */
      GPIO_PDD_SetPortDataOutputMask(GPTB_MODULE_BASE_ADDRESS,
        ((GPTB_TPortValue)GPTB_PTB16_GSM_PWRKEY_MASK)
        & ((GPTB_TPortValue)(Mask << GPTB_PTB16_GSM_PWRKEY_START_BIT))
      );
      break;
    }
    case PTB17_GPS_WAKE_UP: {          /* bit field #5 */
      GPIO_PDD_SetPortDataOutputMask(GPTB_MODULE_BASE_ADDRESS,
        ((GPTB_TPortValue)GPTB_PTB17_GPS_WAKE_UP_MASK)
        & ((GPTB_TPortValue)(Mask << GPTB_PTB17_GPS_WAKE_UP_START_BIT))
      );
      break;
    }
    case PTB18_GPS_TIMEPULSE: {        /* bit field #6 */
      GPIO_PDD_SetPortDataOutputMask(GPTB_MODULE_BASE_ADDRESS,
        ((GPTB_TPortValue)GPTB_PTB18_GPS_TIMEPULSE_MASK)
        & ((GPTB_TPortValue)(Mask << GPTB_PTB18_GPS_TIMEPULSE_START_BIT))
      );
      break;
    }
    case PTB19_GPS_PWR_EN: {           /* bit field #7 */
      GPIO_PDD_SetPortDataOutputMask(GPTB_MODULE_BASE_ADDRESS,
        ((GPTB_TPortValue)GPTB_PTB19_GPS_PWR_EN_MASK)
        & ((GPTB_TPortValue)(Mask << GPTB_PTB19_GPS_PWR_EN_START_BIT))
      );
      break;
    }
    default:
      break;                           /* Invalid Field is not treated, result is undefined */
  } /* switch (Field) */
}

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
void GPTB_ToggleFieldBits(LDD_TDeviceData *DeviceDataPtr, LDD_GPIO_TBitField Field, GPTB_TFieldValue Mask)
{
  (void)DeviceDataPtr;                 /* Parameter is not used, suppress unused argument warning */
  switch (Field) {                     /* no break */
    case PTB0_GSM_ASSEMBLY: {          /* bit field #0 */
      GPIO_PDD_TogglePortDataOutputMask(GPTB_MODULE_BASE_ADDRESS,
        ((GPTB_TPortValue)GPTB_PTB0_GSM_ASSEMBLY_MASK)
        & Mask
      );
      break;
    }
    case PTB1_GPS_ASSEMBLY: {          /* bit field #1 */
      GPIO_PDD_TogglePortDataOutputMask(GPTB_MODULE_BASE_ADDRESS,
        ((GPTB_TPortValue)GPTB_PTB1_GPS_ASSEMBLY_MASK)
        & ((GPTB_TPortValue)(Mask << GPTB_PTB1_GPS_ASSEMBLY_START_BIT))
      );
      break;
    }
    case PTB2_BT_ASSEMBLY: {           /* bit field #2 */
      GPIO_PDD_TogglePortDataOutputMask(GPTB_MODULE_BASE_ADDRESS,
        ((GPTB_TPortValue)GPTB_PTB2_BT_ASSEMBLY_MASK)
        & ((GPTB_TPortValue)(Mask << GPTB_PTB2_BT_ASSEMBLY_START_BIT))
      );
      break;
    }
    case PTB3_GSM_PWR_EN: {            /* bit field #3 */
      GPIO_PDD_TogglePortDataOutputMask(GPTB_MODULE_BASE_ADDRESS,
        ((GPTB_TPortValue)GPTB_PTB3_GSM_PWR_EN_MASK)
        & ((GPTB_TPortValue)(Mask << GPTB_PTB3_GSM_PWR_EN_START_BIT))
      );
      break;
    }
    case PTB16_GSM_PWRKEY: {           /* bit field #4 */
      GPIO_PDD_TogglePortDataOutputMask(GPTB_MODULE_BASE_ADDRESS,
        ((GPTB_TPortValue)GPTB_PTB16_GSM_PWRKEY_MASK)
        & ((GPTB_TPortValue)(Mask << GPTB_PTB16_GSM_PWRKEY_START_BIT))
      );
      break;
    }
    case PTB17_GPS_WAKE_UP: {          /* bit field #5 */
      GPIO_PDD_TogglePortDataOutputMask(GPTB_MODULE_BASE_ADDRESS,
        ((GPTB_TPortValue)GPTB_PTB17_GPS_WAKE_UP_MASK)
        & ((GPTB_TPortValue)(Mask << GPTB_PTB17_GPS_WAKE_UP_START_BIT))
      );
      break;
    }
    case PTB18_GPS_TIMEPULSE: {        /* bit field #6 */
      GPIO_PDD_TogglePortDataOutputMask(GPTB_MODULE_BASE_ADDRESS,
        ((GPTB_TPortValue)GPTB_PTB18_GPS_TIMEPULSE_MASK)
        & ((GPTB_TPortValue)(Mask << GPTB_PTB18_GPS_TIMEPULSE_START_BIT))
      );
      break;
    }
    case PTB19_GPS_PWR_EN: {           /* bit field #7 */
      GPIO_PDD_TogglePortDataOutputMask(GPTB_MODULE_BASE_ADDRESS,
        ((GPTB_TPortValue)GPTB_PTB19_GPS_PWR_EN_MASK)
        & ((GPTB_TPortValue)(Mask << GPTB_PTB19_GPS_PWR_EN_START_BIT))
      );
      break;
    }
    default:
      break;                           /* Invalid Field is not treated, result is undefined */
  } /* switch (Field) */
}

/* END GPTB. */

#ifdef __cplusplus
}  /* extern "C" */
#endif 

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
