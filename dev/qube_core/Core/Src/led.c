// ============================================================================
// Module name    : led
// File name      : led.c
// Purpose        : LED driver
// Author         : Quentin Biache
// Creation date  : June 06th, 2022
// ============================================================================

// ============================================================================
// Description
// ============================================================================
// 

// ============================================================================
// Includes
// ============================================================================
#include "led.h"
#include "cpu.h"
#include "cube.h"

// Define color palette
const RGB_LED LED_RED     = {.R = 0xFF, .G = 0x00, .B = 0x00};
const RGB_LED LED_GREEN   = {.R = 0x00, .G = 0xFF, .B = 0x00};
const RGB_LED LED_BLUE    = {.R = 0x00, .G = 0x00, .B = 0xFF};
const RGB_LED LED_YELLOW  = {.R = 0xFF, .G = 0x55, .B = 0x00};
const RGB_LED LED_ORANGE  = {.R = 0xFF, .G = 0x20, .B = 0x00};
const RGB_LED LED_WHITE   = {.R = 0xFF, .G = 0xFF, .B = 0x70};
const RGB_LED LED_VIOLET  = {.R = 0xA0, .G = 0x12, .B = 0xCC};
const RGB_LED LED_PINK    = {.R = 0xEC, .G = 0x12, .B = 0xE9};

// ----------------------------------------------------------------------------
// LED panel initialisation 
// ----------------------------------------------------------------------------
// TODO: rename and remove the LED value init (redundant with LEDs_update)
void LEDs_init(void)
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
    I2C_buff[1 + (LED*3) + 0] = LED_RED.R; // red channel
    I2C_buff[1 + (LED*3) + 1] = LED_RED.B; // blue channel
    I2C_buff[1 + (LED*3) + 2] = LED_RED.G; // green channel
  }
  I2C_stat = HAL_I2C_Master_Transmit(&hi2c1, ADDR_LED_DRV_0, I2C_buff, 15+1, HAL_MAX_DELAY);

  I2C_buff[0] = REG_PWM0 | AUTO_INC_OPT;
  for (LED = 0; LED < 4; LED++)
  {
    I2C_buff[1 + (LED*3) + 0] = LED_RED.R; // red channel
    I2C_buff[1 + (LED*3) + 1] = LED_RED.B; // blue channel
    I2C_buff[1 + (LED*3) + 2] = LED_RED.G; // green channel
  }
  I2C_stat = HAL_I2C_Master_Transmit(&hi2c1, ADDR_LED_DRV_1, I2C_buff, 12+1, HAL_MAX_DELAY);

  // Individual LED reference current
  I2C_buff[0] = REG_IREF0 | AUTO_INC_OPT;
  for (i = 0; i < 16; i++)
  {
    I2C_buff[i+1] = 0x10;
  }
  I2C_stat = HAL_I2C_Master_Transmit(&hi2c1, ADDR_LED_DRV_0, I2C_buff, 17, HAL_MAX_DELAY);
  I2C_stat = HAL_I2C_Master_Transmit(&hi2c1, ADDR_LED_DRV_1, I2C_buff, 17, HAL_MAX_DELAY);
}

// ----------------------------------------------------------------------------
// LED panel update function
// ----------------------------------------------------------------------------
// LED ordering on the PCB: 
// +-------+-------+-------+
// | LED 0 | LED 3 | LED 6 |
// +-------+-------+-------+
// | LED 1 | LED 4 | LED 7 |
// +-------+-------+-------+
// | LED 2 | LED 5 | LED 8 |
// +-------+-------+-------+

// TODO:
// Attention; suivant le board ID, la correspondance tableau <-> LEDs peut être permutée (miroir ? rotation ?) 
// Tout dépend comment la carte a été orientée.
// Dans tous les cas, il faut être capable de l'indiquer.
// Ce sont des considérations bas niveau et les fonctions de rotation dans la librairie cube.c n'ont pas à s'en préoccuper
void LEDs_update(RGB_LED (*cube)[9], uint8_t face)
{
  uint8_t LED_drv_0_buff[17];
  uint8_t LED_drv_1_buff[17];

  // --------------------------------------------------------------------------
  // Build I2C message for the first LED driver
  // --------------------------------------------------------------------------
  LED_drv_0_buff[0] = REG_PWM0 | AUTO_INC_OPT;

  LED_drv_0_buff[1] = cube[face][6].R;
  LED_drv_0_buff[2] = cube[face][6].B;
  LED_drv_0_buff[3] = cube[face][6].G;

  LED_drv_0_buff[4] = cube[face][4].R;
  LED_drv_0_buff[5] = cube[face][4].B;
  LED_drv_0_buff[6] = cube[face][4].G;

  LED_drv_0_buff[7] = cube[face][1].R;
  LED_drv_0_buff[8] = cube[face][1].B;
  LED_drv_0_buff[9] = cube[face][1].G;

  LED_drv_0_buff[10] = cube[face][3].R;
  LED_drv_0_buff[11] = cube[face][3].B;
  LED_drv_0_buff[12] = cube[face][3].G;

  LED_drv_0_buff[13] = cube[face][0].R;
  LED_drv_0_buff[14] = cube[face][0].B;
  LED_drv_0_buff[15] = cube[face][0].G;

  // Last LED is not connected.
  LED_drv_0_buff[16] = 0x00;

  // --------------------------------------------------------------------------
  // Build I2C message for the second LED driver
  // --------------------------------------------------------------------------
  LED_drv_1_buff[0] = REG_PWM0 | AUTO_INC_OPT;

  LED_drv_1_buff[1] = cube[face][2].R;
  LED_drv_1_buff[2] = cube[face][2].B;
  LED_drv_1_buff[3] = cube[face][2].G;

  LED_drv_1_buff[4] = cube[face][7].R;
  LED_drv_1_buff[5] = cube[face][7].B;
  LED_drv_1_buff[6] = cube[face][7].G;

  LED_drv_1_buff[7] = cube[face][5].R;
  LED_drv_1_buff[8] = cube[face][5].B;
  LED_drv_1_buff[9] = cube[face][5].G;

  LED_drv_1_buff[10] = cube[face][8].R;
  LED_drv_1_buff[11] = cube[face][8].B;
  LED_drv_1_buff[12] = cube[face][8].G;

  // Last LEDs channels are not connected.
  LED_drv_1_buff[13] = 0x00;
  LED_drv_1_buff[14] = 0x00;
  LED_drv_1_buff[15] = 0x00;
  LED_drv_1_buff[16] = 0x00;

  // Commit to I2C
  if (face == CUBE_FACE_F)
  {
    HAL_I2C_Master_Transmit(&hi2c1, ADDR_LED_DRV_0, LED_drv_0_buff, 17, HAL_MAX_DELAY);
    HAL_I2C_Master_Transmit(&hi2c1, ADDR_LED_DRV_1, LED_drv_1_buff, 17, HAL_MAX_DELAY);
  }
}

