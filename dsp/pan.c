#include "fix.h"
#include "pan.h"
#include "table.h"

fract32 sineTab[PAN_SINE_BUF_SIZE] = {
#include "halfsine_1024.inc"
};



//------------------------------
//--- linear

// return balanced sum of two inputs 
// given pan coefficient in [0,1]
fract32 pan_lin_mix(fract32 inX, fract32 inY, fract32 pan) {
  fract32 ymul = pan;
  fract32 xmul = sub_fr1x32(FR32_MAX, ymul);
  return add_fr1x32( mult_fr1x32x32(inX, xmul), mult_fr1x32x32(inY, ymul) ); 
}

// just calculate balanced coefficients 
// given pan coefficient in [0,1]
void pan_lin_coeff(fract32* a, fract32* b, fract32 pan) {
    // simple linear pan
    *a = pan;
    *b = sub_fr1x32(FR32_MAX, pan);
}

//----------------------
//--- equal-power balanced pan

fract32 pan_bal_mix(fract32 inX, fract32 inY, fract32 pan) {
  // shift to get 16.16 idx
  fix16 yIdx = pan >> PAN_SINE_BUF_RSHIFT;
  fix16 xIdx = fix16_sub(PAN_SINE_BUF_MAX16, yIdx);
  fract32 xmul = table_lookup_idx_mask(sineTab, PAN_SINE_BUF_MASK, xIdx);
  fract32 ymul = table_lookup_idx(sineTab, PAN_SINE_BUF_MASK, yIdx);
  return add_fr1x32( mult_fr1x32x32(inX, xmul), mult_fr1x32x32(inY, ymul) ); 
}

/* void pan_bal_coeff(fract32* a, fract32* b, fract32 pan) { */
/*   fix16 yIdx = pan >> PAN_SINE_BUF_RSHIFT; */
/*   fix16 xIdx = fix16_sub(PAN_SINE_BUF_MAX16, yIdx); */
/*   *a = table_lookup_idx_mask(sineTab, PAN_SINE_BUF_MASK, xIdx); */
/*   *b = table_lookup_idx(sineTab, PAN_SINE_BUF_MASK, yIdx); */
/* } */
