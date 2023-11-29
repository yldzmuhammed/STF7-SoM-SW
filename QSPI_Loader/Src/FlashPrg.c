
/* Includes ------------------------------------------------------------------*/
/* Private includes ----------------------------------------------------------*/
#include <string.h>
#include "FlashOS.H"        // FlashOS Structures
#include "stm32f7xx_hal.h"

#include "stf7som_nor.h"
#include "stf7som_qspi.h"


/* Private typedef -----------------------------------------------------------*/

/** 
  * @brief  GPIO Configuration Mode enumeration 
  */   
typedef enum
{ 
  GPIO_Mode_IN   = 0x00, /*!< GPIO Input Mode */
  GPIO_Mode_OUT  = 0x01, /*!< GPIO Output Mode */
  GPIO_Mode_AF   = 0x02, /*!< GPIO Alternate function Mode */
  GPIO_Mode_AN   = 0x03  /*!< GPIO Analog Mode */
}GPIOMode_TypeDef;
//

/** 
  * @brief  GPIO Output type enumeration 
  */  
typedef enum
{ 
  GPIO_OType_PP = 0x00,
  GPIO_OType_OD = 0x01
}GPIOOType_TypeDef;
//

/** 
  * @brief  GPIO Configuration PullUp PullDown enumeration 
  */ 
typedef enum
{ 
  GPIO_PuPd_NOPULL = 0x00,
  GPIO_PuPd_UP     = 0x01,
  GPIO_PuPd_DOWN   = 0x02
}GPIOPuPd_TypeDef;
//

/** 
  * @brief  GPIO Output Maximum frequency enumeration 
  */  
typedef enum
{ 
  GPIO_Speed_2MHz   = 0x00, /*!< Low speed */
  GPIO_Speed_25MHz  = 0x01, /*!< Medium speed */
  GPIO_Speed_50MHz  = 0x02, /*!< Fast speed */
  GPIO_Speed_100MHz = 0x03  /*!< High speed on 30 pF (80 MHz Output max speed on 15 pF) */
}GPIOSpeed_TypeDef;
//

/** 
  * @brief  QUADSPI Init structure definition  
  */
typedef struct
{
  uint32_t QUADSPI_SShift;    /* Specifies the Sample Shift
                              This parameter can be a value of @ref QUADSPI_Sample_Shift*/

  uint32_t QUADSPI_Prescaler; /* Specifies the prescaler value used to divide the QUADSPI clock.
                              This parameter can be a number between 0x00 and 0xFF */ 

  uint32_t QUADSPI_CKMode;    /* Specifies the Clock Mode
                              This parameter can be a value of @ref QUADSPI_Clock_Mode*/

  uint32_t QUADSPI_CSHTime;   /* Specifies the Chip Select High Time
                              This parameter can be a value of @ref QUADSPI_ChipSelectHighTime*/   
 
  uint32_t QUADSPI_FSize;     /* Specifies the Flash Size.
                               QUADSPI_FSize+1 is effectively the number of address bits required to address the flash memory.
                               The flash capacity can be up to 4GB (addressed using 32 bits) in indirect mode, but the
                               addressable space in memory-mapped mode is limited to 512MB
                               This parameter can be a number between 0x00 and 0x1F */
  uint32_t QUADSPI_FSelect;   /* Specifies the Flash which will be used,
                                 This parameter can be a value of @ref QUADSPI_Fash_Select*/
  uint32_t QUADSPI_DFlash;   /* Specifies the Dual Flash Mode State
                                 This parameter can be a value of @ref QUADSPI_Dual_Flash*/
}QUADSPI_InitTypeDef;
//

/** 
  * @brief  QUADSPI Communication Configuration Init structure definition  
  */
typedef struct
{
  
  uint32_t QUADSPI_ComConfig_FMode;            /* Specifies the Functional Mode
                                          This parameter can be a value of @ref QUADSPI_ComConfig_Functional_Mode*/  
	
  uint32_t QUADSPI_ComConfig_DDRMode;          /* Specifies the Double Data Rate Mode
                                          This parameter can be a value of @ref QUADSPI_ComConfig_DoubleDataRateMode*/
	
  uint32_t QUADSPI_ComConfig_DHHC;            /* Specifies the Delay Half Hclk Cycle
                                          This parameter can be a value of @ref QUADSPI_ComConfig_DelayHalfHclkCycle*/
  
  uint32_t QUADSPI_ComConfig_SIOOMode;         /* Specifies the Send Instruction Only Once Mode
                                          This parameter can be a value of @ref QUADSPI_ComConfig_SendInstructionOnlyOnceMode*/
  
  uint32_t QUADSPI_ComConfig_DMode;            /* Specifies the Data Mode
                                          This parameter can be a value of @ref QUADSPI_ComConfig_DataMode*/
  
  uint32_t QUADSPI_ComConfig_DummyCycles;      /* Specifies the Number of Dummy Cycles.
                                           This parameter can be a number between 0x00 and 0x1F */             

  uint32_t QUADSPI_ComConfig_ABSize;           /* Specifies the Alternate Bytes Size
                                          This parameter can be a value of @ref QUADSPI_ComConfig_AlternateBytesSize*/ 

  uint32_t QUADSPI_ComConfig_ABMode;           /* Specifies the Alternate Bytes Mode
                                          This parameter can be a value of @ref QUADSPI_ComConfig_AlternateBytesMode*/     
 
  uint32_t QUADSPI_ComConfig_ADSize;           /* Specifies the Address Size
                                          This parameter can be a value of @ref QUADSPI_ComConfig_AddressSize*/

  uint32_t QUADSPI_ComConfig_ADMode;           /* Specifies the Address Mode
                                          This parameter can be a value of @ref QUADSPI_ComConfig_AddressMode*/
  
  uint32_t QUADSPI_ComConfig_IMode;            /* Specifies the Instruction Mode
                                          This parameter can be a value of @ref QUADSPI_ComConfig_InstructionMode*/

  uint32_t QUADSPI_ComConfig_Ins;      /* Specifies the Instruction Mode
                                          This parameter can be a value of @ref QUADSPI_ComConfig_Instruction*/
  
}QUADSPI_ComConfig_InitTypeDef;
//

/** 
  * @brief   GPIO Init structure definition  
  */ 
typedef struct
{
  uint32_t GPIO_Pin;              /*!< Specifies the GPIO pins to be configured.
                                       This parameter can be any value of @ref GPIO_pins_define */

  GPIOMode_TypeDef GPIO_Mode;     /*!< Specifies the operating mode for the selected pins.
                                       This parameter can be a value of @ref GPIOMode_TypeDef */

  GPIOSpeed_TypeDef GPIO_Speed;   /*!< Specifies the speed for the selected pins.
                                       This parameter can be a value of @ref GPIOSpeed_TypeDef */

  GPIOOType_TypeDef GPIO_OType;   /*!< Specifies the operating output type for the selected pins.
                                       This parameter can be a value of @ref GPIOOType_TypeDef */

  GPIOPuPd_TypeDef GPIO_PuPd;     /*!< Specifies the operating Pull-up/Pull down for the selected pins.
                                       This parameter can be a value of @ref GPIOPuPd_TypeDef */
}GPIO_InitStructTypeDef;
//

/* Private define ------------------------------------------------------------*/

#define QUADSPI_FLAG_TO                 						QUADSPI_SR_TOF
#define QUADSPI_FLAG_SM                 						QUADSPI_SR_SMF
#define QUADSPI_FLAG_FT                 						QUADSPI_SR_FTF
#define QUADSPI_FLAG_TC                 						QUADSPI_SR_TCF
#define QUADSPI_FLAG_TE                 						QUADSPI_SR_TEF
#define QUADSPI_FLAG_BUSY               						QUADSPI_SR_BUSY

#define  QUADSPI_CR_SSHIFT_0							((uint32_t)0x00000010)            /*!< Bit 0 */
//#define  QUADSPI_DCR_CSHT_0               ((uint32_t)0x00000100)            /*!< Bit 0 */
//#define  QUADSPI_DCR_CSHT_1               ((uint32_t)0x00000200)            /*!< Bit 1 */
//#define  QUADSPI_DCR_CSHT_2               ((uint32_t)0x00000400)            /*!< Bit 2 */
//#define  QUADSPI_CCR_ADSIZE_1							((uint32_t)0x00002000)            /*!< Bit 1 */

#define QUADSPI_CCR_CLEAR_MASK											((uint32_t)0x90800000)
#define QUADSPI_CR_CLEAR_FIFOTHRESHOLD_MASK					((uint32_t)0xFFFFF0FF)

#define QUADSPI_PMM_AND                 						((uint32_t)0x00000000)
#define QUADSPI_PMM_OR                  						((uint32_t)QUADSPI_CR_PMM)

#define QUADSPI_ComConfig_DDRMode_Disable           ((uint32_t)0x00000000)
#define QUADSPI_ComConfig_DDRMode_Enable            ((uint32_t)QUADSPI_CCR_DDRM)

#define QUADSPI_ComConfig_DHHC_Disable           		((uint32_t)0x00000000)
#define QUADSPI_ComConfig_DHHC_Enable            		((uint32_t)QUADSPI_CCR_DHHC)

#define QUADSPI_ComConfig_ADSize_8bit            		((uint32_t)0x00000000)
#define QUADSPI_ComConfig_ADSize_16bit           		((uint32_t)QUADSPI_CCR_ADSIZE_0)
#define QUADSPI_ComConfig_ADSize_24bit           		((uint32_t)QUADSPI_CCR_ADSIZE_1)
#define QUADSPI_ComConfig_ADSize_32bit           		((uint32_t)QUADSPI_CCR_ADSIZE)

#define QUADSPI_ComConfig_IMode_NoInstruction       ((uint32_t)0x00000000)
#define QUADSPI_ComConfig_IMode_1Line               ((uint32_t)QUADSPI_CCR_IMODE_0)
#define QUADSPI_ComConfig_IMode_2Line               ((uint32_t)QUADSPI_CCR_IMODE_1)
#define QUADSPI_ComConfig_IMode_4Line               ((uint32_t)QUADSPI_CCR_IMODE)

#define QUADSPI_ComConfig_ABMode_NoAlternateByte    ((uint32_t)0x00000000)
#define QUADSPI_ComConfig_ABMode_1Line              ((uint32_t)QUADSPI_CCR_ABMODE_0)
#define QUADSPI_ComConfig_ABMode_2Line              ((uint32_t)QUADSPI_CCR_ABMODE_1)
#define QUADSPI_ComConfig_ABMode_4Line              ((uint32_t)QUADSPI_CCR_ABMODE)

#define QUADSPI_SShift_NoShift											((uint32_t)0x00000000)
#define QUADSPI_CKMode_Mode0												((uint32_t)0x00000000)
#define QUADSPI_DFlash_Disable           						((uint32_t)0x00000000)
#define QUADSPI_FSelect_1														((uint32_t)0x00000000)
#define QUADSPI_CR_CLEAR_MASK												((uint32_t)0x00FFFFCF)
#define QUADSPI_DCR_CLEAR_MASK            					((uint32_t)0xFFE0F7FE)
#define QUADSPI_ComConfig_ABSize_8bit								((uint32_t)0x00000000)
#define QUADSPI_ComConfig_SIOOMode_Disable					((uint32_t)0x00000000)
#define QUADSPI_ComConfig_FMode_Indirect_Write			((uint32_t)0x00000000)

#define QUADSPI_ComConfig_DMode_NoData          		((uint32_t)0x00000000)
#define QUADSPI_ComConfig_DMode_1Line           		((uint32_t)QUADSPI_CCR_DMODE_0)
#define QUADSPI_ComConfig_DMode_2Line           		((uint32_t)QUADSPI_CCR_DMODE_1)
#define QUADSPI_ComConfig_DMode_4Line           		((uint32_t)QUADSPI_CCR_DMODE)

