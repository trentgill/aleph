#ifndef _ALEPH_DSP_PAN_H_
#define _ALEPH_DSP_PAN_H_

#include "types.h"


#define PAN_SINE_BUF_SIZE 1024
#define PAN_SINE_BUF_SIZE_MASK 1023
// shift down from unipolar fract32 to get 16.16 index
#define PAN_SINE_BUF_RSHIFT 5
#define PAN_SINE_BUF_SIZE_FIX (1024 << 16)
#define PAN_SINE_BUF_MAX16 (1024 * FIX16_ONE - 1)

fract32 sineTab[PAN_SINE_BUF_SIZE] ;

//--------------------------------
// --- simple linear pan
// return panned sum of two inputs
// given pan coefficient in [0,1]
inline fract32 pan_lin_mix(fract32 inX, fract32 inY, fract32 pan);

// return coefficients only
// given pan coefficient in [0,1]
inline void pan_lin_coeff(fract32* a, fract32* b, fract32 pan);

//------
// TODO: a simple "triangle pan" would probably be useful


//-----------------
//--- balanced, equal-power

// return panned sum of two inputs 
// given pan coefficient in [0,1]
inline fract32 pan_bal_mix(fract32 inX, fract32 inY, fract32 pan);

/* // return just the pan coefficients  */
/* // given pan coefficient in [0,1] */
/* inline void pan_bal_coeff(fract32* a, fract32* b, fract32 pan); */


/////////
////////////
//// macro-ized version of balanced pan coefficients

#define pan_bal_coeff(a, b, tmp)			\
  tmp = pan >> PAN_SINE_BUF_RSHIFT;			\
  b = table_lookup_idx_mask(sineTab, PAN_SINE_BUF_MASK, tmp);	\
  a = table_lookup_idx_mask(sineTab, PAN_SINE_BUF_MASK,		\
			    fix16_sub(PAN_SINE_BUF_MAX16, tmp));
  

#endif
