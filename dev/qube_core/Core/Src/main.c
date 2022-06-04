/* USER CODE BEGIN Header */
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

/* Private includes ----------------------------------------------------------*/
/* USER CODE BEGIN Includes */

/* USER CODE END Includes */

/* Private typedef -----------------------------------------------------------*/
/* USER CODE BEGIN PTD */

/* USER CODE END PTD */

/* Private define ------------------------------------------------------------*/
/* USER CODE BEGIN PD */
/* USER CODE END PD */

/* Private macro -------------------------------------------------------------*/
/* USER CODE BEGIN PM */

/* USER CODE END PM */

/* Private variables ---------------------------------------------------------*/
I2C_HandleTypeDef hi2c1;
I2C_HandleTypeDef hi2c2;

TIM_HandleTypeDef htim2;

UART_HandleTypeDef huart1;

// Define color palette
const RGB_LED LED_RED     = {.R = 0xFF, .G = 0x00, .B = 0x00};
const RGB_LED LED_GREEN   = {.R = 0x5B, .G = 0xE9, .B = 0x4B};
const RGB_LED LED_BLUE    = {.R = 0x46, .G = 0xD0, .B = 0xEE};
const RGB_LED LED_YELLOW  = {.R = 0xEE, .G = 0xFA, .B = 0x33};
const RGB_LED LED_ORANGE  = {.R = 0xFA, .G = 0xAC, .B = 0x33};
const RGB_LED LED_WHITE   = {.R = 0xFF, .G = 0xFF, .B = 0xFF};
const RGB_LED LED_VIOLET  = {.R = 0xA0, .G = 0x12, .B = 0xEC};
const RGB_LED LED_PINK    = {.R = 0xEC, .G = 0x12, .B = 0xE9};

/* Private function prototypes -----------------------------------------------*/
void SystemClock_Config(void);
static void MX_GPIO_Init(void);
static void MX_I2C1_Init(void);
static void MX_I2C2_Init(void);
static void MX_USART1_UART_Init(void);
static void MX_TIM2_Init(void);

volatile uint8_t timer_IT = 0;


int main(void)
{

  uint8_t i             = 0;
  uint8_t PB_array[6]   = {0};
  uint8_t PB_array_z[6] = {0};

  RGB_LED LED_array[9] =
  {
    {.R = 0x00, .G = 0x00, .B = 0x00},
    {.R = 0x00, .G = 0x00, .B = 0x00},
    {.R = 0x00, .G = 0x00, .B = 0x00},
    {.R = 0x00, .G = 0x00, .B = 0x00},
    {.R = 0x00, .G = 0x00, .B = 0x00},
    {.R = 0x00, .G = 0x00, .B = 0x00},
    {.R = 0x00, .G = 0x00, .B = 0x00},
    {.R = 0x00, .G = 0x00, .B = 0x00},
    {.R = 0x00, .G = 0x00, .B = 0x00}
  };


  // 0    = green
  // 64   = red
  // 128  = blue
  uint8_t LED_phase[9] = {0,0,0,64,64,64,128,128,128};

  enum PB_FSM{WAIT_1, WAIT_2, MOVE, WAIT_RELEASE};

  // -----------------
  // MCU configuration
  // -----------------
  // Reset of all peripherals, Initializes the Flash interface and the Systick.
  HAL_Init();

  // Configure the system clock
  SystemClock_Config();

  // Initialize all configured peripherals
  MX_GPIO_Init();
  MX_I2C1_Init();
  MX_I2C2_Init();
  MX_USART1_UART_Init();
  MX_TIM2_Init();

  // Start the timer
  HAL_TIM_Base_Start_IT(&htim2);

  // Initialise the LED panel
  HAL_Delay(1);
  LEDs_init();
  
  // --------------------------------------------------------------------------
  // Main loop
  // --------------------------------------------------------------------------
  while (1)
  {

    if (timer_IT)
    {
      timer_IT = 0;

      // Sample the push button array
      PB_array[0] = 0;
      PB_array[0] |= (HAL_GPIO_ReadPin(PB_left_GPIO_Port, PB_left_Pin) == 0 ? 0x01 : 0x00);
      PB_array[0] |= (HAL_GPIO_ReadPin(PB_right_GPIO_Port, PB_right_Pin) == 0 ? 0x02 : 0x00);
      PB_array[0] |= (HAL_GPIO_ReadPin(PB_up_GPIO_Port, PB_up_Pin) == 0 ? 0x04 : 0x00);
      PB_array[0] |= (HAL_GPIO_ReadPin(PB_down_GPIO_Port, PB_down_Pin) == 0 ? 0x08 : 0x00);

      // Loop on the LEDs and update their state variable individually
      for (i = 0; i < 9; i++)
      {
        if (LED_phase[i] < 64)
        {
          LED_array[i].R = LED_phase[i];
          LED_array[i].G = 63-LED_phase[i];
          LED_array[i].B = 0;
        }
        else if ((LED_phase[i] >= 64) && (LED_phase[i] < 128))
        {
          LED_array[i].R = 63-(LED_phase[i]-64);
          LED_array[i].G = 0;
          LED_array[i].B = (LED_phase[i]-64);
        }
        else
        {
          LED_array[i].R = 0;
          LED_array[i].G = (LED_phase[i]-128);
          LED_array[i].B = 63-(LED_phase[i]-128);
        }

        // Detect overflow on the counter
        if ((LED_phase[i] == 191))
        {
          LED_phase[i] = 0;
        }
        else
        {
          //LED_phase[i]++;
        }
      }

      // "Move" the LEDs according to the button pressed
      rubiks_move(LED_phase, PB_array[0] & (~PB_array_z[0]));
      
      // Update the button state
      PB_array_z[0] = PB_array[0];

      // Show that!
      LEDs_update(LED_array);
    }

  }

}

