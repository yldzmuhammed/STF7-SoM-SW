/**
  ******************************************************************************
  * @file    stf7som_qspi.c
  * @author  yldzmuhammed
  * @version V1.0
  * @date    13-06-2020
  * @brief   qspi flash Underlying application function bsp 
  ******************************************************************************
  * @attention
  *
  * experiment platform: YiLDiZ IoT STF7-SoM Board BSP Library
  * web-site: https://www.yildiziot.com
  ******************************************************************************
*/

#include "stf7som.h"
#include "stf7som_qspi.h"


/* Private typedef -----------------------------------------------------------*/
/* Private define ------------------------------------------------------------*/
#define DUMMY_CLOCK_ENABLE                  (0x01)
/* Private macro -------------------------------------------------------------*/
/* Private variables ---------------------------------------------------------*/
bool QPIMode_Entered;

QSPI_HandleTypeDef QSPIHandle;
/* Private function prototypes -----------------------------------------------*/
/* Private user code ---------------------------------------------------------*/
/* Private user function ------------------------------------------------------*/
/* Public user function ------------------------------------------------------*/
static uint8_t QSPI_ResetMemory(void);
static uint8_t QSPI_WriteEnable(void);
static uint8_t QSPI_DummyCyclesCfg(void);
static uint8_t QSPI_AutoPollingMemReady(uint32_t Timeout);

/**
  * @brief  Reset QSPI memory.
  * @param  QSPIHandle: QSPI handle
  * @retval NONE
  */
static uint8_t QSPI_ResetMemory(void)
{
	QSPI_CommandTypeDef s_command;
	
	/* Initialize reset enable command */
	s_command.DummyCycles       = 0;
	s_command.DataMode          = QSPI_DATA_NONE;
	s_command.Instruction       = RESET_ENABLE_CMD;
	s_command.AddressMode       = QSPI_ADDRESS_NONE;
	s_command.DdrMode           = QSPI_DDR_MODE_DISABLE;
	s_command.InstructionMode   = QSPI_INSTRUCTION_1_LINE;
	s_command.SIOOMode          = QSPI_SIOO_INST_EVERY_CMD;
	s_command.AlternateByteMode = QSPI_ALTERNATE_BYTES_NONE;
	s_command.DdrHoldHalfCycle  = QSPI_DDR_HHC_ANALOG_DELAY;

	/* send command */
	if (HAL_QSPI_Command(&QSPIHandle, &s_command, HAL_QPSI_TIMEOUT_DEFAULT_VALUE) != HAL_OK)
		return QSPI_ERROR;

	/* Send reset memory command */
	s_command.Instruction = RESET_MEMORY_CMD;
	if (HAL_QSPI_Command(&QSPIHandle, &s_command, HAL_QPSI_TIMEOUT_DEFAULT_VALUE) != HAL_OK)
		return QSPI_ERROR;

	/* Configure automatic polling mode to wait for memory ready */  
	if (QSPI_AutoPollingMemReady(HAL_QPSI_TIMEOUT_DEFAULT_VALUE) != QSPI_OK)
    return QSPI_ERROR;
	
	return QSPI_OK;
}
//

/**
  * @brief  Send write enable, wait for it to be valid.
  * @param  QSPIHandle: QSPI handle
  * @retval NONE
  */
static uint8_t QSPI_WriteEnable(void)
{
	QSPI_CommandTypeDef     s_command;
	QSPI_AutoPollingTypeDef s_config;
	
	/* Enable write operation */
	s_command.DummyCycles       = 0;
	s_command.DataMode          = QSPI_DATA_NONE;
	s_command.Instruction       = WRITE_ENABLE_CMD;
	s_command.AddressMode       = QSPI_ADDRESS_NONE;
	s_command.DdrMode           = QSPI_DDR_MODE_DISABLE;
	s_command.InstructionMode   = QSPI_INSTRUCTION_1_LINE;
	s_command.SIOOMode          = QSPI_SIOO_INST_EVERY_CMD;
	s_command.AlternateByteMode = QSPI_ALTERNATE_BYTES_NONE;
	s_command.DdrHoldHalfCycle  = QSPI_DDR_HHC_ANALOG_DELAY;
	if (HAL_QSPI_Command(&QSPIHandle, &s_command, HAL_QPSI_TIMEOUT_DEFAULT_VALUE) != HAL_OK)
		return QSPI_ERROR;

	/* Configure automatic polling mode to wait for write enablement */
	s_config.StatusBytesSize = 1;
	s_config.Interval        = 0x10;
	s_config.Match           = W25Q128FV_FSR_WREN;
	s_config.Mask            = W25Q128FV_FSR_WREN;
	s_config.MatchMode       = QSPI_MATCH_MODE_AND;
	s_config.AutomaticStop   = QSPI_AUTOMATIC_STOP_ENABLE;

	s_command.NbData         = 1;
	s_command.DataMode       = QSPI_DATA_1_LINE;
	s_command.Instruction    = READ_STATUS_REG1_CMD;
	if (HAL_QSPI_AutoPolling(&QSPIHandle, &s_command, &s_config, HAL_QPSI_TIMEOUT_DEFAULT_VALUE) != HAL_OK)
		return QSPI_ERROR;
	
	return QSPI_OK;
}
//

/**
  * @brief  This function configure the dummy cycles on memory side.
  * @param  hqspi: QSPI handle
  * @retval None
  */
