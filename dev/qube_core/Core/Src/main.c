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

  // Array of RGB 6 faces x 9 LEDs representing the cube
  rgbLedType cube[6][9];
  
  uint8_t pb[6] = {0};
  uint8_t state = 0;


  // -----------------
  // MCU configuration
  // -----------------
  cpuInit();

  // Read the board ID
  uint8_t boardID = getBoardID();

  // Initialise the LED driver
  HAL_Delay(1);
  ledInitDriver();
  
  // Init the colors of each face
  paintFace(cube[CUBE_FACE_F], ORANGE_LED);
  paintFace(cube[CUBE_FACE_B], RED_LED);
  paintFace(cube[CUBE_FACE_L], BLUE_LED);
  paintFace(cube[CUBE_FACE_R], GREEN_LED);
  paintFace(cube[CUBE_FACE_U], PINK_LED);
  paintFace(cube[CUBE_FACE_D], YELLOW_LED);
  
  // Commit the changes to the LEDs
  ledUpdateFace(cube, CUBE_FACE_F);

  // --------------------------------------------------------------------------
  // Main loop
  // --------------------------------------------------------------------------
  
  // Use this to tune colors
  // cube[CUBE_FACE_F][F_UL] = LED_RED;
  // cube[CUBE_FACE_F][F_ML] = LED_GREEN;
  // cube[CUBE_FACE_F][F_DL] = LED_BLUE;
  // cube[CUBE_FACE_F][F_UM] = LED_YELLOW;
  // cube[CUBE_FACE_F][F_MM] = LED_ORANGE;
  // cube[CUBE_FACE_F][F_DM] = LED_VIOLET;
  // cube[CUBE_FACE_F][F_UR] = LED_WHITE;
  // cube[CUBE_FACE_F][F_MR] = LED_PINK;
  // cube[CUBE_FACE_F][F_DR] = LED_RED;
  // ledUpdateFace(cube, CUBE_FACE_F);

  while (1)
  {

    if (timer2_expired)
    {
      timer2_expired = 0;

      if (state == 0)
      {
        move(cube, CUBE_FACE_D, ROTATE_90);
        ledUpdateFace(cube, CUBE_FACE_F);
        state = 1;
      }
      else if (state == 1)
      {
        move(cube, CUBE_FACE_R, ROTATE_90);
        ledUpdateFace(cube, CUBE_FACE_F);
        state = 2;
      }
      else if (state == 2)
      {
        move(cube, CUBE_FACE_U, ROTATE_90);      
        ledUpdateFace(cube, CUBE_FACE_F);
        state = 3;
      }
      else if (state == 3)
      {
        move(cube, CUBE_FACE_L, ROTATE_90);
        ledUpdateFace(cube, CUBE_FACE_F);
        state = 4;
      }
      else
      {
        move(cube, CUBE_FACE_F, ROTATE_90);
        ledUpdateFace(cube, CUBE_FACE_F);
        state = 0;
      }

      // Probe the push button arrays (one hot coding)
      pb[CUBE_FACE_F] = 0U;
      pb[CUBE_FACE_F] |= (HAL_GPIO_ReadPin(PB_left_GPIO_Port, PB_left_Pin)    == 0 ? PB_L_MASK : 0x00);
      pb[CUBE_FACE_F] |= (HAL_GPIO_ReadPin(PB_right_GPIO_Port, PB_right_Pin)  == 0 ? PB_R_MASK : 0x00);
      pb[CUBE_FACE_F] |= (HAL_GPIO_ReadPin(PB_up_GPIO_Port, PB_up_Pin)        == 0 ? PB_U_MASK : 0x00);
      pb[CUBE_FACE_F] |= (HAL_GPIO_ReadPin(PB_down_GPIO_Port, PB_down_Pin)    == 0 ? PB_D_MASK : 0x00);

      // Request push button status on the other boards
      //PB_array[FACE_B] = getPushButtons(); 
      //PB_array[FACE_L] = getPushButtons();
      //PB_array[FACE_R] = getPushButtons();
      //PB_array[FACE_U] = getPushButtons();
      //PB_array[FACE_D] = getPushButtons();
    }
  }


  return 0;
}



