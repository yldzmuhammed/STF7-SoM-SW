/**
  ******************************************************************************
  * @file    stf7som.h
  * @author  yldzmuhammed
  * @version V1.0
  * @date    13-06-2020
  * @brief   This file contains the common defines and functions prototypes for
  *          the stf7som_nor.c driver.
  ******************************************************************************
  * @attention
  *
  * experiment platform: YiLDiZ IoT STF7-SoM Board BSP Library
  * web-site: https://www.yildiziot.com
  ******************************************************************************
*/



/* Dependencies
- stm32f7xx_hal_nor.c
- stm32f7xx_ll_fmc.c
- stm32f7xx_hal_gpio.c
- stm32f7xx_hal_rcc_ex.h
EndDependencies */

/* Includes ------------------------------------------------------------------*/
#include "stf7som_fmc.h"
#include "stf7som_nor.h"

/** @addtogroup BSP
  * @{
  */

/** @addtogroup STM32756G_EVAL
  * @{
  */ 
  
/** @defgroup STM32756G_EVAL_NOR STM32756G_EVAL NOR
  * @{
  */ 
  
/* Private typedef -----------------------------------------------------------*/

/** @defgroup STM32756G_EVAL_NOR_Private_Types_Definitions NOR Private Types Definitions
  * @{
  */ 
/**
  * @}
  */  
/* Private define ------------------------------------------------------------*/
/* Constants to define address to set to write a command */
#define NOR_CMD_ADDRESS_FIRST                 (uint16_t)0x0555
#define NOR_CMD_ADDRESS_SECOND                (uint16_t)0x02AA
#define NOR_CMD_ADDRESS_THIRD                 (uint16_t)0x0555

/* Constants to define data to program a command */
#define NOR_CMD_DATA_READ_RESET               (uint16_t)0x00F0
#define NOR_CMD_DATA_FIRST                    (uint16_t)0x00AA
#define NOR_CMD_DATA_SECOND                   (uint16_t)0x0055
/** @defgroup STM32756G_EVAL_NOR_Private_Defines NOR Private Defines
  * @{
  */
/**
  * @}
  */  
/* Private macro -------------------------------------------------------------*/

/** @defgroup STM32756G_EVAL_NOR_Private_Macros NOR Private Macros
  * @{
  */  
/**
  * @}
  */  
/* Private variables ---------------------------------------------------------*/

/** @defgroup STM32756G_EVAL_NOR_Private_Variables NOR Private Variables
  * @{
  */
static FMC_NORSRAM_TimingTypeDef  Timing;
static NOR_HandleTypeDef          norHandle;

/**
  * @}
  */ 

/* Private function prototypes -----------------------------------------------*/

/** @defgroup STM32756G_EVAL_NOR_Private_Functions_Prototypes NOR Private Functions Prototypes
  * @{
  */ 
/**
  * @}
  */ 
/* Private functions ---------------------------------------------------------*/
    
/** @defgroup STM32756G_EVAL_NOR_Private_Functions NOR Private Functions
  * @{
  */

/**
  * @brief  Initializes the NOR device.
  * @retval NOR memory status
  */
uint8_t BSP_NOR_Init(void)
{ 
  uint8_t nor_status                = NOR_STATUS_ERROR;
  
  /* Enable FMC clock */
  __HAL_RCC_FMC_CLK_ENABLE();

  norHandle.Instance                = FMC_NORSRAM_DEVICE;
  norHandle.Extended                = FMC_NORSRAM_EXTENDED_DEVICE;
  
  /* NOR device configuration */
  /* Timing configuration derived from system clock (up to 216Mhz)
     for 108Mhz as NOR clock frequency */
    
  Timing.CLKDivision                = 2;
  Timing.DataLatency                = 2;
  Timing.DataSetupTime              = 7;
  Timing.AddressHoldTime            = 1;//3;
  Timing.AddressSetupTime           = 4;
  Timing.BusTurnAroundDuration      = 1;
  Timing.AccessMode                 = FMC_ACCESS_MODE_A;
  
  norHandle.Init.NSBank             = FMC_NORSRAM_BANK1;
  norHandle.Init.PageSize           = FMC_PAGE_SIZE_NONE;
  norHandle.Init.MemoryType         = FMC_MEMORY_TYPE_NOR;
  norHandle.Init.WriteFifo          = FMC_WRITE_FIFO_DISABLE;
  norHandle.Init.WaitSignal         = FMC_WAIT_SIGNAL_DISABLE;//FMC_WAIT_SIGNAL_ENABLE;
  norHandle.Init.WriteBurst         = FMC_WRITE_BURST_DISABLE;
  norHandle.Init.ExtendedMode       = FMC_EXTENDED_MODE_DISABLE;
  norHandle.Init.WaitSignalActive   = FMC_WAIT_TIMING_BEFORE_WS;
  norHandle.Init.WriteOperation     = FMC_WRITE_OPERATION_ENABLE;
  norHandle.Init.DataAddressMux     = FMC_DATA_ADDRESS_MUX_DISABLE;
  norHandle.Init.MemoryDataWidth    = FMC_NORSRAM_MEM_BUS_WIDTH_16;
  norHandle.Init.AsynchronousWait   = FMC_ASYNCHRONOUS_WAIT_DISABLE;//FMC_ASYNCHRONOUS_WAIT_ENABLE;
  norHandle.Init.WaitSignalPolarity = FMC_WAIT_SIGNAL_POLARITY_LOW;
  norHandle.Init.BurstAccessMode    = FMC_BURST_ACCESS_MODE_DISABLE;
  norHandle.Init.ContinuousClock    = FMC_CONTINUOUS_CLOCK_SYNC_ONLY;
  
  /* NOR controller initialization */
  BSP_NOR_MspInit(&norHandle, NULL); /* __weak function can be rewritten by the application */
  
  if(HAL_NOR_Init(&norHandle, &Timing, &Timing) != HAL_OK)
  {
    nor_status = NOR_STATUS_ERROR;
  }
  else
  {
    nor_status = NOR_STATUS_OK;
  }
  return nor_status;
}
//