static uint8_t QSPI_DummyCyclesCfg(void)
{
	uint8_t reg = DUMMY_CLOCK_CYCLES_8;
	QSPI_CommandTypeDef	s_command	= {0};

	/* Enable write operations */
	if (QSPI_WriteEnable() != QSPI_OK)
		return QSPI_ERROR;

	/* Initialize the read volatile configuration register command */
    s_command.NbData            = 1;
	s_command.DataMode          = QSPI_DATA_4_LINES;
	s_command.AddressMode       = QSPI_ADDRESS_NONE;
	s_command.DdrMode           = QSPI_DDR_MODE_DISABLE;
	s_command.Instruction       = SET_READ_PARAMETERS_CMD;
	s_command.InstructionMode   = QSPI_INSTRUCTION_4_LINES;
	s_command.SIOOMode          = QSPI_SIOO_INST_EVERY_CMD;
	s_command.DdrHoldHalfCycle  = QSPI_DDR_HHC_ANALOG_DELAY;
	s_command.AlternateByteMode = QSPI_ALTERNATE_BYTES_NONE;
	if (HAL_QSPI_Command(&QSPIHandle, &s_command, HAL_QPSI_TIMEOUT_DEFAULT_VALUE) != HAL_OK)
		return QSPI_ERROR;

	/* Transmission of the data */
	if (HAL_QSPI_Transmit(&QSPIHandle, &reg, HAL_QPSI_TIMEOUT_DEFAULT_VALUE) != HAL_OK)
		return QSPI_ERROR;

	if(QSPI_AutoPollingMemReady(HAL_QPSI_TIMEOUT_DEFAULT_VALUE) != QSPI_OK)
		return QSPI_ERROR;
	
  return QSPI_OK;
}
//

/**
  * @brief  Read the SR of the memory and wait for EOP
  * @param  QSPIHandle: QSPI handle
  * @param  Timeout time out
  * @retval NONE
  */
static uint8_t QSPI_AutoPollingMemReady(uint32_t Timeout)
{
	QSPI_CommandTypeDef     s_command;
	QSPI_AutoPollingTypeDef s_config;
	
	/* Configure automatic polling mode to wait for memory ready */  
	s_command.DummyCycles       = 0;
	s_command.DataMode          = QSPI_DATA_1_LINE;
	s_command.AddressMode       = QSPI_ADDRESS_NONE;
	s_command.Instruction       = READ_STATUS_REG1_CMD;
	s_command.DdrMode           = QSPI_DDR_MODE_DISABLE;
	s_command.InstructionMode   = QSPI_INSTRUCTION_1_LINE;
	s_command.SIOOMode          = QSPI_SIOO_INST_EVERY_CMD;
	s_command.AlternateByteMode = QSPI_ALTERNATE_BYTES_NONE;
	s_command.DdrHoldHalfCycle  = QSPI_DDR_HHC_ANALOG_DELAY;

	s_config.Match           	= 0;
	s_config.StatusBytesSize 	= 1;
	s_config.Interval        	= 0x10;
	s_config.Mask            	= W25Q128FV_FSR_BUSY;
	s_config.MatchMode       	= QSPI_MATCH_MODE_AND;
	s_config.AutomaticStop   	= QSPI_AUTOMATIC_STOP_ENABLE;
	if (HAL_QSPI_AutoPolling(&QSPIHandle, &s_command, &s_config, Timeout) != HAL_OK)
		return QSPI_ERROR;
		
	return QSPI_OK;
}
//

/**
* @brief QSPI MSP Initialization
* This function configures the hardware resources used in this example
* @param hqspi: QSPI handle pointer
* @retval None
*/
void HAL_QSPI_MspInit(QSPI_HandleTypeDef* hqspi)
{
    GPIO_InitTypeDef GPIO_InitStruct = {0};
    /* Peripheral clock enable */
    QSPI_FLASH_CLK_ENABLE();
  
    GPIO_InitStruct.Pull        = GPIO_NOPULL;
    GPIO_InitStruct.Mode        = GPIO_MODE_AF_PP;
    GPIO_InitStruct.Speed       = GPIO_SPEED_FREQ_VERY_HIGH;

    /** QUADSPI GPIO Configuration
      * PB2     ------> QUADSPI_CLK
      * PB6     ------> QUADSPI_BK1_NCS
      * PF8     ------> QUADSPI_BK1_IO0
      * PF9     ------> QUADSPI_BK1_IO1
      * PE2     ------> QUADSPI_BK1_IO2
      * PF6     ------> QUADSPI_BK1_IO3
      */

    /* Before any bsp function, please call BSP_GPIOCLK_Enable(); function! */

    /*!< Configure the QSPI_FLASH pin: CLK */
    GPIO_InitStruct.Pin         = QSPI_FLASH_CLK_PIN;
    GPIO_InitStruct.Alternate   = QSPI_FLASH_CLK_GPIO_AF;
    HAL_GPIO_Init(QSPI_FLASH_CLK_GPIO_PORT, &GPIO_InitStruct);

    /*!< Configure the QSPI_FLASH pin: IO0 */
    GPIO_InitStruct.Alternate   = QSPI_FLASH_BK1_IO0_AF;
    GPIO_InitStruct.Pin         = QSPI_FLASH_BK1_IO0_PIN;
    HAL_GPIO_Init(QSPI_FLASH_BK1_IO0_PORT, &GPIO_InitStruct);

    /*!< Configure the QSPI_FLASH pin: IO1 */
    GPIO_InitStruct.Alternate   = QSPI_FLASH_BK1_IO1_AF;
    GPIO_InitStruct.Pin         = QSPI_FLASH_BK1_IO1_PIN;
    HAL_GPIO_Init(QSPI_FLASH_BK1_IO1_PORT, &GPIO_InitStruct);

    /*!< Configure the QSPI_FLASH pin: IO2 */
    GPIO_InitStruct.Alternate = QSPI_FLASH_BK1_IO2_AF;
    GPIO_InitStruct.Pin 		  = QSPI_FLASH_BK1_IO2_PIN;
    HAL_GPIO_Init(QSPI_FLASH_BK1_IO2_PORT, &GPIO_InitStruct);

    /*!< Configure the QSPI_FLASH pin: IO3 */
    GPIO_InitStruct.Alternate = QSPI_FLASH_BK1_IO3_AF;
    GPIO_InitStruct.Pin 		  = QSPI_FLASH_BK1_IO3_PIN;
    HAL_GPIO_Init(QSPI_FLASH_BK1_IO3_PORT, &GPIO_InitStruct);

    /*!< Configure the QSPI_FLASH pin: NCS */
    GPIO_InitStruct.Pin 		  = QSPI_FLASH_CS_PIN;
    GPIO_InitStruct.Alternate = QSPI_FLASH_CS_GPIO_AF;
    HAL_GPIO_Init(QSPI_FLASH_CS_GPIO_PORT, &GPIO_InitStruct);
}
//

