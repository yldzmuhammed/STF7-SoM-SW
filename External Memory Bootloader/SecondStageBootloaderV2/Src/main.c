/**
  ******************************************************************************
  * @file           : main.c
  * @brief          : Main program body
  ******************************************************************************
  * @attention
  *
  * <h2><center>&copy; Copyright (c) 2020 STMicroelectronics.
  * All rights reserved.</center></h2>
  *
  * This software component is licensed by ST under Ultimate Liberty license
  * SLA0044, the "License"; You may not use this file except in compliance with
  * the License. You may obtain a copy of the License at:
  *                             www.st.com/SLA0044
  *
  ******************************************************************************
  */
/* Includes ------------------------------------------------------------------*/
#include "main.h"

/* Private includes ----------------------------------------------------------*/
#include "stf7som.h"
#include "stf7som_nor.h"
#include "stf7som_qspi.h"
#include "stf7som_sdram.h"
#include "stf7som_bootloader.h"

#include "usb_device.h"
#include "usbd_core.h"
#include "usbd_desc.h"
#include "usbd_customhid.h"
#include "usbd_custom_hid_if.h"

#include "otaa.h"
#include "port.h"

#include "cmsis_os.h"
/* Private typedef -----------------------------------------------------------*/
typedef void (*pFunction)(void);

typedef enum
{
	Cmd_NONE,
	Cmd_Jump,
	Cmd_Restart,
	Cmd_UploadROM,
	Cmd_DownloadROM,
	Cmd_SectorErase,
	Cmd_FullChipErase,
} Command_TypepDef;

/* Private define ------------------------------------------------------------*/
#define QSPI_FLASH_PARTITION_LOCATION								(0x00000000)
#define PARALLEL_FLASH_PARTITION_LOCATION						(0x00000000)

#define COMMAND_ERASE
#define COMMAND_FORMAT
#define COMMAND_UPLOAD
#define COMMAND_DOWNLOAD
/* Private macro -------------------------------------------------------------*/
/* Private variables ---------------------------------------------------------*/
static uint32_t JumpAddress;

static Command_TypepDef Command;

static NOR_IDTypeDef xNORID;
static xQSPIFlashID_TypeDef xQSPIID;

static Sector_Erase_TypeDef Chip_Erase		= {.Count = 128 };
static Sector_Erase_TypeDef Sector_Erase;

/* Private function prototypes -----------------------------------------------*/
void SystemClock_Config(void);
static void JumpToApplication(void);
static void Load_Default_Partition(void);

static void OTAAUSBPollerTaskHandler(void const * argument);
static void CommandExecuterTaskHandler(void const * argument);

static uint32_t VerifyChecksum(uint32_t buffer[], uint32_t len);
/* Private user code ---------------------------------------------------------*/

/**
  * @brief  The application entry point.
  * @retval int
  */
int main(void)
{
  /* MCU Configuration--------------------------------------------------------*/

  /* Reset of all peripherals, Initializes the Flash interface and the Systick. */
  HAL_Init();

  /* Configure the system clock */
  SystemClock_Config();

  /* Initialize GPIO Clock */
  BSP_GPIOCLK_Enable();
  
	BSP_QSPI_Init();
	
	BSP_NOR_Init();
	
	BSP_SDRAM_Init();
	
	BSP_NOR_Read_ID(&xNORID);
	volatile bool flag = false;
	while(!flag);
	uint16_t buffer[] = { 0x00, 0x01, 0x02, 0x03, 0x04 };
	BSP_NOR_Erase_Block(0);
	BSP_NOR_WriteData(0, buffer, sizeof(buffer) / sizeof(buffer[0]));
	
  /* add mutexes, ... */
  /* add semaphores, ... */
  /* start timers, add new ones, ... */
  /* add queues, ... */
  /* Create the thread(s) */
  /* definition and creation of OTAA Poller */
  osThreadDef(OTAAUSBPollerTask, OTAAUSBPollerTaskHandler, osPriorityNormal, 0, 128);
  osThreadCreate(osThread(OTAAUSBPollerTask), NULL);

  /* definition and creation of defaultTask */
  osThreadDef(CommandExecuterTask, CommandExecuterTaskHandler, osPriorityNormal, 0, 128);
  osThreadCreate(osThread(CommandExecuterTask), NULL);

  /* add threads, ... */
  /* Start scheduler */
  osKernelStart();

  /* We should never get here as control is now taken by the scheduler */
  /* Infinite loop */
  while (1)
  {
  }
}
//