#define QUADSPI_ComConfig_ADMode_NoAddress          ((uint32_t)0x00000000)
#define QUADSPI_ComConfig_ADMode_1Line              ((uint32_t)QUADSPI_CCR_ADMODE_0)
#define QUADSPI_ComConfig_ADMode_2Line              ((uint32_t)QUADSPI_CCR_ADMODE_1)
#define QUADSPI_ComConfig_ADMode_4Line              ((uint32_t)QUADSPI_CCR_ADMODE)

#define QUADSPI_ComConfig_FMode_Indirect_Read      	((uint32_t)QUADSPI_CCR_FMODE_0)
#define QUADSPI_ComConfig_FMode_Auto_Polling       	((uint32_t)QUADSPI_CCR_FMODE_1)
#define QUADSPI_ComConfig_FMode_Memory_Mapped      	((uint32_t)QUADSPI_CCR_FMODE)

#define QUADSPI_SShift_HalfCycleShift								((uint32_t)QUADSPI_CR_SSHIFT_0)
#define QUADSPI_ComConfig_ADSize_24bit           		((uint32_t)QUADSPI_CCR_ADSIZE_1)
#define QUADSPI_ComConfig_IMode_1Line								((uint32_t)QUADSPI_CCR_IMODE_0)
#define QUADSPI_CSHTime_6Cycle           						((uint32_t)QUADSPI_DCR_CSHT_2 | QUADSPI_DCR_CSHT_0)

#define RCC_SYSCLK_Div1                  	((uint32_t)0x00000000)
#define RCC_AHB3Periph_QSPI              	((uint32_t)0x00000002)

#define RCC_AHB1Periph_GPIOA             	((uint32_t)0x00000001)
#define RCC_AHB1Periph_GPIOB             	((uint32_t)0x00000002)
#define RCC_AHB1Periph_GPIOC             	((uint32_t)0x00000004)
#define RCC_AHB1Periph_GPIOD             	((uint32_t)0x00000008)
#define RCC_AHB1Periph_GPIOE             	((uint32_t)0x00000010)
#define RCC_AHB1Periph_GPIOF             	((uint32_t)0x00000020)
#define RCC_AHB1Periph_GPIOG             	((uint32_t)0x00000040)
#define RCC_AHB1Periph_GPIOH             	((uint32_t)0x00000080)
#define RCC_AHB1Periph_GPIOI             	((uint32_t)0x00000100)
#define RCC_AHB1Periph_GPIOJ             	((uint32_t)0x00000200)
#define RCC_AHB1Periph_GPIOK             	((uint32_t)0x00000400)

#define GPIO_Pin_0                 				((uint16_t)0x0001)  /* Pin 0 selected */
#define GPIO_Pin_1                 				((uint16_t)0x0002)  /* Pin 1 selected */
#define GPIO_Pin_2                 				((uint16_t)0x0004)  /* Pin 2 selected */
#define GPIO_Pin_3                 				((uint16_t)0x0008)  /* Pin 3 selected */
#define GPIO_Pin_4                 				((uint16_t)0x0010)  /* Pin 4 selected */
#define GPIO_Pin_5                 				((uint16_t)0x0020)  /* Pin 5 selected */
#define GPIO_Pin_6                 				((uint16_t)0x0040)  /* Pin 6 selected */
#define GPIO_Pin_7                 				((uint16_t)0x0080)  /* Pin 7 selected */
#define GPIO_Pin_8                 				((uint16_t)0x0100)  /* Pin 8 selected */
#define GPIO_Pin_9                 				((uint16_t)0x0200)  /* Pin 9 selected */
#define GPIO_Pin_10                				((uint16_t)0x0400)  /* Pin 10 selected */
#define GPIO_Pin_11                				((uint16_t)0x0800)  /* Pin 11 selected */
#define GPIO_Pin_12                				((uint16_t)0x1000)  /* Pin 12 selected */
#define GPIO_Pin_13                				((uint16_t)0x2000)  /* Pin 13 selected */
#define GPIO_Pin_14                				((uint16_t)0x4000)  /* Pin 14 selected */
#define GPIO_Pin_15                				((uint16_t)0x8000)  /* Pin 15 selected */

#define GPIO_PinSource0            				((uint8_t)0x00)
#define GPIO_PinSource1            				((uint8_t)0x01)
#define GPIO_PinSource2            				((uint8_t)0x02)
#define GPIO_PinSource3            				((uint8_t)0x03)
#define GPIO_PinSource4            				((uint8_t)0x04)
#define GPIO_PinSource5            				((uint8_t)0x05)
#define GPIO_PinSource6            				((uint8_t)0x06)
#define GPIO_PinSource7            				((uint8_t)0x07)
#define GPIO_PinSource8            				((uint8_t)0x08)
#define GPIO_PinSource9            				((uint8_t)0x09)
#define GPIO_PinSource10           				((uint8_t)0x0A)
#define GPIO_PinSource11           				((uint8_t)0x0B)
#define GPIO_PinSource12           				((uint8_t)0x0C)
#define GPIO_PinSource13           				((uint8_t)0x0D)
#define GPIO_PinSource14           				((uint8_t)0x0E)
#define GPIO_PinSource15           				((uint8_t)0x0F)

#define RCC_APB2Periph_SYSCFG            	((uint32_t)0x00004000)

/* Private macro -------------------------------------------------------------*/
#define M8(adr)  		(*((volatile uint8_t  *) (adr)))
#define M16(adr) 		(*((volatile uint16_t *) (adr)))
#define M32(adr) 		(*((volatile uint32_t *) (adr)))

#define ADDR_SHIFT    	(1)  /* sjift address according data width ? */

#define SRAM_ADDRESS1	((uint32_t *)(0x2004befe))
#define SRAM_ADDRESS2	((uint32_t *)(0x2004beff))
/* Private variables ---------------------------------------------------------*/
uint8_t chipErased = 0;
uint8_t qspiIniited = 0;

uint8_t buffer[128];

unsigned long base_adr;

QUADSPI_InitTypeDef QUADSPI_InitStructure;
QUADSPI_ComConfig_InitTypeDef QUADSPI_ComConfig_InitStructure;
/* Private function prototypes -----------------------------------------------*/
/* Private user code ---------------------------------------------------------*/
 
static void SystemClock_Config(void)
{
	RCC_OscInitTypeDef RCC_OscInitStruct = {0};
	RCC_ClkInitTypeDef RCC_ClkInitStruct = {0};
	RCC_PeriphCLKInitTypeDef PeriphClkInitStruct = {0};

	/** Configure LSE Drive Capability 
	*/
	HAL_PWR_EnableBkUpAccess();
	/** Configure the main internal regulator output voltage 
	*/
	__HAL_RCC_PWR_CLK_ENABLE();
	__HAL_PWR_VOLTAGESCALING_CONFIG(PWR_REGULATOR_VOLTAGE_SCALE1);
	/** Initializes the CPU, AHB and APB busses clocks 
	*/
	RCC_OscInitStruct.OscillatorType = RCC_OSCILLATORTYPE_HSE;
	RCC_OscInitStruct.HSEState = RCC_HSE_ON;
	RCC_OscInitStruct.PLL.PLLState = RCC_PLL_ON;
	RCC_OscInitStruct.PLL.PLLSource = RCC_PLLSOURCE_HSE;
	RCC_OscInitStruct.PLL.PLLM = 12;
	RCC_OscInitStruct.PLL.PLLN = 200;
	RCC_OscInitStruct.PLL.PLLP = RCC_PLLP_DIV2;
	RCC_OscInitStruct.PLL.PLLQ = 8;
	HAL_RCC_OscConfig(&RCC_OscInitStruct);
	
	/** Activate the Over-Drive mode */
	HAL_PWREx_EnableOverDrive();
	
	/** Initializes the CPU, AHB and APB busses clocks */
	RCC_ClkInitStruct.ClockType = RCC_CLOCKTYPE_HCLK | RCC_CLOCKTYPE_SYSCLK | RCC_CLOCKTYPE_PCLK1 | RCC_CLOCKTYPE_PCLK2;
	RCC_ClkInitStruct.SYSCLKSource = RCC_SYSCLKSOURCE_PLLCLK;
	RCC_ClkInitStruct.AHBCLKDivider = RCC_SYSCLK_DIV1;
	RCC_ClkInitStruct.APB1CLKDivider = RCC_HCLK_DIV4;
	RCC_ClkInitStruct.APB2CLKDivider = RCC_HCLK_DIV2;
	HAL_RCC_ClockConfig(&RCC_ClkInitStruct, FLASH_LATENCY_6);
	
	PeriphClkInitStruct.PeriphClockSelection = RCC_PERIPHCLK_PLLI2S;
	PeriphClkInitStruct.PLLI2S.PLLI2SN = 50;
	PeriphClkInitStruct.PLLI2S.PLLI2SP = RCC_PLLP_DIV2;
	PeriphClkInitStruct.PLLI2S.PLLI2SR = 2;
	PeriphClkInitStruct.PLLI2S.PLLI2SQ = 2;
	PeriphClkInitStruct.PLLI2SDivQ = 1;
	HAL_RCCEx_PeriphCLKConfig(&PeriphClkInitStruct);
}
//

/**
 * Override default HAL_InitTick function
 */
HAL_StatusTypeDef HAL_InitTick(uint32_t TickPeriode)
{
	return HAL_OK;
}
//

/**
 * Override default HAL_GetTick function
 */
uint32_t HAL_GetTick (void)
{
	static uint32_t ticks = 0U;
	uint32_t i;
	
	/** If Kernel is not running wait approximately 1 ms then increment
		* and return auxiliary tick counter value
		*/
	for (i = (SystemCoreClock >> 14U); i > 0U; i--)
	{
		__NOP(); __NOP(); __NOP(); __NOP(); __NOP(); __NOP();
		__NOP(); __NOP(); __NOP(); __NOP(); __NOP(); __NOP();
	}
	return ++ticks;
}
//

/**
 * Override default HAL_InitTick function
 */
void HAL_Delay(uint32_t Delay)
{
	uint32_t tickstart = HAL_GetTick();
	uint32_t wait = Delay;
	/* Add a period to guaranty minimum wait */
	if (wait < HAL_MAX_DELAY)
	{
		wait += (uint32_t)(1);
	}
	while((HAL_GetTick() - tickstart) < wait)
	{
		__NOP();
	}
}
//

/**
  * @brief Enable GPIO Clock
  * @param None
  * @retval None
  */
void BSP_GPIOCLK_Enable(void)
{
    /* GPIO Ports Clock Enable */
    __HAL_RCC_GPIOA_CLK_ENABLE();
    __HAL_RCC_GPIOB_CLK_ENABLE();
    __HAL_RCC_GPIOC_CLK_ENABLE();
    __HAL_RCC_GPIOD_CLK_ENABLE();
    __HAL_RCC_GPIOE_CLK_ENABLE();
    __HAL_RCC_GPIOF_CLK_ENABLE();
    __HAL_RCC_GPIOG_CLK_ENABLE();
    __HAL_RCC_GPIOH_CLK_ENABLE();
    __HAL_RCC_GPIOI_CLK_ENABLE();
    __HAL_RCC_GPIOJ_CLK_ENABLE();
    __HAL_RCC_GPIOK_CLK_ENABLE();
}
//