/**
* @brief QSPI MSP De-Initialization
* This function freeze the hardware resources used in this example
* @param hqspi: QSPI handle pointer
* @retval None
*/
void HAL_QSPI_MspDeInit(QSPI_HandleTypeDef* hqspi)
{
    /* Peripheral clock disable */
    __HAL_RCC_QSPI_CLK_DISABLE();

    /** QUADSPI GPIO Configuration
      * PB2     ------> QUADSPI_CLK
      * PB6     ------> QUADSPI_BK1_NCS
      * PF8     ------> QUADSPI_BK1_IO0
      * PF9     ------> QUADSPI_BK1_IO1
      * PE2     ------> QUADSPI_BK1_IO2
      * PF6     ------> QUADSPI_BK1_IO3
      */

    HAL_GPIO_DeInit(QSPI_FLASH_CS_GPIO_PORT,    QSPI_FLASH_CS_PIN);
    HAL_GPIO_DeInit(QSPI_FLASH_CLK_GPIO_PORT,   QSPI_FLASH_CLK_PIN);
    HAL_GPIO_DeInit(QSPI_FLASH_BK1_IO0_PORT,    QSPI_FLASH_BK1_IO0_PIN);
    HAL_GPIO_DeInit(QSPI_FLASH_BK1_IO1_PORT,    QSPI_FLASH_BK1_IO1_PIN);
    HAL_GPIO_DeInit(QSPI_FLASH_BK1_IO2_PORT,    QSPI_FLASH_BK1_IO2_PIN);
    HAL_GPIO_DeInit(QSPI_FLASH_BK1_IO3_PORT,    QSPI_FLASH_BK1_IO3_PIN);
}
//

/**
  * @brief  QSPI_FLASH pin initialization
  * @param  NONE
  * @retval NONE
  */  
uint8_t BSP_QSPI_Init()
{
    /* QSPI_FLASH mode configuration */
    QSPIHandle.Instance             = QUADSPI;

    /*divided by two, the clock is 200 / (1 + 1) = 100MHz */
    QSPIHandle.Init.ClockPrescaler  = 1;

    /* FIFO threshold is 4 bytes */
    QSPIHandle.Init.FifoThreshold   = 4;

    /* Sample shift half cycle */
    QSPIHandle.Init.SampleShifting  = QSPI_SAMPLE_SHIFTING_HALFCYCLE;

    /* Flash size is 16M bytes, 2^24, so the weight is 24-1=23 */
    QSPIHandle.Init.FlashSize       = 23;

    /* Chip select high hold time, at least 50ns, corresponding to the number of cycles 6 * 9.2ns = 55.2ns */
    QSPIHandle.Init.ChipSelectHighTime = QSPI_CS_HIGH_TIME_6_CYCLE;

    /* Clock mode select mode 0, when CLK is high (chip select release), CLK must be held low */
    QSPIHandle.Init.ClockMode = QSPI_CLOCK_MODE_0;

    /* Select the first piece of Flash based on the hardware connection */
    QSPIHandle.Init.FlashID = QSPI_FLASH_ID_1;
    HAL_QSPI_Init(&QSPIHandle);

    /* QSPI memory reset */
    if (QSPI_ResetMemory() != QSPI_OK)
        return QSPI_NOT_SUPPORTED;

#if (DUMMY_CLOCK_ENABLE == 1)
    if(QSPI_DummyCyclesCfg() != QSPI_OK)
        return QSPI_NOT_SUPPORTED;
#endif

    return QSPI_OK;
}
//

/**
  * @brief  De-Initializes the QSPI interface.
  * @retval QSPI memory status
  */
uint8_t BSP_QSPI_DeInit(void)
{ 
	QSPIHandle.Instance = QUADSPI;

	/* Call the DeInit function to reset the driver */
	if (HAL_QSPI_DeInit(&QSPIHandle) != HAL_OK)
		return QSPI_ERROR;

	/* 1- Disable the NVIC for QSPI */
	HAL_NVIC_DisableIRQ(QUADSPI_IRQn);

	/* 2- Disable peripherals and GPIO Clocks */
	/* De-Configure QSPI pins */
	HAL_GPIO_DeInit(QSPI_FLASH_CS_GPIO_PORT, QSPI_FLASH_CS_PIN);
	HAL_GPIO_DeInit(QSPI_FLASH_CLK_GPIO_PORT, QSPI_FLASH_CLK_PIN);
	HAL_GPIO_DeInit(QSPI_FLASH_BK1_IO0_PORT, QSPI_FLASH_BK1_IO0_PIN);
	HAL_GPIO_DeInit(QSPI_FLASH_BK1_IO1_PORT, QSPI_FLASH_BK1_IO1_PIN);
	HAL_GPIO_DeInit(QSPI_FLASH_BK1_IO2_PORT, QSPI_FLASH_BK1_IO2_PIN);
	HAL_GPIO_DeInit(QSPI_FLASH_BK1_IO3_PORT, QSPI_FLASH_BK1_IO3_PIN);

	/* 3- Reset peripherals */
	/* Reset the QuadSPI memory interface */
	__QSPI_FORCE_RESET();
	__QSPI_RELEASE_RESET();

	/* Disable the QuadSPI memory interface clock */
	__QSPI_CLK_DISABLE();

	return QSPI_OK;
}
//

