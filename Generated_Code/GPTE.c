/* ###################################################################
**     THIS COMPONENT MODULE IS GENERATED BY THE TOOL. DO NOT MODIFY IT.
**     Filename    : GPTE.c
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
**          Component name                                 : GPTE
**          Port                                           : PTE
**          Port width                                     : 32 bits
**          Mask of allocated pins                         : E0000000
**          Interrupt service/event                        : Disabled
**          Bit fields                                     : 3
**            Bit field                                    : 
**              Field name                                 : PTE29_PWR_OFF
**              Pins                                       : 1
**                Pin                                      : 
**                  Pin                                    : CMP0_IN5/ADC0_SE4b/PTE29/TPM0_CH2/TPM_CLKIN0
**                  Pin signal                             : 
**                  Initial pin direction                  : Output
**                    Initial output state                 : 0
**                  Initial pin event                      : Disabled
**            Bit field                                    : 
**              Field name                                 : PTE30_GSM_RESETB
**              Pins                                       : 1
**                Pin                                      : 
**                  Pin                                    : DAC0_OUT/ADC0_SE23/CMP0_IN4/PTE30/TPM0_CH3/TPM_CLKIN1
**                  Pin signal                             : 
**                  Initial pin direction                  : Output
**                    Initial output state                 : 0
**                  Initial pin event                      : Disabled
**            Bit field                                    : 
**              Field name                                 : PTE31_GSM_ON_IND
**              Pins                                       : 1
**                Pin                                      : 
**                  Pin                                    : PTE31/TPM0_CH4
**                  Pin signal                             : 
**                  Initial pin direction                  : Input
**                  Initial pin event                      : Disabled
**          Initialization                                 : 
**            Auto initialization                          : yes
**            Event mask                                   : 
**              OnPortEvent                                : Disabled
**     Contents    :
**         Init            - LDD_TDeviceData* GPTE_Init(LDD_TUserData *UserDataPtr);
**         SetFieldValue   - void GPTE_SetFieldValue(LDD_TDeviceData *DeviceDataPtr, LDD_GPIO_TBitField...
**         GetFieldValue   - GPTE_TFieldValue GPTE_GetFieldValue(LDD_TDeviceData *DeviceDataPtr,...
**         ClearFieldBits  - void GPTE_ClearFieldBits(LDD_TDeviceData *DeviceDataPtr, LDD_GPIO_TBitField...
**         SetFieldBits    - void GPTE_SetFieldBits(LDD_TDeviceData *DeviceDataPtr, LDD_GPIO_TBitField...
**         ToggleFieldBits - void GPTE_ToggleFieldBits(LDD_TDeviceData *DeviceDataPtr, LDD_GPIO_TBitField...
**
**     Copyright : 1997 - 2013 Freescale Semiconductor, Inc. All Rights Reserved.
**     SOURCE DISTRIBUTION PERMISSIBLE as directed in End User License Agreement.
**     
**     http      : www.freescale.com
**     mail      : support@freescale.com
** ###################################################################*/
/*!
** @file GPTE.c
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
**  @addtogroup GPTE_module GPTE module documentation
**  @{
*/         

/* MODULE GPTE. */

#include "GPTE.h"
/* {Default RTOS Adapter} No RTOS includes */
#include "IO_Map.h"

