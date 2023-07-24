/* USER CODE BEGIN Header */
/**
  ******************************************************************************
  * File Name          : FMC.c
  * Description        : This file provides code for the configuration
  *                      of the FMC peripheral.
  ******************************************************************************
  * @attention
  *
  * Copyright (c) 2023 STMicroelectronics.
  * All rights reserved.
  *
  * This software is licensed under terms that can be found in the LICENSE file
  * in the root directory of this software component.
  * If no LICENSE file comes with this software, it is provided AS-IS.
  *
  ******************************************************************************
  */
/* USER CODE END Header */

/* Includes ------------------------------------------------------------------*/
#include "fmc.h"

/* USER CODE BEGIN 0 */
typedef struct
{
  uint32_t TargetBank;           /*!< Target Bank                             */
  uint32_t RefreshMode;          /*!< Refresh Mode                            */
  uint32_t RefreshRate;          /*!< Refresh Rate                            */
  uint32_t BurstLength;          /*!< Burst Length                            */
  uint32_t BurstType;            /*!< Burst Type                              */
  uint32_t CASLatency;           /*!< CAS Latency                             */
  uint32_t OperationMode;        /*!< Operation Mode                          */
  uint32_t WriteBurstMode;       /*!< Write Burst Mode                        */
} IS42S32800J_Context_t;

#define REFRESH_COUNT            ((uint32_t)0x0603) /* SDRAM refresh counter (100Mhz SD clock) */
#define SDRAM_DEVICE_ADDR        0xD0000000U
#define SDRAM_DEVICE_SIZE        0x800000U
#define IS42S32800J_TIMEOUT      ((uint32_t)0xFFFF)

/**
  * @}
  */

/** @defgroup IS42S32800J_Exported_Constants
  * @{
  */
#define IS42S32800J_OK                (0)
#define IS42S32800J_ERROR             (-1)

/* Register Mode */
#define IS42S32800J_BURST_LENGTH_1              0x00000000U
#define IS42S32800J_BURST_LENGTH_2              0x00000001U
#define IS42S32800J_BURST_LENGTH_4              0x00000002U
#define IS42S32800J_BURST_LENGTH_8              0x00000004U
#define IS42S32800J_BURST_TYPE_SEQUENTIAL       0x00000000U
#define IS42S32800J_BURST_TYPE_INTERLEAVED      0x00000008U
#define IS42S32800J_CAS_LATENCY_2               0x00000020U
#define IS42S32800J_CAS_LATENCY_3               0x00000030U
#define IS42S32800J_OPERATING_MODE_STANDARD     0x00000000U
#define IS42S32800J_WRITEBURST_MODE_PROGRAMMED  0x00000000U
#define IS42S32800J_WRITEBURST_MODE_SINGLE      0x00000200U

/* Command Mode */
#define IS42S32800J_NORMAL_MODE_CMD             0x00000000U
#define IS42S32800J_CLK_ENABLE_CMD              0x00000001U
#define IS42S32800J_PALL_CMD                    0x00000002U
#define IS42S32800J_AUTOREFRESH_MODE_CMD        0x00000003U
#define IS42S32800J_LOAD_MODE_CMD               0x00000004U
#define IS42S32800J_SELFREFRESH_MODE_CMD        0x00000005U
#define IS42S32800J_POWERDOWN_MODE_CMD          0x00000006U