/**
  * @brief System Clock Configuration
  * @retval None
  */
void SystemClock_Config(void)
{
  RCC_OscInitTypeDef RCC_OscInitStruct = {0};
  RCC_ClkInitTypeDef RCC_ClkInitStruct = {0};
  RCC_PeriphCLKInitTypeDef PeriphClkInit = {0};

  /** Configure the main internal regulator output voltage
  */
  HAL_PWREx_ControlVoltageScaling(PWR_REGULATOR_VOLTAGE_SCALE1_BOOST);
  /** Initializes the RCC Oscillators according to the specified parameters
  * in the RCC_OscInitTypeDef structure.
  */
  RCC_OscInitStruct.OscillatorType = RCC_OSCILLATORTYPE_HSI;
  RCC_OscInitStruct.HSIState = RCC_HSI_ON;
  RCC_OscInitStruct.HSICalibrationValue = RCC_HSICALIBRATION_DEFAULT;
  RCC_OscInitStruct.PLL.PLLState = RCC_PLL_ON;
  RCC_OscInitStruct.PLL.PLLSource = RCC_PLLSOURCE_HSI;
  RCC_OscInitStruct.PLL.PLLM = RCC_PLLM_DIV1;
  RCC_OscInitStruct.PLL.PLLN = 20;
  RCC_OscInitStruct.PLL.PLLP = RCC_PLLP_DIV2;
  RCC_OscInitStruct.PLL.PLLQ = RCC_PLLQ_DIV2;
  RCC_OscInitStruct.PLL.PLLR = RCC_PLLR_DIV2;
  if (HAL_RCC_OscConfig(&RCC_OscInitStruct) != HAL_OK)
  {
    Error_Handler();
  }
  /** Initializes the CPU, AHB and APB buses clocks
  */
  RCC_ClkInitStruct.ClockType = RCC_CLOCKTYPE_HCLK|RCC_CLOCKTYPE_SYSCLK
                              |RCC_CLOCKTYPE_PCLK1|RCC_CLOCKTYPE_PCLK2;
  RCC_ClkInitStruct.SYSCLKSource = RCC_SYSCLKSOURCE_PLLCLK;
  RCC_ClkInitStruct.AHBCLKDivider = RCC_SYSCLK_DIV1;
  RCC_ClkInitStruct.APB1CLKDivider = RCC_HCLK_DIV1;
  RCC_ClkInitStruct.APB2CLKDivider = RCC_HCLK_DIV1;

  if (HAL_RCC_ClockConfig(&RCC_ClkInitStruct, FLASH_LATENCY_4) != HAL_OK)
  {
    Error_Handler();
  }
  /** Initializes the peripherals clocks
  */
  PeriphClkInit.PeriphClockSelection = RCC_PERIPHCLK_USART1|RCC_PERIPHCLK_I2C1
                              |RCC_PERIPHCLK_I2C2;
  PeriphClkInit.Usart1ClockSelection = RCC_USART1CLKSOURCE_PCLK2;
  PeriphClkInit.I2c1ClockSelection = RCC_I2C1CLKSOURCE_PCLK1;
  PeriphClkInit.I2c2ClockSelection = RCC_I2C2CLKSOURCE_PCLK1;
  if (HAL_RCCEx_PeriphCLKConfig(&PeriphClkInit) != HAL_OK)
  {
    Error_Handler();
  }
}

