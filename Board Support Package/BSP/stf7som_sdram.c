/**
  ******************************************************************************
  * @file    STF7SoM_sdram.c
  * @author  MCD Application Team
  * @brief   This file includes the SDRAM driver for the MT48LC4M32B2B5-7 memory
  *          device mounted on STM32F7508-Discovery board.
  @verbatim
   1. How To use this driver:
   --------------------------
      - This driver is used to drive the MT48LC4M32B2B5-7 SDRAM external memory mounted
        on STM32F7508-Discovery board.
      - This driver does not need a specific component driver for the SDRAM device
        to be included with.

   2. Driver description:
   ---------------------
     + Initialization steps:
        o Initialize the SDRAM external memory using the BSP_SDRAM_Init() function. This
          function includes the MSP layer hardware resources initialization and the
          FMC controller configuration to interface with the external SDRAM memory.
        o It contains the SDRAM initialization sequence to program the SDRAM external
          device using the function BSP_SDRAM_Initialization_sequence(). Note that this
          sequence is standard for all SDRAM devices, but can include some differences
          from a device to another. If it is the case, the right sequence should be
          implemented separately.

     + SDRAM read/write operations
        o SDRAM external memory can be accessed with read/write operations once it is
          initialized.
          Read/write operation can be performed with AHB access using the functions
          BSP_SDRAM_ReadData()/BSP_SDRAM_WriteData(), or by DMA transfer using the functions
          BSP_SDRAM_ReadData_DMA()/BSP_SDRAM_WriteData_DMA().
        o The AHB access is performed with 32-bit width transaction, the DMA transfer
          configuration is fixed at single (no burst) word transfer (see the
          SDRAM_MspInit() static function).
        o User can implement his own functions for read/write access with his desired
          configurations.
        o If interrupt mode is used for DMA transfer, the function BSP_SDRAM_DMA_IRQHandler()
          is called in IRQ handler file, to serve the generated interrupt once the DMA
          transfer is complete.
        o You can send a command to the SDRAM device in runtime using the function
          BSP_SDRAM_Sendcmd(), and giving the desired command as parameter chosen between
          the predefined commands of the "FMC_SDRAM_CommandTypeDef" structure.

  @endverbatim
  ******************************************************************************
  * @attention
  *
  * <h2><center>&copy; Copyright (c) 2018 STMicroelectronics.
  * All rights reserved.</center></h2>
  *
  * This software component is licensed by ST under BSD 3-Clause license,
  * the "License"; You may not use this file except in compliance with the
  * License. You may obtain a copy of the License at:
  *                        opensource.org/licenses/BSD-3-Clause
  *
  ******************************************************************************
  */

/* Dependencies
- stm32f7xx_hal_sdram.c
- stm32f7xx_ll_fmc.c
- stm32f7xx_hal_dma.c
- stm32f7xx_hal_gpio.c
- stm32f7xx_hal_cortex.c
- stm32f7xx_hal_rcc_ex.h
EndDependencies */

/* Includes ------------------------------------------------------------------*/
#include "stf7som_fmc.h"
#include "stf7som_sdram.h"

/** @addtogroup BSP
  * @{
  */

/** @addtogroup STF7SoM
  * @{
  */

/** @defgroup STF7SoM_SDRAM STF7SoM_SDRAM
  * @{
  */

/** @defgroup STF7SoM_SDRAM_Private_Types_Definitions STF7SoM_SDRAM Private Types Definitions
  * @{
  */
/**
  * @}
  */

/** @defgroup STF7SoM_SDRAM_Private_Defines STF7SoM_SDRAM Private Defines
  * @{
  */
/**
  * @}
  */

/** @defgroup STF7SoM_SDRAM_Private_Macros STF7SoM_SDRAM Private Macros
  * @{
  */
/**
  * @}
  */

/** @defgroup STF7SoM_SDRAM_Private_Variables STF7SoM_SDRAM Private Variables
  * @{
  */
SDRAM_HandleTypeDef sdramHandle;
static FMC_SDRAM_TimingTypeDef Timing;
static FMC_SDRAM_CommandTypeDef Command;
/**
  * @}
  */