/**
  * @brief  DeInitializes the NOR device.
  * @retval NOR status
  */
uint8_t BSP_NOR_DeInit(void)
{
  uint8_t nor_status = NOR_ERROR;
  
  /* NOR device de-initialization */
  norHandle.Instance = FMC_NORSRAM_DEVICE;
  norHandle.Extended = FMC_NORSRAM_EXTENDED_DEVICE;

  if(HAL_NOR_DeInit(&norHandle) != HAL_OK)
  {
    nor_status = NOR_STATUS_ERROR;
  }
  else
  {
    nor_status = NOR_STATUS_OK;
  }
  
  /* NOR controller de-initialization */
  BSP_NOR_MspDeInit(&norHandle, NULL);
  
  return nor_status;
}
//

/**
  * @brief  Reads an amount of data from the NOR device.
  * @param  uwStartAddress: Read start address
  * @param  pData: Pointer to data to be read
  * @param  uwDataSize: Size of data to read    
  * @retval NOR memory status
  */
uint8_t BSP_NOR_ReadData(uint32_t uwStartAddress, uint16_t* pData, uint32_t uwDataSize)
{
  if(HAL_NOR_ReadBuffer(&norHandle, NOR_DEVICE_ADDR + uwStartAddress, pData, uwDataSize) != HAL_OK)
  {
    return NOR_STATUS_ERROR;
  }
  else
  {
    return NOR_STATUS_OK;
  }
}
//

/**
  * @brief  Reads an amount of data from the NOR device.
  * @param  uwStartAddress: Read start address
  * @param  pData: Pointer to data to be read
  * @param  uwDataSize: Size of data to read    
  * @retval NOR memory status
  */
uint8_t BSP_NOR_ReadBuffer(uint32_t uwStartAddress, uint16_t* pData, uint32_t uwDataSize)
{
  uint32_t deviceaddress = 0;
  uint32_t uwAddress = NOR_DEVICE_ADDR + uwStartAddress;
    
  /* Process Locked */
  __HAL_LOCK(&norHandle);
  
  /* Check the NOR controller state */
  if(norHandle.State == HAL_NOR_STATE_BUSY)
  {
     return HAL_BUSY;
  }
  
  /* Select the NOR device address */
  if (norHandle.Init.NSBank == FMC_NORSRAM_BANK1)
  {
    deviceaddress = NOR_MEMORY_ADRESS1;
  }
  else if (norHandle.Init.NSBank == FMC_NORSRAM_BANK2)
  {
    deviceaddress = NOR_MEMORY_ADRESS2;
  }
  else if (norHandle.Init.NSBank == FMC_NORSRAM_BANK3)
  {
    deviceaddress = NOR_MEMORY_ADRESS3;
  }
  else /* FMC_NORSRAM_BANK4 */
  {
    deviceaddress = NOR_MEMORY_ADRESS4;
  }  
    
  /* Update the NOR controller state */
  norHandle.State = HAL_NOR_STATE_BUSY;
  
  /* Send read data command */
  NOR_WRITE(NOR_ADDR_SHIFT(deviceaddress, NOR_MEMORY_16B, NOR_CMD_ADDRESS_FIRST), NOR_CMD_DATA_FIRST); 
  NOR_WRITE(NOR_ADDR_SHIFT(deviceaddress, NOR_MEMORY_16B, NOR_CMD_ADDRESS_SECOND), NOR_CMD_DATA_SECOND);  
  NOR_WRITE(NOR_ADDR_SHIFT(deviceaddress, NOR_MEMORY_16B, NOR_CMD_ADDRESS_THIRD), NOR_CMD_DATA_READ_RESET);
  
#warning "After here, first WORD is ignoring and then address is increasing one step!"
  /* Read buffer */
//  volatile uint16_t tmp = *(__IO uint16_t *)uwAddress;
//  uwAddress += 2;
  while( uwDataSize > 0) 
  {
    *pData++ = *(__IO uint16_t *)uwAddress;
    uwAddress += 2;
    uwDataSize--;
  } 
  
  /* Check the NOR controller state */
  norHandle.State = HAL_NOR_STATE_READY;
  
  /* Process unlocked */
  __HAL_UNLOCK(&norHandle);
  
  return NOR_STATUS_OK;
}
//