/**
  * @brief  Configures the AHB clock (HCLK).
  * @note   Depending on the device voltage range, the software has to set correctly
  *         these bits to ensure that HCLK not exceed the maximum allowed frequency
  *         (for more details refer to section above
  *           "CPU, AHB and APB busses clocks configuration functions")
  * @param  RCC_SYSCLK: defines the AHB clock divider. This clock is derived from 
  *         the system clock (SYSCLK).
  *          This parameter can be one of the following values:
  *            @arg RCC_SYSCLK_Div1: AHB clock = SYSCLK
  *            @arg RCC_SYSCLK_Div2: AHB clock = SYSCLK/2
  *            @arg RCC_SYSCLK_Div4: AHB clock = SYSCLK/4
  *            @arg RCC_SYSCLK_Div8: AHB clock = SYSCLK/8
  *            @arg RCC_SYSCLK_Div16: AHB clock = SYSCLK/16
  *            @arg RCC_SYSCLK_Div64: AHB clock = SYSCLK/64
  *            @arg RCC_SYSCLK_Div128: AHB clock = SYSCLK/128
  *            @arg RCC_SYSCLK_Div256: AHB clock = SYSCLK/256
  *            @arg RCC_SYSCLK_Div512: AHB clock = SYSCLK/512
  * @retval None
  */
void RCC_HCLKConfig(uint32_t RCC_SYSCLK)
{
  uint32_t tmpreg = 0;
  
  /* Check the parameters */
  assert_param(IS_RCC_HCLK(RCC_SYSCLK));

  tmpreg = RCC->CFGR;

  /* Clear HPRE[3:0] bits */
  tmpreg &= ~RCC_CFGR_HPRE;

  /* Set HPRE[3:0] bits according to RCC_SYSCLK value */
  tmpreg |= RCC_SYSCLK;

  /* Store the new value */
  RCC->CFGR = tmpreg;
}
//

/**
  * @brief  Enables or disables the AHB1 peripheral clock.
  * @note   After reset, the peripheral clock (used for registers read/write access)
  *         is disabled and the application software has to enable this clock before 
  *         using it.   
  * @param  RCC_AHBPeriph: specifies the AHB1 peripheral to gates its clock.
  *          This parameter can be any combination of the following values:
  *            @arg RCC_AHB1Periph_GPIOA:       GPIOA clock
  *            @arg RCC_AHB1Periph_GPIOB:       GPIOB clock 
  *            @arg RCC_AHB1Periph_GPIOC:       GPIOC clock
  *            @arg RCC_AHB1Periph_GPIOD:       GPIOD clock
  *            @arg RCC_AHB1Periph_GPIOE:       GPIOE clock
  *            @arg RCC_AHB1Periph_GPIOF:       GPIOF clock
  *            @arg RCC_AHB1Periph_GPIOG:       GPIOG clock
  *            @arg RCC_AHB1Periph_GPIOG:       GPIOG clock
  *            @arg RCC_AHB1Periph_GPIOI:       GPIOI clock
  *            @arg RCC_AHB1Periph_GPIOJ:       GPIOJ clock 
  *            @arg RCC_AHB1Periph_GPIOK:       GPIOK clock  
  *            @arg RCC_AHB1Periph_CRC:         CRC clock
  *            @arg RCC_AHB1Periph_BKPSRAM:     BKPSRAM interface clock
  *            @arg RCC_AHB1Periph_CCMDATARAMEN CCM data RAM interface clock
  *            @arg RCC_AHB1Periph_DMA1:        DMA1 clock
  *            @arg RCC_AHB1Periph_DMA2:        DMA2 clock
  *            @arg RCC_AHB1Periph_DMA2D:       DMA2D clock  
  *            @arg RCC_AHB1Periph_ETH_MAC:     Ethernet MAC clock
  *            @arg RCC_AHB1Periph_ETH_MAC_Tx:  Ethernet Transmission clock
  *            @arg RCC_AHB1Periph_ETH_MAC_Rx:  Ethernet Reception clock
  *            @arg RCC_AHB1Periph_ETH_MAC_PTP: Ethernet PTP clock
  *            @arg RCC_AHB1Periph_OTG_HS:      USB OTG HS clock
  *            @arg RCC_AHB1Periph_OTG_HS_ULPI: USB OTG HS ULPI clock
  * @param  NewState: new state of the specified peripheral clock.
  *          This parameter can be: ENABLE or DISABLE.
  * @retval None
  */
void RCC_AHB1PeriphClockCmd(uint32_t RCC_AHB1Periph, FunctionalState NewState)
{
  /* Check the parameters */
  assert_param(IS_RCC_AHB1_CLOCK_PERIPH(RCC_AHB1Periph));

  assert_param(IS_FUNCTIONAL_STATE(NewState));
  if (NewState != DISABLE)
  {
    RCC->AHB1ENR |= RCC_AHB1Periph;
  }
  else
  {
    RCC->AHB1ENR &= ~RCC_AHB1Periph;
  }
}
//

/**
  * @brief  Forces or releases AHB3 peripheral reset.
  * @param  RCC_AHB3Periph: specifies the AHB3 peripheral to reset.
  *          This parameter must be: RCC_AHB3Periph_FMC or RCC_AHB3Periph_QSPI
  * @param  NewState: new state of the specified peripheral reset.
  *          This parameter can be: ENABLE or DISABLE.
  * @retval None
  */
void RCC_AHB3PeriphResetCmd(uint32_t RCC_AHB3Periph, FunctionalState NewState)
{
  /* Check the parameters */
  assert_param(IS_RCC_AHB3_PERIPH(RCC_AHB3Periph));
  assert_param(IS_FUNCTIONAL_STATE(NewState));

  if (NewState != DISABLE)
  {
    RCC->AHB3RSTR |= RCC_AHB3Periph;
  }
  else
  {
    RCC->AHB3RSTR &= ~RCC_AHB3Periph;
  }
}
//

/**
  * @brief  Enables or disables the High Speed APB (APB2) peripheral clock.
  * @note   After reset, the peripheral clock (used for registers read/write access)
  *         is disabled and the application software has to enable this clock before 
  *         using it.
  * @param  RCC_APB2Periph: specifies the APB2 peripheral to gates its clock.
  *          This parameter can be any combination of the following values:
  *            @arg RCC_APB2Periph_TIM1:   TIM1 clock
  *            @arg RCC_APB2Periph_TIM8:   TIM8 clock
  *            @arg RCC_APB2Periph_USART1: USART1 clock
  *            @arg RCC_APB2Periph_USART6: USART6 clock
  *            @arg RCC_APB2Periph_ADC1:   ADC1 clock
  *            @arg RCC_APB2Periph_ADC2:   ADC2 clock
  *            @arg RCC_APB2Periph_ADC3:   ADC3 clock
  *            @arg RCC_APB2Periph_SDIO:   SDIO clock
  *            @arg RCC_APB2Periph_SPI1:   SPI1 clock
  *            @arg RCC_APB2Periph_SPI4:   SPI4 clock
  *            @arg RCC_APB2Periph_SYSCFG: SYSCFG clock
  *            @arg RCC_APB2Periph_TIM9:   TIM9 clock
  *            @arg RCC_APB2Periph_TIM10:  TIM10 clock
  *            @arg RCC_APB2Periph_TIM11:  TIM11 clock
  *            @arg RCC_APB2Periph_SPI5:   SPI5 clock
  *            @arg RCC_APB2Periph_SPI6:   SPI6 clock
  *            @arg RCC_APB2Periph_SAI1:   SAI1 clock 
  *            @arg RCC_APB2Periph_SAI2:   SAI2 clock 
  *            @arg RCC_APB2Periph_LTDC:   LTDC clock 
  * @param  NewState: new state of the specified peripheral clock.
  *          This parameter can be: ENABLE or DISABLE.
  * @retval None
  */
void RCC_APB2PeriphClockCmd(uint32_t RCC_APB2Periph, FunctionalState NewState)
{
  /* Check the parameters */
  assert_param(IS_RCC_APB2_PERIPH(RCC_APB2Periph));
  assert_param(IS_FUNCTIONAL_STATE(NewState));

  if (NewState != DISABLE)
  {
    RCC->APB2ENR |= RCC_APB2Periph;
  }
  else
  {
    RCC->APB2ENR &= ~RCC_APB2Periph;
  }
}
//

/**
  * @brief  Changes the mapping of the specified pin.
  * @param  GPIOx: where x can be (A..K) to select the GPIO peripheral for STM32F529X device or
  *                      x can be (A..I) to select the GPIO peripheral for STM32F50XX and STM32F527X devices. 
  * @param  GPIO_PinSource: specifies the pin for the Alternate function.
  *         This parameter can be GPIO_PinSourcex where x can be (0..15).
  * @param  GPIO_AFSelection: selects the pin to used as Alternate function.
  *          This parameter can be one of the following values:
  *            @arg GPIO_AF_RTC_50Hz: Connect RTC_50Hz pin to AF0 (default after reset) 
  *            @arg GPIO_AF_MCO: Connect MCO pin (MCO1 and MCO2) to AF0 (default after reset) 
  *            @arg GPIO_AF_TAMPER: Connect TAMPER pins (TAMPER_1 and TAMPER_2) to AF0 (default after reset) 
  *            @arg GPIO_AF_SWJ: Connect SWJ pins (SWD and JTAG)to AF0 (default after reset) 
  *            @arg GPIO_AF_TRACE: Connect TRACE pins to AF0 (default after reset)
  *            @arg GPIO_AF_TIM1: Connect TIM1 pins to AF1
  *            @arg GPIO_AF_TIM2: Connect TIM2 pins to AF1
  *            @arg GPIO_AF_TIM3: Connect TIM3 pins to AF2
  *            @arg GPIO_AF_TIM4: Connect TIM4 pins to AF2
  *            @arg GPIO_AF_TIM5: Connect TIM5 pins to AF2
  *            @arg GPIO_AF_TIM8: Connect TIM8 pins to AF3
  *            @arg GPIO_AF_TIM9: Connect TIM9 pins to AF3
  *            @arg GPIO_AF_TIM10: Connect TIM10 pins to AF3
  *            @arg GPIO_AF_TIM11: Connect TIM11 pins to AF3
  *            @arg GPIO_AF_I2C1: Connect I2C1 pins to AF4
  *            @arg GPIO_AF_I2C2: Connect I2C2 pins to AF4
  *            @arg GPIO_AF_I2C3: Connect I2C3 pins to AF4
  *            @arg GPIO_AF_SPI1: Connect SPI1 pins to AF5
  *            @arg GPIO_AF_SPI2: Connect SPI2/I2S2 pins to AF5
  *            @arg GPIO_AF_SPI4: Connect SPI4 pins to AF5 
  *            @arg GPIO_AF_SPI5: Connect SPI5 pins to AF5 
  *            @arg GPIO_AF_SPI6: Connect SPI6 pins to AF5
  *            @arg GPIO_AF_SAI1: Connect SAI1 pins to AF6 for STM32F529X device        
  *            @arg GPIO_AF_SPI3: Connect SPI3/I2S3 pins to AF6
  *            @arg GPIO_AF_I2S3ext: Connect I2S3ext pins to AF7
  *            @arg GPIO_AF_USART1: Connect USART1 pins to AF7
  *            @arg GPIO_AF_USART2: Connect USART2 pins to AF7
  *            @arg GPIO_AF_USART3: Connect USART3 pins to AF7
  *            @arg GPIO_AF_UART4: Connect UART4 pins to AF8
  *            @arg GPIO_AF_UART5: Connect UART5 pins to AF8
  *            @arg GPIO_AF_USART6: Connect USART6 pins to AF8
  *            @arg GPIO_AF_UART7: Connect UART7 pins to AF8
  *            @arg GPIO_AF_UART8: Connect UART8 pins to AF8
  *            @arg GPIO_AF_CAN1: Connect CAN1 pins to AF9
  *            @arg GPIO_AF_CAN2: Connect CAN2 pins to AF9
  *            @arg GPIO_AF_TIM12: Connect TIM12 pins to AF9
  *            @arg GPIO_AF_TIM13: Connect TIM13 pins to AF9
  *            @arg GPIO_AF_TIM14: Connect TIM14 pins to AF9
  *            @arg GPIO_AF_OTG_FS: Connect OTG_FS pins to AF10
  *            @arg GPIO_AF_OTG_HS: Connect OTG_HS pins to AF10
  *            @arg GPIO_AF_ETH: Connect ETHERNET pins to AF11
  *            @arg GPIO_AF_FSMC: Connect FSMC pins to AF12 
  *            @arg GPIO_AF_FMC: Connect FMC pins to AF12 for STM32F529X device   
  *            @arg GPIO_AF_OTG_HS_FS: Connect OTG HS (configured in FS) pins to AF12
  *            @arg GPIO_AF_SDIO: Connect SDIO pins to AF12
  *            @arg GPIO_AF_DCMI: Connect DCMI pins to AF13
  *            @arg GPIO_AF_LTDC: Connect LTDC pins to AF14 for STM32F529X device  
  *            @arg GPIO_AF_EVENTOUT: Connect EVENTOUT pins to AF15
  * @retval None
  */
