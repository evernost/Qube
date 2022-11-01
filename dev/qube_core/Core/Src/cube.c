// ============================================================================
// Module name     : cube
// File name       : cube.c
// Purpose         : Rubik's cube library
// Author          : Quentin Biache
// Creation date   : June 06th, 2022
// ============================================================================

// ============================================================================
// Description
// ============================================================================
// Rubik's cube library

// ============================================================================
// Includes
// ============================================================================
#include "cube.h"

// Private function prototypes
static void swap(rgbLedType (*)[9], uint8_t, uint8_t);
static void circPerm(rgbLedType (*)[9], uint8_t, uint8_t, uint8_t, uint8_t, uint8_t);

// ----------------------------------------------------------------------------
// Rubik's cube move function
// ----------------------------------------------------------------------------
// Standard notations: 
// https://ruwix.com/the-rubiks-cube/notation/
void move(rgbLedType (*cube)[9], uint8_t moveID, uint8_t angle)
{
  if (angle == ROTATE_0)
  {
    // Nothing to do, don't bother.
  }
  else
  {
    switch(moveID)
    {
      // Front move (F) -------------------------------------------------------
      case CUBE_MOVE_F :
        circPerm(cube, angle, F_UL, F_UR, F_DR, F_DL);
        circPerm(cube, angle, F_UM, F_MR, F_DM, F_ML);
        
        // U -> R -> D -> L (clockwise)
        circPerm(cube, angle, U_DL, R_UL, D_UR, L_DR);
        circPerm(cube, angle, U_DR, R_DL, D_UL, L_UR);
        circPerm(cube, angle, U_UM, R_ML, D_UM, L_MR);
        break;

      // Bottom move (B) ------------------------------------------------------
      case CUBE_MOVE_B :
        circPerm(cube, angle, B_UL, B_UR, B_DR, B_DL);
        circPerm(cube, angle, B_UM, B_MR, B_DM, B_ML);
        
        // U -> L -> D -> R (clockwise)
        circPerm(cube, angle, U_UR, L_UL, D_DL, R_DR);
        circPerm(cube, angle, U_UL, L_DL, D_DR, R_UR);
        circPerm(cube, angle, U_UM, L_ML, D_DM, R_MR);
        break;

      // Left move (L) --------------------------------------------------------
      case CUBE_MOVE_L :
        circPerm(cube, angle, L_UL, L_UR, L_DR, L_DL);
        circPerm(cube, angle, L_UM, L_MR, L_DM, L_ML);
        
        // U -> F -> D -> B (clockwise)
        circPerm(cube, angle, U_UL, F_UL, D_UL, B_DR);
        circPerm(cube, angle, U_ML, F_ML, D_ML, B_MR);
        circPerm(cube, angle, U_DL, F_DL, D_DL, B_UR);
        break;

      // Right move (R) -------------------------------------------------------
      case CUBE_MOVE_R :
        circPerm(cube, angle, R_UL, R_UR, R_DR, R_DL);
        circPerm(cube, angle, R_UM, R_MR, R_DM, R_ML);
        
        // U -> B -> D -> F (clockwise)
        circPerm(cube, angle, U_UR, B_DL, D_UR, F_UR);
        circPerm(cube, angle, U_MR, B_ML, D_MR, F_MR);
        circPerm(cube, angle, U_DR, B_UL, D_DR, F_DR);
        break;

      // Up move (U) ----------------------------------------------------------
      case CUBE_MOVE_U :
        circPerm(cube, angle, U_UL, U_UR, U_DR, U_DL);
        circPerm(cube, angle, U_UM, U_MR, U_DM, U_ML);
        
        // F -> L -> B -> R (clockwise)
        circPerm(cube, angle, F_UL, L_UL, B_UL, R_UL);
        circPerm(cube, angle, F_UM, L_UM, B_UM, R_UM);
        circPerm(cube, angle, F_UR, L_UR, B_UR, R_UR);
        break;

      // Down move (D) --------------------------------------------------------
      case CUBE_MOVE_D :
        circPerm(cube, angle, D_UL, D_UR, D_DR, D_DL);
        circPerm(cube, angle, D_UM, D_MR, D_DM, D_ML);
        
        // U -> B -> D -> F (clockwise)
        circPerm(cube, angle, F_DL, R_DL, B_DL, L_DL);
        circPerm(cube, angle, F_DM, R_DM, B_DM, L_DM);
        circPerm(cube, angle, F_DR, R_DR, B_DR, L_DR);
        break;

      // Middle move (M) ------------------------------------------------------
      case CUBE_MOVE_M :
        circPerm(cube, angle, F_UM, D_UM, B_DM, U_UM);
        circPerm(cube, angle, F_MM, D_MM, B_MM, U_MM);
        circPerm(cube, angle, F_DM, D_DM, B_UM, U_DM);    
        break;

      // Equator move (E) -----------------------------------------------------
      case CUBE_MOVE_E :
        circPerm(cube, angle, F_ML, R_ML, B_ML, L_ML);
        circPerm(cube, angle, F_MM, R_MM, B_MM, L_MM);
        circPerm(cube, angle, F_MR, R_MR, B_MR, L_MR);
        break;

      // Standing move (S) ----------------------------------------------------
      case CUBE_MOVE_S :
        circPerm(cube, angle, U_ML, R_UM, D_MR, L_DM);
        circPerm(cube, angle, U_MM, R_MM, D_MM, L_MM);
        circPerm(cube, angle, U_MR, R_DM, D_ML, U_UM);
        break;
    }
  }
}

