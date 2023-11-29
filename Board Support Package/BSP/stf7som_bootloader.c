/**
  ******************************************************************************
  * @file    quectel_m66.v
  * @author  YiLDiZ IoT Application Team
  * @brief   This file contains the common defines and functions prototypes for
  *          the quectel_m66.c driver.
  ******************************************************************************
  * @attention
  *
  * <h2><center>&copy; Copyright (c) 2019 YiLDiZ IoT.
  * All rights reserved.</center></h2>
  *
  * This software component is licensed by YiLDiZ IoT under BSD 3-Clause license,
  * the "License"; You may not use this file except in compliance with the
  * License. You may obtain a copy of the License at:
  *                        opensource.org/licenses/BSD-3-Clause
  *
  ******************************************************************************
  */

/* Includes ------------------------------------------------------------------*/
#include <string.h>
#include "errhandler.h"
#include "stf7som_nor.h"
#include "stf7som_qspi.h"
#include "stf7som_bootloader.h"

/* Public variables ----------------------------------------------------------*/
xMasterBootRecoder_TypeDef xMasterBootRecord;
/* Private typedef -----------------------------------------------------------*/
typedef void (*pFunction)(void);
/* Private define ------------------------------------------------------------*/
/* Private macro -------------------------------------------------------------*/
/* Private variables ---------------------------------------------------------*/
/* Private function prototypes -----------------------------------------------*/
/* Private user code ---------------------------------------------------------*/
//static uint8_t BSP_Bootloader_Get_BootTable(void)
//{
//	uint16_t NORBuffer[256] = {0};
//	
//	/* try to read from nor flash */
//	uint8_t Status = BSP_NOR_ReadBuffer(BOOTTABLE_BASE, NORBuffer, sizeof(NORBuffer) / sizeof(NORBuffer[0]));
//	
//	/* copy nor buffer to struct */
//	memcpy((uint8_t *)&xMasterBootRecord, (uint8_t *)NORBuffer, sizeof(xMasterBootRecoder_TypeDef));
//	
//	return Status;
//}
//



eBootloaderErrorCodes BSP_Bootloader_Init(void)
{
	/* create master boot recoder */
//	if(BSP_Bootloader_Get_BootTable() != NOR_STATUS_OK)
//		return BL_FLASH_ERROR;
	
	/* return with no error */
	return BL_OK;
}
//

/*** Jump to System Memory (ST Bootloader) ************************************/
void BSP_Bootloader_JumpToSysMem(void)
{
    uint32_t  JumpAddress = *(__IO uint32_t*)(SYSMEM_ADDRESS + 4);
    pFunction Jump = (pFunction)JumpAddress;
    
    HAL_RCC_DeInit();
    HAL_DeInit();
    
    SysTick->CTRL = 0;
    SysTick->LOAD = 0;
    SysTick->VAL  = 0;
    
    __HAL_RCC_SYSCFG_CLK_ENABLE();
    
    __set_MSP(*(__IO uint32_t*)SYSMEM_ADDRESS);
    Jump();
}
//



/*** Jump to application ******************************************************/
//void BSP_Bootloader_JumpToApplication(void)
//{
//    uint32_t  JumpAddress = *(__IO uint32_t*)(address + 4);
//    pFunction Jump = (pFunction)JumpAddress;
//    
//    HAL_RCC_DeInit();
//    HAL_DeInit();
//    
//    SysTick->CTRL = 0;
//    SysTick->LOAD = 0;
//    SysTick->VAL  = 0;
//    
//#if (SET_VECTOR_TABLE)
//    SCB->VTOR = address;
//#endif
//    
//    __set_MSP(*(__IO uint32_t*)address);
//    Jump();
//}
//



/*** Erase flash **************************************************************/
uint8_t BSP_Bootloader_Erase(void)
{
#if FLASH_MEMORY == FLASHTYPE_INTERNAL
#else
	return 0xFF;
#endif
}
//

bool BSP_BootLoader_FormatFlash(void)
{
	/** format boot table area */
	uint8_t err = BSP_QSPI_Erase_Sector(BOOTTABLE_BASE);
	
	/** handle possible errors */
//	ErrorHandler(err);
	
	/** return format status */
	return (err == QSPI_OK) ? true : false;
}
//