/**
  * @brief I2C1 Initialization Function
  * @param None
  * @retval None
  */
static void MX_I2C1_Init(void)
{

  /* USER CODE BEGIN I2C1_Init 0 */

  /* USER CODE END I2C1_Init 0 */

  /* USER CODE BEGIN I2C1_Init 1 */

  /* USER CODE END I2C1_Init 1 */
  hi2c1.Instance = I2C1;
  hi2c1.Init.Timing = 0x00F07BFF;
  hi2c1.Init.OwnAddress1 = 0;
  hi2c1.Init.AddressingMode = I2C_ADDRESSINGMODE_7BIT;
  hi2c1.Init.DualAddressMode = I2C_DUALADDRESS_DISABLE;
  hi2c1.Init.OwnAddress2 = 0;
  hi2c1.Init.OwnAddress2Masks = I2C_OA2_NOMASK;
  hi2c1.Init.GeneralCallMode = I2C_GENERALCALL_DISABLE;
  hi2c1.Init.NoStretchMode = I2C_NOSTRETCH_DISABLE;
  if (HAL_I2C_Init(&hi2c1) != HAL_OK)
  {
    Error_Handler();
  }
  /** Configure Analogue filter
  */
  if (HAL_I2CEx_ConfigAnalogFilter(&hi2c1, I2C_ANALOGFILTER_ENABLE) != HAL_OK)
  {
    Error_Handler();
  }
  /** Configure Digital filter
  */
  if (HAL_I2CEx_ConfigDigitalFilter(&hi2c1, 0) != HAL_OK)
  {
    Error_Handler();
  }
  /* USER CODE BEGIN I2C1_Init 2 */

  /* USER CODE END I2C1_Init 2 */

}

/**
  * @brief I2C2 Initialization Function
  * @param None
  * @retval None
  */
static void MX_I2C2_Init(void)
{

  /* USER CODE BEGIN I2C2_Init 0 */

  /* USER CODE END I2C2_Init 0 */

  /* USER CODE BEGIN I2C2_Init 1 */

  /* USER CODE END I2C2_Init 1 */
  hi2c2.Instance = I2C2;
  hi2c2.Init.Timing = 0x00F07BFF;
  hi2c2.Init.OwnAddress1 = 0;
  hi2c2.Init.AddressingMode = I2C_ADDRESSINGMODE_7BIT;
  hi2c2.Init.DualAddressMode = I2C_DUALADDRESS_DISABLE;
  hi2c2.Init.OwnAddress2 = 0;
  hi2c2.Init.OwnAddress2Masks = I2C_OA2_NOMASK;
  hi2c2.Init.GeneralCallMode = I2C_GENERALCALL_DISABLE;
  hi2c2.Init.NoStretchMode = I2C_NOSTRETCH_DISABLE;
  if (HAL_I2C_Init(&hi2c2) != HAL_OK)
  {
    Error_Handler();
  }
  /** Configure Analogue filter
  */
  if (HAL_I2CEx_ConfigAnalogFilter(&hi2c2, I2C_ANALOGFILTER_ENABLE) != HAL_OK)
  {
    Error_Handler();
  }
  /** Configure Digital filter
  */
  if (HAL_I2CEx_ConfigDigitalFilter(&hi2c2, 0) != HAL_OK)
  {
    Error_Handler();
  }
  /* USER CODE BEGIN I2C2_Init 2 */

  /* USER CODE END I2C2_Init 2 */

}