/**
  * @brief System Clock Configuration
  * @retval None
  */
void SystemClock_Config(void)
{
  RCC_OscInitTypeDef RCC_OscInitStruct = {0};
  RCC_ClkInitTypeDef RCC_ClkInitStruct = {0};
  RCC_PeriphCLKInitTypeDef PeriphClkInitStruct = {0};

  /** Configure LSE Drive Capability */
//  HAL_PWR_EnableBkUpAccess();
  
  /** Configure the main internal regulator output voltage */
  __HAL_RCC_PWR_CLK_ENABLE();
  __HAL_PWR_VOLTAGESCALING_CONFIG(PWR_REGULATOR_VOLTAGE_SCALE1);
  
  /** Initializes the CPU, AHB and APB busses clocks */
  RCC_OscInitStruct.PLL.PLLQ        = 8;
  RCC_OscInitStruct.PLL.PLLM        = 12;
  RCC_OscInitStruct.PLL.PLLN        = 200;
  RCC_OscInitStruct.HSEState        = RCC_HSE_ON;
  RCC_OscInitStruct.PLL.PLLState    = RCC_PLL_ON;
  RCC_OscInitStruct.PLL.PLLP        = RCC_PLLP_DIV2;
  RCC_OscInitStruct.PLL.PLLSource   = RCC_PLLSOURCE_HSE;
  RCC_OscInitStruct.OscillatorType  = RCC_OSCILLATORTYPE_HSE;
  if (HAL_RCC_OscConfig(&RCC_OscInitStruct) != HAL_OK)
  {
    Error_Handler();
  }
  
  /** Activate the Over-Drive mode */
  if (HAL_PWREx_EnableOverDrive() != HAL_OK)
  {
    Error_Handler();
  }
  
  /** Initializes the CPU, AHB and APB busses clocks */
  RCC_ClkInitStruct.APB1CLKDivider  = RCC_HCLK_DIV4;
  RCC_ClkInitStruct.APB2CLKDivider  = RCC_HCLK_DIV2;
  RCC_ClkInitStruct.AHBCLKDivider   = RCC_SYSCLK_DIV1;
  RCC_ClkInitStruct.SYSCLKSource    = RCC_SYSCLKSOURCE_PLLCLK;
  RCC_ClkInitStruct.ClockType       = RCC_CLOCKTYPE_HCLK | RCC_CLOCKTYPE_SYSCLK | RCC_CLOCKTYPE_PCLK1 | RCC_CLOCKTYPE_PCLK2;

  if (HAL_RCC_ClockConfig(&RCC_ClkInitStruct, FLASH_LATENCY_6) != HAL_OK)
  {
    Error_Handler();
  }
  
  PeriphClkInitStruct.PLLSAIDivQ            = 1;
  PeriphClkInitStruct.PLLSAI.PLLSAIR        = 2;
  PeriphClkInitStruct.PLLSAI.PLLSAIQ        = 2;
  PeriphClkInitStruct.PLLSAI.PLLSAIN        = 192;
  PeriphClkInitStruct.PLLSAIDivR            = RCC_PLLSAIDIVR_2;
  PeriphClkInitStruct.PLLSAI.PLLSAIP        = RCC_PLLSAIP_DIV8;
  PeriphClkInitStruct.Clk48ClockSelection   = RCC_CLK48SOURCE_PLLSAIP;
  PeriphClkInitStruct.Sdmmc1ClockSelection  = RCC_SDMMC1CLKSOURCE_CLK48;
  PeriphClkInitStruct.PeriphClockSelection  = RCC_PERIPHCLK_SDMMC1 | RCC_CLK48SOURCE_PLLSAIP;
  if (HAL_RCCEx_PeriphCLKConfig(&PeriphClkInitStruct) != HAL_OK)
  {
    Error_Handler();
  }
  
  /** Enables the Clock Security System */
  HAL_RCC_EnableCSS();
}
//

