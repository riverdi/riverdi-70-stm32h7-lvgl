/* USER CODE BEGIN Header */
/**
  ******************************************************************************
  * @file    dsihost.c
  * @brief   This file provides code for the configuration
  *          of the DSIHOST instances.
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
#include "dsihost.h"

/* USER CODE BEGIN 0 */
void BSP_LCD_Reset(uint32_t Instance)
{
  GPIO_InitTypeDef gpio_init_structure;

  gpio_init_structure.Pin = LCD_DISP_Pin;
  gpio_init_structure.Mode = GPIO_MODE_OUTPUT_PP;
  gpio_init_structure.Pull = GPIO_NOPULL;
  gpio_init_structure.Speed = GPIO_SPEED_FREQ_LOW;
  HAL_GPIO_Init(LCD_DISP_GPIO_Port, &gpio_init_structure);

  /* activate XRES active low */
  HAL_GPIO_WritePin(LCD_DISP_GPIO_Port, LCD_DISP_Pin, GPIO_PIN_RESET);
  HAL_Delay(20);

  /* deactivate XRES */
  HAL_GPIO_WritePin(LCD_DISP_GPIO_Port, LCD_DISP_Pin, GPIO_PIN_SET);

  /* wait for 10ms after releasing XRES before sending commands */
  HAL_Delay(10);
}
/* USER CODE END 0 */

DSI_HandleTypeDef hdsi;

/* DSIHOST init function */