int32_t IS42S32800J_Init(SDRAM_HandleTypeDef *Ctx, IS42S32800J_Context_t *pRegMode);
int32_t IS42S32800J_ClockEnable(SDRAM_HandleTypeDef *Ctx, uint32_t Interface);
int32_t IS42S32800J_Precharge(SDRAM_HandleTypeDef *Ctx, uint32_t Interface);
int32_t IS42S32800J_ModeRegConfig(SDRAM_HandleTypeDef *Ctx, IS42S32800J_Context_t *pRegMode);
int32_t IS42S32800J_TimingConfig(SDRAM_HandleTypeDef *Ctx, FMC_SDRAM_TimingTypeDef *pTiming);
int32_t IS42S32800J_RefreshMode(SDRAM_HandleTypeDef *Ctx, uint32_t Interface, uint32_t RefreshMode);
int32_t IS42S32800J_RefreshRate(SDRAM_HandleTypeDef *Ctx, uint32_t RefreshCount);
int32_t IS42S32800J_EnterPowerMode(SDRAM_HandleTypeDef *Ctx, uint32_t Interface);
int32_t IS42S32800J_ExitPowerMode(SDRAM_HandleTypeDef *Ctx, uint32_t Interface);
int32_t IS42S32800J_Sendcmd(SDRAM_HandleTypeDef *Ctx, FMC_SDRAM_CommandTypeDef *SdramCmd);

static FMC_SDRAM_CommandTypeDef Command;
/**
  * @}
  */

/** @defgroup IS42S32800J_Function_Prototypes IS42S32800J Function Prototypes
  * @{
  */
int32_t IS42S32800J_Delay(uint32_t Delay);

/**
  * @}
  */

/** @defgroup IS42S32800J_Exported_Functions IS42S32800J Exported Functions
  * @{
  */
/**
  * @brief  Initializes the IS42S32800J SDRAm memory
  * @param  Ctx Component object pointer
  * @param  pRegMode : Pointer to Register Mode structure
  * @retval error status
  */
int32_t IS42S32800J_Init(SDRAM_HandleTypeDef *Ctx, IS42S32800J_Context_t *pRegMode)
{
  int32_t ret = IS42S32800J_ERROR;

  /* Step 1: Configure a clock configuration enable command */
  if(IS42S32800J_ClockEnable(Ctx, pRegMode->TargetBank) == IS42S32800J_OK)
  {
    /* Step 2: Insert 100 us minimum delay */
    /* Inserted delay is equal to 1 ms due to systick time base unit (ms) */
    HAL_Delay(1);
    IS42S32800J_ClockEnable(Ctx, pRegMode->TargetBank);
    HAL_Delay(1);

    /* Step 3: Configure a PALL (precharge all) command */
    if(IS42S32800J_Precharge(Ctx, pRegMode->TargetBank) == IS42S32800J_OK)
    {
      HAL_Delay(1);

      /* Step 4: Configure a Refresh command */
      if(IS42S32800J_RefreshMode(Ctx, pRegMode->TargetBank, pRegMode->RefreshMode) == IS42S32800J_OK)
      {
        HAL_Delay(1);

        /* Step 5: Program the external memory mode register */
        if(IS42S32800J_ModeRegConfig(Ctx, pRegMode) == IS42S32800J_OK)
        {
          HAL_Delay(1);

          /* Step 6: Set the refresh rate counter */
          if(IS42S32800J_RefreshRate(Ctx, pRegMode->RefreshRate) == IS42S32800J_OK)
          {
            ret = IS42S32800J_OK;
          }
        }
      }
    }
  }
  return ret;
}

/**
  * @brief  Enable SDRAM clock
  * @param  Ctx Component object pointer
  * @param  Interface Could be FMC_SDRAM_CMD_TARGET_BANK1 or FMC_SDRAM_CMD_TARGET_BANK2
  * @retval error status
  */
int32_t IS42S32800J_ClockEnable(SDRAM_HandleTypeDef *Ctx, uint32_t Interface)
{
  Command.CommandMode            = IS42S32800J_CLK_ENABLE_CMD;
  Command.CommandTarget          = FMC_SDRAM_CMD_TARGET_BANK2;
  Command.AutoRefreshNumber      = 1;
  Command.ModeRegisterDefinition = 0;

  /* Send the command */
  if(HAL_SDRAM_SendCommand(Ctx, &Command, IS42S32800J_TIMEOUT) != HAL_OK)
  {
    return IS42S32800J_ERROR;
  }
  else
  {
    return IS42S32800J_OK;
  }
}

/**
  * @brief  Precharge all sdram banks
  * @param  Ctx Component object pointer
  * @param  Interface Could be FMC_SDRAM_CMD_TARGET_BANK1 or FMC_SDRAM_CMD_TARGET_BANK2
  * @retval error status
  */