void GPIO_PinAFConfig(GPIO_TypeDef* GPIOx, uint16_t GPIO_PinSource, uint8_t GPIO_AF)
{
  uint32_t temp = 0x00;
  uint32_t temp_2 = 0x00;
  
  /* Check the parameters */
  assert_param(IS_GPIO_ALL_PERIPH(GPIOx));
  assert_param(IS_GPIO_PIN_SOURCE(GPIO_PinSource));
  assert_param(IS_GPIO_AF(GPIO_AF));
  
  temp = ((uint32_t)(GPIO_AF) << ((uint32_t)((uint32_t)GPIO_PinSource & (uint32_t)0x07) * 4)) ;
  GPIOx->AFR[GPIO_PinSource >> 0x03] &= ~((uint32_t)0xF << ((uint32_t)((uint32_t)GPIO_PinSource & (uint32_t)0x07) * 4)) ;
  temp_2 = GPIOx->AFR[GPIO_PinSource >> 0x03] | temp;
  GPIOx->AFR[GPIO_PinSource >> 0x03] = temp_2;
}
//

/**
  * @brief  Initializes the GPIOx peripheral according to the specified parameters in the GPIO_InitStruct.
  * @param  GPIOx: where x can be (A..K) to select the GPIO peripheral for STM32F529X device or
  *                      x can be (A..I) to select the GPIO peripheral for STM32F50XX and STM32F527X devices. 
  * @param  GPIO_InitStruct: pointer to a GPIO_InitTypeDef structure that contains
  *         the configuration information for the specified GPIO peripheral.
  * @retval None
  */
void GPIO_Init(GPIO_TypeDef* GPIOx, GPIO_InitStructTypeDef* GPIO_InitStruct)
{
  uint32_t pinpos = 0x00, pos = 0x00 , currentpin = 0x00;

  /* Check the parameters */
  assert_param(IS_GPIO_ALL_PERIPH(GPIOx));
  assert_param(IS_GPIO_PIN(GPIO_InitStruct->GPIO_Pin));
  assert_param(IS_GPIO_MODE(GPIO_InitStruct->GPIO_Mode));
  assert_param(IS_GPIO_PUPD(GPIO_InitStruct->GPIO_PuPd));

  /* ------------------------- Configure the port pins ---------------- */
  /*-- GPIO Mode Configuration --*/
  for (pinpos = 0x00; pinpos < 0x10; pinpos++)
  {
    pos = ((uint32_t)0x01) << pinpos;
    /* Get the port pins position */
    currentpin = (GPIO_InitStruct->GPIO_Pin) & pos;

    if (currentpin == pos)
    {
      GPIOx->MODER  &= ~(GPIO_MODER_MODER0 << (pinpos * 2));
      GPIOx->MODER |= (((uint32_t)GPIO_InitStruct->GPIO_Mode) << (pinpos * 2));

      if ((GPIO_InitStruct->GPIO_Mode == GPIO_Mode_OUT) || (GPIO_InitStruct->GPIO_Mode == GPIO_Mode_AF))
      {
        /* Check Speed mode parameters */
        assert_param(IS_GPIO_SPEED(GPIO_InitStruct->GPIO_Speed));

        /* Speed mode configuration */
        GPIOx->OSPEEDR &= ~(GPIO_OSPEEDER_OSPEEDR0 << (pinpos * 2));
        GPIOx->OSPEEDR |= ((uint32_t)(GPIO_InitStruct->GPIO_Speed) << (pinpos * 2));

        /* Check Output mode parameters */
        assert_param(IS_GPIO_OTYPE(GPIO_InitStruct->GPIO_OType));

        /* Output mode configuration*/
        GPIOx->OTYPER  &= ~((GPIO_OTYPER_OT_0) << ((uint16_t)pinpos)) ;
        GPIOx->OTYPER |= (uint16_t)(((uint16_t)GPIO_InitStruct->GPIO_OType) << ((uint16_t)pinpos));
      }

      /* Pull-up Pull down resistor configuration*/
      GPIOx->PUPDR &= ~(GPIO_PUPDR_PUPDR0 << ((uint16_t)pinpos * 2));
      GPIOx->PUPDR |= (((uint32_t)GPIO_InitStruct->GPIO_PuPd) << (pinpos * 2));
    }
  }
}
//

/**
  * @brief  Checks whether the specified QUADSPI flag is set or not.  
  * @param  QUADSPI_FLAG: specifies the QUADSPI flag to check. 
  *          This parameter can be one of the following values:
  *            @arg QUADSPI_FLAG_TO: Timeout interrupt flag
  *            @arg QUADSPI_SR_SMF: Status Match interrupt flag
  *            @arg QUADSPI_FLAG_FT: FIFO Threshold flag
  *            @arg QUADSPI_FLAG_TC: Transfer Complete flag
  *            @arg QUADSPI_FLAG_TE: Transfer Error flag
  *            @arg QUADSPI_FLAG_BUSY: Busy flag      
  * @retval The new state of QUADSPI_FLAG (SET or RESET).
  */
FlagStatus QUADSPI_GetFlagStatus(uint32_t QUADSPI_FLAG)
{
  FlagStatus bitstatus = RESET;
  /* Check the parameters */
  assert_param(IS_QUADSPI_GET_FLAG(QUADSPI_FLAG));

  /* Check the status of the specified QUADSPI flag */
  if (QUADSPI->SR & QUADSPI_FLAG)
  {
    /* QUADSPI_FLAG is set */
    bitstatus = SET;
  }
  else
  {
    /* QUADSPI_FLAG is reset */
    bitstatus = RESET;
  }
  /* Return the QUADSPI_FLAG status */
  return  bitstatus;
}
//

/**
  * @brief  Clears the QUADSPI flag.
  * @param  QUADSPI_FLAG: specifies the QUADSPI flag to clear. 
  *          This parameter can be one of the following values:
  *            @arg QUADSPI_FCLEAR_TO: Timeout interrupt flag
  *            @arg QUADSPI_FCLEAR_SM: Status Match interrupt flag
  *            @arg QUADSPI_FCLEAR_TC: Transfer Complete flag
  *            @arg QUADSPI_FCLEAR_TE: Transfer Error flag
  * @retval None
  */
void QUADSPI_ClearFlag(uint32_t QUADSPI_FLAG)
{
  /* Check the parameters */
  assert_param(IS_QUADSPI_CLEAR_FLAG(QUADSPI_FLAG));

  /* Clear the selected QUADSPI flags */
  QUADSPI->FCR = QUADSPI_FLAG;
}
//

/**
  * @brief  Deinitializes the QUADSPI peripheral registers to their default
  *         reset values.
  * @param  None
  * @retval None
  */
void QUADSPI_DeInit(void)
{
    /* Enable QUADSPI reset state */
    RCC_AHB3PeriphResetCmd(RCC_AHB3Periph_QSPI, ENABLE);
    /* Release QUADSPI from reset state */
    RCC_AHB3PeriphResetCmd(RCC_AHB3Periph_QSPI, DISABLE);
}
//

/**
  * @brief  Transmits a 8bit Data through the QUADSPI peripheral.
  * @param  Data: Data to be transmitted.
  * @retval None
  */
void QUADSPI_SendData8(uint8_t Data)
{
  uint32_t quadspibase = 0;

  quadspibase = (uint32_t)QUADSPI; 
  quadspibase += 0x20;
  
  *(__IO uint8_t *) quadspibase = Data;
}
//

/**
  * @brief  Enables or disables QUADSPI peripheral.
  * @param  NewState: new state of the QUADSPI peripheral. 
  *          This parameter can be: ENABLE or DISABLE.
  * @retval None
  */
void QUADSPI_Cmd(FunctionalState NewState)
{
  /* Check the parameters */
  assert_param(IS_FUNCTIONAL_STATE(NewState));

  if (NewState != DISABLE)
  {
    /* Enable QUADSPI peripheral */
    QUADSPI->CR |= QUADSPI_CR_EN;
  }
  else
  {
    /* Disable QUADSPI peripheral */
    QUADSPI->CR &= ~ QUADSPI_CR_EN;
  }
}
//

/**
  * @brief  Sets the value of the Address
  * @param  QUADSPI_Address: Address to be send to the external flash memory.  
  *         This parameter can be any value of between 0x00000000 and 0xFFFFFFFF
  * @note   This function is used only in Indirect Mode  
  * @retval None
  */
void QUADSPI_SetAddress(uint32_t QUADSPI_Address)
{
  if (!(QUADSPI->SR & QUADSPI_SR_BUSY))
  /* Device is not Busy */
  {
    /* Write the AR Register */
    QUADSPI->AR = QUADSPI_Address;
  }
}
//

/**
  * @brief  Sets the value of the Amternate Bytes
  * @param  QUADSPI_AlternateByte: Optional data to be send to the external QUADSPI device right after the address. 
  *         This parameter can be any value of between 0x00000000 and 0xFFFFFFFF
  * @note   This function is used only in Indirect Mode  
  * @retval None
  */
void QUADSPI_SetAlternateByte(uint32_t QUADSPI_AlternateByte)
{
  if (!(QUADSPI->SR & QUADSPI_SR_BUSY))
  /* Device is not Busy */
  {
    /* Write the ABR Register */
    QUADSPI->ABR = QUADSPI_AlternateByte;
  }
}
//

/**
  * @brief  Sets number of Bytes to be transferred 
  * @param  QUADSPI_DataLength: Number of data to be retrieved (value+1) 
  *         in indirect and status-polling modes. A value no greater than 3 
  *         (indicating 4 bytes) should be used for status-polling mode. 
  *         All 1s in indirect mode means undefined length, where QuadSPI will 
  *         continue until the end of memory, as defined by FSIZE
  *         This parameter can be any value of between 0x00000000 and 0xFFFFFFFF
  *             0x0000_0000: 1 byte is to be transferred
  *             0x0000_0001: 2 bytes are to be transferred
  *             0x0000_0002: 3 bytes are to be transferred
  *             0x0000_0003: 4 bytes are to be transferred
  *             ...
  *             0xFFFF_FFFD: 4,294,967,294 (4G-2) bytes are to be transferred
  *             0xFFFF_FFFE: 4,294,967,295 (4G-1) bytes are to be transferred
  *             0xFFFF_FFFF: undefined length -- all bytes until the end of flash memory (as defined
  *             by FSIZE) are to be transferred
  * @note   This function is not used in Memory Mapped Mode.
  * @retval None
  */
