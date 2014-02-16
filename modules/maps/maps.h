/*
  maps.h

  global variables for aleph-maps.

  this DSP module will experiment with different memory structures for optimization.

 */ 


#define MAPS_NVOICES 4

#include "noise.h"
#include "slew.h"



///////////
///////////
/* 
   maps data is "classless" and dumb.
   all state variables are layed out as flat arrays at the global scope!
 */

// fract32 exponential slew
#define MAPS_VOICE_NSLEW 3
// each map has slew for each param, plus output amp
fract32 slew_x[MAPS_NVOICES][MAPS_VOICE_NSLEW];
fract32 slew_y[MAPS_NVOICES][MAPS_VOICE_NSLEW];
fract32 slew_c[MAPS_NVOICES][MAPS_VOICE_NSLEW];

// "map" data. these are discrete iterative function variables (ie, LCPRNG)
fract32 maps_[MAPS_NVOICES];

// amp smoother
extern SlewExp ampSlew[MAPS_NVOICES];