/**
  * @brief  Function implementing the defaultTask thread.
  * @param  argument: Not used
  * @retval None
  */
static void OTAAUSBPollerTaskHandler(void const * argument)
{
	/* Init OTAA Stack */
	if(OTAA_Init(OTAA_Channel_USB) == OTAA_ErrCode_NOERR)
	{
		/* Enable OTAA stack */
		OTAA_Enable();
	}
  
  /* Infinite loop */
  for(;;)
  {
	  /* Poll Stack */
		OTAA_Poll();
  }
}
//

static void CommandExecuterTaskHandler(void const * argument)
{
	for(;;)
	{
		switch(Command)
		{
			case Cmd_NONE: {break;}
			case Cmd_Jump:
			{
				/* jump to the applicaiton */
				JumpToApplication();
				break;
			}
			case Cmd_Restart:
			{
				/* reset cpu */
				NVIC_SystemReset();
				break;
			}
			case Cmd_UploadROM:
			case Cmd_DownloadROM:
			{
				break;
			}
			case Cmd_SectorErase:
			{
				/* Mark flag as in progress */
				Sector_Erase.Status = OTAA_FormatState_InProgress;
					
				/* Check for memory type */
				if(Sector_Erase.Memory == OTAA_MemType_NORFlash)
				{
					/* start erasing sectors */
					for(; Sector_Erase.Start < Sector_Erase.Count; Sector_Erase.Start++)
					{
						/* Try to clear the sector */
						if(BSP_NOR_Erase_Block(Sector_Erase.Start) != NOR_STATUS_OK)
						{
							/* Something went wrong. Cancel the process */
							Sector_Erase.Status = OTAA_FormatState_Failed;
							
							/* break the operation */
							break;
						}
					}
				}
				/* Check for other memory type */
				else if(Sector_Erase.Memory == OTAA_MemType_QSPIFlash)
				{
					/* start erasing sectors */
					for(; Sector_Erase.Start < Sector_Erase.Count; Sector_Erase.Start++)
					{
						/* Try to clear the sector. QSPI Flash has 64 KB sector widht. */
						if(BSP_QSPI_Erase_Block(Sector_Erase.Start, BLOCKSIZE_64KB) != QSPI_OK && BSP_QSPI_Erase_Block(Sector_Erase.Start + 1, BLOCKSIZE_64KB) != QSPI_OK)
						{
							/* Something went wrong. Cancel the process */
							Sector_Erase.Status = OTAA_FormatState_Failed;
							
							/* break the operation */
							break;
						}
					}
				}
				
				/* clear erase flag if everything is ok */
				if(Sector_Erase.Status == OTAA_FormatState_InProgress)
				{
					Sector_Erase.Status = OTAA_FormatState_Completed;
				}
				
				break;
			}
			case Cmd_FullChipErase:
			{
				/* Mark flag as in progress */
				Chip_Erase.Status = OTAA_FormatState_InProgress;
					
				/* Disable memory mapped mode so we can write/delete */
				if(BSP_QSPI_EnableMemoryMappedMode(false) != QSPI_OK)
				{
					Chip_Erase.Status = OTAA_FormatState_Failed;
				}
				//
				
				/* erase sector by sector */
				for(Chip_Erase.Start = 0; Chip_Erase.Start < Chip_Erase.Count; Chip_Erase.Start++)
				{
					if(Chip_Erase.Status == OTAA_FormatState_InProgress)
					{
						/* Try to clear the sector */
						if(BSP_NOR_Erase_Block(Chip_Erase.Start) != NOR_STATUS_OK)
						{
							/* Something went wrong. Cancel the process */
							Chip_Erase.Status = OTAA_FormatState_Failed;
						}
						//
						/* Try to clear the sector. QSPI Flash has 64 KB sector widht. */
						if(BSP_QSPI_Erase_Block(Chip_Erase.Start, BLOCKSIZE_64KB) != QSPI_OK)
						{
							/* Something went wrong. Cancel the process */
							Chip_Erase.Status = OTAA_FormatState_Failed;
						}
						//
						
						/* Try to clear the sector. QSPI Flash has 64 KB sector widht. */
						if(BSP_QSPI_Erase_Block(Chip_Erase.Start + 1, BLOCKSIZE_64KB) != QSPI_OK)
						{
							/* Something went wrong. Cancel the process */
							Chip_Erase.Status = OTAA_FormatState_Failed;
						}
						//
					}
					else
					{
						break;
					}
				}
				//
				
				if(Chip_Erase.Status == OTAA_FormatState_InProgress)
				{
					/* load default partition table value */
					Load_Default_Partition();
					
					/* Write partition table to the flash */
					BSP_NOR_WriteData(PARALLEL_FLASH_PARTITION_LOCATION, (uint16_t *)&ExtParallelFlash, sizeof(ExtParallelFlash) / 2);
					
					/* enable memory mapped mode */
					BSP_NOR_ReturnToReadMode();
					
					/* write partition table to the flash */
					BSP_QSPI_Write((uint8_t *)&ExtQSPIFlash, QSPI_FLASH_PARTITION_LOCATION, sizeof(ExtQSPIFlash));
					
					/* clear erase flag if everything is ok */
					Chip_Erase.Status = OTAA_FormatState_Completed;
					
					/* try to enable memory mapped mode */
					if(BSP_QSPI_EnableMemoryMappedMode(true) != QSPI_OK)
					{
						/* qspi memory format failed */
						Chip_Erase.Status = OTAA_FormatState_Failed;
					}
				}
				//
				
				break;
			}
			default:
			{
				break;
			}
		}
		
		Command = Cmd_NONE;
		
		/* suspend thread for 500 ms */
		osDelay(500);
	}
}
//