/** @defgroup STF7SoM_SDRAM_Private_Function_Prototypes STF7SoM_SDRAM Private Function Prototypes
  * @{
  */
/**
  * @}
  */

/** @defgroup STF7SoM_SDRAM_Exported_Functions STF7SoM_SDRAM Exported Functions
  * @{
  */

/**
  * @brief  Programs the SDRAM device.
  * @param  RefreshCount: SDRAM refresh counter value
  * @retval None
  */
void BSP_SDRAM_Initialization_sequence(uint32_t RefreshCount)
{
    __IO uint32_t tmpmrd = 0;

    /* Step 1: Configure a clock configuration enable command */
    Command.CommandMode            = FMC_SDRAM_CMD_CLK_ENABLE;
    Command.CommandTarget          = FMC_SDRAM_CMD_TARGET_BANK1;
    Command.AutoRefreshNumber      = 1;
    Command.ModeRegisterDefinition = 0;

    /* Send the command */
    HAL_SDRAM_SendCommand(&sdramHandle, &Command, SDRAM_TIMEOUT);

    /* Step 2: Insert 100 us minimum delay */
    /* Inserted delay is equal to 1 ms due to systick time base unit (ms) */
    HAL_Delay(1);

    /* Step 3: Configure a PALL (precharge all) command */
    Command.CommandMode            = FMC_SDRAM_CMD_PALL;
    Command.CommandTarget          = FMC_SDRAM_CMD_TARGET_BANK1;
    Command.AutoRefreshNumber      = 1;
    Command.ModeRegisterDefinition = 0;

    /* Send the command */
    HAL_SDRAM_SendCommand(&sdramHandle, &Command, SDRAM_TIMEOUT);

    /* Step 4: Configure an Auto Refresh command */
    Command.CommandMode            = FMC_SDRAM_CMD_AUTOREFRESH_MODE;
    Command.CommandTarget          = FMC_SDRAM_CMD_TARGET_BANK1;
    Command.AutoRefreshNumber      = 8;
    Command.ModeRegisterDefinition = 0;

    /* Send the command */
    HAL_SDRAM_SendCommand(&sdramHandle, &Command, SDRAM_TIMEOUT);

    /* Step 5: Program the external memory mode register */
    tmpmrd = (uint32_t)	SDRAM_MODEREG_CAS_LATENCY_2				| \
						SDRAM_MODEREG_BURST_LENGTH_1 									| \
						SDRAM_MODEREG_BURST_TYPE_SEQUENTIAL						| \
						SDRAM_MODEREG_WRITEBURST_MODE_SINGLE					| \
						SDRAM_MODEREG_OPERATING_MODE_STANDARD;

    Command.CommandMode            = FMC_SDRAM_CMD_LOAD_MODE;
    Command.CommandTarget          = FMC_SDRAM_CMD_TARGET_BANK1;
    Command.AutoRefreshNumber      = 1;
    Command.ModeRegisterDefinition = tmpmrd;

    /* Send the command */
    HAL_SDRAM_SendCommand(&sdramHandle, &Command, SDRAM_TIMEOUT);

    /* Step 6: Set the refresh rate counter */
    /* Set the device refresh rate */
    HAL_SDRAM_ProgramRefreshRate(&sdramHandle, RefreshCount);
    
    /* Step 6: Set the refresh rate counter */
    /* (15.62 us x Freq) - 20 */
    /* Set the device refresh counter */
    sdramHandle.Instance->SDRTR |= ((uint32_t)((1292)<< 1));
}
//

/**
  * @brief  Initializes SDRAM MSP.
  * @param  hsdram: SDRAM handle
  * @param  Params
  * @retval None
  */
