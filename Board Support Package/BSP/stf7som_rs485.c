/**
  ******************************************************************************
  * @file    stf7som_rs485
  * @author  yldzmuhammed
  * @version V1.0
  * @date    13-06-2020
  * @brief   This file contains the common defines and functions prototypes for
  *          the stf7som_rs485.c driver.
  ******************************************************************************
  * @attention
  *
  * experiment platform: YiLDiZ IoT STF7-SoM Board BSP Library
  * web-site: https://www.yildiziot.com
  ******************************************************************************
*/



/* Dependencies
- stm32f7xx_hal_uart.c
- stm32f7xx_hal_gpio.c
- stm32f7xx_hal_rcc_ex.h
EndDependencies */

/* Includes ------------------------------------------------------------------*/
#include "stf7som.h"
#include "stf7som_rs485.h"

/** @addtogroup BSP
  * @{
  */

/** @addtogroup STM32756G_EVAL
  * @{
  */ 
  
/** @defgroup STF7SoM_RS485 STM32756G_EVAL RS485
  * @{
  */ 
  
/* Private typedef -----------------------------------------------------------*/

/** @defgroup STF7SoM_RS485_Private_Types_Definitions RS485 Private Types Definitions
  * @{
  */ 
/**
  * @}
  */  
/* Private define ------------------------------------------------------------*/
/** @defgroup STF7SoM_RS485_Private_Defines RS485 Private Defines
  * @{
  */
/**
  * @}
  */  
/* Private macro -------------------------------------------------------------*/

/** @defgroup STF7SoM_RS485_Private_Macros RS485 Private Macros
  * @{
  */  
/**
  * @}
  */  
/* Private variables ---------------------------------------------------------*/

/** @defgroup STF7SoM_RS485_Private_Variables RS485 Private Variables
  * @{
  */
UART_HandleTypeDef huart3;
/**
  * @}
  */ 

/* Private function prototypes -----------------------------------------------*/

/** @defgroup STF7SoM_RS485_Private_Functions_Prototypes RS485 Private Functions Prototypes
  * @{
  */ 
/**
  * @}
  */ 
/* Private functions ---------------------------------------------------------*/
    
/** @defgroup STF7SoM_RS485_Private_Functions RS485 Private Functions
  * @{
  */

/**
  * @brief  Initializes the RS485 device.
  * @retval RS485 status
  */
uint8_t BSP_RS485_Init(uint32_t baudrate)
{
    huart3.Instance             = USART3;
    huart3.Init.BaudRate        = baudrate;
    huart3.Init.StopBits        = UART_STOPBITS_1;
    huart3.Init.Mode            = UART_MODE_TX_RX;
    huart3.Init.Parity          = UART_PARITY_NONE;
    huart3.Init.WordLength      = UART_WORDLENGTH_8B;
    huart3.Init.OverSampling    = UART_OVERSAMPLING_16;
    huart3.Init.HwFlowCtl       = UART_HWCONTROL_RTS_CTS;

    return HAL_UART_Init(&huart3);
}
//

/**
  * @brief  DeInitializes the UART device.
  * @retval NOR status
  */
uint8_t BSP_RS485_DeInit(void)
{
  return HAL_UART_DeInit(&huart3);
}
//

/**
  * @brief  Reads an amount of data from the RS485 device.
  * @param  uwStartAddress: Read start address
  * @param  pData: Pointer to data to be read
  * @param  uwDataSize: Size of data to read    
  * @retval NOR memory status
  */
void BSP_RS485_WriteData(uint32_t uwStartAddress, uint8_t* pData, uint32_t uwDataSize)
{
    BSP_RS485_ChangeMode(eRS485Mode_Tranciever);
    
    HAL_UART_Transmit(&huart3, pData, uwDataSize, 1000);
    
    BSP_RS485_ChangeMode(eRS485Mode_Receiver);
}
//

/**
  * @brief  Reads an amount of data from the NOR device.
  * @param  uwStartAddress: Read start address
  * @param  pData: Pointer to data to be read
  * @param  uwDataSize: Size of data to read    
  * @retval NOR memory status
  */
void BSP_RS485_ReadData(uint32_t uwStartAddress, uint8_t* pData, uint32_t *uwDataSize)
{
    BSP_RS485_ChangeMode(eRS485Mode_Receiver);
    
    HAL_UART_Receive(&huart3, pData, *uwDataSize, 1000);
}
//

/**
  * @brief  Returns the NOR memory to read mode.
  * @retval None
  */
void BSP_RS485_ChangeMode(eRS485Mode_TypeDef emode)
{
    HAL_Delay(1);
    
    if(emode == eRS485Mode_Receiver)
        HAL_GPIO_WritePin(RS485_DE_GPIO_Port, RS485_DE_Pin, GPIO_PIN_RESET);
    else if(emode == eRS485Mode_Tranciever)
        HAL_GPIO_WritePin(RS485_DE_GPIO_Port, RS485_DE_Pin, GPIO_PIN_SET);
    
    HAL_Delay(1);
}
//

/**
  * @}
  */  
  
/**
  * @}
  */ 
  
/**
  * @}
  */ 
  
/**
  * @}
  */ 


/************************ (C) COPYRIGHT YiLDiZ IoT END OF FILE ************************/
