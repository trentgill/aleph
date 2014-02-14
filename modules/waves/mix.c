/*
  mix.c

  optimized mix routines

 */

#include "fract_math.h"

#include "bfin_core.h"
#include "waves.h"

// temp
//static fract16 src[4];
//static fract32 dst[4];

// mix modulation busses
void mix_mod(void) {
#if 1
  /// TEST: only pm01 and pm10, patched directly 
  pmIn[0] = oscOut[1];
  pmIn[1] = oscOut[0];
#else
  /* for(i=0; i<WAVES_NVOICES; ++i) { */
  /*   src[i] = trunc_fr1x32(oscOut[i]);     */
  /* } */
  /* for(i=0; i<WAVES_NVOICES; ++i) { */
  /*   for(j=0; j<WAVES_NVOICES; ++j) { */
  /*     dst[j] = add_fr1x32(pmIn[j], mult_fr1x32(src, trunc_fr1x32(mix_osc_pm[i][j]) ) ); */
  /*   } */
  /*   for(j=0; j<WAVES_NVOICES; ++j) { */
  /*     pmIn[j] = dst[j]; */
  /*     //      wmIn[j] = add_fr1x32(wmIn[j], mult_fr1x32(src[i], trunc_fr1x32(mix_osc_wm[i][j]) ) ); */
  /*     //      amIn[j] = add_fr1x32(amIn[j], mult_fr1x32(src[i], trunc_fr1x32(mix_osc_am[i][j]) ) ); */
  /*     //      fmIn[j] = add_fr1x32(fmIn[j], mult_fr1x32(src[i], trunc_fr1x32(mix_osc_fm[i][j]) ) ); */
  /*   } */
  /* } */
#endif
}

// mix voice output to output busses
void mix_voice(void) {
  int i, j;

#if 1

  /* fract32* pSrc = voiceOut; */
  /* fract32* pDst = out; */
  /* fract16* pMix; */
  
  /* for(i=0; i<WAVES_NVOICES; ++i) { */
  /*   pMix = mix_osc_dac[i]; */
  /*   for(j=0; j<4; ++j) { */
  /*     //      *pDst = add_fr1x32(*pDst, mult_fr1x32(*pMix++, trunc_fr1x32(*pSrc))); */
  /*     ///// TEST */
  /*     *pDst = add_fr1x32(*pDst, mult_fr1x32(0x7fff, trunc_fr1x32(*pSrc))); */
  /*     pDst++; */
  /*   } */
  /*   pSrc++; */
  /* } */


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
    
    for(j=0; j<4; ++j) {

      out[j] = add_fr1x32( out[j], 
			      mult_fr1x32( mix_osc_dac[i][j], 
					   trunc_fr1x32(voiceOut[i])
					   )
			      );
    }
  }
   
#endif

/* #else */
/*   for(i=0; i<WAVES_NVOICES; ++i) { */
/*     src[i] = trunc_fr1x32(voiceOut[i]); */
/*   } */

/*   for(i=0; i<WAVES_NVOICES; ++i) { */

/*     for(j=0; j<4; ++j) { */
/*       dst[j] = add_fr1x32(out[j], mult_fr1x32(src[i], trunc_fr1x32(mix_osc_dac[i][j]) ) ); */
/*     } */
/*     for(j=0; j<4; ++j) { */
/*       out[j] = dst[j]; */
/*     } */


/*   } */
/* #endif */
}

// mix adc input to output busses
void mix_adc (void) { 
  int i, j;
  fract32* pSrc = in;
  fract32* pDst = out;
  fract16* pMix;
#if 1
  for(i=0; i<4; ++i) {
    pMix = mix_adc_dac[i];
    for(j=0; j<4; ++j) {
      *pDst = add_fr1x32(*pDst, mult_fr1x32(trunc_fr1x32( *pSrc), *pMix++));
      pDst++;
    }
    pSrc++;
  }
		    

#else
  for(i=0; i<4; ++i) {
    src[i] = trunc_fr1x32(in[i]);
  }
  for(i=0; i<4; ++i) {
    for(j=0; j<4; ++j) {
      dst[j] = add_fr1x32(out[j], mult_fr1x32(src[i], trunc_fr1x32(mix_adc_dac[i][j]) ) );
    }
    for(j=0; j<4; ++j) {
      out[j] = dst[j];
    }
  }
#endif
}
