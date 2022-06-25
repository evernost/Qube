// ============================================================================
// Module name    : main 
// File name      : main.c
// Purpose        : Qube entry point
// Author         : Quentin Biache
// Creation date  : June 4th, 2022
// ============================================================================

// ============================================================================
// Description
// ============================================================================
// Qube application entry point.

// ============================================================================
// Includes
// ============================================================================
#include "main.h"
#include "cpu.h"
#include "led.h"
#include "push_button.h"
#include "cube.h"

// Private variables ----------------------------------------------------------

// Private function prototypes ------------------------------------------------


// ============================================================================
// Main code
// ============================================================================
int main(void)
{

  
  RGB_LED cube[6][9];
  uint8_t pb[6] = {0};
  uint8_t state = 0;


  // -----------------
  // MCU configuration
  // -----------------
  cpu_init();

  // Read the board ID
  uint8_t BOARD_ID = get_board_ID();

  // Initialise the LED panel
  HAL_Delay(1);
  LEDs_init();
  
  paint(cube[CUBE_FACE_F], LED_ORANGE);
  paint(cube[CUBE_FACE_B], LED_RED);
  paint(cube[CUBE_FACE_L], LED_BLUE);
  paint(cube[CUBE_FACE_R], LED_GREEN);
  paint(cube[CUBE_FACE_U], LED_PINK);
  paint(cube[CUBE_FACE_D], LED_YELLOW);
  LEDs_update(cube, CUBE_FACE_F);

  // --------------------------------------------------------------------------
  // Main loop
  // --------------------------------------------------------------------------
  
  // Use this to tune colors
  // cube[CUBE_FACE_F][0] = LED_RED;
  // cube[CUBE_FACE_F][1] = LED_GREEN;
  // cube[CUBE_FACE_F][2] = LED_BLUE;
  // cube[CUBE_FACE_F][3] = LED_YELLOW;
  // cube[CUBE_FACE_F][4] = LED_ORANGE;
  // cube[CUBE_FACE_F][5] = LED_VIOLET;
  // cube[CUBE_FACE_F][6] = LED_WHITE;
  // cube[CUBE_FACE_F][7] = LED_PINK;
  // cube[CUBE_FACE_F][8] = LED_RED;
  // LEDs_update(cube, CUBE_FACE_F);

  while (1)
  {

    if (timer_IT)
    {
      uint8_t i = 0;
      timer_IT = 0;

      if (state == 0)
      {
        move(cube, CUBE_FACE_D, ROTATE_90);
        LEDs_update(cube, CUBE_FACE_F);
        state = 1;
      }
      else if (state == 1)
      {
        move(cube, CUBE_FACE_R, ROTATE_90);
        LEDs_update(cube, CUBE_FACE_F);
        state = 2;
      }
      else if (state == 2)
      {
        move(cube, CUBE_FACE_U, ROTATE_90);      
        LEDs_update(cube, CUBE_FACE_F);
        state = 3;
      }
      else if (state == 3)
      {
        move(cube, CUBE_FACE_L, ROTATE_90);
        LEDs_update(cube, CUBE_FACE_F);
        state = 4;
      }
      else
      {
        move(cube, CUBE_FACE_F, ROTATE_90);
        LEDs_update(cube, CUBE_FACE_F);
        state = 0;
      }

      // Sample the push button arrays
      // pb[CUBE_FACE_F] = 0U;
      // pb[CUBE_FACE_F] |= (HAL_GPIO_ReadPin(PB_left_GPIO_Port, PB_left_Pin) == 0 ? PB_L_MASK : 0x00);
      // pb[CUBE_FACE_F] |= (HAL_GPIO_ReadPin(PB_right_GPIO_Port, PB_right_Pin) == 0 ? PB_R_MASK : 0x00);
      // pb[CUBE_FACE_F] |= (HAL_GPIO_ReadPin(PB_up_GPIO_Port, PB_up_Pin) == 0 ? 0x04 : 0x00);
      // pb[CUBE_FACE_F] |= (HAL_GPIO_ReadPin(PB_down_GPIO_Port, PB_down_Pin) == 0 ? 0x08 : 0x00);

      // Request push button status on the other boards
      //PB_array[FACE_B] = get_pb_conf(); 
      //PB_array[FACE_L] = get_pb_conf();
      //PB_array[FACE_R] = get_pb_conf();
      //PB_array[FACE_U] = get_pb_conf();
      //PB_array[FACE_D] = get_pb_conf();
      
      // --------------------------------
      // Funky gradient (TODO: remove me)
      // --------------------------------
      // for (i = 0; i < 9; i++)
      // {
      //   if (LED_phase[i] < 64)
      //   {
      //     LED_array[i].R = LED_phase[i];
      //     LED_array[i].G = 63-LED_phase[i];
      //     LED_array[i].B = 0;
      //   }
      //   else if ((LED_phase[i] >= 64) && (LED_phase[i] < 128))
      //   {
      //     LED_array[i].R = 63-(LED_phase[i]-64);
      //     LED_array[i].G = 0;
      //     LED_array[i].B = (LED_phase[i]-64);
      //   }
      //   else
      //   {
      //     LED_array[i].R = 0;
      //     LED_array[i].G = (LED_phase[i]-128);
      //     LED_array[i].B = 63-(LED_phase[i]-128);
      //   }

      //   // Detect overflow on the counter
      //   if ((LED_phase[i] == 191))
      //   {
      //     LED_phase[i] = 0;
      //   }
      //   else
      //   {
      //     //LED_phase[i]++;
      //   }
      // }

      // Display that!
      //LEDs_update(LED_array);
    }
  }


  return 0;
}



