// ============================================================================
// Module name     : led
// File name       : led.h
// Purpose         : LED driver
// Author          : Quentin Biache
// Creation date   : June 06th, 2022
// ============================================================================

// Define to prevent recursive inclusion --------------------------------------
#ifndef __LED_H
#define __LED_H

#ifdef __cplusplus
extern "C" {
#endif

// Includes -------------------------------------------------------------------
#include <stdint.h>
#include "stm32g4xx_hal.h"

extern I2C_HandleTypeDef hi2c1; // defined in cpu.c

// Defines --------------------------------------------------------------------
#define ADDR_LED_DRV_0      0x2A
#define ADDR_LED_DRV_1      0x32

#define REG_MODE1           0x00
#define REG_MODE2           0x01

#define REG_LEDOUT0         0x02
#define REG_LEDOUT1         0x03
#define REG_LEDOUT2         0x04
#define REG_LEDOUT3         0x05

#define REG_GRPPWM          0x06

#define REG_GRPFREQ         0x07

#define REG_PWM0            0x08
#define REG_PWM1            0x09
#define REG_PWM2            0x0A
#define REG_PWM3            0x0B
#define REG_PWM4            0x0C
#define REG_PWM5            0x0D
#define REG_PWM6            0x0E
#define REG_PWM7            0x0F
#define REG_PWM8            0x10
#define REG_PWM9            0x11
#define REG_PWM10           0x12
#define REG_PWM11           0x13
#define REG_PWM12           0x14
#define REG_PWM13           0x15
#define REG_PWM14           0x16
#define REG_PWM15           0x17

#define REG_IREF0           0x18
#define REG_IREF1           0x19
#define REG_IREF2           0x1A
#define REG_IREF3           0x1B
#define REG_IREF4           0x1C
#define REG_IREF5           0x1D
#define REG_IREF6           0x1E
#define REG_IREF7           0x1F
#define REG_IREF8           0x20
#define REG_IREF9           0x21
#define REG_IREF10          0x22
#define REG_IREF11          0x23
#define REG_IREF12          0x24
#define REG_IREF13          0x25
#define REG_IREF14          0x26
#define REG_IREF15          0x27

#define REG_RAMP_RATE_GRP0  0x28
#define REG_STEP_TIME_GRP0  0x29
#define REG_HOLD_CNTL_GRP0  0x2A
#define REG_IREF_GRP0       0x2B

#define REG_RAMP_RATE_GRP1  0x2C
#define REG_STEP_TIME_GRP1  0x2D
#define REG_HOLD_CNTL_GRP1  0x2E
#define REG_IREF_GRP1       0x2F

#define REG_RAMP_RATE_GRP2  0x30
#define REG_STEP_TIME_GRP2  0x31
#define REG_HOLD_CNTL_GRP2  0x32
#define REG_IREF_GRP2       0x33

#define REG_RAMP_RATE_GRP3  0x34
#define REG_STEP_TIME_GRP3  0x35
#define REG_HOLD_CNTL_GRP3  0x36
#define REG_IREF_GRP3       0x37

#define REG_GRAD_MODE_SEL0  0x38
#define REG_GRAD_MODE_SEL1  0x39

#define REG_GRAD_GRP_SEL0   0x3A
#define REG_GRAD_GRP_SEL1   0x3B
#define REG_GRAD_GRP_SEL2   0x3C
#define REG_GRAD_GRP_SEL3   0x3D

#define REG_GRAD_CNTL       0x3E

#define REG_OFFSET          0x3F

#define REG_SUBADR1         0x40
#define REG_SUBADR2         0x41
#define REG_SUBADR3         0x42

#define REG_ALLCALLADR      0x43
#define REG_PWMALL          0x44
#define REG_IREFALL         0x45

#define REG_EFLAG0          0x46
#define REG_EFLAG1          0x47
#define REG_EFLAG2          0x48
#define REG_EFLAG3          0x49

#define AUTO_INC_OPT        (1 << 7)


// Types ----------------------------------------------------------------------
typedef struct RGB_LED
{
  uint8_t R;
  uint8_t G;
  uint8_t B;
} RGB_LED;


// Functions prototypes -------------------------------------------------------

// Public
void LEDs_init(void);
void LEDs_update(RGB_LED (*)[9], uint8_t);


// Private


#ifdef __cplusplus
}
#endif

#endif // __LED_H