#ifdef __cplusplus
extern "C" {
#endif 

typedef struct {
  LDD_TUserData *UserData;             /* RTOS device data structure */
} GPTE_TDeviceData, *GPTE_TDeviceDataPtr; /* Device data structure type */
/* {Default RTOS Adapter} Static object used for simulation of dynamic driver memory allocation */
static GPTE_TDeviceData DeviceDataPrv__DEFAULT_RTOS_ALLOC;
/*
** ===================================================================
**     Method      :  GPTE_Init (component GPIO_LDD)
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
LDD_TDeviceData* GPTE_Init(LDD_TUserData *UserDataPtr)
{
  /* Allocate LDD device structure */
  GPTE_TDeviceData *DeviceDataPrv;

  /* {Default RTOS Adapter} Driver memory allocation: Dynamic allocation is simulated by a pointer to the static object */
  DeviceDataPrv = &DeviceDataPrv__DEFAULT_RTOS_ALLOC;
  /* Save RTOS Device structure */
  DeviceDataPrv->UserData = UserDataPtr; /* Store the RTOS device structure */
  /* GPIOE_PDOR: PDO&=~0x60000000 */
  GPIOE_PDOR &= (uint32_t)~(uint32_t)(GPIO_PDOR_PDO(0x60000000));                                   
  /* GPIOE_PDDR: PDD&=~0x80000000,PDD|=0x60000000 */
  GPIOE_PDDR = (uint32_t)((GPIOE_PDDR & (uint32_t)~(uint32_t)(
                GPIO_PDDR_PDD(0x80000000)
               )) | (uint32_t)(
                GPIO_PDDR_PDD(0x60000000)
               ));                                  
  /* Initialization of Port Control registers */
  /* PORTE_PCR29: ISF=0,MUX=1 */
  PORTE_PCR29 = (uint32_t)((PORTE_PCR29 & (uint32_t)~(uint32_t)(
                 PORT_PCR_ISF_MASK |
                 PORT_PCR_MUX(0x06)
                )) | (uint32_t)(
                 PORT_PCR_MUX(0x01)
                ));                                  
  /* PORTE_PCR30: ISF=0,MUX=1 */
  PORTE_PCR30 = (uint32_t)((PORTE_PCR30 & (uint32_t)~(uint32_t)(
                 PORT_PCR_ISF_MASK |
                 PORT_PCR_MUX(0x06)
                )) | (uint32_t)(
                 PORT_PCR_MUX(0x01)
                ));                                  
  /* PORTE_PCR31: ISF=0,MUX=1 */
  PORTE_PCR31 = (uint32_t)((PORTE_PCR31 & (uint32_t)~(uint32_t)(
                 PORT_PCR_ISF_MASK |
                 PORT_PCR_MUX(0x06)
                )) | (uint32_t)(
                 PORT_PCR_MUX(0x01)
                ));                                  
  /* Registration of the device structure */
  PE_LDD_RegisterDeviceStructure(PE_LDD_COMPONENT_GPTE_ID,DeviceDataPrv);
  return ((LDD_TDeviceData *)DeviceDataPrv);
}