/**
  * @brief  Returns the NOR memory to read mode.
  * @retval None
  */
void BSP_NOR_ReturnToReadMode(void)
{
   HAL_NOR_ReturnToReadMode(&norHandle);
  uint32_t deviceaddress = 0;  
  
  /* Check the NOR controller state */
  if(norHandle.State == HAL_NOR_STATE_BUSY)
  {
     return;
  }
  
  /* Select the NOR device address */
  if (norHandle.Init.NSBank == FMC_NORSRAM_BANK1)
  {
    deviceaddress = NOR_MEMORY_ADRESS1;
  }
  else if (norHandle.Init.NSBank == FMC_NORSRAM_BANK2)
  {
    deviceaddress = NOR_MEMORY_ADRESS2;
  }
  else if (norHandle.Init.NSBank == FMC_NORSRAM_BANK3)
  {
    deviceaddress = NOR_MEMORY_ADRESS3;
  }
  else /* FMC_NORSRAM_BANK4 */
  {
    deviceaddress = NOR_MEMORY_ADRESS4;
  }  
   
  NOR_WRITE(deviceaddress, NOR_CMD_DATA_READ_RESET);

  /* Check the NOR controller state */
  norHandle.State = HAL_NOR_STATE_READY;
}
//

/**
  * @brief  Writes an amount of data to the NOR device.
  * @param  uwStartAddress: Write start address
  * @param  pData: Pointer to data to be written
  * @param  uwDataSize: Size of data to write    
  * @retval NOR memory status
  */
uint8_t BSP_NOR_WriteData(uint32_t uwStartAddress, uint16_t* pData, uint32_t uwDataSize)
{
  uint32_t index = uwDataSize;
  
  while(index > 0)
  {
    /* Write data to NOR */
    HAL_NOR_Program(&norHandle, (uint32_t *)(NOR_DEVICE_ADDR + uwStartAddress), pData);
    
    /* Read NOR device status */
    if(HAL_NOR_GetStatus(&norHandle, NOR_DEVICE_ADDR, PROGRAM_TIMEOUT) != HAL_NOR_STATUS_SUCCESS)
    {
      return NOR_STATUS_ERROR;
    }
    
    /* Update the counters */
    index--;
    pData++;
    uwStartAddress += 2;
  }
  
  return NOR_STATUS_OK;
}
//

/**
  * @brief  Programs an amount of data to the NOR device.
  * @param  uwStartAddress: Write start address
  * @param  pData: Pointer to data to be written
  * @param  uwDataSize: Size of data to write    
  * @retval NOR memory status
  */
uint8_t BSP_NOR_WriteBuffer(uint32_t uwStartAddress, uint16_t* pData, uint32_t uwDataSize)
{
  /* Send NOR program buffer operation */
  HAL_NOR_ProgramBuffer(&norHandle, (NOR_DEVICE_ADDR + uwStartAddress), pData, uwDataSize);
  
  /* Return the NOR memory status */
  if(HAL_NOR_GetStatus(&norHandle, NOR_DEVICE_ADDR, PROGRAM_TIMEOUT) != HAL_NOR_STATUS_SUCCESS)
  {
    return NOR_STATUS_ERROR;
  }
  else
  {
    return NOR_STATUS_OK;
  }
}
//

/**
  * @brief  Erases the specified block of the NOR device. 
  * @param  BlockAddress: Block address to erase  
  * @retval NOR memory status
  */
uint8_t BSP_NOR_Erase_Block(uint32_t BlockAddress)
{
  /* Send NOR erase block operation */
  HAL_NOR_Erase_Block(&norHandle, BlockAddress, NOR_DEVICE_ADDR);
  
  /* Return the NOR memory status */  
  if(HAL_NOR_GetStatus(&norHandle, NOR_DEVICE_ADDR, BLOCKERASE_TIMEOUT) != HAL_NOR_STATUS_SUCCESS)
  {
    return NOR_STATUS_ERROR;
  }
  else
  {
    return NOR_STATUS_OK;
  }
}
//

