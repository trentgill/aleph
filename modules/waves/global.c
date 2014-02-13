// initialize aleph-waves global variables

#include "osc.h"
#include "slew.h"
#include "waves.h"

// oscillator
ComplexOsc osc[WAVES_NVOICES];
// filter
 filter_svf svf[WAVES_NVOICES];
// mixed amplitude envelope
 fract32 voiceAmp[WAVES_NVOICES];

// osc output bus
 fract32 oscOut[WAVES_NVOICES] = { 0, 0 };
// filter output bus
 fract32 svfOut[WAVES_NVOICES] = { 0, 0 };
// mixed output
 fract32 voiceOut[WAVES_NVOICES] = { 0, 0 };

// dry mix
 fract32 fDry[WAVES_NVOICES];
// wet mix
 fract32 fWet[WAVES_NVOICES];

// amp smoother
 SlewExp ampSlew[WAVES_NVOICES];
// cutoff smoother
 SlewExp cutSlew[WAVES_NVOICES];
// rq smoother
 SlewExp rqSlew[WAVES_NVOICES];

// phase mod input
 fract32 pmIn[WAVES_NVOICES] = { 0, 0 };
// shape mod input
 fract32 wmIn[WAVES_NVOICES] = { 0, 0 };
// amp mod input
 fract32 amIn[WAVES_NVOICES] = { 0, 0 };
// cutoff mod input
fract32 fmIn[WAVES_NVOICES] = { 0, 0 };


/// mixes
// each input -> each output
fract32 mix_adc_dac[4][4] = { 
  { 0, 0, 0, 0 },
  { 0, 0, 0, 0 },
  { 0, 0, 0, 0 },
  { 0, 0, 0, 0 }
};


// each osc -> each output
fract32 mix_osc_dac[WAVES_NVOICES][4] = { 
  { 0, 0, 0, 0 }, 
  { 0, 0, 0, 0 } 
};


// each osc -> each osc, phase mod
fract32 mix_osc_pm[WAVES_NVOICES][WAVES_NVOICES] = {
  {0, 0}, 
  {0, 0},
};

// each osc -> each osc, shape mod
 fract32 mix_osc_wm[WAVES_NVOICES][WAVES_NVOICES]  = {
  {0, 0}, 
  {0, 0},
};

// each osc -> each osc, amp mod
fract32 mix_osc_am[WAVES_NVOICES][WAVES_NVOICES] = {
  {0, 0}, 
  {0, 0},
};

// each osc -> each svf, cutoff mod
 fract32 mix_osc_fm[WAVES_NVOICES][WAVES_NVOICES]  = {
  {0, 0}, 
  {0, 0},
};