/**
  * @brief  Read the current state of the QSPI memory
  * @retval QSPI memory status
  */
uint8_t BSP_QSPI_GetStatus(void)
{
	uint8_t reg;
	QSPI_CommandTypeDef s_command;
	
	/* Initialize read status register command */
	s_command.DummyCycles       = 0;
	s_command.NbData            = 1;
	s_command.DataMode          = QSPI_DATA_1_LINE;
	s_command.AddressMode       = QSPI_ADDRESS_NONE;
	s_command.Instruction       = READ_STATUS_REG1_CMD;
	s_command.DdrMode           = QSPI_DDR_MODE_DISABLE;
	s_command.InstructionMode   = QSPI_INSTRUCTION_1_LINE;
	s_command.SIOOMode          = QSPI_SIOO_INST_EVERY_CMD;
	s_command.AlternateByteMode = QSPI_ALTERNATE_BYTES_NONE;
	s_command.DdrHoldHalfCycle  = QSPI_DDR_HHC_ANALOG_DELAY;
	if (HAL_QSPI_Command(&QSPIHandle, &s_command, HAL_QPSI_TIMEOUT_DEFAULT_VALUE) != HAL_OK)
		return QSPI_ERROR;
	
	/* Receive data */
	if (HAL_QSPI_Receive(&QSPIHandle, &reg, HAL_QPSI_TIMEOUT_DEFAULT_VALUE) != HAL_OK)
		return QSPI_ERROR;
	
	/* Check the value of the register */
	if((reg & W25Q128FV_FSR_BUSY) != 0)
		return QSPI_BUSY;
	else
		return QSPI_OK;
}
//

/**
  * @brief  Erase the entire QSPI memory
  * @retval QSPI memory status
  */
uint8_t BSP_QSPI_Erase_Chip(void)
{
	QSPI_CommandTypeDef s_command;
	
	/* Initialize erase command */
	s_command.DummyCycles       = 0;
	s_command.DataMode          = QSPI_DATA_NONE;
	s_command.Instruction       = CHIP_ERASE_CMD;
	s_command.AddressMode       = QSPI_ADDRESS_NONE;
	s_command.DdrMode           = QSPI_DDR_MODE_DISABLE;
	s_command.InstructionMode   = QSPI_INSTRUCTION_1_LINE;
	s_command.SIOOMode          = QSPI_SIOO_INST_EVERY_CMD;
	s_command.AlternateByteMode = QSPI_ALTERNATE_BYTES_NONE;
	s_command.DdrHoldHalfCycle  = QSPI_DDR_HHC_ANALOG_DELAY;
	
	/* Enable write operation */
	if (QSPI_WriteEnable() != QSPI_OK)
		return QSPI_ERROR;
	
	/* send command */
	if (HAL_QSPI_Command(&QSPIHandle, &s_command, HAL_QPSI_TIMEOUT_DEFAULT_VALUE) != HAL_OK)
		return QSPI_ERROR;
	
	/* Configure automatic polling mode to wait for end of erase */  
	if (QSPI_AutoPollingMemReady(W25Q128FV_BULK_ERASE_MAX_TIME) != QSPI_OK)
		return QSPI_ERROR;
	
	return QSPI_OK;
}
//

/**
  * @brief  Initialize QSPI memory
  * @retval QSPI memory status
  */
uint8_t BSP_QSPI_Enter_QPIMode(void)
{
	QSPI_CommandTypeDef sCommand = {0};
	
	/** enable qpi mode instruction */
	if(BSP_QSPI_Enable_QPIMode() != QSPI_OK)
		return QSPI_ERROR;
	
	/* Set the four-way enable status register to enable four-channel IO2 and IO3 pins */
	sCommand.NbData            	= 1;
	sCommand.DummyCycles       	= 0;
	sCommand.DataMode          	= QSPI_DATA_1_LINE;
	sCommand.AddressMode       	= QSPI_ADDRESS_NONE;
	sCommand.Instruction        = ENTER_QPI_MODE_CMD;
	sCommand.DdrMode           	= QSPI_DDR_MODE_DISABLE;
	sCommand.InstructionMode   	= QSPI_INSTRUCTION_1_LINE;
	sCommand.SIOOMode          	= QSPI_SIOO_INST_EVERY_CMD;
	sCommand.DdrHoldHalfCycle  	= QSPI_DDR_HHC_ANALOG_DELAY;
	sCommand.AlternateByteMode 	= QSPI_ALTERNATE_BYTES_NONE;
	if (HAL_QSPI_Command(&QSPIHandle, &sCommand, HAL_QPSI_TIMEOUT_DEFAULT_VALUE) != HAL_OK)
		return QSPI_ERROR;
	
	/** configure dummy clock cycles */
	if(QSPI_DummyCyclesCfg() != QSPI_OK)
		return QSPI_ERROR;
	
	QPIMode_Entered = true;
	
	return QSPI_OK;
}
//

/**
  * @brief  Initialize QSPI memory
  * @retval QSPI memory status
  */