void MX_DSIHOST_DSI_Init(void)
{

  /* USER CODE BEGIN DSIHOST_Init 0 */

  /* USER CODE END DSIHOST_Init 0 */

  DSI_PLLInitTypeDef PLLInit = {0};
  DSI_HOST_TimeoutTypeDef HostTimeouts = {0};
  DSI_PHY_TimerTypeDef PhyTimings = {0};
  DSI_VidCfgTypeDef VidCfg = {0};

  /* USER CODE BEGIN DSIHOST_Init 1 */
  uint32_t LcdClock  = 41666;
  uint32_t laneByteClk_kHz = 62000;
  BSP_LCD_Reset(0);
  /* USER CODE END DSIHOST_Init 1 */
  hdsi.Instance = DSI;
  hdsi.Init.AutomaticClockLaneControl = DSI_AUTO_CLK_LANE_CTRL_DISABLE;
  hdsi.Init.TXEscapeCkdiv = 4;
  hdsi.Init.NumberOfLanes = DSI_TWO_DATA_LANES;
  PLLInit.PLLNDIV = 62;
  PLLInit.PLLIDF = DSI_PLL_IN_DIV1;
  PLLInit.PLLODF = DSI_PLL_OUT_DIV2;
  if (HAL_DSI_Init(&hdsi, &PLLInit) != HAL_OK)
  {
    Error_Handler();
  }
  HostTimeouts.TimeoutCkdiv = 1;
  HostTimeouts.HighSpeedTransmissionTimeout = 0;
  HostTimeouts.LowPowerReceptionTimeout = 0;
  HostTimeouts.HighSpeedReadTimeout = 0;
  HostTimeouts.LowPowerReadTimeout = 0;
  HostTimeouts.HighSpeedWriteTimeout = 0;
  HostTimeouts.HighSpeedWritePrespMode = DSI_HS_PM_DISABLE;
  HostTimeouts.LowPowerWriteTimeout = 0;
  HostTimeouts.BTATimeout = 0;
  if (HAL_DSI_ConfigHostTimeouts(&hdsi, &HostTimeouts) != HAL_OK)
  {
    Error_Handler();
  }
  PhyTimings.ClockLaneHS2LPTime = 28;
  PhyTimings.ClockLaneLP2HSTime = 32;
  PhyTimings.DataLaneHS2LPTime = 15;
  PhyTimings.DataLaneLP2HSTime = 24;
  PhyTimings.DataLaneMaxReadTime = 0;
  PhyTimings.StopWaitTime = 0;
  if (HAL_DSI_ConfigPhyTimer(&hdsi, &PhyTimings) != HAL_OK)
  {
    Error_Handler();
  }
  if (HAL_DSI_ConfigFlowControl(&hdsi, DSI_FLOW_CONTROL_BTA) != HAL_OK)
  {
    Error_Handler();
  }
  if (HAL_DSI_SetLowPowerRXFilter(&hdsi, 10000) != HAL_OK)
  {
    Error_Handler();
  }
  if (HAL_DSI_ConfigErrorMonitor(&hdsi, HAL_DSI_ERROR_NONE) != HAL_OK)
  {
    Error_Handler();
  }
  VidCfg.VirtualChannelID = 0;
  VidCfg.ColorCoding = DSI_RGB888;
  VidCfg.LooselyPacked = DSI_LOOSELY_PACKED_DISABLE;
  VidCfg.Mode = DSI_VID_MODE_BURST;
  VidCfg.PacketSize = 1024;
  VidCfg.NumberOfChunks = 0;
  VidCfg.NullPacketSize = 0;
  VidCfg.HSPolarity = DSI_HSYNC_ACTIVE_LOW;
  VidCfg.VSPolarity = DSI_VSYNC_ACTIVE_LOW;
  VidCfg.DEPolarity = DSI_DATA_ENABLE_ACTIVE_HIGH;
  VidCfg.HorizontalSyncActive = 70;
  VidCfg.HorizontalBackPorch = 193;
  VidCfg.HorizontalLine = 1708;
  VidCfg.VerticalSyncActive = 10;
  VidCfg.VerticalBackPorch = 23;
  VidCfg.VerticalFrontPorch = 12;
  VidCfg.VerticalActive = VACT;
  VidCfg.LPCommandEnable = DSI_LP_COMMAND_DISABLE;
  VidCfg.LPLargestPacketSize = 0;
  VidCfg.LPVACTLargestPacketSize = 0;
  VidCfg.LPHorizontalFrontPorchEnable = DSI_LP_HFP_ENABLE;
  VidCfg.LPHorizontalBackPorchEnable = DSI_LP_HBP_ENABLE;
  VidCfg.LPVerticalActiveEnable = DSI_LP_VACT_ENABLE;
  VidCfg.LPVerticalFrontPorchEnable = DSI_LP_VFP_ENABLE;
  VidCfg.LPVerticalBackPorchEnable = DSI_LP_VBP_ENABLE;
  VidCfg.LPVerticalSyncActiveEnable = DSI_LP_VSYNC_ENABLE;
  VidCfg.FrameBTAAcknowledgeEnable = DSI_FBTAA_DISABLE;
  if (HAL_DSI_ConfigVideoMode(&hdsi, &VidCfg) != HAL_OK)
  {
    Error_Handler();
  }
  if (HAL_DSI_SetGenericVCID(&hdsi, 0) != HAL_OK)
  {
    Error_Handler();
  }
  /* USER CODE BEGIN DSIHOST_Init 2 */
  VidCfg.VirtualChannelID = 0;
  VidCfg.ColorCoding = DSI_RGB888;
  VidCfg.LooselyPacked = DSI_LOOSELY_PACKED_DISABLE;
  VidCfg.Mode = DSI_VID_MODE_BURST;
  VidCfg.PacketSize = HACT;
  VidCfg.NumberOfChunks = 0;
  VidCfg.NullPacketSize = 0;
  VidCfg.HSPolarity = DSI_HSYNC_ACTIVE_LOW;
  VidCfg.VSPolarity = DSI_VSYNC_ACTIVE_LOW;
  VidCfg.DEPolarity = DSI_DATA_ENABLE_ACTIVE_HIGH;
  VidCfg.HorizontalSyncActive =   HSYNC * laneByteClk_kHz/LcdClock;
  VidCfg.HorizontalBackPorch = (uint32_t) (HBP * laneByteClk_kHz/LcdClock);
  VidCfg.HorizontalLine = (HACT + HSYNC + HBP + HFP) * laneByteClk_kHz/LcdClock;
  VidCfg.VerticalSyncActive = VSYNC;
  VidCfg.VerticalBackPorch = VBP;
  VidCfg.VerticalFrontPorch = VFP;
  VidCfg.VerticalActive = VACT;
  VidCfg.LPCommandEnable = DSI_LP_COMMAND_ENABLE;
  VidCfg.LPLargestPacketSize = 23;
  VidCfg.LPVACTLargestPacketSize = 0;
  VidCfg.LPHorizontalFrontPorchEnable = DSI_LP_HFP_ENABLE;
  VidCfg.LPHorizontalBackPorchEnable = DSI_LP_HBP_ENABLE;
  VidCfg.LPVerticalActiveEnable = DSI_LP_VACT_ENABLE;
  VidCfg.LPVerticalFrontPorchEnable = DSI_LP_VFP_ENABLE;
  VidCfg.LPVerticalBackPorchEnable = DSI_LP_VBP_ENABLE;
  VidCfg.LPVerticalSyncActiveEnable = DSI_LP_VSYNC_ENABLE;
  VidCfg.FrameBTAAcknowledgeEnable = DSI_FBTAA_DISABLE;
  if (HAL_DSI_ConfigVideoMode(&hdsi, &VidCfg) != HAL_OK)
  {
    Error_Handler();
  }
  if (HAL_DSI_SetGenericVCID(&hdsi, 0) != HAL_OK)
  {
    Error_Handler();
  }
  HAL_DSI_Start(&hdsi);
  HAL_DSI_ShortWrite(&hdsi, 0, DSI_DCS_SHORT_PKT_WRITE_P0, DSI_SOFT_RESET, 0x0);
  HAL_Delay(120);

  HAL_DSI_ShortWrite(&hdsi, 0, DSI_GEN_SHORT_PKT_WRITE_P2, 0x87, 0x5A);
  HAL_DSI_ShortWrite(&hdsi, 0, DSI_GEN_SHORT_PKT_WRITE_P2, 0xB0, 0x80);
  HAL_DSI_ShortWrite(&hdsi, 0, DSI_GEN_SHORT_PKT_WRITE_P2, 0xB2, 0x50);
  HAL_DSI_ShortWrite(&hdsi, 0, DSI_GEN_SHORT_PKT_WRITE_P2, 0x80, 0x4B);
  HAL_DSI_ShortWrite(&hdsi, 0, DSI_GEN_SHORT_PKT_WRITE_P2, 0x81, 0xFF);
  HAL_DSI_ShortWrite(&hdsi, 0, DSI_GEN_SHORT_PKT_WRITE_P2, 0x82, 0x1A);
  HAL_DSI_ShortWrite(&hdsi, 0, DSI_GEN_SHORT_PKT_WRITE_P2, 0x83, 0x88);
  HAL_DSI_ShortWrite(&hdsi, 0, DSI_GEN_SHORT_PKT_WRITE_P2, 0x84, 0x8F);
  HAL_DSI_ShortWrite(&hdsi, 0, DSI_GEN_SHORT_PKT_WRITE_P2, 0x85, 0x35);
  HAL_DSI_ShortWrite(&hdsi, 0, DSI_GEN_SHORT_PKT_WRITE_P2, 0x86, 0xB0);
  HAL_Delay(50);

  HAL_DSI_ShortWrite(&hdsi, 0, DSI_DCS_SHORT_PKT_WRITE_P0, DSI_EXIT_SLEEP_MODE, 0x0);
  HAL_Delay(120);

  HAL_DSI_ShortWrite(&hdsi, 0, DSI_DCS_SHORT_PKT_WRITE_P0, DSI_SET_DISPLAY_ON, 0x0);
  HAL_Delay(120);
  /* USER CODE END DSIHOST_Init 2 */
}

