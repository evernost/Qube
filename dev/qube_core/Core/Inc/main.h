// ============================================================================
// Module name    : main
// File name      : main.h
// Purpose        : 
// Author         : Quentin Biache
// Creation date  : June 06th, 2022
// ============================================================================

// Define to prevent recursive inclusion --------------------------------------
#ifndef __MAIN_H
#define __MAIN_H

#ifdef __cplusplus
extern "C" {
#endif

// Includes -------------------------------------------------------------------
#include "led.h"

// LED colors are defined in led.c
extern const rgbLedType RED_LED;
extern const rgbLedType GREEN_LED;
extern const rgbLedType BLUE_LED;
extern const rgbLedType YELLOW_LED;
extern const rgbLedType ORANGE_LED;
extern const rgbLedType WHITE_LED;
extern const rgbLedType VIOLET_LED;
extern const rgbLedType PINK_LED;

// Defined in cpu.c
extern volatile uint8_t timer2_expired;

// Exported functions prototypes ----------------------------------------------


// Private defines ------------------------------------------------------------


// Functions prototypes -------------------------------------------------------
// Public


// Private


#ifdef __cplusplus
}
#endif

#endif // __MAIN_H