uint8_t BSP_QSPI_Enable_QPIMode(void)
{
	uint8_t reg = W25Q128FV_FSR_QE;
	
	QSPI_CommandTypeDef sCommand 		= {0};
	GPIO_InitTypeDef 	GPIO_InitStruct = {0};
	
	/* Enable write operation */
	if (QSPI_WriteEnable() != QSPI_OK)
		return QSPI_ERROR;
	
	/*!< Configure the QSPI_FLASH pin: GPIO */
	GPIO_InitStruct.Pull		= GPIO_NOPULL;
	GPIO_InitStruct.Mode 		= GPIO_MODE_OUTPUT_PP;
	GPIO_InitStruct.Pin 		= QSPI_FLASH_BK1_IO2_PIN;
	GPIO_InitStruct.Speed		= GPIO_SPEED_FREQ_VERY_HIGH;
	HAL_GPIO_Init(QSPI_FLASH_BK1_IO2_PORT, &GPIO_InitStruct);
	HAL_GPIO_WritePin(QSPI_FLASH_BK1_IO2_PORT, QSPI_FLASH_BK1_IO2_PIN, GPIO_PIN_SET);
  
	/* Set the four-way enable status register to enable four-channel IO2 and IO3 pins */
	sCommand.NbData            	= 1;
	sCommand.DummyCycles       	= 0;
	sCommand.DataMode          	= QSPI_DATA_1_LINE;
	sCommand.AddressMode       	= QSPI_ADDRESS_NONE;
	sCommand.DdrMode           	= QSPI_DDR_MODE_DISABLE;
	sCommand.Instruction        = WRITE_STATUS_REG2_CMD;
	sCommand.InstructionMode   	= QSPI_INSTRUCTION_1_LINE;
	sCommand.SIOOMode          	= QSPI_SIOO_INST_EVERY_CMD;
	sCommand.DdrHoldHalfCycle  	= QSPI_DDR_HHC_ANALOG_DELAY;
	sCommand.AlternateByteMode 	= QSPI_ALTERNATE_BYTES_NONE;
	if (HAL_QSPI_Command(&QSPIHandle, &sCommand, HAL_QPSI_TIMEOUT_DEFAULT_VALUE) != HAL_OK)
		return QSPI_ERROR;
	
	/* transfer data */
	if (HAL_QSPI_Transmit(&QSPIHandle, &reg, HAL_QPSI_TIMEOUT_DEFAULT_VALUE) != HAL_OK)
		return QSPI_ERROR;
	
	/* Automatic polling mode waits for memory ready */  
	if (QSPI_AutoPollingMemReady(W25Q128FV_SUBSECTOR_ERASE_MAX_TIME) != QSPI_OK)
		return QSPI_ERROR;
	
	sCommand.Instruction       	= READ_STATUS_REG2_CMD;
	if (HAL_QSPI_Command(&QSPIHandle, &sCommand, HAL_QPSI_TIMEOUT_DEFAULT_VALUE) != HAL_OK)
		return QSPI_ERROR;
	
	if(HAL_QSPI_Receive(&QSPIHandle, &reg, HAL_QPSI_TIMEOUT_DEFAULT_VALUE) != HAL_OK)
		return QSPI_ERROR;
	
	/* transfer data */
	reg = 0x80;
	if (HAL_QSPI_Transmit(&QSPIHandle, &reg, HAL_QPSI_TIMEOUT_DEFAULT_VALUE) != HAL_OK)
		return QSPI_ERROR;
	
	/* Automatic polling mode waits for memory ready */  
	if (QSPI_AutoPollingMemReady(W25Q128FV_SUBSECTOR_ERASE_MAX_TIME) != QSPI_OK)
		return QSPI_ERROR;
	
	/*!< Configure the QSPI_FLASH pin: IO2 */
	GPIO_InitStruct.Mode 				= GPIO_MODE_AF_PP;
	GPIO_InitStruct.Alternate 	= QSPI_FLASH_BK1_IO2_AF;
	HAL_GPIO_Init(QSPI_FLASH_BK1_IO2_PORT, &GPIO_InitStruct);
  
	return QSPI_OK;
}
//

/**
  * @brief  Read FLASH ID
  * @param 	Pointer for Flash ID
  * @retval FLASH ID
  */
uint8_t BSP_QSPI_ReadID(uint32_t *id)
{
	uint8_t pData[3];
	QSPI_CommandTypeDef s_command;
	
	s_command.NbData            = 3;
	s_command.DummyCycles       = 0;
	s_command.DataMode          = QSPI_DATA_1_LINE;
	s_command.AddressMode       = QSPI_ADDRESS_NONE;
	s_command.Instruction       = READ_JEDEC_ID_CMD;
	s_command.AddressSize       = QSPI_ADDRESS_24_BITS;
	s_command.DdrMode           = QSPI_DDR_MODE_DISABLE;
	s_command.InstructionMode   = QSPI_INSTRUCTION_1_LINE;
	s_command.SIOOMode          = QSPI_SIOO_INST_EVERY_CMD;
	s_command.DdrHoldHalfCycle  = QSPI_DDR_HHC_ANALOG_DELAY;
	s_command.AlternateByteMode = QSPI_ALTERNATE_BYTES_NONE;
	if (HAL_QSPI_Command(&QSPIHandle, &s_command, HAL_QPSI_TIMEOUT_DEFAULT_VALUE) != HAL_OK)
		return QSPI_ERROR;
	
	if (HAL_QSPI_Receive(&QSPIHandle, pData, HAL_QPSI_TIMEOUT_DEFAULT_VALUE) != HAL_OK)
		return QSPI_ERROR;

	*id = ( pData[2] | pData[1] << 8 )| ( pData[0] << 16 );

	return QSPI_OK;
}
//

/**
  * @brief  Read FLASH Device ID
  * @param 	Pointer for devid
  * @retval FLASH Device ID
  */
