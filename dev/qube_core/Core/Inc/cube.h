// ============================================================================
// Module name    : cube 
// File name      : cube.h
// Purpose        : 
// Author         : Quentin Biache
// Creation date  : June 06th, 2022
// ============================================================================

// Define to prevent recursive inclusion --------------------------------------
#ifndef __CUBE_H
#define __CUBE_H

#ifdef __cplusplus
extern "C" {
#endif

// Includes -------------------------------------------------------------------
#include "led.h"

// Defines --------------------------------------------------------------------
#define CUBE_FACE_F  0U
#define CUBE_FACE_B  1U
#define CUBE_FACE_L  2U
#define CUBE_FACE_R  3U
#define CUBE_FACE_U  4U
#define CUBE_FACE_D  5U

#define F_UL  0x00
#define F_ML  0x01
#define F_DL  0x02
#define F_UM  0x03
#define F_MM  0x04
#define F_DM  0x05
#define F_UR  0x06
#define F_MR  0x07
#define F_DR  0x08

#define B_UL  0x10
#define B_ML  0x11
#define B_DL  0x12
#define B_UM  0x13
#define B_MM  0x14
#define B_DM  0x15
#define B_UR  0x16
#define B_MR  0x17
#define B_DR  0x18

#define L_UL  0x20
#define L_ML  0x21
#define L_DL  0x22
#define L_UM  0x23
#define L_MM  0x24
#define L_DM  0x25
#define L_UR  0x26
#define L_MR  0x27
#define L_DR  0x28

#define R_UL  0x30
#define R_ML  0x31
#define R_DL  0x32
#define R_UM  0x33
#define R_MM  0x34
#define R_DM  0x35
#define R_UR  0x36
#define R_MR  0x37
#define R_DR  0x38

#define U_UL  0x40
#define U_ML  0x41
#define U_DL  0x42
#define U_UM  0x43
#define U_MM  0x44
#define U_DM  0x45
#define U_UR  0x46
#define U_MR  0x47
#define U_DR  0x48

#define D_UL  0x50
#define D_ML  0x51
#define D_DL  0x52
#define D_UM  0x53
#define D_MM  0x54
#define D_DM  0x55
#define D_UR  0x56
#define D_MR  0x57
#define D_DR  0x58

#define CUBE_MOVE_F 0U
#define CUBE_MOVE_B 1U
#define CUBE_MOVE_L 2U
#define CUBE_MOVE_R 3U
#define CUBE_MOVE_U 4U
#define CUBE_MOVE_D 5U
#define CUBE_MOVE_M 6U
#define CUBE_MOVE_E 7U
#define CUBE_MOVE_S 8U

#define ROTATE_0    0U
#define ROTATE_90   1U
#define ROTATE_180  2U
#define ROTATE_270  3U

// Types ----------------------------------------------------------------------


// These constants have to be seen when "midi.h" is included.
// The "extern" indicates that these are defined elsewhere (in this case, in midi.c)
// Definition cannot be given here, otherwise these constants would be defined
// everytime "midi.h" is included.
// extern const float midi_note_freq[128];
// extern const midi_note_event MIDI_NOTE_EVENT_EMPTY;

// Functions prototypes -------------------------------------------------------
void move(rgbLedType (*)[9], uint8_t, uint8_t);
void paintFace(rgbLedType*, rgbLedType);

#ifdef __cplusplus
}
#endif

#endif // __CUBE_H