/**
  * @brief  Erases the entire NOR chip.
  * @retval NOR memory status
  */
uint8_t BSP_NOR_Erase_Chip(void)
{
  /* Send NOR Erase chip operation */
  HAL_NOR_Erase_Chip(&norHandle, NOR_DEVICE_ADDR);
  
  /* Return the NOR memory status */
  if(HAL_NOR_GetStatus(&norHandle, NOR_DEVICE_ADDR, CHIPERASE_TIMEOUT) != HAL_NOR_STATUS_SUCCESS)
  {
    return NOR_STATUS_ERROR;
  }
  else
  {
    return NOR_STATUS_OK;
  } 
}
//

/**
  * @brief  Reads NOR flash IDs.
  * @param  pNOR_ID : Pointer to NOR ID structure
  * @retval NOR memory status
  */
uint8_t BSP_NOR_Read_ID(NOR_IDTypeDef *pNOR_ID)
{
  if(HAL_NOR_Read_ID(&norHandle, pNOR_ID) != HAL_OK)
  {
    return NOR_STATUS_ERROR;
  }
  else
  {
    return NOR_STATUS_OK;
  }
}
//

/**
  * @brief  Initializes the NOR MSP.
  * @param  hnor: NOR handle  
  * @param  Params  
  * @retval None
  */
__weak void BSP_NOR_MspInit(NOR_HandleTypeDef *hnor, void *Params)
{
  GPIO_InitTypeDef gpio_init_structure;
  
  /* Common GPIO configuration */
  BSP_FMC_Init_CommonPins();
  
  /* Common GPIO configuration */
  gpio_init_structure.Pull      = GPIO_PULLUP;
  gpio_init_structure.Alternate = GPIO_AF12_FMC;
  gpio_init_structure.Mode      = GPIO_MODE_AF_PP;
  gpio_init_structure.Speed     = GPIO_SPEED_FREQ_VERY_HIGH;
  
  /* GPIOD configuration */
  gpio_init_structure.Pin       = FMC_NE_Pin  | FMC_NOE_Pin | FMC_NWE_Pin | FMC_NWAIT_Pin |
                                  FMC_A17_Pin | FMC_A18_Pin | FMC_A16_Pin;
  HAL_GPIO_Init(GPIOD, &gpio_init_structure);

  /* GPIOE configuration */  
  gpio_init_structure.Pin       = FMC_A19_Pin | FMC_A20_Pin   | FMC_A21_Pin;
  HAL_GPIO_Init(GPIOE, &gpio_init_structure);
  
  /* GPIOG configuration */  
  gpio_init_structure.Pin       = FMC_A13_Pin;
  HAL_GPIO_Init(GPIOG, &gpio_init_structure); 
}
//

/**
  * @brief  DeInitializes NOR MSP.
  * @param  hnor: NOR handle
  * @param  Params  
  * @retval None
  */
__weak void BSP_NOR_MspDeInit(NOR_HandleTypeDef  *hnor, void *Params)
{  
    /* GPIO pins clock, FMC clock can be shut down in the application
       by surcharging this __weak function */
  
  __HAL_RCC_FMC_CLK_DISABLE();
  
  BSP_FMC_DeInit_CommonPins();
  
  
  HAL_GPIO_DeInit(GPIOD,  FMC_NE_Pin  | FMC_NOE_Pin | FMC_NWE_Pin | FMC_NWAIT_Pin |
                          FMC_A17_Pin | FMC_A18_Pin | FMC_A16_Pin);

  HAL_GPIO_DeInit(GPIOE,  FMC_A19_Pin | FMC_A20_Pin   | FMC_A21_Pin);

  HAL_GPIO_DeInit(GPIOG,  FMC_A13_Pin);
}
//

/**
  * @brief  NOR BSP Wait for Ready/Busy signal.
  * @param  hnor: Pointer to NOR handle
  * @param  Timeout: Timeout duration  
  * @retval None
  */
void HAL_NOR_MspWait(NOR_HandleTypeDef *hnor, uint32_t Timeout)
{
  uint32_t timeout = Timeout;
  /* Polling on Ready/Busy signal */
  while((HAL_GPIO_ReadPin(NOR_READY_BUSY_GPIO, NOR_READY_BUSY_PIN) != NOR_BUSY_STATE) && (timeout > 0)) 
  {
    timeout--;
  }
  
  timeout = Timeout;
  /* Polling on Ready/Busy signal */
  while((HAL_GPIO_ReadPin(NOR_READY_BUSY_GPIO, NOR_READY_BUSY_PIN) != NOR_READY_STATE) && (timeout > 0)) 
  {
    timeout--;
  }
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
