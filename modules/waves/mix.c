/*
  mix.c

  optimized mix routines

 */

#include <fract_base.h>
#include <fract_math.h>

// temp
//static fract16 src[4];
//static fract32 dst[4];

static  int i, j;

// mix modulation busses
void mix_mod(const fract32* pin, fract32* pout, const fract16** ppmix) {
  pout[0] = pin[1];
  pout[1] = pin[0];
}

// mix voice output to output busses
void mix_voice(const fract32* pin, fract32* pout, const fract16** ppmix) {
  fract32 s = *pin++;
  s = add_fr1x32(s, *pin);

  *pout++ = s;
  *pout++ = s;
  *pout++ = s;
  *pout = s;
}

void mix_adc(fract32* pin, fract32* pout, const fract16** ppmix) {
  //  fract32* pin = in;
  //  fract32* pout = out;
  //  fract16** ppmix = mix;
  //  int i, j;
  *pout = add_fr1x32(*pout, *pin);
  pout++;
  *pout = add_fr1x32(*pout, *pin);
  pout++;
  *pout = add_fr1x32(*pout, *pin);
  pout++;
  *pout = add_fr1x32(*pout, *pin);
}