// ----------------------------------------------------------------------------
// Timer 2 initialisation 
// ----------------------------------------------------------------------------
static void MX_TIM2_Init(void)
{

  TIM_ClockConfigTypeDef sClockSourceConfig = {0};
  TIM_MasterConfigTypeDef sMasterConfig = {0};


  htim2.Instance = TIM2;
  htim2.Init.Prescaler = 160-1;
  htim2.Init.CounterMode = TIM_COUNTERMODE_UP;
  htim2.Init.Period = 10000-1;
  htim2.Init.ClockDivision = TIM_CLOCKDIVISION_DIV1;
  htim2.Init.AutoReloadPreload = TIM_AUTORELOAD_PRELOAD_DISABLE;
  if (HAL_TIM_Base_Init(&htim2) != HAL_OK)
  {
    Error_Handler();
  }
  sClockSourceConfig.ClockSource = TIM_CLOCKSOURCE_INTERNAL;
  if (HAL_TIM_ConfigClockSource(&htim2, &sClockSourceConfig) != HAL_OK)
  {
    Error_Handler();
  }
  sMasterConfig.MasterOutputTrigger = TIM_TRGO_RESET;
  sMasterConfig.MasterSlaveMode = TIM_MASTERSLAVEMODE_DISABLE;
  if (HAL_TIMEx_MasterConfigSynchronization(&htim2, &sMasterConfig) != HAL_OK)
  {
    Error_Handler();
  }
}

// ----------------------------------------------------------------------------
// Timer 2 callback function  
// ----------------------------------------------------------------------------
void HAL_TIM_PeriodElapsedCallback(TIM_HandleTypeDef *htim)
{
  if (htim == &htim2)
  {
    timer_IT = 1;
  }
}

/**
  * @brief USART1 Initialization Function
  * @param None
  * @retval None
  */
static void MX_USART1_UART_Init(void)
{
  huart1.Instance = USART1;
  huart1.Init.BaudRate = 115200;
  huart1.Init.WordLength = UART_WORDLENGTH_8B;
  huart1.Init.StopBits = UART_STOPBITS_1;
  huart1.Init.Parity = UART_PARITY_NONE;
  huart1.Init.Mode = UART_MODE_TX_RX;
  huart1.Init.HwFlowCtl = UART_HWCONTROL_NONE;
  huart1.Init.OverSampling = UART_OVERSAMPLING_16;
  huart1.Init.OneBitSampling = UART_ONE_BIT_SAMPLE_DISABLE;
  huart1.Init.ClockPrescaler = UART_PRESCALER_DIV1;
  huart1.AdvancedInit.AdvFeatureInit = UART_ADVFEATURE_NO_INIT;

  if (HAL_UART_Init(&huart1) != HAL_OK)
  {
    Error_Handler();
  }

  if (HAL_UARTEx_SetTxFifoThreshold(&huart1, UART_TXFIFO_THRESHOLD_1_8) != HAL_OK)
  {
    Error_Handler();
  }

  if (HAL_UARTEx_SetRxFifoThreshold(&huart1, UART_RXFIFO_THRESHOLD_1_8) != HAL_OK)
  {
    Error_Handler();
  }

  if (HAL_UARTEx_DisableFifoMode(&huart1) != HAL_OK)
  {
    Error_Handler();
  }
}

/**
  * @brief GPIO Initialization Function
  * @param None
  * @retval None
  */
