/* ###################################################################
**     Filename    : Events.c
**     Project     : ProcessorExpert
**     Processor   : MKL26Z256VLH4
**     Component   : Events
**     Version     : Driver 01.00
**     Compiler    : GNU C Compiler
**     Date/Time   : 2016-07-29, 09:33, # CodeGen: 0
**     Abstract    :
**         This is user's event module.
**         Put your event handler code here.
**     Settings    :
**     Contents    :
**         Cpu_OnNMIINT - void Cpu_OnNMIINT(void);
**
** ###################################################################*/
/*!
** @file Events.c
** @version 01.00
** @brief
**         This is user's event module.
**         Put your event handler code here.
*/         
/*!
**  @addtogroup Events_module Events module documentation
**  @{
*/         
/* MODULE Events */

#include "Cpu.h"
#include "Events.h"
#include "HOST.h"
#include "uart_comm.h"

#ifdef __cplusplus
extern "C" {
#endif 


/* User includes (#include below this line is not maintained by Processor Expert) */
extern int g_timer_tick;

/*
** ===================================================================
**     Event       :  Cpu_OnNMIINT (module Events)
**
**     Component   :  Cpu [MKL26Z256LH4]
**     Description :
**         This event is called when the Non maskable interrupt had
**         occurred. This event is automatically enabled when the <NMI
**         interrupt> property is set to 'Enabled'.
**     Parameters  : None
**     Returns     : Nothing
** ===================================================================
*/
void Cpu_OnNMIINT(void)
{
  /* Write your code here ... */
}

/*
** ===================================================================
**     Event       :  uart2_OnBlockReceived (module Events)
**
**     Component   :  uart2 [Serial_LDD]
*/
/*!
**     @brief
**         This event is called when the requested number of data is
**         moved to the input buffer.
**     @param
**         UserDataPtr     - Pointer to the user or
**                           RTOS specific data. This pointer is passed
**                           as the parameter of Init method.
*/
/* ===================================================================*/
void uart2_OnBlockReceived(LDD_TUserData *UserDataPtr)
{
  /* Write your code here ... */
	GPRSStartRecv_internal();
}

/*
** ===================================================================
**     Event       :  uart2_OnBlockSent (module Events)
**
**     Component   :  uart2 [Serial_LDD]
*/
/*!
**     @brief
**         This event is called after the last character from the
**         output buffer is moved to the transmitter. 
**     @param
**         UserDataPtr     - Pointer to the user or
**                           RTOS specific data. This pointer is passed
**                           as the parameter of Init method.
*/
/* ===================================================================*/
void uart2_OnBlockSent(LDD_TUserData *UserDataPtr)
{
  /* Write your code here ... */
}

/*
** ===================================================================
**     Event       :  uart1_OnBlockReceived (module Events)
**
**     Component   :  uart1 [Serial_LDD]
*/
/*!
**     @brief
**         This event is called when the requested number of data is
**         moved to the input buffer.
**     @param
**         UserDataPtr     - Pointer to the user or
**                           RTOS specific data. This pointer is passed
**                           as the parameter of Init method.
*/
/* ===================================================================*/
void uart1_OnBlockReceived(LDD_TUserData *UserDataPtr)
{
  /* Write your code here ... */
	//log_printf("HOST buffer full. Restart recv new data from buffer head.\n");
	HOSTStartRecv_internal(1);//start recv again and mark as wrapped
}

/*
** ===================================================================
**     Event       :  uart1_OnBlockSent (module Events)
**
**     Component   :  uart1 [Serial_LDD]
*/
/*!
**     @brief
**         This event is called after the last character from the
**         output buffer is moved to the transmitter. 
**     @param
**         UserDataPtr     - Pointer to the user or
**                           RTOS specific data. This pointer is passed
**                           as the parameter of Init method.
*/
/* ===================================================================*/
void uart1_OnBlockSent(LDD_TUserData *UserDataPtr)
{
  /* Write your code here ... */
}

/*
** ===================================================================
**     Event       :  uart0_OnBlockReceived (module Events)
**
**     Component   :  uart0 [Serial_LDD]
*/
/*!
**     @brief
**         This event is called when the requested number of data is
**         moved to the input buffer.
**     @param
**         UserDataPtr     - Pointer to the user or
**                           RTOS specific data. This pointer is passed
**                           as the parameter of Init method.
*/
/* ===================================================================*/
extern int g_wifi_connected;
void uart0_OnBlockReceived(LDD_TUserData *UserDataPtr)
{
  /* Write your code here ... */
	if(g_wifi_connected)
		WiFiStartRecv_internal();
	else
		GPSStartRecv_internal();
}

/*
** ===================================================================
**     Event       :  uart0_OnBlockSent (module Events)
**
**     Component   :  uart0 [Serial_LDD]
*/
/*!
**     @brief
**         This event is called after the last character from the
**         output buffer is moved to the transmitter. 
**     @param
**         UserDataPtr     - Pointer to the user or
**                           RTOS specific data. This pointer is passed
**                           as the parameter of Init method.
*/
/* ===================================================================*/
void uart0_OnBlockSent(LDD_TUserData *UserDataPtr)
{
  /* Write your code here ... */
}

/*
** ===================================================================
**     Event       :  TI100ms_OnInterrupt (module Events)
**
**     Component   :  TI100ms [TimerInt_LDD]
*/
/*!
**     @brief
**         Called if periodic event occur. Component and OnInterrupt
**         event must be enabled. See <SetEventMask> and <GetEventMask>
**         methods. This event is available only if a <Interrupt
**         service/event> is enabled.
**     @param
**         UserDataPtr     - Pointer to the user or
**                           RTOS specific data. The pointer passed as
**                           the parameter of Init method.
*/
/* ===================================================================*/
void TI100ms_OnInterrupt(LDD_TUserData *UserDataPtr)
{
  /* Write your code here ... */
	commTimerFunc(&g_timer_tick);
}

/*
** ===================================================================
**     Event       :  InnerFlash_OnOperationComplete (module Events)
**
**     Component   :  InnerFlash [FLASH_LDD]
*/
/*!
**     @brief
**         Called at the end of the whole write / erase operation. if
**         the event is enabled. See SetEventMask() and GetEventMask()
**         methods.
**     @param
**         UserDataPtr     - Pointer to the user or
**                           RTOS specific data. This pointer is passed
**                           as the parameter of Init method.
*/
/* ===================================================================*/
extern int g_flash_complete;
void InnerFlash_OnOperationComplete(LDD_TUserData *UserDataPtr)
{
  /* Write your code here ... */
	g_flash_complete =1 ;
}

/* END Events */

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
