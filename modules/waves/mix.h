#ifndef _ALEPH_WAVES_MIX_H_
#define _ALEPH_WAVES_MIX_H_

extern void mix_voice (const fract32* pin, fract32* pout, const fract16* mix);
extern void mix_adc(fract32* pin, fract32* pout, const fract16* mix);

#endif