__weak void BSP_SDRAM_MspInit(SDRAM_HandleTypeDef  *hsdram)
{
  DMA_HandleTypeDef dma_handle;
  GPIO_InitTypeDef gpio_init_structure;

  /* Enable FMC clock */
  __HAL_RCC_FMC_CLK_ENABLE();

  /* Enable chosen DMAx clock */
//  __DMAx_CLK_ENABLE();

  /** FMC GPIO Configuration
    * PG5		------> FMC_BA1
    * PG4		------> FMC_BA0
    * -------------------------
    * PE0		------> FMC_NBL0
    * PE1		------> FMC_NBL1
    * -------------------------
    * PH5		------> FMC_SDNWE
    * PG8		------> FMC_SDCLK
    * -------------------------
    * PH3		------> FMC_SDNE0
    * PH2		------> FMC_SDCKE0
    * -------------------------
    * PF11	------> FMC_SDNRAS
    * PG15	------> FMC_SDNCAS
    * -------------------------
    */
  /* Common GPIO configuration */
  BSP_FMC_Init_CommonPins();
  
  gpio_init_structure.Pull      	    = GPIO_PULLUP;
  gpio_init_structure.Alternate 	    = GPIO_AF12_FMC;
  gpio_init_structure.Mode      	    = GPIO_MODE_AF_PP;
  gpio_init_structure.Speed     	    = GPIO_SPEED_FREQ_VERY_HIGH;

  /* GPIOC configuration */
  gpio_init_structure.Pin   			    = FMC_SDNWE_Pin;
  HAL_GPIO_Init(GPIOC, &gpio_init_structure);

  /* GPIOF configuration */
  gpio_init_structure.Pin   			    = FMC_SDNRAS_Pin;
  HAL_GPIO_Init(GPIOF, &gpio_init_structure);

  /* GPIOG configuration */
  gpio_init_structure.Pin   			    = FMC_SDNCAS_Pin | FMC_SDCLK_Pin;
  HAL_GPIO_Init(GPIOG, &gpio_init_structure);

  /* GPIOH configuration */
  gpio_init_structure.Pin   			    = FMC_SDNE0_Pin | FMC_SDCKE0_Pin;
  HAL_GPIO_Init(GPIOH, &gpio_init_structure);

  /* Associate the DMA handle */
//  __HAL_LINKDMA(hsdram, hdma, dma_handle);

//  /* Configure common DMA parameters */
//  dma_handle.Init.Mode                = DMA_NORMAL;
//  dma_handle.Init.PeriphInc           = DMA_PINC_ENABLE;
//  dma_handle.Init.MemInc              = DMA_MINC_ENABLE;
//  dma_handle.Instance 				        = SDRAM_DMAx_STREAM;
//  dma_handle.Init.Priority            = DMA_PRIORITY_HIGH;
//  dma_handle.Init.MemBurst            = DMA_MBURST_SINGLE;
//  dma_handle.Init.PeriphBurst         = DMA_PBURST_SINGLE;
//  dma_handle.Init.Channel             = SDRAM_DMAx_CHANNEL;
//  dma_handle.Init.PeriphDataAlignment = DMA_PDATAALIGN_WORD;
//  dma_handle.Init.MemDataAlignment    = DMA_MDATAALIGN_WORD;
//  dma_handle.Init.FIFOMode            = DMA_FIFOMODE_DISABLE;
//  dma_handle.Init.Direction           = DMA_MEMORY_TO_MEMORY;
//  dma_handle.Init.FIFOThreshold       = DMA_FIFO_THRESHOLD_FULL;

//  /* Deinitialize the stream for new transfer */
//  HAL_DMA_DeInit(&dma_handle);

//  /* Configure the DMA stream */
//  HAL_DMA_Init(&dma_handle);

//  /* NVIC configuration for DMA transfer complete interrupt */
//  HAL_NVIC_SetPriority(SDRAM_DMAx_IRQn, 0x0F, 0);
//  HAL_NVIC_EnableIRQ(SDRAM_DMAx_IRQn);
}
//

/**
  * @brief  DeInitializes SDRAM MSP.
  * @param  hsdram: SDRAM handle
  * @param  Params
  * @retval None
  */
