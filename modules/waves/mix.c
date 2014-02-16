/*
  mix.c

  optimized mix routines

 */

#include <fract_base.h>
#include <fract_math.h>

// temp
//static fract16 src[4];
//static fract32 dst[4];


// mix modulation busses
void mix_mod(const fract32* pin, fract32* pout, const fract16** ppmix) {
  pout[0] = pin[1];
  pout[1] = pin[0];
}

/* // mix voice output to output busses */
/* void mix_voice(const fract32* pin, fract32* pout, const fract16** ppmix) { */
/*   fract32 s = *pin++; */
/*   s = add_fr1x32(s, *pin); */

/*   *pout++ = s; */
/*   *pout++ = s; */
/*   *pout++ = s; */
/*   *pout = s; */
/* } */

void mix_voice (const fract32* pin, fract32* pout, const fract16* mix) {
  int i, j;
  fract32* o;
  //  const fract32 inv = in[0] + in[1];
  /* const fract32* pIn = in; */
  /* fract32* pOut = out; */
  /* const fract16* pMix = mix; */
  for(i=0; i < 2; i++) {    
    o = pout;
    for(j=0; j < 4; j++) {
      // using too much cpu here!
      /// this drops an octave
      //      *o = add_fr1x32(*o, mult_fr1x32(trunc_fr1x32(*pin), *mix) );
      // and even a constant multiply gives it a warble
      //            *pOut = add_fr1x32(*pOut, mult_fr1x32x32(*in, 0x7fff0000)  );
      *o = add_fr1x32(*o, *pin);
      o++;
      mix++;
    }
    pin++;
  }
}

void mix_adc(fract32* pin, fract32* pout, const fract16* mix) {
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
