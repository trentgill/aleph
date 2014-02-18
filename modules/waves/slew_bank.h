/*

  slew_bank.h
  
  in the interest of speed, 
  all required parameter-smoothing constructs are computed in one place.
  
  in c++, we would use a nice templated class for this,
  but here it is simplest just to customize the slew sources for each module.

 */

#ifndef    _ALEPH_WAVES_SLEW_BANK_H_
#define    _ALEPH_WAVES_SLEW_BANK_H_

#include <fract_base.h>

// inclusion difficulty: we need VOICE_COUNT, 
// but waves.h needs this header, so put VOICE_COUNT in params
#include "params.h"

//------------------------------
//---- types and definitions

/*   each voice has 32b slew for:
     - oscillator increment (index per sample)
     - filter cutoff
     - filter rq
     - output amp
 */
#define WAVES_SLEW32_PER_VOICE 4
#define WAVES_SLEW32_COUNT (WAVES_VOICE_COUNT * WAVES_SLEW32_PER_VOICE)
enum {
  eSlew32_oscInc,
  eSlew32_voiceAmp,
  eSlew32_svfCut,
  eSlew32_svfRq,
};

/*   each voice has 16b slew for:
     - PM amount
     - waveshape
     - filter wet
     - filter dry
     [ - mix? (not yet) ]
*/
#define WAVES_SLEW16_PER_VOICE 4
#define WAVES_SLEW16_COUNT (WAVES_VOICE_COUNT * WAVES_SLEW16_PER_VOICE)
enum {
  eSlew16_oscPm,
  eSlew16_oscShape,
  eSlew16_svfWet,
  eSlew16_svfDry,
};

/* // 32-bit (long slew is possible) */
typedef struct _slewBank32 {
  fract32 c[WAVES_SLEW32_COUNT];
  fract32 x[WAVES_SLEW32_COUNT];
  fract32 y[WAVES_SLEW32_COUNT];
} SlewBank32;

/* // 16-bit (max slew is ~1s at audio sampling rate) */
typedef struct _slewBank16 {
  // count should be even for alignment!
  fract16 c[WAVES_SLEW16_COUNT];
  fract16 x[WAVES_SLEW16_COUNT];
  fract16 y[WAVES_SLEW16_COUNT];
} SlewBank16;

//-----------------
//--- extern variables
extern SlewBank32 slew32;
extern SlewBank16 slew16;

//----------------------------
//--- extern functions
// intialize all slew data
extern void slew_bank_init(void);
  
// calculate one frame for all 32b smoothers
extern void slew_bank_32_calc_frame(void);

// calculate 1 frame for all 16b smoothers
extern void slew_bank_16_calc_frame(void);

#endif // h guard