__weak void BSP_SDRAM_MspDeInit(SDRAM_HandleTypeDef  *hsdram)
{
  DMA_HandleTypeDef dma_handle;

  /* Disable NVIC configuration for DMA interrupt */
  HAL_NVIC_DisableIRQ(SDRAM_DMAx_IRQn);

  /* Deinitialize the stream for new transfer */
  dma_handle.Instance = SDRAM_DMAx_STREAM;
  HAL_DMA_DeInit(&dma_handle);

  /* GPIO pins clock, FMC clock and DMA clock can be shut down in the applications
     by surcharging this __weak function */
  
  /* Enable FMC clock */
  __HAL_RCC_FMC_CLK_DISABLE();

  /* Enable chosen DMAx clock */
  __DMAx_CLK_DISABLE();
}
//

/**
  * @brief  Initializes the SDRAM device.
  * @retval SDRAM status
  */
uint8_t BSP_SDRAM_Init(void)
{
    uint8_t sdramstatus 			  = SDRAM_ERROR;
	
    sdramHandle.Instance 				= FMC_SDRAM_DEVICE;                               /* SDRAM device configuration */

    /* Timing configuration for 100Mhz as SD clock frequency (System clock is up to 200Mhz) */
    Timing.RPDelay              		    = 2;
    Timing.RCDDelay             		    = 2;
    Timing.RowCycleDelay        		    = 6;
    Timing.SelfRefreshTime      		    = 4;
    Timing.LoadToActiveDelay    		    = 2;
    Timing.WriteRecoveryTime    		    = 2;
    Timing.ExitSelfRefreshDelay 		    = 7;

    /* SDRAM controller initialization */
    sdramHandle.Init.SDClockPeriod      = SDCLOCK_PERIOD;
    sdramHandle.Init.SDBank             = FMC_SDRAM_BANK1;
    sdramHandle.Init.MemoryDataWidth    = SDRAM_MEMORY_WIDTH;
    sdramHandle.Init.CASLatency         = FMC_SDRAM_CAS_LATENCY_2;
    sdramHandle.Init.ReadBurst          = FMC_SDRAM_RBURST_ENABLE;
    sdramHandle.Init.ReadPipeDelay      = FMC_SDRAM_RPIPE_DELAY_0;
    sdramHandle.Init.RowBitsNumber      = FMC_SDRAM_ROW_BITS_NUM_13;
    sdramHandle.Init.ColumnBitsNumber   = FMC_SDRAM_COLUMN_BITS_NUM_9;
    sdramHandle.Init.InternalBankNumber = FMC_SDRAM_INTERN_BANKS_NUM_4;
    sdramHandle.Init.WriteProtection    = FMC_SDRAM_WRITE_PROTECTION_DISABLE;
    
    BSP_SDRAM_MspInit(&sdramHandle);

    if(HAL_SDRAM_Init(&sdramHandle, &Timing) != HAL_OK)
    {
        sdramstatus = SDRAM_ERROR;
    }
    else
    {
        sdramstatus = SDRAM_OK;
    }

    /* SDRAM initialization sequence */
    BSP_SDRAM_Initialization_sequence(REFRESH_COUNT);

    return sdramstatus;
}
//

/**
  * @brief  DeInitializes the SDRAM device.
  * @retval SDRAM status
  */
uint8_t BSP_SDRAM_DeInit(void)
{
    uint8_t sdramstatus = SDRAM_ERROR;
	
    /* SDRAM controller de-initialization */
    BSP_SDRAM_MspDeInit(&sdramHandle);

    /* SDRAM device de-initialization */
    sdramHandle.Instance = FMC_SDRAM_DEVICE;

    if(HAL_SDRAM_DeInit(&sdramHandle) != HAL_OK)
    {
        sdramstatus = SDRAM_ERROR;
    }
    else
    {
        sdramstatus = SDRAM_OK;
    }

    return sdramstatus;
}
//

/**
  * @brief  Checks the SDRAM device.
  * @retval SDRAM status
  */