void QUADSPI_SetDataLength(uint32_t QUADSPI_DataLength)
{
  if (!(QUADSPI->SR & QUADSPI_SR_BUSY))
  /* Device is not Busy */
  {
    /* Write the DLR Register */
    QUADSPI->DLR = QUADSPI_DataLength;
  }
}
//

/**
  * @brief  Initializes the QUADSPI peripheral according to the specified 
  *         parameters in the QUADSPI_InitStruct.
  * @param  QUADSPI_InitStruct: pointer to a QUADSPI_InitTypeDef structure that
  *         contains the configuration information for the specified QUADSPI peripheral.
  * @retval None
  */
void QUADSPI_Init(QUADSPI_InitTypeDef* QUADSPI_InitStruct)
{
  uint32_t tmpreg = 0;
  
  /* Check the QUADSPI parameters */
  assert_param(IS_QUADSPI_SSHIFT(QUADSPI_InitStruct->QUADSPI_SShift));
  assert_param(IS_QUADSPI_PRESCALER(QUADSPI_InitStruct->QUADSPI_Prescaler));
  assert_param(IS_QUADSPI_CKMODE(QUADSPI_InitStruct->QUADSPI_CKMode));
  assert_param(IS_QUADSPI_CSHTIME(QUADSPI_InitStruct->QUADSPI_CSHTime));
  assert_param(IS_QUADSPI_FSIZE(QUADSPI_InitStruct->QUADSPI_FSize));
	assert_param(IS_QUADSPI_FSEL(QUADSPI_InitStruct->QUADSPI_FSelect));
	assert_param(IS_QUADSPI_DFM(QUADSPI_InitStruct->QUADSPI_DFlash));
  
  /*------------------------ QUADSPI CR Configuration ------------------------*/
  /* Get the QUADSPI CR1 value */
  tmpreg = QUADSPI->CR;
  /* Clear PRESCALER and SSHIFT bits */
  tmpreg &= QUADSPI_CR_CLEAR_MASK;
  /* Configure QUADSPI: Prescaler and Sample Shift */
  tmpreg |= (uint32_t)(((QUADSPI_InitStruct->QUADSPI_Prescaler)<<24)
                        |(QUADSPI_InitStruct->QUADSPI_SShift)
	                      |(QUADSPI_InitStruct->QUADSPI_FSelect)
	                      |(QUADSPI_InitStruct->QUADSPI_DFlash));  
  /* Write to QUADSPI CR */
  QUADSPI->CR = tmpreg;
  
  /*------------------------ QUADSPI DCR Configuration ------------------------*/
  /* Get the QUADSPI DCR value */
  tmpreg = QUADSPI->DCR;
  /* Clear FSIZE, CSHT and CKMODE bits */
  tmpreg &= QUADSPI_DCR_CLEAR_MASK;
  /* Configure QUADSPI: Flash Size, Chip Select High Time and Clock Mode */
  tmpreg |= (uint32_t)(((QUADSPI_InitStruct->QUADSPI_FSize)<<16)
                        |(QUADSPI_InitStruct->QUADSPI_CSHTime)
                        |(QUADSPI_InitStruct->QUADSPI_CKMode));  
  /* Write to QUADSPI DCR */
  QUADSPI->DCR = tmpreg;  
}
//

/**
  * @brief  Sets the FIFO Threshold
  * @param  QUADSPI_FIFOThres: Defines, in indirect mode, the threshold number 
  *           of bytes in the FIFO which will cause the FIFO Threshold Flag 
  *           FTF to be set.
  *         This parameter can be any value of between 0x00 and 0x0F
  * @retval None
  */
void QUADSPI_SetFIFOThreshold(uint32_t QUADSPI_FIFOThreshold)
{
  uint32_t tmpreg = 0;
  
  /* Check the parameters */
  assert_param(IS_QUADSPI_FIFOTHRESHOLD(QUADSPI_FIFOThreshold));

  /* Read the CR Register */
  tmpreg = QUADSPI->CR ;
  /* Clear FIFO Threshold Bits */
  tmpreg &= QUADSPI_CR_CLEAR_FIFOTHRESHOLD_MASK ;
  /* Set FIFO Threshold Bits */
  tmpreg |= (QUADSPI_FIFOThreshold << 8);
  /* Write the CR Register */
  QUADSPI->CR = tmpreg;
}
//

/**
  * @brief  Enables or disables Automatic Polling Mode Stop when a match occurs.
  * @param  NewState: new state of the Automatic Polling Mode Stop. 
  *          This parameter can be: ENABLE or DISABLE.
  * @note   This function is used only in Automatic Polling Mode.    
  * @retval None
  */
void QUADSPI_AutoPollingModeStopCmd(FunctionalState NewState)
{
  /* Check the parameters */
  assert_param(IS_FUNCTIONAL_STATE(NewState));

  if (!(QUADSPI->SR & QUADSPI_SR_BUSY))
  /* Device is not Busy */
  {
    if (NewState != DISABLE)
    {
      /* Enable Automatic Polling Mode Stop */
      QUADSPI->CR |= QUADSPI_CR_APMS;
    }
    else
    {
      /* Disable Automatic Polling Mode Stop */
      QUADSPI->CR &= ~ QUADSPI_CR_APMS;
    }
  }
}
//

/**
  * @brief  Initializes the QUADSPI CCR according to the specified 
  *         parameters in the QUADSPI_ComConfig_InitStruct.
  * @param  QUADSPI_ComConfig_InitStruct: pointer to a QUADSPI_ComConfig_InitTypeDef structure that
  *         contains the communiction configuration informations about QUADSPI peripheral.
  * @retval None
  */
void QUADSPI_ComConfig_Init(QUADSPI_ComConfig_InitTypeDef* QUADSPI_ComConfig_InitStruct)
{
  uint32_t tmpreg = 0;

  /* Check the QUADSPI Communication Control parameters */
  assert_param(IS_QUADSPI_FMODE       (QUADSPI_ComConfig_InitStruct->QUADSPI_ComConfig_FMode));
  assert_param(IS_QUADSPI_SIOOMODE    (QUADSPI_ComConfig_InitStruct->QUADSPI_ComConfig_SIOOMode));
  assert_param(IS_QUADSPI_DMODE       (QUADSPI_ComConfig_InitStruct->QUADSPI_ComConfig_DMode));
  assert_param(IS_QUADSPI_DCY         (QUADSPI_ComConfig_InitStruct->QUADSPI_ComConfig_DummyCycles));
  assert_param(IS_QUADSPI_ABSIZE      (QUADSPI_ComConfig_InitStruct->QUADSPI_ComConfig_ABSize));
  assert_param(IS_QUADSPI_ABMODE      (QUADSPI_ComConfig_InitStruct->QUADSPI_ComConfig_ABMode));
  assert_param(IS_QUADSPI_ADSIZE      (QUADSPI_ComConfig_InitStruct->QUADSPI_ComConfig_ADSize));
  assert_param(IS_QUADSPI_ADMODE      (QUADSPI_ComConfig_InitStruct->QUADSPI_ComConfig_ADMode));
  assert_param(IS_QUADSPI_IMODE       (QUADSPI_ComConfig_InitStruct->QUADSPI_ComConfig_IMode));
  assert_param(IS_QUADSPI_INSTRUCTION (QUADSPI_ComConfig_InitStruct->QUADSPI_ComConfig_Ins));
	assert_param(IS_QUADSPI_DDRMODE     (QUADSPI_ComConfig_InitStruct->QUADSPI_ComConfig_DDRMode));
	assert_param(IS_QUADSPI_DHHC        (QUADSPI_ComConfig_InitStruct->QUADSPI_ComConfig_DHHC));
  
  /*------------------------ QUADSPI CCR Configuration ------------------------*/
  /* Get the QUADSPI CCR value */
  tmpreg = QUADSPI->CCR;
  /* Clear FMODE Mode bits */
  tmpreg &= QUADSPI_CCR_CLEAR_MASK;
  /* Configure QUADSPI: CCR Configuration */
  tmpreg |=  (uint32_t)( (QUADSPI_ComConfig_InitStruct->QUADSPI_ComConfig_FMode)
                       | (QUADSPI_ComConfig_InitStruct->QUADSPI_ComConfig_DDRMode)
											 | (QUADSPI_ComConfig_InitStruct->QUADSPI_ComConfig_DHHC)
                       | (QUADSPI_ComConfig_InitStruct->QUADSPI_ComConfig_SIOOMode)
                       | (QUADSPI_ComConfig_InitStruct->QUADSPI_ComConfig_DMode)
                       | (QUADSPI_ComConfig_InitStruct->QUADSPI_ComConfig_ABSize)
                       | (QUADSPI_ComConfig_InitStruct->QUADSPI_ComConfig_ABMode)                                                                       
                       | (QUADSPI_ComConfig_InitStruct->QUADSPI_ComConfig_ADSize)
                       | (QUADSPI_ComConfig_InitStruct->QUADSPI_ComConfig_ADMode)
                       | (QUADSPI_ComConfig_InitStruct->QUADSPI_ComConfig_IMode)
                       | (QUADSPI_ComConfig_InitStruct->QUADSPI_ComConfig_Ins)
                       |((QUADSPI_ComConfig_InitStruct->QUADSPI_ComConfig_DummyCycles)<<18));    
  /* Write to QUADSPI DCR */
  QUADSPI->CCR = tmpreg;      
}
//

/**
  * @brief  Configure the QUADSPI Automatic Polling Mode.
  * @param  QUADSPI_Match: Value to be compared with the masked status register to get a match. 
  *          This parameter can be any value between 0x00000000 and 0xFFFFFFFF.
  * @param  QUADSPI_Mask: Mask to be applied to the status bytes received in polling mode.. 
  *          This parameter can be any value between 0x00000000 and 0xFFFFFFFF.
  * @param  QUADSPI_Match_Mode: indicates which method should be used for determining a “match?during
  *         automatic polling mode. 
  *          This parameter can be any value of :
  *            @arg QUADSPI_PMM_AND: AND match mode- SMF is set if all the unmasked bits received from the flash match
  *                 the corresponding bits in the match register
  *            @arg QUADSPI_PMM_OR: OR match mode- SMF is set if any one of the unmasked bits received from the flash
                    matches its corresponding bit in the match register.
  * @note   This function is used only in Automatic Polling Mode
  * @retval None
  */
void QUADSPI_AutoPollingMode_Config(uint32_t QUADSPI_Match, uint32_t QUADSPI_Mask , uint32_t QUADSPI_Match_Mode)
{
  /* Check the parameters */
  assert_param(IS_QUADSPI_PMM(QUADSPI_Match_Mode));

  if (!(QUADSPI->SR & QUADSPI_SR_BUSY))
  /* Device is not Busy */
  {
    /* Set the Match Register */
    QUADSPI->PSMAR = QUADSPI_Match ;

    /* Set the Mask Register */
    QUADSPI->PSMKR = QUADSPI_Mask ;
    
    /* Set the Polling Match Mode */
    if(QUADSPI_Match_Mode)
    /* OR Match Mode */
    {
      /* Set the PMM bit */
      QUADSPI->CR |= QUADSPI_CR_PMM;
    }
    else
    /* AND Match Mode */
    {
      /* Reset the PMM bit */
      QUADSPI->CR &= ~ QUADSPI_CR_PMM;
    }
  }
}
//

/**
  * @brief  Fills each QUADSPI_ComConfig_InitStruct member with its default value.
  * @param  QUADSPI_ComConfig_InitStruct: pointer to a QUADSPI_ComConfig_InitTypeDef structure which will be initialized.
  * @retval None
  */
