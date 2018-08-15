/* ###################################################################
**     Filename    : Events.c
**     Project     : KW01_TPMS_LPSCI
**     Processor   : MKW01Z128CHN4
**     Component   : Events
**     Version     : Driver 01.00
**     Compiler    : GNU C Compiler
**     Date/Time   : 2015-10-05, 14:37, # CodeGen: 0
**     Abstract    :
**         This is user's event module.
**         Put your event handler code here.
**     Settings    :
**     Contents    :
**         Cpu_OnNMI - void Cpu_OnNMI(void);
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
#include "rtos_main_task.h"

#ifdef __cplusplus
extern "C" {
#endif 


/* User includes (#include below this line is not maintained by Processor Expert) */
#include "rtos_main_task.h"
/*
** ===================================================================
**     Event       :  Cpu_OnNMI (module Events)
**
**     Component   :  Cpu [MKW01Z128HN4_4SDK]
*/
/*!
**     @brief
**         This event is called when the Non maskable interrupt had
**         occurred. This event is automatically enabled when the [NMI
**         interrupt] property is set to 'Enabled'.
*/
/* ===================================================================*/
void Cpu_OnNMI(void)
{
  /* Write your code here ... */
}

void uart1_TxCallback(uint32_t instance, void * uartState)
{
  /* Write your code here ... */
}
void uart1_RxCallback(uint32_t instance, void * uartState)
{
  /* Write your code here ... */
}

/*! spiCom1 IRQ handler */
void spiCom1_IRQHandler(void)
{
#if SPICOM1_DMA_MODE
  SPI_DRV_DmaIRQHandler(FSL_SPICOM1);
#else
  SPI_DRV_IRQHandler(FSL_SPICOM1);
#endif
  /* Write your code here ... */
}

void lpsciCom1_RxCallback(uint32_t instance, void * uartState)
{
  /* Write your code here ... */

}

void hwTim1_OnTimeOut(void* data)
{
  curTick++;
}


/*! i2cCom1 IRQ handler */
void i2cCom1_IRQHandler(void)
{
  I2C_DRV_IRQHandler(FSL_I2CCOM1);
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
**     This file was created by Processor Expert 10.5 [05.21]
**     for the Freescale Kinetis series of microcontrollers.
**
** ###################################################################
*/