OTAA_ErrorCode_TypeDef OTAA_Cmd_ReadID_Callback(uint8_t *trx_pld, uint16_t *trx_len, OTAA_MemoryType_TypeDef eMemory)
{
	OTAA_ErrorCode_TypeDef eStatus = OTAA_ErrCode_NOERR;
	
	if(eMemory == OTAA_MemType_InternalFlash)
	{
		*trx_len = 12;
		
		uint32_t uid0 = HAL_GetUIDw0();
		uint32_t uid1 = HAL_GetUIDw1();
		uint32_t uid2 = HAL_GetUIDw2();
		
		for(int i = 0; i < 12; i++)
		{
			trx_pld[i] = READ_REG(*((uint8_t *)UID_BASE + i));
		}
	}
	else if(eMemory == OTAA_MemType_NORFlash)
	{
		if(BSP_NOR_Read_ID(&xNORID) != NOR_STATUS_OK)
		{
			eStatus = OTAA_ErrCode_IO;
		}
		else
		{
			*trx_len = sizeof(NOR_IDTypeDef);
			
			memcpy(trx_pld, &xNORID, *trx_len);
		}
	}
	else if(eMemory == OTAA_MemType_QSPIFlash)
	{
		if(BSP_QSPI_ReadID(&xQSPIID.JEDECID) 				== QSPI_OK && 
			 BSP_QSPI_ReadDeviceID(&xQSPIID.DeviceID) == QSPI_OK &&
			 BSP_QSPI_ReadUniqDevID(&xQSPIID.UniqUD) == QSPI_OK)
		{
			*trx_len = (uint16_t)sizeof(xQSPIFlashID_TypeDef);
			
			memcpy(trx_pld, &xQSPIID, *trx_len);
		}
		else
		{
			eStatus = OTAA_ErrCode_IO;
		}
			
	}
	else if(eMemory == OTAA_MemType_EEPROM)
	{
		eStatus = OTAA_ErrCode_IO;
	}
	
	return eStatus;
}
//