static void MX_GPIO_Init(void)
{
  GPIO_InitTypeDef GPIO_InitStruct = {0};

  /* GPIO Ports Clock Enable */
  __HAL_RCC_GPIOF_CLK_ENABLE();
  __HAL_RCC_GPIOA_CLK_ENABLE();
  __HAL_RCC_GPIOB_CLK_ENABLE();

  /*Configure GPIO pin Output Level */
  HAL_GPIO_WritePin(GPIOA, DRIVER_0_RESET_Pin|DRIVER_1_RESET_Pin, GPIO_PIN_RESET);

  /*Configure GPIO pin Output Level */
  HAL_GPIO_WritePin(GPIOA, DRIVER_0_OE_Pin|DRIVER_1_OE_Pin, GPIO_PIN_SET);

  /*Configure GPIO pin Output Level */
  HAL_GPIO_WritePin(GPIOB, GPIO_PIN_8, GPIO_PIN_RESET);

  /*Configure GPIO pins : DRIVER_0_RESET_Pin DRIVER_1_RESET_Pin DRIVER_0_OE_Pin DRIVER_1_OE_Pin */
  GPIO_InitStruct.Pin = DRIVER_0_RESET_Pin|DRIVER_1_RESET_Pin|DRIVER_0_OE_Pin|DRIVER_1_OE_Pin;
  GPIO_InitStruct.Mode = GPIO_MODE_OUTPUT_PP;
  GPIO_InitStruct.Pull = GPIO_NOPULL;
  GPIO_InitStruct.Speed = GPIO_SPEED_FREQ_LOW;
  HAL_GPIO_Init(GPIOA, &GPIO_InitStruct);

  /*Configure GPIO pins : BOARD_ID_0_Pin PB_right_Pin BOARD_ID_1_Pin PB_down_Pin
                           PB_left_Pin */
  GPIO_InitStruct.Pin = BOARD_ID_0_Pin|PB_right_Pin|BOARD_ID_1_Pin|PB_down_Pin
                          |PB_left_Pin;
  GPIO_InitStruct.Mode = GPIO_MODE_INPUT;
  GPIO_InitStruct.Pull = GPIO_NOPULL;
  HAL_GPIO_Init(GPIOA, &GPIO_InitStruct);

  /*Configure GPIO pins : BOARD_ID_2_Pin PB_up_Pin */
  GPIO_InitStruct.Pin = BOARD_ID_2_Pin|PB_up_Pin;
  GPIO_InitStruct.Mode = GPIO_MODE_INPUT;
  GPIO_InitStruct.Pull = GPIO_NOPULL;
  HAL_GPIO_Init(GPIOB, &GPIO_InitStruct);

  /*Configure GPIO pin : PB8 */
  GPIO_InitStruct.Pin = GPIO_PIN_8;
  GPIO_InitStruct.Mode = GPIO_MODE_OUTPUT_PP;
  GPIO_InitStruct.Pull = GPIO_NOPULL;
  GPIO_InitStruct.Speed = GPIO_SPEED_FREQ_LOW;
  HAL_GPIO_Init(GPIOB, &GPIO_InitStruct);
}