void QUADSPI_ComConfig_StructInit(QUADSPI_ComConfig_InitTypeDef* QUADSPI_ComConfig_InitStruct)
{
/*--------- Reset QUADSPI ComConfig init structure parameters default values ------------*/
    
/* Set QUADSPI Communication configuration structure parameters default values */
  /* Initialize the QUADSPI_ComConfig_DDRMode member */
  QUADSPI_ComConfig_InitStruct->QUADSPI_ComConfig_DDRMode = QUADSPI_ComConfig_DDRMode_Disable ;
  /* Initialize the QUADSPI_ComConfig_DHHC member */
  QUADSPI_ComConfig_InitStruct->QUADSPI_ComConfig_DHHC = QUADSPI_ComConfig_DHHC_Disable ;
  /* Initialize the QUADSPI_ComConfig_SIOOMode member */
  QUADSPI_ComConfig_InitStruct->QUADSPI_ComConfig_SIOOMode = QUADSPI_ComConfig_SIOOMode_Disable ;
  /* Initialize the QUADSPI_ComConfig_FMode member */
  QUADSPI_ComConfig_InitStruct->QUADSPI_ComConfig_FMode = QUADSPI_ComConfig_FMode_Indirect_Write ;
  /* Initialize the QUADSPI_ComConfig_DMode member */
  QUADSPI_ComConfig_InitStruct->QUADSPI_ComConfig_DMode = QUADSPI_ComConfig_DMode_NoData ;
  /* Initialize the QUADSPI_ComConfig_DummyCycles member */
  QUADSPI_ComConfig_InitStruct->QUADSPI_ComConfig_DummyCycles = 0 ;
  /* Initialize the QUADSPI_ComConfig_ABSize member */
  QUADSPI_ComConfig_InitStruct->QUADSPI_ComConfig_ABSize = QUADSPI_ComConfig_ABSize_8bit ;
  /* Initialize the QUADSPI_ComConfig_ABMode member */
  QUADSPI_ComConfig_InitStruct->QUADSPI_ComConfig_ABMode = QUADSPI_ComConfig_ABMode_NoAlternateByte ;
  /* Initialize the QUADSPI_ComConfig_ADSize member */
  QUADSPI_ComConfig_InitStruct->QUADSPI_ComConfig_ADSize = QUADSPI_ComConfig_ADSize_8bit ;
  /* Initialize the QUADSPI_ComConfig_ADMode member */
  QUADSPI_ComConfig_InitStruct->QUADSPI_ComConfig_ADMode = QUADSPI_ComConfig_ADMode_NoAddress ;
  /* Initialize the QUADSPI_ComConfig_IMode member */
  QUADSPI_ComConfig_InitStruct->QUADSPI_ComConfig_IMode = QUADSPI_ComConfig_IMode_NoInstruction ;
  /* Initialize the QUADSPI_ComConfig_Ins member */
  QUADSPI_ComConfig_InitStruct->QUADSPI_ComConfig_Ins = 0 ;
}
//

/**
  * @brief  Enable write operations on the Flash memory (the polling on status
   *        register id done by hardware)
  * @param  None
  * @retval None
  */
void QUADSPI_Valid_WriteEnable_PH(void)
{
  QUADSPI_SetFIFOThreshold(0);
  QUADSPI_SetDataLength(0);
  
  QUADSPI_ComConfig_InitStructure.QUADSPI_ComConfig_DummyCycles = 0;
  QUADSPI_ComConfig_InitStructure.QUADSPI_ComConfig_Ins 				= WRITE_ENABLE_CMD ; 
  QUADSPI_ComConfig_InitStructure.QUADSPI_ComConfig_IMode 			= QUADSPI_ComConfig_IMode_1Line;
  QUADSPI_ComConfig_InitStructure.QUADSPI_ComConfig_ABSize 			= QUADSPI_ComConfig_ABSize_8bit;
  QUADSPI_ComConfig_InitStructure.QUADSPI_ComConfig_ADSize 			= QUADSPI_ComConfig_ADSize_24bit;
  QUADSPI_ComConfig_InitStructure.QUADSPI_ComConfig_DMode 			= QUADSPI_ComConfig_DMode_NoData;
  QUADSPI_ComConfig_InitStructure.QUADSPI_ComConfig_ADMode 			= QUADSPI_ComConfig_ADMode_NoAddress;
  QUADSPI_ComConfig_InitStructure.QUADSPI_ComConfig_SIOOMode 		= QUADSPI_ComConfig_SIOOMode_Disable;
  QUADSPI_ComConfig_InitStructure.QUADSPI_ComConfig_FMode 			= QUADSPI_ComConfig_FMode_Indirect_Write;
  QUADSPI_ComConfig_InitStructure.QUADSPI_ComConfig_ABMode 			= QUADSPI_ComConfig_ABMode_NoAlternateByte;
  QUADSPI_ComConfig_Init(&QUADSPI_ComConfig_InitStructure);
  while(QUADSPI_GetFlagStatus(QUADSPI_SR_BUSY) == SET);
  
  QUADSPI_AutoPollingMode_Config(W25Q128FV_FSR_WREN, W25Q128FV_FSR_WREN, QSPI_MATCH_MODE_AND);
  QUADSPI_AutoPollingModeStopCmd(ENABLE);
  QUADSPI_SetDataLength(0x00);

  QUADSPI_ComConfig_InitStructure.QUADSPI_ComConfig_Ins 				= READ_STATUS_REG1_CMD;
  QUADSPI_ComConfig_InitStructure.QUADSPI_ComConfig_DMode 			= QUADSPI_ComConfig_DMode_1Line;
  QUADSPI_ComConfig_InitStructure.QUADSPI_ComConfig_ADMode 			= QUADSPI_ComConfig_ADMode_NoAddress;
  QUADSPI_ComConfig_InitStructure.QUADSPI_ComConfig_FMode 			= QUADSPI_ComConfig_FMode_Auto_Polling;
  QUADSPI_ComConfig_Init(&QUADSPI_ComConfig_InitStructure); 
  
  while(QUADSPI_GetFlagStatus(QUADSPI_SR_SMF)==RESET);
	
  QUADSPI_ClearFlag(QUADSPI_SR_SMF);
  QUADSPI_ClearFlag(QUADSPI_SR_TCF);
	
  while(QUADSPI_GetFlagStatus(QUADSPI_SR_BUSY)==SET);
}
//

/**
  * @brief  Mass Erase of Flash Memory   
  * @param  None
  *         
  * @retval None
  */
void QUADSPI_Valid_BulkErase(void)
{
  
  QUADSPI_Valid_WriteEnable_PH();
  
  /* Send Erase Sector Command */
  QUADSPI_SetDataLength(0);
  QUADSPI_ComConfig_StructInit(&QUADSPI_ComConfig_InitStructure); 
  QUADSPI_ComConfig_InitStructure.QUADSPI_ComConfig_Ins 			= CHIP_ERASE_CMD;
  QUADSPI_ComConfig_InitStructure.QUADSPI_ComConfig_IMode 		= QUADSPI_ComConfig_IMode_1Line;
  QUADSPI_ComConfig_InitStructure.QUADSPI_ComConfig_DMode 		= QUADSPI_ComConfig_DMode_NoData;
  QUADSPI_ComConfig_InitStructure.QUADSPI_ComConfig_ADSize 		= QUADSPI_ComConfig_ADSize_24bit;
  QUADSPI_ComConfig_InitStructure.QUADSPI_ComConfig_ADMode 		= QUADSPI_ComConfig_ADMode_NoAddress;
  QUADSPI_ComConfig_InitStructure.QUADSPI_ComConfig_FMode 		= QUADSPI_ComConfig_FMode_Indirect_Read;
  QUADSPI_ComConfig_Init(&QUADSPI_ComConfig_InitStructure);
  
  /* Reconfigure Quadspi to automatic polling mode to wait for end of erase */
  QUADSPI_Valid_WriteEnable_PH();
	
  QUADSPI_SetDataLength(0x00);
	
  QUADSPI_AutoPollingMode_Config(0x00, 0x01, QUADSPI_PMM_AND);
	
  QUADSPI_AutoPollingModeStopCmd(ENABLE);
	
  QUADSPI_SetDataLength(0);
	
  QUADSPI_ClearFlag(QUADSPI_SR_SMF);
	
  QUADSPI_ComConfig_InitStructure.QUADSPI_ComConfig_Ins 		= READ_STATUS_REG1_CMD;
  QUADSPI_ComConfig_InitStructure.QUADSPI_ComConfig_DMode 	= QUADSPI_ComConfig_DMode_1Line;
  QUADSPI_ComConfig_InitStructure.QUADSPI_ComConfig_ADMode 	= QUADSPI_ComConfig_ADMode_NoAddress;
  QUADSPI_ComConfig_InitStructure.QUADSPI_ComConfig_FMode 	= QUADSPI_ComConfig_FMode_Auto_Polling;
  QUADSPI_ComConfig_Init(&QUADSPI_ComConfig_InitStructure); /* Read SR1 Command */
  
  /* Wait for end of Erase */
  while(QUADSPI_GetFlagStatus(QUADSPI_SR_SMF)==RESET);
	
  QUADSPI_ClearFlag(QUADSPI_SR_SMF);
	
  while(QUADSPI_GetFlagStatus(QUADSPI_FLAG_BUSY)==SET);
}
//

/**
  * @brief  Erases specified flash memory sector.  
  * @param  SectorAddr: Address of the sector that will be erased.  
  *         This parameter can be any value of between 0x00000000 and 0xFFFFFFFF
  * @retval None
  */
void QUADSPI_Valid_EraseSector(uint32_t SectorAddr)
{
  
  QUADSPI_Valid_WriteEnable_PH();
  
  /* Send Erase Sector Command */
  QUADSPI_SetDataLength(0);
  QUADSPI_ComConfig_StructInit(&QUADSPI_ComConfig_InitStructure); 
  QUADSPI_ComConfig_InitStructure.QUADSPI_ComConfig_IMode = QUADSPI_ComConfig_IMode_1Line;
  QUADSPI_ComConfig_InitStructure.QUADSPI_ComConfig_ADMode = QUADSPI_ComConfig_ADMode_1Line;
  QUADSPI_ComConfig_InitStructure.QUADSPI_ComConfig_ADSize = QUADSPI_ComConfig_ADSize_24bit;
  QUADSPI_ComConfig_InitStructure.QUADSPI_ComConfig_FMode = QUADSPI_ComConfig_FMode_Indirect_Read;
  QUADSPI_ComConfig_InitStructure.QUADSPI_ComConfig_DMode = QUADSPI_ComConfig_DMode_NoData;
  QUADSPI_ComConfig_InitStructure.QUADSPI_ComConfig_Ins = SECTOR_ERASE_CMD;

  QUADSPI_ComConfig_Init(&QUADSPI_ComConfig_InitStructure); 
  /* Set sector address to erase */
  QUADSPI_SetAddress(SectorAddr);
  
  /* Reconfigure Quadspi to automatic polling mode to wait for end of erase */
  QUADSPI_Valid_WriteEnable_PH();
  QUADSPI_SetDataLength(0x00);
  QUADSPI_AutoPollingMode_Config(0x00,0x01,QUADSPI_PMM_AND);
  QUADSPI_AutoPollingModeStopCmd(ENABLE);
  QUADSPI_SetDataLength(0);
  QUADSPI_ClearFlag(QUADSPI_FLAG_SM);
  QUADSPI_ComConfig_InitStructure.QUADSPI_ComConfig_FMode = QUADSPI_ComConfig_FMode_Auto_Polling;
  QUADSPI_ComConfig_InitStructure.QUADSPI_ComConfig_ADMode = QUADSPI_ComConfig_ADMode_NoAddress;
  QUADSPI_ComConfig_InitStructure.QUADSPI_ComConfig_DMode = QUADSPI_ComConfig_DMode_1Line;
  QUADSPI_ComConfig_InitStructure.QUADSPI_ComConfig_Ins = READ_STATUS_REG1_CMD;
  QUADSPI_ComConfig_Init(&QUADSPI_ComConfig_InitStructure); /* Read SR1 Command */
  
  /* Wait for end of Erase */
  while(QUADSPI_GetFlagStatus(QUADSPI_FLAG_SM)==RESET);
  QUADSPI_ClearFlag(QUADSPI_FLAG_SM);
  while(QUADSPI_GetFlagStatus(QUADSPI_FLAG_BUSY)==SET);
}
//