/*
** ===================================================================
**     Method      :  GPTE_SetFieldValue (component GPIO_LDD)
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
void GPTE_SetFieldValue(LDD_TDeviceData *DeviceDataPtr, LDD_GPIO_TBitField Field, GPTE_TFieldValue Value)
{
  (void)DeviceDataPtr;                 /* Parameter is not used, suppress unused argument warning */
  switch (Field) {                     /* no break */
    case PTE29_PWR_OFF: {              /* bit field #0 */
      GPIO_PDD_SetPortDataOutput(GPTE_MODULE_BASE_ADDRESS,
        (
          GPIO_PDD_GetPortDataOutput(GPTE_MODULE_BASE_ADDRESS)
          & ((GPTE_TPortValue)(~((GPTE_TPortValue)GPTE_PTE29_PWR_OFF_MASK)))
        )
        | (
          ((GPTE_TPortValue)(Value << GPTE_PTE29_PWR_OFF_START_BIT))
          & ((GPTE_TPortValue)GPTE_PTE29_PWR_OFF_MASK)
        )
      );
      break;
    }
    case PTE30_GSM_RESETB: {           /* bit field #1 */
      GPIO_PDD_SetPortDataOutput(GPTE_MODULE_BASE_ADDRESS,
        (
          GPIO_PDD_GetPortDataOutput(GPTE_MODULE_BASE_ADDRESS)
          & ((GPTE_TPortValue)(~((GPTE_TPortValue)GPTE_PTE30_GSM_RESETB_MASK)))
        )
        | (
          ((GPTE_TPortValue)(Value << GPTE_PTE30_GSM_RESETB_START_BIT))
          & ((GPTE_TPortValue)GPTE_PTE30_GSM_RESETB_MASK)
        )
      );
      break;
    }
    case PTE31_GSM_ON_IND: {           /* bit field #2 */
      GPIO_PDD_SetPortDataOutput(GPTE_MODULE_BASE_ADDRESS,
        (
          GPIO_PDD_GetPortDataOutput(GPTE_MODULE_BASE_ADDRESS)
          & ((GPTE_TPortValue)(~((GPTE_TPortValue)GPTE_PTE31_GSM_ON_IND_MASK)))
        )
        | (
          ((GPTE_TPortValue)(Value << GPTE_PTE31_GSM_ON_IND_START_BIT))
          & ((GPTE_TPortValue)GPTE_PTE31_GSM_ON_IND_MASK)
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
**     Method      :  GPTE_GetFieldValue (component GPIO_LDD)
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
GPTE_TFieldValue GPTE_GetFieldValue(LDD_TDeviceData *DeviceDataPtr, LDD_GPIO_TBitField Field)
{
  (void)DeviceDataPtr;                 /* Parameter is not used, suppress unused argument warning */
  switch (Field) {                     /* no break */
    case PTE29_PWR_OFF: {              /* bit field #0 */
      return
        (GPTE_TFieldValue)(
          (
            GPIO_PDD_GetPortDataInput(GPTE_MODULE_BASE_ADDRESS)
            & (GPTE_TPortValue)GPTE_PTE29_PWR_OFF_MASK
          )
          >> GPTE_PTE29_PWR_OFF_START_BIT
        );
    }
    case PTE30_GSM_RESETB: {           /* bit field #1 */
      return
        (GPTE_TFieldValue)(
          (
            GPIO_PDD_GetPortDataInput(GPTE_MODULE_BASE_ADDRESS)
            & (GPTE_TPortValue)GPTE_PTE30_GSM_RESETB_MASK
          )
          >> GPTE_PTE30_GSM_RESETB_START_BIT
        );
    }
    case PTE31_GSM_ON_IND: {           /* bit field #2 */
      return
        (GPTE_TFieldValue)(
          (
            GPIO_PDD_GetPortDataInput(GPTE_MODULE_BASE_ADDRESS)
            & (GPTE_TPortValue)GPTE_PTE31_GSM_ON_IND_MASK
          )
          >> GPTE_PTE31_GSM_ON_IND_START_BIT
        );
    }
    default:
      break;                           /* Invalid BitField is not treated, result is undefined */
  } /* switch (Field) */
  return (GPTE_TFieldValue)0U;
}

/*
** ===================================================================
**     Method      :  GPTE_ClearFieldBits (component GPIO_LDD)
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
void GPTE_ClearFieldBits(LDD_TDeviceData *DeviceDataPtr, LDD_GPIO_TBitField Field, GPTE_TFieldValue Mask)
{
  (void)DeviceDataPtr;                 /* Parameter is not used, suppress unused argument warning */
  switch (Field) {                     /* no break */
    case PTE29_PWR_OFF: {              /* bit field #0 */
      GPIO_PDD_ClearPortDataOutputMask(GPTE_MODULE_BASE_ADDRESS,
        ((GPTE_TPortValue)GPTE_PTE29_PWR_OFF_MASK)
        & ((GPTE_TPortValue)(Mask << GPTE_PTE29_PWR_OFF_START_BIT))
      );
      break;
    }
    case PTE30_GSM_RESETB: {           /* bit field #1 */
      GPIO_PDD_ClearPortDataOutputMask(GPTE_MODULE_BASE_ADDRESS,
        ((GPTE_TPortValue)GPTE_PTE30_GSM_RESETB_MASK)
        & ((GPTE_TPortValue)(Mask << GPTE_PTE30_GSM_RESETB_START_BIT))
      );
      break;
    }
    case PTE31_GSM_ON_IND: {           /* bit field #2 */
      GPIO_PDD_ClearPortDataOutputMask(GPTE_MODULE_BASE_ADDRESS,
        ((GPTE_TPortValue)GPTE_PTE31_GSM_ON_IND_MASK)
        & ((GPTE_TPortValue)(Mask << GPTE_PTE31_GSM_ON_IND_START_BIT))
      );
      break;
    }
    default:
      break;                           /* Invalid Field is not treated, result is undefined */
  } /* switch (Field) */
}

/*
** ===================================================================
**     Method      :  GPTE_SetFieldBits (component GPIO_LDD)
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
void GPTE_SetFieldBits(LDD_TDeviceData *DeviceDataPtr, LDD_GPIO_TBitField Field, GPTE_TFieldValue Mask)
{
  (void)DeviceDataPtr;                 /* Parameter is not used, suppress unused argument warning */
  switch (Field) {                     /* no break */
    case PTE29_PWR_OFF: {              /* bit field #0 */
      GPIO_PDD_SetPortDataOutputMask(GPTE_MODULE_BASE_ADDRESS,
        ((GPTE_TPortValue)GPTE_PTE29_PWR_OFF_MASK)
        & ((GPTE_TPortValue)(Mask << GPTE_PTE29_PWR_OFF_START_BIT))
      );
      break;
    }
    case PTE30_GSM_RESETB: {           /* bit field #1 */
      GPIO_PDD_SetPortDataOutputMask(GPTE_MODULE_BASE_ADDRESS,
        ((GPTE_TPortValue)GPTE_PTE30_GSM_RESETB_MASK)
        & ((GPTE_TPortValue)(Mask << GPTE_PTE30_GSM_RESETB_START_BIT))
      );
      break;
    }
    case PTE31_GSM_ON_IND: {           /* bit field #2 */
      GPIO_PDD_SetPortDataOutputMask(GPTE_MODULE_BASE_ADDRESS,
        ((GPTE_TPortValue)GPTE_PTE31_GSM_ON_IND_MASK)
        & ((GPTE_TPortValue)(Mask << GPTE_PTE31_GSM_ON_IND_START_BIT))
      );
      break;
    }
    default:
      break;                           /* Invalid Field is not treated, result is undefined */
  } /* switch (Field) */
}

/*
** ===================================================================
**     Method      :  GPTE_ToggleFieldBits (component GPIO_LDD)
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
void GPTE_ToggleFieldBits(LDD_TDeviceData *DeviceDataPtr, LDD_GPIO_TBitField Field, GPTE_TFieldValue Mask)
{
  (void)DeviceDataPtr;                 /* Parameter is not used, suppress unused argument warning */
  switch (Field) {                     /* no break */
    case PTE29_PWR_OFF: {              /* bit field #0 */
      GPIO_PDD_TogglePortDataOutputMask(GPTE_MODULE_BASE_ADDRESS,
        ((GPTE_TPortValue)GPTE_PTE29_PWR_OFF_MASK)
        & ((GPTE_TPortValue)(Mask << GPTE_PTE29_PWR_OFF_START_BIT))
      );
      break;
    }
    case PTE30_GSM_RESETB: {           /* bit field #1 */
      GPIO_PDD_TogglePortDataOutputMask(GPTE_MODULE_BASE_ADDRESS,
        ((GPTE_TPortValue)GPTE_PTE30_GSM_RESETB_MASK)
        & ((GPTE_TPortValue)(Mask << GPTE_PTE30_GSM_RESETB_START_BIT))
      );
      break;
    }
    case PTE31_GSM_ON_IND: {           /* bit field #2 */
      GPIO_PDD_TogglePortDataOutputMask(GPTE_MODULE_BASE_ADDRESS,
        ((GPTE_TPortValue)GPTE_PTE31_GSM_ON_IND_MASK)
        & ((GPTE_TPortValue)(Mask << GPTE_PTE31_GSM_ON_IND_START_BIT))
      );
      break;
    }
    default:
      break;                           /* Invalid Field is not treated, result is undefined */
  } /* switch (Field) */
}

/* END GPTE. */

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
