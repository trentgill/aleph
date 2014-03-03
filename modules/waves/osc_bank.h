/* osc_bank.h
   aleph-waves

   bank of wavetable oscillators.

*/

#ifndef _ALEPH_WAVES_OSC_H_
#define _ALEPH_WAVES_OSC_H_

#include "params.h"


// base-frequency limits in fix16
#define OSC_HZ_MIN 0x00010000      // 1 hz
#define OSC_HZ_MAX 0x40000000    // 16384 hz
#define OSC_HZ_RADIX 15

//---- expected parameters for wavetable data

// how many oscillators
#define WAVES_OSC_COUNT WAVES_VOICE_COUNT


//=================================
//===== defines and types

// how many wavetables
#define WAVE_TAB_NUM 5
// bits for wavetable selection
#define WAVE_TAB_SEL_BITS 3
// size of each table
#define WAVE_TAB_BITS   10
#define WAVE_TAB_SIZE 	1 << (WAVE_TAB_BITS)
#define WAVE_TAB_SIZE_1 (WAVE_TAB_SIZE) - 1
// rshift from s16 shape variable to get table index
#define WAVE_TAB_IDX_SHIFT 16 - (WAVE_TAB_SEL_BITS)
// mask shape variable get interpolation constant
#define WAVE_TAB_IDX_SHAPE_MASK (1 << (WAVE_TAB_IDX_SHIFT)) - 1
// lshift after mask to get fract16 multiplier 
#define WAVE_TAB_MUL_SHIFT (WAVE_TAB_SEL_BITS) - 1
// shift normalized phase to get index into wavetable
#define WAVE_PHASE_IDX_SHIFT 32 - (WAVE_TAB_BITS)
// mask normalized phase to get interpolation coefficient
#define WAVE_PHASE_INTERP_MASK (1 << (WAVE_PHASE_IDX_SHIFT)) - 1
// rshift after mask to get fract16 multiplier
#define WAVE_PHASE_INTERP_SHIFT 16 - (WAVE_TAB_BITS)
// normalized (32b) phase increment at 1hz
#define WAVES_OSC_INC_1HZ 0xae3c


// state variables data type
typedef struct _oscBank {
  fix16 hz [WAVES_OSC_COUNT];
  fix16 ratio [WAVES_OSC_COUNT];
  /// phase is signed 32b, [0, 0xffffffff],  allowing overflow
  // phase increment
  fract32 inc [WAVES_OSC_COUNT];
  // base phase
  fract32 phase [WAVES_OSC_COUNT];
  // modulated phase
  fract32 phaseMod [ WAVES_OSC_COUNT ] ;
  // phase modulation input bus
  fract32 phaseModBus[WAVES_OSC_COUNT];
  
} OscBank;

//=============================
//===== extern variables

// bank of wavetables
extern const fract32 wavtab[WAVE_TAB_NUM][WAVE_TAB_SIZE];

// state variables declaration
extern OscBank oscBank;

//=================================
//==== extern functions

// init osc bank
extern void init_osc_bank(void);

// calculate one frame of all oscillators
extern void osc_bank_calc_frame(void);

// set hz
extern void osc_bank_set_hz(int i, fix16 v);
// set tune
extern void osc_bank_set_tune(int i, fix16 v);

#endif //  h guard
