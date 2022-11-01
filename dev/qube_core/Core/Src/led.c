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
const rgbLedType RED_LED     = {.r = 0xFF, .g = 0x00, .b = 0x00};
const rgbLedType GREEN_LED   = {.r = 0x00, .g = 0xFF, .b = 0x00};
const rgbLedType BLUE_LED    = {.r = 0x00, .g = 0x00, .b = 0xFF};
const rgbLedType YELLOW_LED  = {.r = 0xFF, .g = 0x55, .b = 0x00};
const rgbLedType ORANGE_LED  = {.r = 0xFF, .g = 0x20, .b = 0x00};
const rgbLedType WHITE_LED   = {.r = 0xFF, .g = 0xFF, .b = 0x70};
const rgbLedType VIOLET_LED  = {.r = 0xA0, .g = 0x12, .b = 0xCC};
const rgbLedType PINK_LED    = {.r = 0xEC, .g = 0x12, .b = 0xE9};



// ----------------------------------------------------------------------------
// LED drivers initialisation 
// ----------------------------------------------------------------------------
// Description:
// Initialise the 2 LED drivers on the board.
// - release hardware reset
// - set to PWM mode
//
// Example: 
// ledInitDriver();
// 
// TODO: 
// - Rename and remove the LED value init (redundant with LEDs_update)
// - Make use of the I2C LLD return value?
// 
void ledInitDriver(void)
{
  const uint8_t nBytes = 17;
  uint8_t I2C_buff[nBytes];
  uint8_t LED = 0;
  uint8_t i = 0;
  HAL_StatusTypeDef I2C_stat;

  // Release hardware reset
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

  // // Individual LED PWM value
  // I2C_buff[0] = REG_PWM0 | AUTO_INC_OPT;
  // for (LED = 0; LED < 5; LED++)
  // {
  //   I2C_buff[1 + (LED*3) + 0] = RED_LED.r; // red channel
  //   I2C_buff[1 + (LED*3) + 1] = RED_LED.b; // blue channel
  //   I2C_buff[1 + (LED*3) + 2] = RED_LED.g; // green channel
  // }
  // I2C_stat = HAL_I2C_Master_Transmit(&hi2c1, ADDR_LED_DRV_0, I2C_buff, 15+1, HAL_MAX_DELAY);

  // I2C_buff[0] = REG_PWM0 | AUTO_INC_OPT;
  // for (LED = 0; LED < 4; LED++)
  // {
  //   I2C_buff[1 + (LED*3) + 0] = RED_LED.r; // red channel
  //   I2C_buff[1 + (LED*3) + 1] = RED_LED.b; // blue channel
  //   I2C_buff[1 + (LED*3) + 2] = RED_LED.g; // green channel
  // }
  // I2C_stat = HAL_I2C_Master_Transmit(&hi2c1, ADDR_LED_DRV_1, I2C_buff, 12+1, HAL_MAX_DELAY);

  // Individual LED reference current (global brightness)
  I2C_buff[0] = REG_IREF0 | AUTO_INC_OPT;
  for (i = 0; i < 16; i++)
  {
    I2C_buff[i+1] = 0x10;
  }
  I2C_stat = HAL_I2C_Master_Transmit(&hi2c1, ADDR_LED_DRV_0, I2C_buff, nBytes, HAL_MAX_DELAY);
  I2C_stat = HAL_I2C_Master_Transmit(&hi2c1, ADDR_LED_DRV_1, I2C_buff, nBytes, HAL_MAX_DELAY);
}



// ----------------------------------------------------------------------------
// LED panel update function
// ----------------------------------------------------------------------------
// Description:
// Update the LEDs display on face <face> with the content of the cube 
// description array <cube> to the LED drivers.
//
// Example: 
// ledUpdateFace(cube, CUBE_FACE_F);
// 
// LED ordering on the PCB: 
// +-------+-------+-------+
// | LED 0 | LED 3 | LED 6 |
// +-------+-------+-------+
// | LED 1 | LED 4 | LED 7 |
// +-------+-------+-------+
// | LED 2 | LED 5 | LED 8 |
// +-------+-------+-------+
//
// TODO:
// Attention; suivant le board ID, la correspondance tableau <-> LEDs peut être permutée (miroir ? rotation ?) 
// Tout dépend comment la carte a été orientée.
// Dans tous les cas, il faut être capable de l'indiquer.
// Ce sont des considérations bas niveau et les fonctions de rotation dans la librairie cube.c n'ont pas à s'en préoccuper
void ledUpdateFace(rgbLedType (*cube)[9], uint8_t face)
{
  const uint8_t nBytes = 17;
  uint8_t ledDriver0Buff[nBytes];
  uint8_t ledDriver1Buff[nBytes];

  // --------------------------------------------------------------------------
  // Build I2C message for the first LED driver
  // --------------------------------------------------------------------------
  ledDriver0Buff[0] = REG_PWM0 | AUTO_INC_OPT;

  ledDriver0Buff[1] = cube[face][6].r;
  ledDriver0Buff[2] = cube[face][6].b;
  ledDriver0Buff[3] = cube[face][6].g;

  ledDriver0Buff[4] = cube[face][4].r;
  ledDriver0Buff[5] = cube[face][4].b;
  ledDriver0Buff[6] = cube[face][4].g;

  ledDriver0Buff[7] = cube[face][1].r;
  ledDriver0Buff[8] = cube[face][1].b;
  ledDriver0Buff[9] = cube[face][1].g;

  ledDriver0Buff[10] = cube[face][3].r;
  ledDriver0Buff[11] = cube[face][3].b;
  ledDriver0Buff[12] = cube[face][3].g;

  ledDriver0Buff[13] = cube[face][0].r;
  ledDriver0Buff[14] = cube[face][0].b;
  ledDriver0Buff[15] = cube[face][0].g;

  // Last output of the first driver is not connected.
  ledDriver0Buff[16] = 0x00;

  // --------------------------------------------------------------------------
  // Build I2C message for the second LED driver
  // --------------------------------------------------------------------------
  ledDriver1Buff[0] = REG_PWM0 | AUTO_INC_OPT;

  ledDriver1Buff[1] = cube[face][2].r;
  ledDriver1Buff[2] = cube[face][2].b;
  ledDriver1Buff[3] = cube[face][2].g;

  ledDriver1Buff[4] = cube[face][7].r;
  ledDriver1Buff[5] = cube[face][7].b;
  ledDriver1Buff[6] = cube[face][7].g;

  ledDriver1Buff[7] = cube[face][5].r;
  ledDriver1Buff[8] = cube[face][5].b;
  ledDriver1Buff[9] = cube[face][5].g;

  ledDriver1Buff[10] = cube[face][8].r;
  ledDriver1Buff[11] = cube[face][8].b;
  ledDriver1Buff[12] = cube[face][8].g;

  // Last output of the second driver are not connected.
  ledDriver1Buff[13] = 0x00;
  ledDriver1Buff[14] = 0x00;
  ledDriver1Buff[15] = 0x00;
  ledDriver1Buff[16] = 0x00;

  // Commit to I2C
  if (face == CUBE_FACE_F)
  {
    HAL_I2C_Master_Transmit(&hi2c1, ADDR_LED_DRV_0, ledDriver0Buff, nBytes, HAL_MAX_DELAY);
    HAL_I2C_Master_Transmit(&hi2c1, ADDR_LED_DRV_1, ledDriver1Buff, nBytes, HAL_MAX_DELAY);
  }
}