// ----------------------------------------------------------------------------
// swap() function (private)
// ----------------------------------------------------------------------------
static void swap(rgbLedType (*cube)[9], uint8_t src, uint8_t dest)
{
  rgbLedType tmp;
  uint8_t src_face, src_square;
  uint8_t dest_face, dest_square;

  // Convert 1D index to 2D index
  src_square  = src & 0x0F;
  src_face    = (src & 0xF0) >> 4;
  dest_square = dest & 0x0F;
  dest_face   = (dest & 0xF0) >> 4;

  tmp = cube[dest_face][dest_square];
  cube[dest_face][dest_square] = cube[src_face][src_square];
  cube[src_face][src_square] = tmp;
}

// ----------------------------------------------------------------------------
// circPerm() function (private)
// ----------------------------------------------------------------------------
// Note: cyclic permutation can be seen as product of transpositions (aka swaps)
// So a swap describes them all.
static void circPerm(rgbLedType (*cube)[9], uint8_t angle, uint8_t square_0, uint8_t square_1, uint8_t square_2, uint8_t square_3)
{
  switch(angle)
  {
    case ROTATE_0 :
      // Nothing to do.
      break;

    case ROTATE_90 :
      swap(cube, square_3, square_2);
      swap(cube, square_2, square_1);
      swap(cube, square_1, square_0);
      break;

    case ROTATE_180 :
      swap(cube, square_0, square_2);
      swap(cube, square_1, square_3);
      break;

    case ROTATE_270 :
      swap(cube, square_0, square_1);
      swap(cube, square_1, square_2);
      swap(cube, square_2, square_3);
      break;

    // TODO: handle unsupported angle value
  }
}

// ----------------------------------------------------------------------------
// paintFace() function
// ----------------------------------------------------------------------------
// Description:
// Automatically fills the 9 elements array <face> array with a given LED color
//
// Example: 
// paintFace(cube[CUBE_FACE_R], GREEN_LED);
// 
// TODO: 
// - Rename and remove the LED value init (redundant with LEDs_update)
// - Make use of the I2C LLD return value?
// 
void paintFace(rgbLedType *face, rgbLedType ledColor)
{
  uint8_t i;
  
  for(i = 0U; i < 9U; i++)
  {
    face[i].r = ledColor.r;
    face[i].g = ledColor.g;
    face[i].b = ledColor.b;
  }
}