OTAA_ErrorCode_TypeDef OTAA_Cmd_JumpApp_Callback(void)
{
	uint32_t Stack_Pointer;
	
	OTAA_ErrorCode_TypeDef eStatus = OTAA_ErrCode_NOERR;

	BSP_NOR_ReturnToReadMode();
	
	/* Do we use first sector for current app? */
	if(ExtParallelFlash.PartitionTable.Binary1.SectorState == Sector_InUse)
	{
		Stack_Pointer = *((uint32_t *)(ExtParallelFlash.peripheral_address + ExtParallelFlash.PartitionTable.Binary1.offset));
		
		/* Do we have valid stack pointer address? */
		if(Stack_Pointer >= ExtParallelFlash.RAM_LowerAddressLimit && Stack_Pointer < ExtParallelFlash.RAM_UpperAddressLimit)
		{
			uint32_t Address = ExtParallelFlash.peripheral_address + ExtParallelFlash.PartitionTable.Binary1.offset;
			/* Does crc matched? */
			if(ExtParallelFlash.PartitionTable.Binary1.Binary.crc == VerifyChecksum(((uint32_t *)Address), ExtParallelFlash.PartitionTable.Binary1.Binary.size))
			{
				/* yes it does! we can jump to this address.... */
				JumpAddress = Stack_Pointer;
				Command = Cmd_Jump;
			}
			else
			{
				/* CRC values not matched. */
				eStatus = OTAA_ErrCode_APPCRC;
			}
		}
		else
		{
			/* There is no app */
			eStatus = OTAA_ErrCode_NOAPP;
		}
	}
	else if(ExtParallelFlash.PartitionTable.Binary2.SectorState == Sector_InUse)
	{
		Stack_Pointer = *((uint32_t *)(ExtParallelFlash.peripheral_address + ExtParallelFlash.PartitionTable.Binary2.offset));
		
		/* Do we have valid stack pointer address? */
		if(Stack_Pointer >= ExtParallelFlash.RAM_LowerAddressLimit && Stack_Pointer < ExtParallelFlash.RAM_UpperAddressLimit)
		{
			uint32_t Address = ExtParallelFlash.peripheral_address + ExtParallelFlash.PartitionTable.Binary2.offset;
			/* Does crc matched? */
			if(ExtParallelFlash.PartitionTable.Binary2.Binary.crc == VerifyChecksum(((uint32_t *)Address), ExtParallelFlash.PartitionTable.Binary2.Binary.size))
			{
				/* yes it does! we can jump to this address.... */
				JumpAddress = Stack_Pointer;
				Command = Cmd_Jump;
			}
			else
			{
				/* CRC values not matched. */
				eStatus = OTAA_ErrCode_APPCRC;
			}
		}
		else
		{
			/* There is no app */
			eStatus = OTAA_ErrCode_NOAPP;
		}
	}
	else
	{
		/* There is no app in the flash. */
		eStatus = OTAA_ErrCode_NOAPP;
	}
	
	return eStatus;
}
//

OTAA_ErrorCode_TypeDef OTAA_Cmd_Restart_Callback(void)
{
	Command = Cmd_Restart;
	return OTAA_ErrCode_NOERR;
}
//

OTAA_ErrorCode_TypeDef OTAA_Cmd_ReadLayout_Callback(uint8_t *trx_pld, uint16_t *trx_len)
{
	uint32_t index = 0;
	uint32_t layoutlen1 = sizeof(InternalFlash);
	uint32_t layoutlen2 = sizeof(ExtParallelFlash);
	uint32_t layoutlen3 = sizeof(ExtQSPIFlash);
	
	
	/* Calculate total layout length. */
	*trx_len = sizeof(InternalFlash) + sizeof(ExtParallelFlash) + sizeof(ExtQSPIFlash);
	
	/* Load defalt layout values */
	Load_Default_Partition();
	
	/* we are not using internal flash. so it is disabled. so we are using only parallel and qspi flash. */
	if(BSP_NOR_ReadBuffer(PARALLEL_FLASH_PARTITION_LOCATION, (uint16_t *)&ExtParallelFlash, layoutlen2 / 2) != NOR_STATUS_OK)
	{
		return OTAA_ErrCode_IO;
	}
	
	if(BSP_QSPI_Read((uint8_t *)&ExtQSPIFlash, QSPI_FLASH_PARTITION_LOCATION, layoutlen3, eReadMode_Read) != QSPI_OK)
	{
		return OTAA_ErrCode_IO;
	}
	
	memcpy(&trx_pld[index], &InternalFlash, layoutlen1);
	index += layoutlen1;
	
	memcpy(&trx_pld[index], &ExtParallelFlash, layoutlen2);
	index += layoutlen2;
	
	memcpy(&trx_pld[index], &ExtQSPIFlash, layoutlen3);
	index += layoutlen3;
	
	return OTAA_ErrCode_NOERR;
}
//