uint8_t BSP_QSPI_ReadDeviceID(uint32_t *devid)
{  
	uint8_t pData[3];
	QSPI_CommandTypeDef s_command = {0};
	
	s_command.NbData            = 2;
	s_command.Instruction       = READ_ID_CMD;
	s_command.DataMode          = QSPI_DATA_1_LINE;
	s_command.AddressMode       = QSPI_ADDRESS_1_LINE;
	s_command.AddressSize       = QSPI_ADDRESS_24_BITS;
	s_command.DdrMode           = QSPI_DDR_MODE_DISABLE;
	s_command.InstructionMode   = QSPI_INSTRUCTION_1_LINE;
	s_command.SIOOMode          = QSPI_SIOO_INST_EVERY_CMD;
	s_command.DdrHoldHalfCycle  = QSPI_DDR_HHC_ANALOG_DELAY;
	s_command.AlternateByteMode = QSPI_ALTERNATE_BYTES_NONE;

	if (HAL_QSPI_Command(&QSPIHandle, &s_command, HAL_QPSI_TIMEOUT_DEFAULT_VALUE) != HAL_OK)
		return QSPI_ERROR;
	
	if (HAL_QSPI_Receive(&QSPIHandle, pData, HAL_QPSI_TIMEOUT_DEFAULT_VALUE) != HAL_OK)
		return QSPI_ERROR;

	*devid = pData[1] |( pData[0]<<8 ) ;

	return QSPI_OK;
}
//

/**
  * @brief  Read FLASH Uniq Device ID
  * @param 	Pointer for devid
  * @retval FLASH Device ID
  */
uint8_t BSP_QSPI_ReadUniqDevID(uint32_t *devid)
{  
	uint8_t pData[3] = {0};
	QSPI_CommandTypeDef s_command = {0};
	
	s_command.NbData            = 2;
    s_command.DummyCycles       = 31;
	s_command.Instruction       = READ_UNIQ_ID_CMD;
	s_command.DataMode          = QSPI_DATA_1_LINE;
	s_command.AddressMode       = QSPI_ADDRESS_NONE;
	s_command.AddressSize       = QSPI_ADDRESS_24_BITS;
	s_command.DdrMode           = QSPI_DDR_MODE_DISABLE;
	s_command.InstructionMode   = QSPI_INSTRUCTION_1_LINE;
	s_command.SIOOMode          = QSPI_SIOO_INST_EVERY_CMD;
	s_command.DdrHoldHalfCycle  = QSPI_DDR_HHC_ANALOG_DELAY;
	s_command.AlternateByteMode = QSPI_ALTERNATE_BYTES_NONE;

	if (HAL_QSPI_Command(&QSPIHandle, &s_command, HAL_QPSI_TIMEOUT_DEFAULT_VALUE) != HAL_OK)
		return QSPI_ERROR;
	
	if (HAL_QSPI_Receive(&QSPIHandle, pData, HAL_QPSI_TIMEOUT_DEFAULT_VALUE) != HAL_OK)
		return QSPI_ERROR;

	*devid = pData[1] | ( pData[0]<<8 ) ;

	return QSPI_OK;
}
//

/**
  * @brief  Configure the QSPI in memory-mapped mode
  * @retval QSPI memory status
  */
uint8_t BSP_QSPI_EnableMemoryMappedMode(bool enable)
{
	if(enable)
	{
		QSPI_CommandTypeDef s_command = {0};
		QSPI_MemoryMappedTypeDef s_mem_mapped_cfg = {0};

		/** enable qpi mode */
		if(BSP_QSPI_Enable_QPIMode() != QSPI_OK)
			return QSPI_ERROR;

		/* Configure the command for the read instruction */
		s_command.DummyCycles           = 4;
		s_command.AlternateBytes        = 0x00;
		s_command.DataMode              = QSPI_DATA_4_LINES;
		s_command.AddressMode           = QSPI_ADDRESS_4_LINES;
		s_command.AddressSize           = QSPI_ADDRESS_24_BITS;
		s_command.DdrMode               = QSPI_DDR_MODE_DISABLE;
		s_command.InstructionMode       = QSPI_INSTRUCTION_1_LINE;
		s_command.SIOOMode              = QSPI_SIOO_INST_EVERY_CMD;
		s_command.Instruction           = QUAD_INOUT_FAST_READ_CMD;
		s_command.DdrHoldHalfCycle      = QSPI_DDR_HHC_ANALOG_DELAY;
		s_command.AlternateBytesSize    = QSPI_ALTERNATE_BYTES_8_BITS;
		s_command.AlternateByteMode     = QSPI_ALTERNATE_BYTES_4_LINES;

		s_mem_mapped_cfg.TimeOutPeriod  = 0;
		s_mem_mapped_cfg.TimeOutActivation = QSPI_TIMEOUT_COUNTER_DISABLE;
		if (HAL_QSPI_MemoryMapped(&QSPIHandle, &s_command, &s_mem_mapped_cfg) != HAL_OK)
			return QSPI_ERROR;
	}
	else
	{
		if(HAL_QSPI_Abort(&QSPIHandle) != HAL_OK)
			return QSPI_ERROR;

		if(BSP_QSPI_DeInit() != QSPI_OK)
			return QSPI_ERROR;

		if(BSP_QSPI_Init() != QSPI_OK)
			return QSPI_ERROR;
	}
	return QSPI_OK;
}
//

/**
  * @brief  Erase the specified block of QSPI memory 
  * @param  BlockAddress: Block address to be erased  
  * @retval QSPI memory status
  */
