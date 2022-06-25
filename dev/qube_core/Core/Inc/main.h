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
extern const RGB_LED LED_RED;
extern const RGB_LED LED_GREEN;
extern const RGB_LED LED_BLUE;
extern const RGB_LED LED_YELLOW;
extern const RGB_LED LED_ORANGE;
extern const RGB_LED LED_WHITE;
extern const RGB_LED LED_VIOLET;
extern const RGB_LED LED_PINK;

// Defined in cpu.c
extern volatile uint8_t timer_IT;

// Exported functions prototypes ----------------------------------------------




// Private defines ------------------------------------------------------------


// Functions prototypes -------------------------------------------------------
// Public


// Private


#ifdef __cplusplus
}
#endif

#endif // __MAIN_H