OTAA_ErrorCode_TypeDef OTAA_Cmd_SectorErase_Callback(Sector_Erase_TypeDef erase, Sector_Erase_TypeDef* status)
{
		/* do we have any ongoing sector erase operation? */
		if(Sector_Erase.Status == OTAA_FormatState_NotStarted)
		{
			if(erase.Memory != 	OTAA_MemType_NORFlash && erase.Memory != OTAA_MemType_QSPIFlash)
			{
				return OTAA_ErrCode_NORES;
			}
			
			Command = Cmd_SectorErase;
			
			Sector_Erase.Start = erase.Start;
			Sector_Erase.Count = erase.Count;
			Sector_Erase.Memory = erase.Memory;
			Sector_Erase.Status = OTAA_FormatState_Started;
		}
		
		status->Start = Sector_Erase.Start;
		status->Count = Sector_Erase.Count;
		status->Memory = Sector_Erase.Memory;
		status->Status = Sector_Erase.Status;
		
		if(Sector_Erase.Status != OTAA_FormatState_InProgress && Sector_Erase.Status != OTAA_FormatState_Started)
		{
			Sector_Erase.Status = OTAA_FormatState_NotStarted;
		}
		return OTAA_ErrCode_NOERR;
}
//

OTAA_ErrorCode_TypeDef OTAA_Cmd_FormatROM_Callback(OTAA_FlashFormatState_TypeDef *eFState)
{
	/* is device busy with other commands? */
	if(Command != Cmd_NONE && Chip_Erase.Status == OTAA_FormatState_NotStarted)
	{
		return OTAA_ErrCode_ILLSTATE;
	}
	/* no it is not. is device currently formatting itself? */
	else if(Command == Cmd_NONE && Chip_Erase.Status == OTAA_FormatState_NotStarted)
	{
		/* no. lets start formatting. */
		Command = Cmd_FullChipErase;
	}
	
	/* in any case. we are keeping upper layer informed about format state */
	*eFState = Chip_Erase.Status;
	
	/* does formatting finished somehow? */
	if(Chip_Erase.Status == OTAA_FormatState_Completed || Chip_Erase.Status == OTAA_FormatState_Failed)
	{
		Chip_Erase.Status = OTAA_FormatState_NotStarted;
	}
	
	return OTAA_ErrCode_NOERR;
}
//

/**
  * @brief  This function is executed in case of error occurrence.
  * @retval None
  */
void Error_Handler(void)
{
}
//

/**
  * @brief  Loads default partition table for this board.
  * @retval NONE
  */