uint8_t BSP_QSPI_Erase_Sector(uint32_t SectorAddress)
{
	QSPI_CommandTypeDef s_command;
	
	/* Enable write operation */
	if (QSPI_WriteEnable() != QSPI_OK)
		return QSPI_ERROR;

	/* Initialize erase command */
	s_command.DummyCycles       = 0;
	s_command.Address           = SectorAddress;
	s_command.DataMode          = QSPI_DATA_NONE;
	s_command.Instruction       = SECTOR_ERASE_CMD;
	s_command.AddressMode       = QSPI_ADDRESS_1_LINE;
	s_command.AddressSize       = QSPI_ADDRESS_24_BITS;
	s_command.DdrMode           = QSPI_DDR_MODE_DISABLE;
	s_command.InstructionMode   = QSPI_INSTRUCTION_1_LINE;
	s_command.SIOOMode          = QSPI_SIOO_INST_EVERY_CMD;
	s_command.AlternateByteMode = QSPI_ALTERNATE_BYTES_NONE;
	s_command.DdrHoldHalfCycle  = QSPI_DDR_HHC_ANALOG_DELAY;
	if (HAL_QSPI_Command(&QSPIHandle, &s_command, HAL_QPSI_TIMEOUT_DEFAULT_VALUE) != HAL_OK)
		return QSPI_ERROR;

	/* Configure automatic polling mode to wait for end of erase */  
	if (QSPI_AutoPollingMemReady(W25Q128FV_SUBSECTOR_ERASE_MAX_TIME) != QSPI_OK)
		return QSPI_ERROR;
	
	return QSPI_OK;
}
//

/**
  * @brief  Erase the specified block of QSPI memory 
  * @param  BlockAddress: Block address to be erased  
  * @retval QSPI memory status
  */
uint8_t BSP_QSPI_Erase_Block(uint32_t BlockAddress, uint8_t blockSize)
{
	QSPI_CommandTypeDef s_command;
	
	/** check for block size */
	if(blockSize != BLOCKSIZE_32KB && blockSize != BLOCKSIZE_64KB)
		return QSPI_NOT_SUPPORTED;
	
	/* Enable write operation */
	if (QSPI_WriteEnable() != QSPI_OK)
		return QSPI_ERROR;

	/* Initialize erase command */
	s_command.DummyCycles       = 0;
	s_command.Instruction       = blockSize;
	s_command.Address           = BlockAddress;
	s_command.DataMode          = QSPI_DATA_NONE;
	s_command.AddressMode       = QSPI_ADDRESS_1_LINE;
	s_command.AddressSize       = QSPI_ADDRESS_24_BITS;
	s_command.DdrMode           = QSPI_DDR_MODE_DISABLE;
	s_command.InstructionMode   = QSPI_INSTRUCTION_1_LINE;
	s_command.SIOOMode          = QSPI_SIOO_INST_EVERY_CMD;
	s_command.AlternateByteMode = QSPI_ALTERNATE_BYTES_NONE;
	s_command.DdrHoldHalfCycle  = QSPI_DDR_HHC_ANALOG_DELAY;
	if (HAL_QSPI_Command(&QSPIHandle, &s_command, HAL_QPSI_TIMEOUT_DEFAULT_VALUE) != HAL_OK)
		return QSPI_ERROR;

	/* Configure automatic polling mode to wait for end of erase */  
	if (QSPI_AutoPollingMemReady(W25Q128FV_SUBSECTOR_ERASE_MAX_TIME) != QSPI_OK)
		return QSPI_ERROR;
	
	return QSPI_OK;
}
//

/**
  * @brief  Write large amounts of data to QSPI memory
  * @param  pData: Pointer to the data to be written
  * @param  WriteAddr: Write start address
  * @param  Size: The size of the data to be written    
  * @retval QSPI memory status
  */
uint8_t BSP_QSPI_Write(uint8_t* pData, uint32_t WriteAddr, uint32_t Size)
{
	uint32_t end_addr 		= 0;
	uint32_t current_size = 0;
	uint32_t current_addr = 0;
	
	QSPI_CommandTypeDef s_command;
	
	/* Calculate the size between the write address and the end of the page */
	while (current_addr <= WriteAddr)
		current_addr += W25Q128FV_PAGE_SIZE;
	
	current_size = current_addr - WriteAddr;

	/* Check if the size of the data is less than the remaining position on the page */
	if (current_size > Size)
		current_size = Size;

	/* Initialize address variable */
	current_addr = WriteAddr;
	end_addr = WriteAddr + Size;

	/* Initial program command */
	s_command.DummyCycles       = 0;
	s_command.DataMode          = QSPI_DATA_1_LINE;
	s_command.AddressMode       = QSPI_ADDRESS_1_LINE;
	s_command.AddressSize       = QSPI_ADDRESS_24_BITS;
	s_command.DdrMode           = QSPI_DDR_MODE_DISABLE;
	s_command.InstructionMode   = QSPI_INSTRUCTION_1_LINE;
	s_command.SIOOMode          = QSPI_SIOO_INST_EVERY_CMD;
	s_command.Instruction       = PAGE_PROG_CMD;
	s_command.AlternateByteMode = QSPI_ALTERNATE_BYTES_NONE;
	s_command.DdrHoldHalfCycle  = QSPI_DDR_HHC_ANALOG_DELAY;

	/* Write on page by page */
	do
	{
		s_command.Address = current_addr;
		s_command.NbData  = current_size;

		/* Enable write operation */
		if (QSPI_WriteEnable() != QSPI_OK)
			return QSPI_ERROR;

		/* Configuration command */
		if (HAL_QSPI_Command(&QSPIHandle, &s_command, HAL_QPSI_TIMEOUT_DEFAULT_VALUE) != HAL_OK)
			return QSPI_ERROR;

		/* transfer data */
		if (HAL_QSPI_Transmit(&QSPIHandle, pData, HAL_QPSI_TIMEOUT_DEFAULT_VALUE) != HAL_OK)
			return QSPI_ERROR;

		/* Configure automatic polling mode to wait for the program to end */  
		if (QSPI_AutoPollingMemReady(HAL_QPSI_TIMEOUT_DEFAULT_VALUE) != QSPI_OK)
			return QSPI_ERROR;
		
		/* Update the address and size variables of the next page of programming */
		pData         += current_size;
		current_addr  += current_size;
		current_size   = ((current_addr + W25Q128FV_PAGE_SIZE) > end_addr) ? (end_addr - current_addr) : W25Q128FV_PAGE_SIZE;
	} while (current_addr < end_addr);
	
	return QSPI_OK;
}
//

