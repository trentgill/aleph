/*
  slew_bank.c
  
  in the interest of speed, 
  all required parameter-smoothing constructs are computed in one place.
  
  in c++, we would use a nice templated class for this,
  but here it is simplest just to customize the slew sources for each module.
*/


#include <fract_math.h>
#include "slew_bank.h"

SlewBank32 slew32;
SlewBank16 slew16;

// intitialize all state variables for both smoother banks
void slew_bank_init(void) {
  int i;
  fract32* c32 = slew32.c;
  fract32* x32 = slew32.x;
  fract32* y32 = slew32.y;
  fract16* c16 = slew16.c;
  fract16* x16 = slew16.x;
  fract16* y16 = slew16.y;

  for(i=0; i<WAVES_SLEW32_COUNT; ++i) {
    *x32++ = *y32++ = *c32++ = 0;
  }
  for(i=0; i<WAVES_SLEW16_COUNT; ++i) {
    *x16++ = *y16++ = *c16++ = 0;
  }
}

// calculate one frame for all 32b smoothers
void slew_bank_32_calc_frame(void) {
  fract32* c = slew32.c;
  fract32* x = slew32.x;
  fract32* y = slew32.y;
  int i;
  for(i=0; i<WAVES_SLEW32_COUNT; ++i) {
    *y = add_fr1x32( *x, mult_fr1x32x32( *c++, sub_fr1x32(*y, *x)));
    ++y;
    ++x;
  }
}

// calculate 1 frame for all 16b smoothers
void slew_bank_16_calc_frame(void) {
  fract16* c = slew16.c;
  fract16* x = slew16.x;
  fract16* y = slew16.y;
  int i;
  for(i=0; i<WAVES_SLEW16_COUNT; ++i) {
    *y = add_fr1x16( *x, mult_fr1x16( *c++, sub_fr1x16(*y, *x)));
    ++y;
    ++x;
  }
}

