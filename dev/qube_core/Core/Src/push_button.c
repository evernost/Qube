// ============================================================================
// Module name     : push_button
// File name       : push_button.c
// Purpose         : push button library
// Author          : Quentin Biache
// Creation date   : June 06th, 2022
// ============================================================================

// ============================================================================
// Description
// ============================================================================
// 

// ============================================================================
// Includes
// ============================================================================
#include "push_button.h"

enum PB_FSM {WAIT_PUSH_1, WAIT_PUSH_2, MOVE, WAIT_RELEASE};

void pb_fsm_init()
{
  uint8_t pb_array[6] = {0};
}

// ----------------------------------------------------------------------------
// pb_fsm_update() function
// ----------------------------------------------------------------------------
// Analyses the push button array and return 
uint8_t pb_fsm_update(uint8_t *pb)
{

}



// void midi_update(uint8_t message, midi* midi_obj)
// {
//   static uint8_t data_count;
//   static midi_note_event tmp_event;
//   uint8_t ctrl_index;

//   switch(midi_obj->midi_state)
//   {
//     // ------------------------------------------------------------------------
//     // MIDI idle state 
//     // ------------------------------------------------------------------------
//     case MIDI_STATE_IDLE :
//       // Init
//       data_count = 0;
//       tmp_event = MIDI_NOTE_EVENT_EMPTY;
      
//       switch(message & 0xF0)
//       {
//         // Switch depending on the command
//         case MIDI_CODE_NOTE_OFF :
//           midi_obj->midi_state = MIDI_STATE_NOTE_OFF;
//           break;

//         case MIDI_CODE_NOTE_ON :
//           midi_obj->midi_state = MIDI_STATE_NOTE_ON;
//           break;
      
//         case MIDI_CODE_CONTROL_CHANGE :
//           midi_obj->midi_state = MIDI_STATE_CONTROL_CHANGE;
//           break;

//         case MIDI_CODE_PROGRAM_CHANGE :
//           // No handled yet!
//           midi_obj->midi_state = MIDI_STATE_IDLE;
//           break;
      
//         case MIDI_CODE_PITCH_BEND :
//           midi_obj->midi_state = MIDI_STATE_PITCH_BEND;
//           break;

//         default : 
//           // Other commands are not supported
//           midi_obj->midi_state = MIDI_STATE_IDLE;   
//       }
//       break;

//     // ------------------------------------------------------------------------
//     // MIDI note OFF state 
//     // ------------------------------------------------------------------------
//     case MIDI_STATE_NOTE_OFF :
//       if (data_count == 0U)
//       {
//         tmp_event.event = MIDI_EVENT_NOTE_OFF;
//         tmp_event.index = message;
//         data_count++;
//       }
//       else
//       {
//         tmp_event.velocity = message;
//         midi_note_push(tmp_event, midi_obj);
//         data_count = 0U;
//         midi_obj->midi_state = MIDI_STATE_IDLE;
//       }
//       break;

//     // ------------------------------------------------------------------------
//     // MIDI note ON state 
//     // ------------------------------------------------------------------------
//     case MIDI_STATE_NOTE_ON :
//       if (data_count == 0U)
//       {
//         tmp_event.event = MIDI_EVENT_NOTE_ON;
//         tmp_event.index = message;
//         data_count++;
//       }
//       else
//       {
//         tmp_event.velocity = message;
//         midi_note_push(tmp_event, midi_obj);
//         data_count = 0U;
//         midi_obj->midi_state = MIDI_STATE_IDLE;
//       }
//       break;

//     // ------------------------------------------------------------------------
//     // MIDI control change state 
//     // ------------------------------------------------------------------------
//     case MIDI_STATE_CONTROL_CHANGE :
//       if (data_count == 0U)
//       {
//         ctrl_index = message;
//         data_count++;
//       }
//       else
//       {
//         midi_obj->ctrl_values[ctrl_index] = message;
//         midi_obj->midi_state = MIDI_STATE_IDLE;
//       }
//       break;

//     // ------------------------------------------------------------------------
//     // MIDI program change state 
//     // ------------------------------------------------------------------------
//     case MIDI_STATE_PROGRAM_CHANGE :
//       // No handled yet!
//       midi_obj->midi_state = MIDI_STATE_IDLE;
//       break;

//     // ------------------------------------------------------------------------
//     // MIDI pitch bend state 
//     // ------------------------------------------------------------------------
//     case MIDI_STATE_PITCH_BEND :
//       if (data_count == 0U)
//       {
//         midi_obj->pitch_bend = 0;
//         midi_obj->pitch_bend = message << 8;
//         data_count++;
//       }
//       else
//       {
//         midi_obj->pitch_bend |= message;
//         midi_obj->midi_state = MIDI_STATE_IDLE;
//       }
//       break;

//     // ------------------------------------------------------------------------
//     // Unsupported MIDI command
//     // ------------------------------------------------------------------------
//     case MIDI_STATE_UNSUPPORTED :
//       // No handled yet!
//       midi_obj->midi_state = MIDI_STATE_IDLE;
//       break;
//   }
// }