// ----------------------------------------------------------------------------
// LED panel initialisation 
// ----------------------------------------------------------------------------
void LEDs_init()
{
  uint8_t I2C_buff[17];
  HAL_StatusTypeDef I2C_stat;
  uint8_t LED = 0;
  uint8_t i = 0;

  // Release reset
  HAL_GPIO_WritePin(GPIOA, DRIVER_0_RESET_Pin | DRIVER_1_RESET_Pin, GPIO_PIN_SET);
  HAL_GPIO_WritePin(GPIOA, DRIVER_0_OE_Pin | DRIVER_1_OE_Pin, GPIO_PIN_RESET);

  // Initialise configuration
  I2C_buff[0] = REG_MODE1;
  I2C_buff[1] = 0x00;
  I2C_stat = HAL_I2C_Master_Transmit(&hi2c1, ADDR_LED_DRV_0, I2C_buff, 2, HAL_MAX_DELAY);
  I2C_stat = HAL_I2C_Master_Transmit(&hi2c1, ADDR_LED_DRV_1, I2C_buff, 2, HAL_MAX_DELAY);
  
  // Set LEDs control to PWM drive
  I2C_buff[0] = REG_LEDOUT0 | AUTO_INC_OPT;
  I2C_buff[1] = 0xAA;
  I2C_buff[2] = 0xAA;
  I2C_buff[3] = 0xAA;
  I2C_buff[4] = 0xAA;
  I2C_stat = HAL_I2C_Master_Transmit(&hi2c1, ADDR_LED_DRV_0, I2C_buff, 5, HAL_MAX_DELAY);
  I2C_stat = HAL_I2C_Master_Transmit(&hi2c1, ADDR_LED_DRV_1, I2C_buff, 5, HAL_MAX_DELAY);

  // GRPPWM
  // Not applicable with LEDOUT = 0b01

  // GRPFREQ
  // Not applicable with LEDOUT = 0b01

  // Individual LED PWM value
  I2C_buff[0] = REG_PWM0 | AUTO_INC_OPT;
  for (LED = 0; LED < 5; LED++)
  {
    I2C_buff[1 + (LED*3) + 0] = LED_PINK.R; // red channel
    I2C_buff[1 + (LED*3) + 1] = LED_PINK.B; // blue channel
    I2C_buff[1 + (LED*3) + 2] = LED_PINK.G; // green channel
  }
  I2C_stat = HAL_I2C_Master_Transmit(&hi2c1, ADDR_LED_DRV_0, I2C_buff, 15+1, HAL_MAX_DELAY);

  I2C_buff[0] = REG_PWM0 | AUTO_INC_OPT;
  for (LED = 0; LED < 4; LED++)
  {
    I2C_buff[1 + (LED*3) + 0] = LED_PINK.R; // red channel
    I2C_buff[1 + (LED*3) + 1] = LED_PINK.B; // blue channel
    I2C_buff[1 + (LED*3) + 2] = LED_PINK.G; // green channel
  }
  I2C_stat = HAL_I2C_Master_Transmit(&hi2c1, ADDR_LED_DRV_1, I2C_buff, 12+1, HAL_MAX_DELAY);

  // Individual LED reference current
  I2C_buff[0] = REG_IREF0 | AUTO_INC_OPT;
  for (i = 0; i < 16; i++)
  {
    I2C_buff[i+1] = 0x08;
  }
  I2C_stat = HAL_I2C_Master_Transmit(&hi2c1, ADDR_LED_DRV_0, I2C_buff, 17, HAL_MAX_DELAY);
  I2C_stat = HAL_I2C_Master_Transmit(&hi2c1, ADDR_LED_DRV_1, I2C_buff, 17, HAL_MAX_DELAY);
}

