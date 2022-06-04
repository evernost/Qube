/* USER CODE BEGIN Header */
/**
  ******************************************************************************
  * @file           : main.h
  * @brief          : Header for main.c file.
  *                   This file contains the common defines of the application.
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

/* Define to prevent recursive inclusion -------------------------------------*/
#ifndef __MAIN_H
#define __MAIN_H

#ifdef __cplusplus
extern "C" {
#endif

/* Includes ------------------------------------------------------------------*/
#include "stm32g4xx_hal.h"
#include "PCA9955B.h"

/* Private includes ----------------------------------------------------------*/
/* USER CODE BEGIN Includes */

/* USER CODE END Includes */

/* Exported types ------------------------------------------------------------*/
/* USER CODE BEGIN ET */

/* USER CODE END ET */

/* Exported constants --------------------------------------------------------*/
/* USER CODE BEGIN EC */

/* USER CODE END EC */

/* Exported macro ------------------------------------------------------------*/
/* USER CODE BEGIN EM */

/* USER CODE END EM */

/* Exported functions prototypes ---------------------------------------------*/
void Error_Handler(void);

void LEDs_init();
void LEDs_update(RGB_LED*);
void rubiks_move(uint8_t*, uint8_t);

/* Private defines -----------------------------------------------------------*/
#define PB_left_Pin GPIO_PIN_12
#define PB_right_Pin GPIO_PIN_5
#define PB_up_Pin GPIO_PIN_4
#define PB_down_Pin GPIO_PIN_7
#define PB_right_GPIO_Port GPIOA
#define PB_down_GPIO_Port GPIOA
#define PB_left_GPIO_Port GPIOA
#define PB_up_GPIO_Port GPIOB

#define PB_MOVE_LEFT  0x01
#define PB_MOVE_RIGHT 0x02
#define PB_MOVE_UP    0x04
#define PB_MOVE_DOWN  0x08

#define I2C2_COMM_SDA_Pin GPIO_PIN_0
#define I2C2_COMM_SDA_GPIO_Port GPIOF
#define I2C2_COMM_SCL_Pin GPIO_PIN_9
#define I2C2_COMM_SCL_GPIO_Port GPIOA
#define I2C1_LED_DRV_SCL_Pin GPIO_PIN_15
#define I2C1_LED_DRV_SCL_GPIO_Port GPIOA
#define I2C1_LED_DRV_SDA_Pin GPIO_PIN_7
#define I2C1_LED_DRV_SDA_GPIO_Port GPIOB

#define DRIVER_0_RESET_Pin GPIO_PIN_0
#define DRIVER_0_RESET_GPIO_Port GPIOA
#define DRIVER_1_RESET_Pin GPIO_PIN_1
#define DRIVER_1_RESET_GPIO_Port GPIOA
#define DRIVER_0_OE_Pin GPIO_PIN_2
#define DRIVER_0_OE_GPIO_Port GPIOA
#define DRIVER_1_OE_Pin GPIO_PIN_3
#define DRIVER_1_OE_GPIO_Port GPIOA

#define BOARD_ID_0_Pin GPIO_PIN_4
#define BOARD_ID_0_GPIO_Port GPIOA
#define BOARD_ID_1_Pin GPIO_PIN_6
#define BOARD_ID_1_GPIO_Port GPIOA
#define BOARD_ID_2_Pin GPIO_PIN_0
#define BOARD_ID_2_GPIO_Port GPIOB
/* USER CODE BEGIN Private defines */

/* USER CODE END Private defines */

#ifdef __cplusplus
}
#endif

#endif /* __MAIN_H */

/************************ (C) COPYRIGHT STMicroelectronics *****END OF FILE****/