int32_t IS42S32800J_Precharge(SDRAM_HandleTypeDef *Ctx, uint32_t Interface)
{
  Command.CommandMode            = FMC_SDRAM_CMD_PALL;
  Command.CommandTarget          = FMC_SDRAM_CMD_TARGET_BANK2;
  Command.AutoRefreshNumber      = 8;
  Command.ModeRegisterDefinition = 0;

  /* Send the command */
  if(HAL_SDRAM_SendCommand(Ctx, &Command, IS42S32800J_TIMEOUT) != HAL_OK)
  {
    return IS42S32800J_ERROR;
  }
  else
  {
    return IS42S32800J_OK;
  }
}

/**
  * @brief  Program the external memory mode register
  * @param  Ctx Component object pointer
  * @param  pRegMode : Pointer to Register Mode structure
  * @retval error status
  */
int32_t IS42S32800J_ModeRegConfig(SDRAM_HandleTypeDef *Ctx, IS42S32800J_Context_t *pRegMode)
{
  uint32_t tmpmrd;

  /* Program the external memory mode register */
  tmpmrd = (uint32_t)pRegMode->BurstLength   |\
                     pRegMode->BurstType     |\
                     pRegMode->CASLatency    |\
                     pRegMode->OperationMode |\
                     pRegMode->WriteBurstMode;

  Command.CommandMode            = FMC_SDRAM_CMD_LOAD_MODE;
  Command.CommandTarget          = FMC_SDRAM_CMD_TARGET_BANK2;
  Command.AutoRefreshNumber      = 1;
  Command.ModeRegisterDefinition = tmpmrd;

  /* Send the command */
  if(HAL_SDRAM_SendCommand(Ctx, &Command, IS42S32800J_TIMEOUT) != HAL_OK)
  {
    return IS42S32800J_ERROR;
  }
  else
  {
    return IS42S32800J_OK;
  }
}

/**
  * @brief  Program the SDRAM timing
  * @param  Ctx Component object pointer
  * @param  pTiming Pointer to SDRAM timing configuration structure
  * @retval error status
  */
int32_t IS42S32800J_TimingConfig(SDRAM_HandleTypeDef *Ctx, FMC_SDRAM_TimingTypeDef *pTiming)
{
  /* Program the SDRAM timing */
  if(HAL_SDRAM_Init(Ctx, pTiming) != HAL_OK)
  {
    return IS42S32800J_ERROR;
  }
  else
  {
    return IS42S32800J_OK;
  }
}

/**
  * @brief  Configure Refresh mode
  * @param  Ctx Component object pointer
  * @param  Interface Could be FMC_SDRAM_CMD_TARGET_BANK1 or FMC_SDRAM_CMD_TARGET_BANK2
  * @param  RefreshMode Could be IS42S32800J_CMD_AUTOREFRESH_MODE or
  *                      IS42S32800J_CMD_SELFREFRESH_MODE
  * @retval error status
  */
int32_t IS42S32800J_RefreshMode(SDRAM_HandleTypeDef *Ctx, uint32_t Interface, uint32_t RefreshMode)
{
  Command.CommandMode            = RefreshMode;
  Command.CommandTarget          = FMC_SDRAM_CMD_TARGET_BANK2;
  Command.AutoRefreshNumber      = 8;
  Command.ModeRegisterDefinition = 0;

  /* Send the command */
  if(HAL_SDRAM_SendCommand(Ctx, &Command, IS42S32800J_TIMEOUT) != HAL_OK)
  {
    return IS42S32800J_ERROR;
  }
  else
  {
    return IS42S32800J_OK;
  }
}

/**
  * @brief  Set the device refresh rate
  * @param  Ctx Component object pointer
  * @param  RefreshCount The refresh rate to be programmed
  * @retval error status
  */
