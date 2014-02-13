/*
  mix.c

  optimized mix routines

 */

#include "fract_math.h"

#include "bfin_core.h"
#include "waves.h"

// temp
static fract16 sig;
static u8 i, j;

// mix modulation busses
void mix_mod(void) {
#if 0
  /// TEST: only pm01 and pm10, patched directly 
  pmIn[0] = oscOut[1];
  pmIn[1] = oscOut[0];
#else
  for(i=0; i<WAVES_NVOICES; ++i) {
    sig = trunc_fr1x32(oscOut[i]);    

    for(j=0; j<WAVES_NVOICES; ++j) {
      pmIn[j] = add_fr1x32(pmIn[j], mult_fr1x32(sig, trunc_fr1x32(mix_osc_pm[i][j]) ) );
      //      wmIn[j] = add_fr1x32(wmIn[j], mult_fr1x32(sig, trunc_fr1x32(mix_osc_wm[i][j]) ) );
      //      amIn[j] = add_fr1x32(amIn[j], mult_fr1x32(sig, trunc_fr1x32(mix_osc_am[i][j]) ) );
      //      fmIn[j] = add_fr1x32(fmIn[j], mult_fr1x32(sig, trunc_fr1x32(mix_osc_fm[i][j]) ) );
    }
  }
#endif
}

// mix voice output to output busses
void mix_voice(void) {
#if 0
  // TEST: all oscs to all outputs
  out[0] = out[1] = out[2] = out[3] = 
    add_fr1x32( 
	       voiceOut[0],
	       add_fr1x32(
			  voiceOut[1],
			  add_fr1x32(
				     voiceOut[2],
				     voiceOut[3]
				     )
			  )
	       );
#else
  for(i=0; i<WAVES_NVOICES; ++i) {
    sig = trunc_fr1x32(voiceOut[i]);

    for(j=0; j<4; ++j) {
      out[j] = add_fr1x32(out[j], mult_fr1x32(sig, trunc_fr1x32(mix_osc_dac[i][j]) ) );
    }
  }
#endif
}

// mix adc input to output busses
void mix_adc (void) { 
#if 1
  // TEST: no input
  ;;
#else
  for(i=0; i<4; ++i) {
    sig = trunc_fr1x32(in[i]);
    for(j=0; j<4; ++j) {
      out[j] = add_fr1x32(out[j], mult_fr1x32(sig, trunc_fr1x32(mix_adc_dac[i][j]) ) );
    }
  }
#endif
}