void HAL_DSI_MspInit(DSI_HandleTypeDef* dsiHandle)
{

  RCC_PeriphCLKInitTypeDef PeriphClkInitStruct = {0};
  if(dsiHandle->Instance==DSI)
  {
  /* USER CODE BEGIN DSI_MspInit 0 */

  /* USER CODE END DSI_MspInit 0 */

  /** Initializes the peripherals clock
  */
    PeriphClkInitStruct.PeriphClockSelection = RCC_PERIPHCLK_DSI;
    PeriphClkInitStruct.DsiClockSelection = RCC_DSICLKSOURCE_PHY;
    if (HAL_RCCEx_PeriphCLKConfig(&PeriphClkInitStruct) != HAL_OK)
    {
      Error_Handler();
    }

    /* DSI clock enable */
    __HAL_RCC_DSI_CLK_ENABLE();
  /* USER CODE BEGIN DSI_MspInit 1 */

  /* USER CODE END DSI_MspInit 1 */
  }
}

void HAL_DSI_MspDeInit(DSI_HandleTypeDef* dsiHandle)
{

  if(dsiHandle->Instance==DSI)
  {
  /* USER CODE BEGIN DSI_MspDeInit 0 */

  /* USER CODE END DSI_MspDeInit 0 */
    /* Peripheral clock disable */
    __HAL_RCC_DSI_CLK_DISABLE();
  /* USER CODE BEGIN DSI_MspDeInit 1 */

  /* USER CODE END DSI_MspDeInit 1 */
  }
}

/* USER CODE BEGIN 1 */

/* USER CODE END 1 */