int32_t IS42S32800J_RefreshRate(SDRAM_HandleTypeDef *Ctx, uint32_t RefreshCount)
{
  /* Set the device refresh rate */
  if(HAL_SDRAM_ProgramRefreshRate(Ctx, RefreshCount) != HAL_OK)
  {
    return IS42S32800J_ERROR;
  }
  else
  {
    HAL_SDRAM_SetAutoRefreshNumber(Ctx,2);
    return IS42S32800J_OK;
  }
}

/**
  * @brief  Enter Power mode
  * @param  Ctx Component object pointer
  * @param  Interface Could be FMC_SDRAM_CMD_TARGET_BANK1 or FMC_SDRAM_CMD_TARGET_BANK2
  * @retval error status
  */
int32_t IS42S32800J_EnterPowerMode(SDRAM_HandleTypeDef *Ctx, uint32_t Interface)
{
  Command.CommandMode            = IS42S32800J_POWERDOWN_MODE_CMD;
  Command.CommandTarget          = Interface;
  Command.AutoRefreshNumber      = 1;
  Command.ModeRegisterDefinition = 0;

  /* Send the command */
  if(HAL_SDRAM_SendCommand(Ctx, &Command, IS42S32800J_TIMEOUT) != HAL_OK)
  {
    return IS42S32800J_ERROR;
  }
  else
  {
    return IS42S32800J_OK;
  }
}

/**
  * @brief  Exit Power mode
  * @param  Ctx Component object pointer
  * @param  Interface Could be FMC_SDRAM_CMD_TARGET_BANK1 or FMC_SDRAM_CMD_TARGET_BANK2
  * @retval error status
  */
int32_t IS42S32800J_ExitPowerMode(SDRAM_HandleTypeDef *Ctx, uint32_t Interface)
{
  Command.CommandMode            = IS42S32800J_NORMAL_MODE_CMD;
  Command.CommandTarget          = Interface;
  Command.AutoRefreshNumber      = 1;
  Command.ModeRegisterDefinition = 0;

  /* Send the command */
  if(HAL_SDRAM_SendCommand(Ctx, &Command, IS42S32800J_TIMEOUT) != HAL_OK)
  {
    return IS42S32800J_ERROR;
  }
  else
  {
    return IS42S32800J_OK;
  }
}

/**
  * @brief  Sends command to the SDRAM bank.
  * @param  Ctx Component object pointer
  * @param  SdramCmd : Pointer to SDRAM command structure
  * @retval SDRAM status
  */
int32_t IS42S32800J_Sendcmd(SDRAM_HandleTypeDef *Ctx, FMC_SDRAM_CommandTypeDef *SdramCmd)
{
  if(HAL_SDRAM_SendCommand(Ctx, SdramCmd, IS42S32800J_TIMEOUT) != HAL_OK)
  {
    return IS42S32800J_ERROR;
  }
  else
  {
    return IS42S32800J_OK;
  }
}

/**
  * @}
  */

/** @defgroup IS42S32800J_Private_Functions IS42S32800J Private Functions
  * @{
  */

/**
  * @brief This function provides accurate delay (in milliseconds)
  * @param Delay : specifies the delay time length, in milliseconds
  * @retval IS42S32800J_OK
  */
int32_t IS42S32800J_Delay(uint32_t Delay)
{
  uint32_t tickstart;
  tickstart = HAL_GetTick();
  while((HAL_GetTick() - tickstart) < Delay)
  {
  }
  return IS42S32800J_OK;
}

void
BSP_SDRAM_Write_word( uint32_t address, uint32_t data )
{
  *( __IO uint32_t * )( SDRAM_DEVICE_ADDR + address ) = data;
}

uint32_t
BSP_SDRAM_Read_word( uint32_t address )
{
  return *( __IO uint32_t * )( SDRAM_DEVICE_ADDR + address );
}

int32_t
BSP_SDRAM_SingleTest( void )
{
  volatile uint32_t i;
  volatile uint32_t w=0;

  for ( i = 0; i < SDRAM_DEVICE_SIZE/4; ++i )
  {
    BSP_SDRAM_Write_word( i * 4, w );
  }

  for ( i = 0; i < SDRAM_DEVICE_SIZE/4; ++i )
  {
    if ( w != BSP_SDRAM_Read_word( i * 4 ) )
    {
      return -1;
    }
  }

  return 0;
}