/*** Get flash protection status **********************************************/
uint8_t BSP_Bootloader_GetProtectionStatus(void)
{
#if FLASH_MEMORY == FLASHTYPE_INTERNAL
    FLASH_OBProgramInitTypeDef OBStruct = {0};
    uint8_t protection = BL_PROTECTION_NONE;
    
    HAL_FLASH_Unlock();
    
    /** Bank 1 **/
    OBStruct.PCROPConfig = FLASH_BANK_1;
    OBStruct.WRPArea = OB_WRPAREA_BANK1_AREAA;
    HAL_FLASHEx_OBGetConfig(&OBStruct);
    /* PCROP */
    if(OBStruct.PCROPEndAddr > OBStruct.PCROPStartAddr)
    {
        if(OBStruct.PCROPStartAddr >= APP_ADDRESS)
        {
            protection |= BL_PROTECTION_PCROP;
        }
    }
    /* WRP Area_A */
    if(OBStruct.WRPEndOffset > OBStruct.WRPStartOffset)
    {
        if((OBStruct.WRPStartOffset * FLASH_PAGE_SIZE + FLASH_BASE) >= APP_ADDRESS)
        {
            protection |= BL_PROTECTION_WRP;
        }
    }
    
    OBStruct.WRPArea = OB_WRPAREA_BANK1_AREAB;
    HAL_FLASHEx_OBGetConfig(&OBStruct);
    /* WRP Area_B */
    if(OBStruct.WRPEndOffset > OBStruct.WRPStartOffset)
    {
        if((OBStruct.WRPStartOffset * FLASH_PAGE_SIZE + FLASH_BASE) >= APP_ADDRESS)
        {
            protection |= BL_PROTECTION_WRP;
        }
    }
    
    /** Bank 2 **/
    OBStruct.PCROPConfig = FLASH_BANK_2;
    OBStruct.WRPArea = OB_WRPAREA_BANK2_AREAA;
    HAL_FLASHEx_OBGetConfig(&OBStruct);
    /* PCROP */
    if(OBStruct.PCROPEndAddr > OBStruct.PCROPStartAddr)
    {
        if(OBStruct.PCROPStartAddr >= APP_ADDRESS)
        {
            protection |= BL_PROTECTION_PCROP;
        }
    }
    /* WRP Area_A */
    if(OBStruct.WRPEndOffset > OBStruct.WRPStartOffset)
    {
        if((OBStruct.WRPStartOffset * FLASH_PAGE_SIZE + FLASH_BASE + FLASH_PAGE_SIZE * FLASH_PAGE_NBPERBANK) >= APP_ADDRESS)
        {
            protection |= BL_PROTECTION_WRP;
        }
    }
    
    OBStruct.WRPArea = OB_WRPAREA_BANK2_AREAB;
    HAL_FLASHEx_OBGetConfig(&OBStruct);
    /* WRP Area_B */
    if(OBStruct.WRPEndOffset > OBStruct.WRPStartOffset)
    {
        if((OBStruct.WRPStartOffset * FLASH_PAGE_SIZE + FLASH_BASE + FLASH_PAGE_SIZE * FLASH_PAGE_NBPERBANK) >= APP_ADDRESS)
        {
            protection |= BL_PROTECTION_WRP;
        }
    }
   
    /** RDP **/
    if(OBStruct.RDPLevel != OB_RDP_LEVEL_0)
    {
        protection |= BL_PROTECTION_RDP;
    }    
    
    HAL_FLASH_Lock();
    return protection;
#else
		return 0;
#endif
}
//



/*** Program 64bit data into flash ********************************************/
uint8_t BSP_Bootloader_FlashNext(uint64_t data)
{
#if FLASH_MEMORY == FLASHTYPE_INTERNAL
    if( !(flash_ptr <= (FLASH_BASE + FLASH_END - 8)) || (flash_ptr < APP_ADDRESS) )
    {
        HAL_FLASH_Lock();
        return BL_WRITE_ERROR;
    }
    
    if(HAL_FLASH_Program(FLASH_TYPEPROGRAM_DOUBLEWORD, flash_ptr, data) == HAL_OK)      
    {
        /* Check the written value */
        if(*(uint64_t*)flash_ptr != data)
        {
            /* Flash content doesn't match source content */
            HAL_FLASH_Lock();
            return BL_WRITE_ERROR;
        }   
        /* Increment Flash destination address */
        flash_ptr += 8;
    }
    else
    {
        /* Error occurred while writing data into Flash */
        HAL_FLASH_Lock();
        return BL_WRITE_ERROR;
    }
    
    return BL_OK;
#else
//		return BL_FLASH_CORRUPT;
	return 0;
#endif
}
//

