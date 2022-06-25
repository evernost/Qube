// ============================================================================
// Module name    : cpu
// File name      : cpu.h
// Purpose        : 
// Author         : Quentin Biache
// Creation date  : June 18th, 2022
// ============================================================================

// Define to prevent recursive inclusion --------------------------------------
#ifndef __CPU_H
#define __CPU_H

#ifdef __cplusplus
extern "C" {
#endif

// Includes -------------------------------------------------------------------
#include <stdint.h>

// Defines --------------------------------------------------------------------
#define PB_left_Pin GPIO_PIN_12
#define PB_right_Pin GPIO_PIN_5
#define PB_up_Pin GPIO_PIN_4
#define PB_down_Pin GPIO_PIN_7
#define PB_right_GPIO_Port GPIOA
#define PB_down_GPIO_Port GPIOA
#define PB_left_GPIO_Port GPIOA
#define PB_up_GPIO_Port GPIOB

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

// Types ----------------------------------------------------------------------

// Functions prototypes -------------------------------------------------------
void cpu_init(void);
uint8_t get_board_ID(void);
void Error_Handler(void);

#ifdef __cplusplus
}
#endif

#endif // __CPU_H