/* USER CODE END 0 */

SDRAM_HandleTypeDef hsdram1;

/* FMC initialization function */
void MX_FMC_Init(void)
{
  /* USER CODE BEGIN FMC_Init 0 */
  static IS42S32800J_Context_t pRegMode;
  /* USER CODE END FMC_Init 0 */

  FMC_SDRAM_TimingTypeDef SdramTiming = {0};

  /* USER CODE BEGIN FMC_Init 1 */

  /* USER CODE END FMC_Init 1 */

  /** Perform the SDRAM1 memory initialization sequence
  */
  hsdram1.Instance = FMC_SDRAM_DEVICE;
  /* hsdram1.Init */
  hsdram1.Init.SDBank = FMC_SDRAM_BANK2;
  hsdram1.Init.ColumnBitsNumber = FMC_SDRAM_COLUMN_BITS_NUM_8;
  hsdram1.Init.RowBitsNumber = FMC_SDRAM_ROW_BITS_NUM_11;
  hsdram1.Init.MemoryDataWidth = FMC_SDRAM_MEM_BUS_WIDTH_32;
  hsdram1.Init.InternalBankNumber = FMC_SDRAM_INTERN_BANKS_NUM_4;
  hsdram1.Init.CASLatency = FMC_SDRAM_CAS_LATENCY_3;
  hsdram1.Init.WriteProtection = FMC_SDRAM_WRITE_PROTECTION_DISABLE;
  hsdram1.Init.SDClockPeriod = FMC_SDRAM_CLOCK_PERIOD_2;
  hsdram1.Init.ReadBurst = FMC_SDRAM_RBURST_ENABLE;
  hsdram1.Init.ReadPipeDelay = FMC_SDRAM_RPIPE_DELAY_0;
  /* SdramTiming */
  SdramTiming.LoadToActiveDelay = 3;
  SdramTiming.ExitSelfRefreshDelay = 7;
  SdramTiming.SelfRefreshTime = 4;
  SdramTiming.RowCycleDelay = 7;
  SdramTiming.WriteRecoveryTime = 4;
  SdramTiming.RPDelay = 3;
  SdramTiming.RCDDelay = 3;

  if (HAL_SDRAM_Init(&hsdram1, &SdramTiming) != HAL_OK)
  {
    Error_Handler( );
  }

  /* USER CODE BEGIN FMC_Init 2 */
  /* External memory mode register configuration */
  pRegMode.TargetBank      = FMC_SDRAM_CMD_TARGET_BANK2;
  pRegMode.RefreshMode     = IS42S32800J_AUTOREFRESH_MODE_CMD;
  pRegMode.RefreshRate     = REFRESH_COUNT;
  pRegMode.BurstLength     = IS42S32800J_BURST_LENGTH_1;
  pRegMode.BurstType       = IS42S32800J_BURST_TYPE_SEQUENTIAL;
  pRegMode.CASLatency      = IS42S32800J_CAS_LATENCY_3;
  pRegMode.OperationMode   = IS42S32800J_OPERATING_MODE_STANDARD;
  pRegMode.WriteBurstMode  = IS42S32800J_WRITEBURST_MODE_SINGLE;

  /* SDRAM initialization sequence */
  if(IS42S32800J_Init(&hsdram1, &pRegMode) != IS42S32800J_OK)
  {
    Error_Handler( );
  }
  /* USER CODE END FMC_Init 2 */
}

static uint32_t FMC_Initialized = 0;