/*** Verify checksum of application located in flash **************************/
//uint8_t BSP_Bootloader_VerifyChecksum(xROM_TypeDef *xROM)
//{
//#if (USE_CHECKSUM)
//    CRC_HandleTypeDef CrcHandle;
//    volatile uint32_t calculatedCrc = 0;
//    
//    __HAL_RCC_CRC_CLK_ENABLE();
//    CrcHandle.Instance = CRC;
//    CrcHandle.Init.DefaultPolynomialUse    = DEFAULT_POLYNOMIAL_ENABLE;
//    CrcHandle.Init.DefaultInitValueUse     = DEFAULT_INIT_VALUE_ENABLE;
//    CrcHandle.Init.InputDataInversionMode  = CRC_INPUTDATA_INVERSION_NONE;
//    CrcHandle.Init.OutputDataInversionMode = CRC_OUTPUTDATA_INVERSION_DISABLE;
//    CrcHandle.InputDataFormat              = CRC_INPUTDATA_FORMAT_WORDS;
//    if(HAL_CRC_Init(&CrcHandle) != HAL_OK)
//    {    
//        return BL_CHKS_ERROR;
//    }
//    
//    calculatedCrc = HAL_CRC_Calculate(&CrcHandle, (uint32_t*)xROM->address, xROM->size);
//    
//    __HAL_RCC_CRC_FORCE_RESET();
//    __HAL_RCC_CRC_RELEASE_RESET();
//    
//    if( xROM->crc == calculatedCrc )
//    {
//        return BL_OK;
//    }
//#endif
//    return BL_CHKS_ERROR;
//}
//

/*** Check for application in user flash **************************************/
uint8_t BSP_Bootloader_CheckForApplication(uint32_t address)
{
//    return ( ((*(__IO uint32_t*)address) & ~(INTERNAL_RAM_SIZE - 1)) == 0x20000000 ) ? BL_OK : BL_NO_APP;
	
	uint32_t stackPointer = *((uint32_t *)(address));
	if( (stackPointer >= RAMDTCM_BASE) && (stackPointer < SRAM2_BASE) )
		return BL_OK;
	else
		return BL_NO_APP;
}
//

/*** Configure flash write protection ***********************************************/
uint8_t Bootloader_ConfigProtection(uint32_t protection)
{
#if FLASH_MEMORY == FLASHTYPE_INTERNAL
    FLASH_OBProgramInitTypeDef  OBStruct = {0};
    HAL_StatusTypeDef           status = HAL_ERROR;
    
    status = HAL_FLASH_Unlock();
    status |= HAL_FLASH_OB_Unlock();

    /* Bank 1 */
    OBStruct.WRPArea = OB_WRPAREA_BANK1_AREAA;    
    OBStruct.OptionType = OPTIONBYTE_WRP;
    if(protection & BL_PROTECTION_WRP)
    {
        /* Enable WRP protection for application space */
        OBStruct.WRPStartOffset = (APP_ADDRESS - FLASH_BASE) / FLASH_PAGE_SIZE;
        OBStruct.WRPEndOffset = FLASH_PAGE_NBPERBANK - 1;
    }
    else
    {
        /* Remove WRP protection */
        OBStruct.WRPStartOffset = 0xFF;
        OBStruct.WRPEndOffset = 0x00;
    }
    status |= HAL_FLASHEx_OBProgram(&OBStruct);

    /* Area B is not used */
    OBStruct.WRPArea = OB_WRPAREA_BANK1_AREAB;    
    OBStruct.OptionType = OPTIONBYTE_WRP;
    OBStruct.WRPStartOffset = 0xFF;
    OBStruct.WRPEndOffset = 0x00;
    status |= HAL_FLASHEx_OBProgram(&OBStruct);

    /* Bank 2 */
    OBStruct.WRPArea = OB_WRPAREA_BANK2_AREAA;
    OBStruct.OptionType = OPTIONBYTE_WRP;
    if(protection & BL_PROTECTION_WRP)
    {
         /* Enable WRP protection for application space */
        OBStruct.WRPStartOffset = 0x00;
        OBStruct.WRPEndOffset = FLASH_PAGE_NBPERBANK - 1;
    }
    else
    {
        /* Remove WRP protection */
        OBStruct.WRPStartOffset = 0xFF;
        OBStruct.WRPEndOffset = 0x00;
    }
    status |= HAL_FLASHEx_OBProgram(&OBStruct);

    /* Area B is not used */
    OBStruct.WRPArea = OB_WRPAREA_BANK2_AREAB;
    OBStruct.OptionType = OPTIONBYTE_WRP;
    OBStruct.WRPStartOffset = 0xFF;
    OBStruct.WRPEndOffset = 0x00;
    status |= HAL_FLASHEx_OBProgram(&OBStruct);
    
    if(status == HAL_OK)
    {
        /* Loading Flash Option Bytes - this generates a system reset. */ 
        status |= HAL_FLASH_OB_Launch();
    }
    
    status |= HAL_FLASH_OB_Lock();
    status |= HAL_FLASH_Lock();

    return (status == HAL_OK) ? BL_OK : BL_OBP_ERROR;
#else
		return 0;
#endif
}
//

//bool BSP_Bootloader_Set_BootTable(xExternalFlash_TypeDef *extFlash)
//{
//	assert_param(extFlash != NULL);
//	
//	uint8_t err = BSP_QSPI_Write((uint8_t *)extFlash, BOOTTABLE_BASE, sizeof(xExternalFlash_TypeDef));
//	
//	return (err == QSPI_OK);
//}
//

/************************ (C) COPYRIGHT YiLDiZ IoT *****************************/