// ----------------------------------------------------------------------------
// LED panel update function
// ----------------------------------------------------------------------------
void LEDs_update(RGB_LED *ptr_RGB_LED_array)
{
  uint8_t LED_drv_0_buff[17];
  uint8_t LED_drv_1_buff[17];

  // --------------------------------------------------------------------------
  // Fill in the buffer for the first LED driver
  // --------------------------------------------------------------------------
  LED_drv_0_buff[0] = REG_PWM0 | AUTO_INC_OPT;

  LED_drv_0_buff[1] = ptr_RGB_LED_array[6].R;
  LED_drv_0_buff[2] = ptr_RGB_LED_array[6].B;
  LED_drv_0_buff[3] = ptr_RGB_LED_array[6].G;

  LED_drv_0_buff[4] = ptr_RGB_LED_array[4].R;
  LED_drv_0_buff[5] = ptr_RGB_LED_array[4].B;
  LED_drv_0_buff[6] = ptr_RGB_LED_array[4].G;

  LED_drv_0_buff[7] = ptr_RGB_LED_array[1].R;
  LED_drv_0_buff[8] = ptr_RGB_LED_array[1].B;
  LED_drv_0_buff[9] = ptr_RGB_LED_array[1].G;

  LED_drv_0_buff[10] = ptr_RGB_LED_array[3].R;
  LED_drv_0_buff[11] = ptr_RGB_LED_array[3].B;
  LED_drv_0_buff[12] = ptr_RGB_LED_array[3].G;

  LED_drv_0_buff[13] = ptr_RGB_LED_array[0].R;
  LED_drv_0_buff[14] = ptr_RGB_LED_array[0].B;
  LED_drv_0_buff[15] = ptr_RGB_LED_array[0].G;

  // Last LED is not connected.
  LED_drv_0_buff[16] = 0x00;

  // --------------------------------------------------------------------------
  // Fill in the buffer for the second LED driver
  // --------------------------------------------------------------------------
  LED_drv_1_buff[0] = REG_PWM0 | AUTO_INC_OPT;

  LED_drv_1_buff[1] = ptr_RGB_LED_array[2].R;
  LED_drv_1_buff[2] = ptr_RGB_LED_array[2].B;
  LED_drv_1_buff[3] = ptr_RGB_LED_array[2].G;

  LED_drv_1_buff[4] = ptr_RGB_LED_array[7].R;
  LED_drv_1_buff[5] = ptr_RGB_LED_array[7].B;
  LED_drv_1_buff[6] = ptr_RGB_LED_array[7].G;

  LED_drv_1_buff[7] = ptr_RGB_LED_array[5].R;
  LED_drv_1_buff[8] = ptr_RGB_LED_array[5].B;
  LED_drv_1_buff[9] = ptr_RGB_LED_array[5].G;

  LED_drv_1_buff[10] = ptr_RGB_LED_array[8].R;
  LED_drv_1_buff[11] = ptr_RGB_LED_array[8].B;
  LED_drv_1_buff[12] = ptr_RGB_LED_array[8].G;

  // Last LEDs are not connected.
  LED_drv_1_buff[13] = 0x00;
  LED_drv_1_buff[14] = 0x00;
  LED_drv_1_buff[15] = 0x00;
  LED_drv_1_buff[16] = 0x00;

  HAL_I2C_Master_Transmit(&hi2c1, ADDR_LED_DRV_0, LED_drv_0_buff, 17, HAL_MAX_DELAY);
  HAL_I2C_Master_Transmit(&hi2c1, ADDR_LED_DRV_1, LED_drv_1_buff, 17, HAL_MAX_DELAY);
}

// ----------------------------------------------------------------------------
// Rubik's cube move function
// ----------------------------------------------------------------------------
void rubiks_move(uint8_t *LED_phase, uint8_t PB_move)
{

  uint8_t LED_phase_tmp;

  if (PB_move & PB_MOVE_LEFT)
  {
    LED_phase_tmp = LED_phase[1];
    LED_phase[1] = LED_phase[4];
    LED_phase[4] = LED_phase[7];
    LED_phase[7] = LED_phase_tmp;
  }
  else if (PB_move & PB_MOVE_RIGHT)
  {
    LED_phase_tmp = LED_phase[7];
    LED_phase[7] = LED_phase[4];
    LED_phase[4] = LED_phase[1];
    LED_phase[1] = LED_phase_tmp;
  }
  else if (PB_move & PB_MOVE_UP)
  {
    LED_phase_tmp = LED_phase[3];
    LED_phase[3] = LED_phase[4];
    LED_phase[4] = LED_phase[5];
    LED_phase[5] = LED_phase_tmp;
  }
  else if (PB_move & PB_MOVE_DOWN)
  {
    LED_phase_tmp = LED_phase[5];
    LED_phase[5] = LED_phase[4];
    LED_phase[4] = LED_phase[3];
    LED_phase[3] = LED_phase_tmp;
  }
  else
  {

  }
}

// ----------------------------------------------------------------------------
// Error handler
// ----------------------------------------------------------------------------
void Error_Handler(void)
{
  /* USER CODE BEGIN Error_Handler_Debug */
  /* User can add his own implementation to report the HAL error return state */

  /* USER CODE END Error_Handler_Debug */
}

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

// End of file.