/**
  * @brief  Read large amounts of data from QSPI memory.
  * @param  pData: Pointer to the data to be read
  * @param  ReadAddr: Read start address
  * @param  Size: The size of the data to read    
  * @retval QSPI memory status
  */
uint8_t BSP_QSPI_Read(uint8_t* pData, uint32_t ReadAddr, uint32_t Size, eReadMode readmode)
{
	QSPI_CommandTypeDef s_command = {0};
	
	/* Initialize read command */
	s_command.NbData                  = Size;
	s_command.Address                 = ReadAddr;
	s_command.AddressSize             = QSPI_ADDRESS_24_BITS;
	s_command.DdrMode                 = QSPI_DDR_MODE_DISABLE;
	s_command.SIOOMode                = QSPI_SIOO_INST_EVERY_CMD;
	s_command.DdrHoldHalfCycle        = QSPI_DDR_HHC_ANALOG_DELAY;
	s_command.AlternateByteMode       = QSPI_ALTERNATE_BYTES_NONE;
	
	switch(readmode)
	{
		case eReadMode_Read:
		{
			s_command.DummyCycles         = 0;
			s_command.Instruction			    = READ_CMD;
			s_command.DataMode				    = QSPI_DATA_1_LINE;
			s_command.AddressMode			    = QSPI_ADDRESS_1_LINE;
			s_command.InstructionMode     = QSPI_INSTRUCTION_1_LINE;
			break;
		}
		case eReadMode_FastRead:
		{
			s_command.DummyCycles         = 8;
			s_command.Instruction			    = FAST_READ_CMD;
			
			if(QPIMode_Entered == true)
			{
				s_command.DataMode			    = QSPI_DATA_4_LINES;
				s_command.AddressMode		    = QSPI_ADDRESS_4_LINES;
				s_command.InstructionMode   = QSPI_INSTRUCTION_4_LINES;
			}
			else
			{
				s_command.DataMode			    = QSPI_DATA_1_LINE;
				s_command.AddressMode 		  = QSPI_ADDRESS_1_LINE;
				s_command.InstructionMode   = QSPI_INSTRUCTION_1_LINE;
			}
			break;
		}
		case eReadMode_FastReadDualO:
		{
			s_command.DummyCycles    		  = 8;
			s_command.DataMode				    = QSPI_DATA_2_LINES;
			s_command.AddressMode 			  = QSPI_ADDRESS_1_LINE;
			s_command.Instruction			    = DUAL_OUT_FAST_READ_CMD;
			s_command.InstructionMode     = QSPI_INSTRUCTION_1_LINE;
			break;
		}
		case eReadMode_FastReadQuadO:
		{
			if(BSP_QSPI_Enable_QPIMode() != QSPI_OK)
				return QSPI_ERROR;
			
			s_command.DummyCycles    		  = 8;
			s_command.DataMode				    = QSPI_DATA_4_LINES;
			s_command.AddressMode 			  = QSPI_ADDRESS_1_LINE;
			s_command.Instruction			    = QUAD_OUT_FAST_READ_CMD;
			s_command.InstructionMode     = QSPI_INSTRUCTION_1_LINE;
			break;
		}
		case eReadMode_FastReadDualIO:
		{
			s_command.DummyCycles			    = 0x08;
			s_command.AlternateBytes		  = 0x00;
			s_command.DataMode				    = QSPI_DATA_2_LINES;
			s_command.AddressMode			    = QSPI_ADDRESS_2_LINES;
			s_command.InstructionMode     = QSPI_INSTRUCTION_1_LINE;
			s_command.Instruction			    = DUAL_INOUT_FAST_READ_CMD;
			s_command.AlternateBytesSize  = QSPI_ALTERNATE_BYTES_8_BITS;
			s_command.AlternateByteMode		= QSPI_ALTERNATE_BYTES_2_LINES;
			break;
		}
		case eReadMode_FastReadQuadIO:
		{
			s_command.DummyCycles			    = 8;
			s_command.AlternateBytes		  = 0x00;
			s_command.DataMode				    = QSPI_DATA_4_LINES;
			s_command.AddressMode			    = QSPI_ADDRESS_4_LINES;
			s_command.InstructionMode		  = QSPI_INSTRUCTION_1_LINE;
			s_command.Instruction			    = QUAD_INOUT_FAST_READ_CMD;
			s_command.AlternateBytesSize 	= QSPI_ALTERNATE_BYTES_8_BITS;
			s_command.AlternateByteMode		= QSPI_ALTERNATE_BYTES_4_LINES;
			break;
		}
		default:
			return QSPI_ERROR;
	}
	
	if (HAL_QSPI_Command(&QSPIHandle, &s_command, HAL_QPSI_TIMEOUT_DEFAULT_VALUE) != HAL_OK)
		return QSPI_ERROR;

	/* Receive data */
	if (HAL_QSPI_Receive(&QSPIHandle, pData, HAL_QPSI_TIMEOUT_DEFAULT_VALUE) != HAL_OK)
		return QSPI_ERROR;
	
	return QSPI_OK;
}
//

/************************ (C) COPYRIGHT YiLDiZ IoT ************************/
