/* USER CODE BEGIN Header */
/**
  ******************************************************************************
  * @file    stm32f7xx_it.c
  * @brief   Interrupt Service Routines.
  ******************************************************************************
  * @attention
  *
  * <h2><center>&copy; Copyright (c) 2020 STMicroelectronics.
  * All rights reserved.</center></h2>
  *
  * This software component is licensed by ST under BSD 3-Clause license,
  * the "License"; You may not use this file except in compliance with the
  * License. You may obtain a copy of the License at:
  *                        opensource.org/licenses/BSD-3-Clause
  *
  ******************************************************************************
  */
/* USER CODE END Header */

/* Includes ------------------------------------------------------------------*/
#include "main.h"
#include "faulthandler.h"
#include "stm32f7xx_it.h"
/* Private includes ----------------------------------------------------------*/
/* Private typedef -----------------------------------------------------------*/
/* Private define ------------------------------------------------------------*/
/* Private macro -------------------------------------------------------------*/
/* Private variables ---------------------------------------------------------*/
/* Private function prototypes -----------------------------------------------*/
/* Private user code ---------------------------------------------------------*/
/* CAN handler declared in "main.c" file */
extern CAN_HandleTypeDef CanHandle;
/* External variables --------------------------------------------------------*/

/******************************************************************************/
/*           Cortex-M7 Processor Interruption and Exception Handlers          */ 
/******************************************************************************/
/**
  * @brief This function handles Non maskable interrupt.
  */
void NMI_Handler(void)
{
}

/**
  * @brief This function handles Hard fault interrupt.
  */
void HardFault_Handler(void)
{
  while (1)
  {
        register uint32_t LR __ASM("lr");
	
        __asm
        (
            "TST    LR, #0x04;      	"
            "ITE    eq;               "
            "MRSEQ  r0, MSP;          "
            "MRSNE  r0, PSP;          "
            "MOV    r1, LR;           "
            "BL			ReportHardFault;	"
        );
        /* Breakpoint. */
        __asm volatile("BKPT #0");

  }
}

/**
  * @brief This function handles Memory management fault.
  */
void MemManage_Handler(void)
{
  while (1)
  {
  }
}

/**
  * @brief This function handles Pre-fetch fault, memory access fault.
  */
void BusFault_Handler(void)
{
  while (1)
  {
  }
}

/**
  * @brief This function handles Undefined instruction or illegal state.
  */
void UsageFault_Handler(void)
{
  while (1)
  {
  }
}

/**
  * @brief This function handles System service call via SWI instruction.
  */
void SVC_Handler(void)
{
}

/**
  * @brief This function handles Debug monitor.
  */
void DebugMon_Handler(void)
{
}

/**
  * @brief This function handles Pendable request for system service.
  */
void PendSV_Handler(void)
{
}

/**
  * @brief  This function handles SysTick Handler.
  * @param  None
  * @retval None
  */
void SysTick_Handler(void)
{
  HAL_IncTick();
}
/******************************************************************************/
/* STM32F7xx Peripheral Interrupt Handlers                                    */
/* Add here the Interrupt Handlers for the used peripherals.                  */
/* For the available peripheral interrupt handler names,                      */
/* please refer to the startup file (startup_stm32f7xx.s).                    */
/******************************************************************************/

/**
* @brief  This function handles CAN1 RX0 interrupt request.
* @param  None
* @retval None
*/
void CAN1_RX0_IRQHandler(void)
{
  HAL_CAN_IRQHandler(&CanHandle);
}


/**
* @brief  This function handles CAN1 RX1 interrupt request.
* @param  None
* @retval None
*/
void CAN1_RX1_IRQHandler(void)
{
  HAL_CAN_IRQHandler(&CanHandle);
}


/**
* @brief  This function handles CAN1 TX interrupt request.
* @param  None
* @retval None
*/
void CAN1_TX_IRQHandler(void)
{
  HAL_CAN_IRQHandler(&CanHandle);
}

/**
  * @brief This function handles RCC global interrupt.
  */
void RCC_IRQHandler(void)
{
  __breakpoint(0);
}

/************************ (C) COPYRIGHT STMicroelectronics *****END OF FILE****/