void Load_Default_Partition(void)
{
	/* Disable internal flash control by stack. It manages itself. */
	InternalFlash.enabled = false;
	InternalFlash.capacity = KB2BYTE(64);
	InternalFlash.page_size = 1;
	InternalFlash.page_count = InternalFlash.capacity / InternalFlash.page_size;
	InternalFlash.sector_size = KB2BYTE(32);
	InternalFlash.sector_count = InternalFlash.capacity / InternalFlash.sector_size;
	InternalFlash.RAM_LowerAddressLimit = RAMDTCM_BASE;
	InternalFlash.RAM_UpperAddressLimit = SRAM2_BASE;
	
	/* Activate and configure external parallel flash. */
	ExtParallelFlash.enabled = true;
	ExtParallelFlash.capacity = MB2BYTE(8);
	ExtParallelFlash.page_size = 32;
	ExtParallelFlash.page_count = ExtParallelFlash.capacity / ExtParallelFlash.page_size;
	ExtParallelFlash.sector_size = KB2BYTE(128);
	ExtParallelFlash.sector_count = ExtParallelFlash.capacity / ExtParallelFlash.sector_size;
	ExtParallelFlash.peripheral_address = 0x60000000;
	ExtParallelFlash.RAM_LowerAddressLimit = RAMDTCM_BASE;
	ExtParallelFlash.RAM_UpperAddressLimit = SRAM2_BASE;

	/* Activate and configure external qspi flash. */
	ExtQSPIFlash.enabled = true;
	ExtQSPIFlash.capacity = MB2BYTE(16);
	ExtQSPIFlash.page_size = 256;
	ExtQSPIFlash.page_count = ExtParallelFlash.capacity / ExtParallelFlash.page_size;
	ExtQSPIFlash.sector_size = KB2BYTE(64);
	ExtQSPIFlash.sector_count = ExtParallelFlash.capacity / ExtParallelFlash.sector_size;
	ExtQSPIFlash.peripheral_address = 0x90000000;
	ExtQSPIFlash.RAM_LowerAddressLimit = RAMDTCM_BASE;
	ExtQSPIFlash.RAM_UpperAddressLimit = SRAM2_BASE;
}
//

/**
  * @brief  Jumps to the requested application address.
  * @retval NONE
  */
void JumpToApplication(void)
{
	uint32_t  jmpaddr = *(__IO uint32_t*)(JumpAddress + 4);
	pFunction Jump = (pFunction)JumpAddress;

	SysTick->CTRL = 0;
	SysTick->LOAD = 0;
	SysTick->VAL  = 0;

	SCB->VTOR = JumpAddress;

	/**	@TODO
		*	Some here around, we should disable all the interrupt requests.
		* And before that, we have to clear the irq flags. */
	
	
	__set_MSP(*(__IO uint32_t*)jmpaddr);
	
	Jump();
}
//

/*** Verify checksum of application located in flash **************************/
static uint32_t VerifyChecksum(uint32_t buffer[], uint32_t len)
{
    CRC_HandleTypeDef CrcHandle;
    volatile uint32_t calculatedCrc;
    
    __HAL_RCC_CRC_CLK_ENABLE();
	
    CrcHandle.Instance 											= CRC;
    CrcHandle.Init.DefaultPolynomialUse    	= DEFAULT_POLYNOMIAL_ENABLE;
    CrcHandle.Init.DefaultInitValueUse     	= DEFAULT_INIT_VALUE_ENABLE;
    CrcHandle.InputDataFormat              	= CRC_INPUTDATA_FORMAT_WORDS;
    CrcHandle.Init.InputDataInversionMode  	= CRC_INPUTDATA_INVERSION_NONE;
    CrcHandle.Init.OutputDataInversionMode 	= CRC_OUTPUTDATA_INVERSION_DISABLE;
    if(HAL_CRC_Init(&CrcHandle) != HAL_OK)
    {    
        return BL_CHKS_ERROR;
    }
    
    calculatedCrc = HAL_CRC_Calculate(&CrcHandle, buffer, len);
    
		/* deinit crc peripheral */
		HAL_CRC_DeInit(&CrcHandle);
		
		/* disable crc clock */
		__HAL_RCC_CRC_CLK_DISABLE();
    
    return calculatedCrc;
}
//


#ifdef  USE_FULL_ASSERT
/**
  * @brief  Reports the name of the source file and the source line number
  *         where the assert_param error has occurred.
  * @param  file: pointer to the source file name
  * @param  line: assert_param error line source number
  * @retval None
  */
void assert_failed(uint8_t *file, uint32_t line)
{ 
  /* USER CODE BEGIN 6 */
  /* User can add his own implementation to report the file name and line number,
     tex: printf("Wrong parameters value: file %s on line %d\r\n", file, line) */
  /* USER CODE END 6 */
}
#endif /* USE_FULL_ASSERT */

/************************ (C) COPYRIGHT STMicroelectronics *****END OF FILE****/