/**
  * @brief  Write the specified buffer to the flash memory  
  * @param  pBuffer: pointer to the buffer that will be written in the flash  
  * @param  WriteAddr: The address where the data will be written in flash  
  * @param  NumByteToWrite: The number of bytes to be will be written in flash
  *         This parameter must be inferior to Flash memroy page size
  * @retval None
  */
void QUADSPI_Valid_WritePage(uint8_t* pBuffer, uint32_t WriteAddr, uint32_t* NumByteToWrite)
{
  if (* NumByteToWrite == 0)
  return;
  
  QUADSPI_Valid_WriteEnable_PH();
  
   /* Writing Sequence -------------------------------------------------------*/

  QUADSPI_SetDataLength(*NumByteToWrite - 1);
  QUADSPI_SetAddress(WriteAddr);
  QUADSPI_ComConfig_StructInit(&QUADSPI_ComConfig_InitStructure); 
  QUADSPI_ComConfig_InitStructure.QUADSPI_ComConfig_IMode = QUADSPI_ComConfig_IMode_1Line;
  QUADSPI_ComConfig_InitStructure.QUADSPI_ComConfig_FMode = QUADSPI_ComConfig_FMode_Indirect_Write;
  QUADSPI_ComConfig_InitStructure.QUADSPI_ComConfig_ADMode = QUADSPI_ComConfig_ADMode_1Line;
  QUADSPI_ComConfig_InitStructure.QUADSPI_ComConfig_ADSize = QUADSPI_ComConfig_ADSize_24bit; /* Enable 4 Byte Addressing Mode */ //MBM 32
  QUADSPI_ComConfig_InitStructure.QUADSPI_ComConfig_DMode = QUADSPI_ComConfig_DMode_4Line;
  QUADSPI_ComConfig_InitStructure.QUADSPI_ComConfig_Ins = QUAD_INPUT_PAGE_PROG_CMD;
  QUADSPI_ComConfig_Init(&QUADSPI_ComConfig_InitStructure); /* Write Buffer to the Flash */
  
  while((* NumByteToWrite)--)
  {
    while(!(QUADSPI_GetFlagStatus(QUADSPI_FLAG_FT)));
    QUADSPI_SendData8(* pBuffer++);
  }
  (* NumByteToWrite) = 0;
  
  while(QUADSPI_GetFlagStatus(QUADSPI_FLAG_BUSY)==SET);
  while(QUADSPI_GetFlagStatus(QUADSPI_FLAG_TC)==RESET);
  QUADSPI_ClearFlag(QUADSPI_FLAG_TC);
  
  QUADSPI_SetDataLength(0x00);
  QUADSPI_AutoPollingMode_Config(0x00,0x01,QUADSPI_PMM_AND);
  QUADSPI_AutoPollingModeStopCmd(ENABLE);
  QUADSPI_SetDataLength(0);

  QUADSPI_ComConfig_InitStructure.QUADSPI_ComConfig_FMode = QUADSPI_ComConfig_FMode_Auto_Polling;
  QUADSPI_ComConfig_InitStructure.QUADSPI_ComConfig_ADMode = QUADSPI_ComConfig_ADMode_NoAddress;
  QUADSPI_ComConfig_InitStructure.QUADSPI_ComConfig_DMode = QUADSPI_ComConfig_DMode_1Line;
  QUADSPI_ComConfig_InitStructure.QUADSPI_ComConfig_Ins = READ_STATUS_REG1_CMD;
  QUADSPI_ComConfig_Init(&QUADSPI_ComConfig_InitStructure); 
  
  while(QUADSPI_GetFlagStatus(QUADSPI_FLAG_SM)==RESET);
  QUADSPI_ClearFlag(QUADSPI_FLAG_SM);
  while(QUADSPI_GetFlagStatus(QUADSPI_FLAG_BUSY)==SET);
}
//

/**
  * @brief  Write the specified buffer to the flash memory  
  * @param  pBuffer: pointer to the buffer that will be written in the flash  
  * @param  WriteAddr: The address where the data will be written in flash  
  * @param  NumByteToWrite: The number of bytes to be will be written in flash
  * @retval None
  */
void QUADSPI_Valid_WriteBuffer(uint8_t* pBuffer, uint32_t WriteAddr, uint32_t* NumByteToWrite)
{
  uint32_t NumOfPage = 0, NumOfSingle = 0, Addr = 0, count = 0, temp = 0;
  uint32_t   QSPI_DataNum = 0;
  
  Addr = WriteAddr % W25Q128FV_PAGE_SIZE;
  count = W25Q128FV_PAGE_SIZE - Addr;
  NumOfPage =  (*NumByteToWrite) / W25Q128FV_PAGE_SIZE;
  NumOfSingle = (*NumByteToWrite) % W25Q128FV_PAGE_SIZE;
  
//  QUADSPI_Valid_4Byte_Address(ENABLE);/* Enable 4 Byte Addressing Mode */

  if (Addr == 0) /*!< WriteAddr is W25Q128FV_PAGE_SIZE aligned  */
  {
    if (NumOfPage == 0) /*!< NumByteToWrite < W25Q128FV_PAGE_SIZE */
    {
      QSPI_DataNum = (*NumByteToWrite);      
      QUADSPI_Valid_WritePage(pBuffer, WriteAddr, &QSPI_DataNum);
    }
    else /*!< NumByteToWrite > W25Q128FV_PAGE_SIZE */
    {
      while (NumOfPage--)
      {
        QSPI_DataNum = W25Q128FV_PAGE_SIZE;
        QUADSPI_Valid_WritePage(pBuffer, WriteAddr, &QSPI_DataNum);
        WriteAddr +=  W25Q128FV_PAGE_SIZE;
        pBuffer += W25Q128FV_PAGE_SIZE;
      }
      
      QSPI_DataNum = NumOfSingle;
      QUADSPI_Valid_WritePage(pBuffer, WriteAddr, &QSPI_DataNum);
    }
  }
  else /*!< WriteAddr is not W25Q128FV_PAGE_SIZE aligned  */
  {
    if (NumOfPage == 0) /*!< NumByteToWrite < W25Q128FV_PAGE_SIZE */
    {
      if (NumOfSingle > count) /*!< (NumByteToWrite + WriteAddr) > W25Q128FV_PAGE_SIZE */
      {
        temp = NumOfSingle - count;
        QSPI_DataNum = count;
        QUADSPI_Valid_WritePage(pBuffer, WriteAddr, &QSPI_DataNum);
        WriteAddr +=  count;
        pBuffer += count;
        
        QSPI_DataNum = temp;
        QUADSPI_Valid_WritePage(pBuffer, WriteAddr, &QSPI_DataNum);
      }
      else
      {
        QSPI_DataNum = (*NumByteToWrite); 
        QUADSPI_Valid_WritePage(pBuffer, WriteAddr, &QSPI_DataNum);
      }
    }
    else /*!< NumByteToWrite > W25Q128FV_PAGE_SIZE */
    {
      (*NumByteToWrite) -= count;
      NumOfPage =  (*NumByteToWrite) / W25Q128FV_PAGE_SIZE;
      NumOfSingle = (*NumByteToWrite) % W25Q128FV_PAGE_SIZE;

      QSPI_DataNum = count;
        
      QUADSPI_Valid_WritePage(pBuffer, WriteAddr, &QSPI_DataNum);
      WriteAddr +=  count;
      pBuffer += count;

      while (NumOfPage--)
      {
        QSPI_DataNum = W25Q128FV_PAGE_SIZE;
        
        QUADSPI_Valid_WritePage(pBuffer, WriteAddr, &QSPI_DataNum);
        WriteAddr +=  W25Q128FV_PAGE_SIZE;
        pBuffer += W25Q128FV_PAGE_SIZE;
      }

      if (NumOfSingle != 0)
      {
        QSPI_DataNum = NumOfSingle;
        
        QUADSPI_Valid_WritePage(pBuffer, WriteAddr, &QSPI_DataNum);
      }
    }
  }
  
//  QUADSPI_Valid_4Byte_Address(DISABLE);/* Disable 4 Byte Addressing Mode */
}
//

/*
 *  Initialize Flash Programming Functions
 *    Parameter:      adr:  Device Base Address
 *                    clk:  Clock Frequency (Hz)
 *                    fnc:  Function Code (1 - Erase, 2 - Program, 3 - Verify)
 *    Return Value:   0 - OK,  1 - Failed
 */