static void HAL_FMC_MspInit(void){
  /* USER CODE BEGIN FMC_MspInit 0 */

  /* USER CODE END FMC_MspInit 0 */
  GPIO_InitTypeDef GPIO_InitStruct = {0};
  if (FMC_Initialized) {
    return;
  }
  FMC_Initialized = 1;

  /* Peripheral clock enable */
  __HAL_RCC_FMC_CLK_ENABLE();

  /** FMC GPIO Configuration
  PI6   ------> FMC_D28
  PI5   ------> FMC_NBL3
  PI4   ------> FMC_NBL2
  PI1   ------> FMC_D25
  PI0   ------> FMC_D24
  PI7   ------> FMC_D29
  PE1   ------> FMC_NBL1
  PI2   ------> FMC_D26
  PH15   ------> FMC_D23
  PH14   ------> FMC_D22
  PE0   ------> FMC_NBL0
  PI3   ------> FMC_D27
  PG15   ------> FMC_SDNCAS
  PD0   ------> FMC_D2
  PH13   ------> FMC_D21
  PI9   ------> FMC_D30
  PD1   ------> FMC_D3
  PI10   ------> FMC_D31
  PG8   ------> FMC_SDCLK
  PF2   ------> FMC_A2
  PF1   ------> FMC_A1
  PF0   ------> FMC_A0
  PG5   ------> FMC_A15
  PG5   ------> FMC_BA1
  PF3   ------> FMC_A3
  PG4   ------> FMC_A14
  PG4   ------> FMC_BA0
  PG2   ------> FMC_A12
  PF5   ------> FMC_A5
  PF4   ------> FMC_A4
  PE10   ------> FMC_D7
  PH5   ------> FMC_SDNWE
  PF13   ------> FMC_A7
  PF14   ------> FMC_A8
  PE9   ------> FMC_D6
  PE11   ------> FMC_D8
  PH10   ------> FMC_D18
  PH11   ------> FMC_D19
  PD15   ------> FMC_D1
  PD14   ------> FMC_D0
  PF12   ------> FMC_A6
  PF15   ------> FMC_A9
  PE12   ------> FMC_D9
  PE15   ------> FMC_D12
  PH9   ------> FMC_D17
  PH12   ------> FMC_D20
  PF11   ------> FMC_SDNRAS
  PG0   ------> FMC_A10
  PE8   ------> FMC_D5
  PE13   ------> FMC_D10
  PH6   ------> FMC_SDNE1
  PH8   ------> FMC_D16
  PD10   ------> FMC_D15
  PD9   ------> FMC_D14
  PG1   ------> FMC_A11
  PE7   ------> FMC_D4
  PE14   ------> FMC_D11
  PH7   ------> FMC_SDCKE1
  PD8   ------> FMC_D13
  */
  /* GPIO_InitStruct */
  GPIO_InitStruct.Pin = D28_Pin|FMC_NBL3_Pin|FMC_NBL2_Pin|D25_Pin
                          |D24_Pin|D29_Pin|D26_Pin|D27__IS42S32800G_DQ27_Pin
                          |D30_Pin|D31_Pin;
  GPIO_InitStruct.Mode = GPIO_MODE_AF_PP;
  GPIO_InitStruct.Pull = GPIO_PULLUP;
  GPIO_InitStruct.Speed = GPIO_SPEED_FREQ_VERY_HIGH;
  GPIO_InitStruct.Alternate = GPIO_AF12_FMC;

  HAL_GPIO_Init(GPIOI, &GPIO_InitStruct);

  /* GPIO_InitStruct */
  GPIO_InitStruct.Pin = FMC_NBL1_Pin|FMC_NBL0_Pin|D7_Pin|D6_Pin
                          |D8_Pin|D9_Pin|D12_Pin|D5_Pin
                          |D10_Pin|D4_Pin|D11_Pin;
  GPIO_InitStruct.Mode = GPIO_MODE_AF_PP;
  GPIO_InitStruct.Pull = GPIO_PULLUP;
  GPIO_InitStruct.Speed = GPIO_SPEED_FREQ_VERY_HIGH;
  GPIO_InitStruct.Alternate = GPIO_AF12_FMC;

  HAL_GPIO_Init(GPIOE, &GPIO_InitStruct);

  /* GPIO_InitStruct */
  GPIO_InitStruct.Pin = D23_Pin|D22_Pin|D21_Pin|SDNWE_Pin
                          |D18_Pin|D19_Pin|D17_Pin|D20_Pin
                          |SDNE1_Pin|D16_Pin|SDCKE1_Pin;
  GPIO_InitStruct.Mode = GPIO_MODE_AF_PP;
  GPIO_InitStruct.Pull = GPIO_PULLUP;
  GPIO_InitStruct.Speed = GPIO_SPEED_FREQ_VERY_HIGH;
  GPIO_InitStruct.Alternate = GPIO_AF12_FMC;

  HAL_GPIO_Init(GPIOH, &GPIO_InitStruct);

  /* GPIO_InitStruct */
  GPIO_InitStruct.Pin = SDNCAS_Pin|SDCLK_Pin|GPIO_PIN_5|GPIO_PIN_4
                          |GPIO_PIN_2|A10_Pin|A11_Pin;
  GPIO_InitStruct.Mode = GPIO_MODE_AF_PP;
  GPIO_InitStruct.Pull = GPIO_PULLUP;
  GPIO_InitStruct.Speed = GPIO_SPEED_FREQ_VERY_HIGH;
  GPIO_InitStruct.Alternate = GPIO_AF12_FMC;

  HAL_GPIO_Init(GPIOG, &GPIO_InitStruct);

  /* GPIO_InitStruct */
  GPIO_InitStruct.Pin = D2_Pin|D3_Pin|D1_Pin|D0_Pin
                          |D15_Pin|D14_Pin|D13_Pin;
  GPIO_InitStruct.Mode = GPIO_MODE_AF_PP;
  GPIO_InitStruct.Pull = GPIO_PULLUP;
  GPIO_InitStruct.Speed = GPIO_SPEED_FREQ_VERY_HIGH;
  GPIO_InitStruct.Alternate = GPIO_AF12_FMC;

  HAL_GPIO_Init(GPIOD, &GPIO_InitStruct);

  /* GPIO_InitStruct */
  GPIO_InitStruct.Pin = A2_Pin|A1_Pin|A0_Pin|A3_Pin
                          |A5_Pin|A4_Pin|A7_Pin|A8_Pin
                          |A6_Pin|A9_Pin|SNDRAS_Pin;
  GPIO_InitStruct.Mode = GPIO_MODE_AF_PP;
  GPIO_InitStruct.Pull = GPIO_PULLUP;
  GPIO_InitStruct.Speed = GPIO_SPEED_FREQ_VERY_HIGH;
  GPIO_InitStruct.Alternate = GPIO_AF12_FMC;

  HAL_GPIO_Init(GPIOF, &GPIO_InitStruct);

  /* USER CODE BEGIN FMC_MspInit 1 */

  /* USER CODE END FMC_MspInit 1 */
}