uint8_t BSP_SDRAM_Check(uint32_t writeValue)
{
	/* Status variables */
	__IO uint32_t uwWriteReadStatus = 0;

	/* Counter index */
	uint32_t uwIndex = 0;

	for(uwIndex = 0; uwIndex < SDRAM_DEVICE_SIZE; uwIndex++)
        *(__IO uint32_t *)(SDRAM_DEVICE_ADDR + 4 * uwIndex) = writeValue;

    for(uwIndex = 0; uwIndex < SDRAM_DEVICE_SIZE; uwIndex++)
    {
        uint32_t value = *(__IO uint32_t *)(SDRAM_DEVICE_ADDR + 4 * uwIndex);
        if(value != writeValue)
            uwWriteReadStatus++;
    }
	return (uwWriteReadStatus == 0) ? SDRAM_OK : SDRAM_ERROR;
}
//

/**
  * @brief  Sends command to the SDRAM bank.
  * @param  SdramCmd: Pointer to SDRAM command structure
  * @retval SDRAM status
  */
uint8_t BSP_SDRAM_Sendcmd(FMC_SDRAM_CommandTypeDef *SdramCmd)
{
    if(HAL_SDRAM_SendCommand(&sdramHandle, SdramCmd, SDRAM_TIMEOUT) != HAL_OK)
    {
        return SDRAM_ERROR;
    }
    else
    {
        return SDRAM_OK;
    }
}
//

/**
  * @brief  Reads an amount of data from the SDRAM memory in polling mode.
  * @param  uwStartAddress: Read start address
  * @param  pData: Pointer to data to be read
  * @param  uwDataSize: Size of read data from the memory
  * @retval SDRAM status
  */
uint8_t BSP_SDRAM_ReadData(uint32_t uwStartAddress, uint32_t *pData, uint32_t uwDataSize)
{
    if(HAL_SDRAM_Read_32b(&sdramHandle, (uint32_t *)uwStartAddress, pData, uwDataSize) != HAL_OK)
    {
        return SDRAM_ERROR;
    }
    else
    {
        return SDRAM_OK;
    }
}
//

/**
  * @brief  Writes an amount of data to the SDRAM memory in polling mode.
  * @param  uwStartAddress: Write start address
  * @param  pData: Pointer to data to be written
  * @param  uwDataSize: Size of written data from the memory
  * @retval SDRAM status
  */
uint8_t BSP_SDRAM_WriteData(uint32_t uwStartAddress, uint32_t *pData, uint32_t uwDataSize)
{
    if(HAL_SDRAM_Write_32b(&sdramHandle, (uint32_t *)uwStartAddress, pData, uwDataSize) != HAL_OK)
    {
        return SDRAM_ERROR;
    }
    else
    {
        return SDRAM_OK;
    }
}
//

/**
  * @brief  Reads an amount of data from the SDRAM memory in DMA mode.
  * @param  uwStartAddress: Read start address
  * @param  pData: Pointer to data to be read
  * @param  uwDataSize: Size of read data from the memory
  * @retval SDRAM status
  */
uint8_t BSP_SDRAM_ReadData_DMA(uint32_t uwStartAddress, uint32_t *pData, uint32_t uwDataSize)
{
    if(HAL_SDRAM_Read_DMA(&sdramHandle, (uint32_t *)uwStartAddress, pData, uwDataSize) != HAL_OK)
    {
        return SDRAM_ERROR;
    }
    else
    {
        return SDRAM_OK;
    }
}
//

/**
  * @brief  Writes an amount of data to the SDRAM memory in DMA mode.
  * @param  uwStartAddress: Write start address
  * @param  pData: Pointer to data to be written
  * @param  uwDataSize: Size of written data from the memory
  * @retval SDRAM status
  */
uint8_t BSP_SDRAM_WriteData_DMA(uint32_t uwStartAddress, uint32_t *pData, uint32_t uwDataSize)
{
    if(HAL_SDRAM_Write_DMA(&sdramHandle, (uint32_t *)uwStartAddress, pData, uwDataSize) != HAL_OK)
    {
        return SDRAM_ERROR;
    }
    else
    {
        return SDRAM_OK;
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

/************************ (C) COPYRIGHT YiLDiZ IoT ********************/