int Init (unsigned long adr, unsigned long clk, unsigned long fnc)
{
  GPIO_InitStructTypeDef GPIO_InitStructure;
	
	base_adr = adr;
	
	SystemInit();
	
	/*  Init structs to Zero*/
	memset( &QUADSPI_InitStructure,0,sizeof(QUADSPI_InitStructure));
	memset( &QUADSPI_ComConfig_InitStructure,0,sizeof(QUADSPI_ComConfig_InitStructure));
	
	/* set interrupt vector address */
//	SCB->VTOR = SRAM2_BASE;
	 
  RCC_HCLKConfig(RCC_SYSCLK_Div1);

	/*  Init structs to Zero*/
	memset( &QUADSPI_InitStructure,0,sizeof(QUADSPI_InitStructure));
	memset( &QUADSPI_ComConfig_InitStructure,0,sizeof(QUADSPI_ComConfig_InitStructure));
	
  /* Enable QUADSPI Clock*/
	RCC->AHB3ENR |= RCC_AHB3Periph_QSPI;

  /* Enable GPIO AHB1 clock */
  RCC_AHB1PeriphClockCmd(	RCC_AHB1Periph_GPIOA | RCC_AHB1Periph_GPIOB |
													RCC_AHB1Periph_GPIOC | RCC_AHB1Periph_GPIOD | 
													RCC_AHB1Periph_GPIOE | RCC_AHB1Periph_GPIOF | 
													RCC_AHB1Periph_GPIOG | RCC_AHB1Periph_GPIOH | 
													RCC_AHB1Periph_GPIOI | RCC_AHB1Periph_GPIOJ | 
													RCC_AHB1Periph_GPIOK,ENABLE);

	/* Enable APB2 clock */
  RCC_APB2PeriphClockCmd(RCC_APB2Periph_SYSCFG,ENABLE);
	
  GPIO_InitStructure.GPIO_Mode = GPIO_Mode_AF;
  GPIO_InitStructure.GPIO_OType = GPIO_OType_PP;
  GPIO_InitStructure.GPIO_PuPd = GPIO_PuPd_NOPULL;
  GPIO_InitStructure.GPIO_Speed = GPIO_Speed_100MHz;
  
	/* CLK */
	GPIO_PinAFConfig(GPIOB, GPIO_PinSource2, 9);
	GPIO_InitStructure.GPIO_Pin = GPIO_Pin_2;
  GPIO_Init(GPIOB,& GPIO_InitStructure);

	/* 1IO0 */
  GPIO_PinAFConfig(GPIOF, GPIO_PinSource8, 10);
  GPIO_InitStructure.GPIO_Pin = GPIO_Pin_8;
  GPIO_Init(GPIOF,& GPIO_InitStructure);

	/* 1IO1 */
  GPIO_PinAFConfig(GPIOF, GPIO_PinSource9, 10);
  GPIO_InitStructure.GPIO_Pin = GPIO_Pin_9;
  GPIO_Init(GPIOF,& GPIO_InitStructure);

	/* 1IO2 */
  GPIO_PinAFConfig(GPIOE, GPIO_PinSource2, 9);
  GPIO_InitStructure.GPIO_Pin = GPIO_Pin_2;
  GPIO_Init(GPIOE,& GPIO_InitStructure);

	/* 1IO3 */
  GPIO_PinAFConfig(GPIOF, GPIO_PinSource6, 9);
  GPIO_InitStructure.GPIO_Pin = GPIO_Pin_6;
  GPIO_Init(GPIOF,& GPIO_InitStructure);

	/* NCS1 */
  GPIO_PinAFConfig(GPIOB, GPIO_PinSource6, 10);
  GPIO_InitStructure.GPIO_Pin = GPIO_Pin_6;
  GPIO_Init(GPIOB,& GPIO_InitStructure);

  RCC_HCLKConfig(RCC_SYSCLK_Div1);
  QUADSPI_DeInit();

  QUADSPI_InitStructure.QUADSPI_FSize 		= 24; // 2^24 = 16MB
  QUADSPI_InitStructure.QUADSPI_Prescaler = 0x1; // 200MHz /(1+1) = 100MHz 
	QUADSPI_InitStructure.QUADSPI_FSelect 	= QUADSPI_FSelect_1;
  QUADSPI_InitStructure.QUADSPI_CKMode 		= QUADSPI_CKMode_Mode0;
	QUADSPI_InitStructure.QUADSPI_DFlash 		= QUADSPI_DFlash_Disable;
  QUADSPI_InitStructure.QUADSPI_CSHTime 	= QUADSPI_CSHTime_6Cycle;
  QUADSPI_InitStructure.QUADSPI_SShift 		= QUADSPI_SShift_HalfCycleShift;
  QUADSPI_Init(&QUADSPI_InitStructure);
  
  QUADSPI_ComConfig_InitStructure.QUADSPI_ComConfig_FMode 			= QUADSPI_ComConfig_FMode_Indirect_Write;
	
  QUADSPI_ComConfig_InitStructure.QUADSPI_ComConfig_DummyCycles = 0;
  QUADSPI_ComConfig_InitStructure.QUADSPI_ComConfig_IMode 			= QUADSPI_ComConfig_IMode_1Line;
  QUADSPI_ComConfig_InitStructure.QUADSPI_ComConfig_ABSize 			= QUADSPI_ComConfig_ABSize_8bit;
  QUADSPI_ComConfig_InitStructure.QUADSPI_ComConfig_DMode 			= QUADSPI_ComConfig_DMode_NoData;
  QUADSPI_ComConfig_InitStructure.QUADSPI_ComConfig_ADSize 			= QUADSPI_ComConfig_ADSize_24bit;
  QUADSPI_ComConfig_InitStructure.QUADSPI_ComConfig_ADMode 			= QUADSPI_ComConfig_ADMode_NoAddress;
  QUADSPI_ComConfig_InitStructure.QUADSPI_ComConfig_SIOOMode 		= QUADSPI_ComConfig_SIOOMode_Disable;
  QUADSPI_ComConfig_InitStructure.QUADSPI_ComConfig_ABMode 			= QUADSPI_ComConfig_ABMode_NoAlternateByte;
	
  
  QUADSPI_Cmd(ENABLE);
  QUADSPI_SetFIFOThreshold(0);
  QUADSPI_SetDataLength(0);	
	
  /* Reset memory config */
	/* Send RESET ENABLE command (0x66) to be able to reset the memory registers */
	while(QUADSPI->SR & 0x20);  /* Wait for busy flag to be cleared */
	QUADSPI->CCR = 0x2166;
	QUADSPI->AR = 0;
	QUADSPI->ABR = 0;
	QUADSPI->DLR = 0;
	__DSB(); 
	 
	/* Send RESET command (0x99) to reset the memory registers */
	while(QUADSPI->SR & 0x20);  /* Wait for busy flag to be cleared */
	QUADSPI->CCR = 0x2199;
	QUADSPI->AR = 0;
	QUADSPI->ABR = 0;
	QUADSPI->DLR = 0;
	__DSB();
	
	/* Configure QUADSPI To Memory Mapped Mode */
  QUADSPI_ComConfig_InitStructure.QUADSPI_ComConfig_DummyCycles	= 8;
  QUADSPI_ComConfig_InitStructure.QUADSPI_ComConfig_Ins 				= QUAD_OUT_FAST_READ_CMD;
  QUADSPI_ComConfig_InitStructure.QUADSPI_ComConfig_DMode 			= QUADSPI_ComConfig_DMode_4Line;
  QUADSPI_ComConfig_InitStructure.QUADSPI_ComConfig_IMode 			= QUADSPI_ComConfig_IMode_1Line;
  QUADSPI_ComConfig_InitStructure.QUADSPI_ComConfig_ADMode 			= QUADSPI_ComConfig_ADMode_1Line;
  QUADSPI_ComConfig_InitStructure.QUADSPI_ComConfig_ADSize 			= QUADSPI_ComConfig_ADSize_24bit;
  QUADSPI_ComConfig_InitStructure.QUADSPI_ComConfig_FMode 			= QUADSPI_ComConfig_FMode_Memory_Mapped;
  QUADSPI_ComConfig_Init(&QUADSPI_ComConfig_InitStructure);
	
	return 0;
}
//

/*
 *  De-Initialize Flash Programming Functions
 *    Parameter:      fnc:  Function Code (1 - Erase, 2 - Program, 3 - Verify)
 *    Return Value:   0 - OK,  1 - Failed
 */
int UnInit (unsigned long fnc)
{
  return (int)(0);
}
//

/*
 *  Erase complete Flash Memory
 *    Return Value:   0 - OK,  1 - Failed
 */
int EraseChip (void)
{
  QUADSPI_Valid_BulkErase();
	return 0;
}
//

/*
 *  Erase Sector in Flash Memory
 *    Parameter:      adr:  Sector Address
 *    Return Value:   0 - OK,  1 - Failed
 */
int EraseSector (unsigned long adr)
{
	uint32_t sectorAddress = (uint32_t)((uint32_t)(adr - base_adr) - ((uint32_t)adr % W25Q128FV_SECTOR_SIZE));
	
	QUADSPI_Valid_EraseSector(sectorAddress);
	
	return 0;
}
//

/*  
 *  Blank Check Checks if Memory is Blank
 *    Parameter:      adr:  Block Start Address
 *                    sz:   Block Size (in bytes)
 *                    pat:  Block Pattern
 *    Return Value:   0 - OK,  1 - Failed
 */
int BlankCheck (unsigned long adr, unsigned long sz, unsigned char pat)
{
	return (int)(1);                                   // Always Force Erase
}
//

/*
 *  Program Page in Flash Memory
 *    Parameter:      adr:  Page Start Address
 *                    sz:   Page Size
 *                    buf:  Page Data
 *    Return Value:   0 - OK,  1 - Failed
 */
int ProgramPage (unsigned long adr, unsigned long sz, unsigned char *buf)
{
	uint32_t Counter = sz;
	
  QUADSPI_Valid_WriteBuffer((uint8_t*)buf, ((adr - base_adr) & 0x0FFFFFFF), &Counter);
	
	return 0;
}
//
#if 0
/**
  * Description :
  * Verify flash memory with RAM buffer and calculates checksum value of
  * the programmed memory
  * Inputs    :
  *      FlashAddr     : Flash address
  *      RAMBufferAddr : RAM buffer address
  *      Size          : Size (in WORD)  
  *      InitVal       : Initial CRC value
  * outputs   :
  *     R0             : Operation failed (address of failure)
  *     R1             : Checksum value
  * Note: Optional for all types of device
  */
unsigned long Verify (unsigned long Addr, unsigned long numBytes, unsigned char *pBuff)
{
	uint32_t VerifiedData = 0;
	
	QUADSPI_SetAlternateByte(0x0000);
	
		/* Configure QSPI: CR register with timeout counter enable */
	MODIFY_REG(QUADSPI->CR, QUADSPI_CR_TCEN, QSPI_TIMEOUT_COUNTER_DISABLE);

	/* Configure QSPI: LPTR register with the low-power timeout value */
	WRITE_REG(QUADSPI->LPTR, 0);
	
	/* Clear interrupt */
	WRITE_REG(QUADSPI->FCR, QSPI_FLAG_TO);

	/* Configure QUADSPI To Memory Mapped Mode */
//  QUADSPI_ComConfig_InitStructure.QUADSPI_ComConfig_DummyCycles	= 4;
//  QUADSPI_ComConfig_InitStructure.QUADSPI_ComConfig_Ins 				= QUAD_INOUT_FAST_READ_CMD;
//  QUADSPI_ComConfig_InitStructure.QUADSPI_ComConfig_IMode 			= QUADSPI_ComConfig_IMode_1Line;
//	QUADSPI_ComConfig_InitStructure.QUADSPI_ComConfig_ABSize			= QUADSPI_ComConfig_ABSize_8bit;
//  QUADSPI_ComConfig_InitStructure.QUADSPI_ComConfig_DMode 			= QUADSPI_ComConfig_DMode_4Line;
//	QUADSPI_ComConfig_InitStructure.QUADSPI_ComConfig_DHHC				= QUADSPI_ComConfig_DHHC_Disable;
//  QUADSPI_ComConfig_InitStructure.QUADSPI_ComConfig_ADMode 			= QUADSPI_ComConfig_ADMode_4Line;
//  QUADSPI_ComConfig_InitStructure.QUADSPI_ComConfig_ADSize 			= QUADSPI_ComConfig_ADSize_24bit;
//	QUADSPI_ComConfig_InitStructure.QUADSPI_ComConfig_ABMode			= QUADSPI_ComConfig_ABMode_4Line;
//	QUADSPI_ComConfig_InitStructure.QUADSPI_ComConfig_DDRMode			= QUADSPI_ComConfig_DDRMode_Disable;
//	QUADSPI_ComConfig_InitStructure.QUADSPI_ComConfig_SIOOMode		= QUADSPI_ComConfig_SIOOMode_Disable;
//  QUADSPI_ComConfig_InitStructure.QUADSPI_ComConfig_FMode 			= QUADSPI_ComConfig_FMode_Memory_Mapped;
//  QUADSPI_ComConfig_Init(&QUADSPI_ComConfig_InitStructure);

	/*---- Command with instruction, address and alternate bytes ----*/
	/* Configure QSPI: CCR register with all communications parameters */
	WRITE_REG(QUADSPI->CCR, (QSPI_DDR_MODE_DISABLE				| QSPI_DDR_HHC_ANALOG_DELAY | QSPI_SIOO_INST_EVERY_CMD 	|
													 QSPI_DATA_4_LINES 						| (4 << 18) 								| 0x00 											|
													 QSPI_ALTERNATE_BYTES_4_LINES	| QSPI_ADDRESS_24_BITS 			| QSPI_ADDRESS_4_LINES 			|
													 QSPI_INSTRUCTION_1_LINE 			| QUAD_INOUT_FAST_READ_CMD 	| QUADSPI_CCR_FMODE));

	unsigned char *pFlash = (unsigned char *)Addr;
	unsigned long returnAddress = Addr + numBytes;
	
	/** validate firmware in the flash */
	do
	{
		if(*pFlash != *pBuff)
		{
			returnAddress = (unsigned long)pFlash;
			break;
		}
		
		pBuff++;
		pFlash++;
		VerifiedData++;
	}while(--numBytes);
		
	return returnAddress;
}
//
#endif