void HAL_SDRAM_MspInit(SDRAM_HandleTypeDef* sdramHandle){
  /* USER CODE BEGIN SDRAM_MspInit 0 */

  /* USER CODE END SDRAM_MspInit 0 */
  HAL_FMC_MspInit();
  /* USER CODE BEGIN SDRAM_MspInit 1 */

  /* USER CODE END SDRAM_MspInit 1 */
}

static uint32_t FMC_DeInitialized = 0;

static void HAL_FMC_MspDeInit(void){
  /* USER CODE BEGIN FMC_MspDeInit 0 */

  /* USER CODE END FMC_MspDeInit 0 */
  if (FMC_DeInitialized) {
    return;
  }
  FMC_DeInitialized = 1;
  /* Peripheral clock enable */
  __HAL_RCC_FMC_CLK_DISABLE();

  /** FMC GPIO Configuration
  PI6   ------> FMC_D28
  PI5   ------> FMC_NBL3
  PI4   ------> FMC_NBL2
  PI1   ------> FMC_D25
  PI0   ------> FMC_D24
  PI7   ------> FMC_D29
  PE1   ------> FMC_NBL1
  PI2   ------> FMC_D26
  PH15   ------> FMC_D23
  PH14   ------> FMC_D22
  PE0   ------> FMC_NBL0
  PI3   ------> FMC_D27
  PG15   ------> FMC_SDNCAS
  PD0   ------> FMC_D2
  PH13   ------> FMC_D21
  PI9   ------> FMC_D30
  PD1   ------> FMC_D3
  PI10   ------> FMC_D31
  PG8   ------> FMC_SDCLK
  PF2   ------> FMC_A2
  PF1   ------> FMC_A1
  PF0   ------> FMC_A0
  PG5   ------> FMC_A15
  PG5   ------> FMC_BA1
  PF3   ------> FMC_A3
  PG4   ------> FMC_A14
  PG4   ------> FMC_BA0
  PG2   ------> FMC_A12
  PF5   ------> FMC_A5
  PF4   ------> FMC_A4
  PE10   ------> FMC_D7
  PH5   ------> FMC_SDNWE
  PF13   ------> FMC_A7
  PF14   ------> FMC_A8
  PE9   ------> FMC_D6
  PE11   ------> FMC_D8
  PH10   ------> FMC_D18
  PH11   ------> FMC_D19
  PD15   ------> FMC_D1
  PD14   ------> FMC_D0
  PF12   ------> FMC_A6
  PF15   ------> FMC_A9
  PE12   ------> FMC_D9
  PE15   ------> FMC_D12
  PH9   ------> FMC_D17
  PH12   ------> FMC_D20
  PF11   ------> FMC_SDNRAS
  PG0   ------> FMC_A10
  PE8   ------> FMC_D5
  PE13   ------> FMC_D10
  PH6   ------> FMC_SDNE1
  PH8   ------> FMC_D16
  PD10   ------> FMC_D15
  PD9   ------> FMC_D14
  PG1   ------> FMC_A11
  PE7   ------> FMC_D4
  PE14   ------> FMC_D11
  PH7   ------> FMC_SDCKE1
  PD8   ------> FMC_D13
  */

  HAL_GPIO_DeInit(GPIOI, D28_Pin|FMC_NBL3_Pin|FMC_NBL2_Pin|D25_Pin
                          |D24_Pin|D29_Pin|D26_Pin|D27__IS42S32800G_DQ27_Pin
                          |D30_Pin|D31_Pin);

  HAL_GPIO_DeInit(GPIOE, FMC_NBL1_Pin|FMC_NBL0_Pin|D7_Pin|D6_Pin
                          |D8_Pin|D9_Pin|D12_Pin|D5_Pin
                          |D10_Pin|D4_Pin|D11_Pin);

  HAL_GPIO_DeInit(GPIOH, D23_Pin|D22_Pin|D21_Pin|SDNWE_Pin
                          |D18_Pin|D19_Pin|D17_Pin|D20_Pin
                          |SDNE1_Pin|D16_Pin|SDCKE1_Pin);

  HAL_GPIO_DeInit(GPIOG, SDNCAS_Pin|SDCLK_Pin|GPIO_PIN_5|GPIO_PIN_4
                          |GPIO_PIN_2|A10_Pin|A11_Pin);

  HAL_GPIO_DeInit(GPIOD, D2_Pin|D3_Pin|D1_Pin|D0_Pin
                          |D15_Pin|D14_Pin|D13_Pin);

  HAL_GPIO_DeInit(GPIOF, A2_Pin|A1_Pin|A0_Pin|A3_Pin
                          |A5_Pin|A4_Pin|A7_Pin|A8_Pin
                          |A6_Pin|A9_Pin|SNDRAS_Pin);

  /* USER CODE BEGIN FMC_MspDeInit 1 */

  /* USER CODE END FMC_MspDeInit 1 */
}

void HAL_SDRAM_MspDeInit(SDRAM_HandleTypeDef* sdramHandle){
  /* USER CODE BEGIN SDRAM_MspDeInit 0 */

  /* USER CODE END SDRAM_MspDeInit 0 */
  HAL_FMC_MspDeInit();
  /* USER CODE BEGIN SDRAM_MspDeInit 1 */

  /* USER CODE END SDRAM_MspDeInit 1 */
}
/**
  * @}
  */

/**
  * @}
  */
