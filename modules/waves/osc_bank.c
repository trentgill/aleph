#include <fract_math.h>

#include "waves.h"
#include "osc_bank.h"

//============================
//=== extern vars (initialized here)
// bank of wavetables
const fract32 wavtab[WAVE_TAB_NUM][WAVE_TAB_SIZE];

///----------------------
// state variables declaration
OscBank oscBank;
//----------------------

//=========================
//=== static vars

/* static fix16 * inc ; */
/* static fix16 * idx ; */
/* static fix16 * idxMod ; */
static fract32* inc;
static fract32* phase;
static fract32* phaseMod;

// bus pointers
static fract32* out ;
static fract32* pmBus;
// param array pointers
static fract16** shape ;
static fract16** pm;


/// temp variables, used in each voice calculation
// wavetable selection
static int tabIdx0;
static int tabIdx1;
// wavetable crossfade coeffs
static fract16 tabMul0;
static fract16 tabMul1;
// wavetable interpolation
static int valIdxInt0;
static int valIdxInt1;
static fract16 valIdxFract;
static fract16 dum; // align
// interpolated wavetable outputs
static fract16 tabVal0;
static fract16 tabVal1;
// normalized wavetable values
static fract32 x;
static fract16 y;
static fract16 dumdum; // align

//=================================
//=== STATIC funcs

// calculate increment from hz and ratio, arbitrary osc
// this is called from parameter set, not at frame calculation
// so it should be as fast as possible, 
// but not at expense of per-frame phase calculation.
static inline void osc_bank_calc_inc(int id) {
  //// TEST
  /*
  *(oscIncIn[id]) = fix16_mul(
			      fix16_mul(oscBank.hz[id], oscBank.ratio[id]), 
			      WAVES_OSC_INC_1HZ
			      );
  */


  //////////////
  *(oscIncIn[id]) = (id ? 0x962fc9 : 0x1c28f5c );
  /// coarse, integer hz
  //  *(oscIncIn[id]) = WAVES_OSC_INC_1HZ * (fix16_mul(oscBank.hz[id], oscBank.ratio[id]) >> 16);
}

// calcualate modulated phase, using current pointers
static inline void osc_bank_calc_phase(void) {
  // non-saturating add, allow overflow / underflow
  *phaseMod = *phase + ( mult_fr1x32(**pm, *pmBus) );
  // zap sign
  *phaseMod &= 0x7fffffff;
}

// advance phase, using current pointers
static inline void osc_bank_advance_phase(void) {
  // use integer add (32b, nonsaturating)
  // zap sign bit so overflow puts us back in positive fract range 
  *phase = (*phase + *inc) & 0x7fffffff;
}


// lookup value from current phase + shape
static inline void osc_bank_calc_value(void) {
  // first table index
  tabIdx0 = (**shape) >> (WAVE_TAB_IDX_SHIFT);
  // FIXME: no bounds check on upper table index...
  tabIdx1 = tabIdx0 + 1;

  // interpolation coefficients for each wavetable output
  tabMul1 = shl_fr1x32(
		       (fract32)((**(shape)) & (WAVE_TAB_IDX_SHAPE_MASK) ),
		       (WAVE_TAB_MUL_SHIFT)
		       );
  tabMul0 = sub_fr1x32(FR32_MAX, tabMul1);

  // now get the values for each table
  // each has same int / fract index for interpolation endpoints
  valIdxInt0 =  *phaseMod >> (WAVE_PHASE_IDX_SHIFT);
  valIdxInt1 = (valIdxInt0 + 1) & (WAVE_TAB_SIZE_1);
  valIdxFract = *phaseMod & (WAVE_PHASE_INTERP_MASK);

  x = wavtab[ tabIdx0 ][ valIdxInt0 ];
  y = trunc_fr1x32( wavtab[ tabIdx0 ][ valIdxInt1 ] );
  tabVal0 = add_fr1x32(
		       x, 
		       mult_fr1x32(
				   sub_fr1x16(y, trunc_fr1x32(x)),
				   valIdxFract) 
		       );

  x = wavtab[ tabIdx1 ][ valIdxInt0 ];
  y = trunc_fr1x32( wavtab[ tabIdx1 ][ valIdxInt1 ] );
  tabVal1 = add_fr1x32(
		       x, 
		       mult_fr1x32(
				   sub_fr1x16(y, trunc_fr1x32(x)),
				   valIdxFract) 
		       );

  // scale and sum the interpolated outputs from the 2 tables
  *out = add_fr1x32( 
		    mult_fr1x32(trunc_fr1x32(tabVal0), tabMul0),
		    mult_fr1x32(trunc_fr1x32(tabVal1), tabMul1)
		     );   
}


//=================================
//=== extern funcs

// init osc bank
void init_osc_bank(void) {
  int i;
  for(i=0; i<WAVES_OSC_COUNT; ++i) {
    oscBank.hz[i] = fix16_from_float(220.0);

  }
}


// set hz
void osc_bank_set_hz(int i, fix16 v) {
  oscBank.hz[i] = v;
  osc_bank_calc_inc(i);
}

// set tune
void osc_bank_set_tune(int i, fix16 v) {
  oscBank.ratio[i] = v;
  osc_bank_calc_inc(i);

}


// calculate one frame of all oscillators
void osc_bank_calc_frame(void) {

  /// FIXME: fract16 would be fine for normalized table data.
  /* fract32 tabVal0; */
  /* fract32 tabVal1; */
  /* fract32 tabMul0; */
  /* fract32 tabMul1; */

  int i;

  // setup pointers
  inc = oscBank.inc;
  phase = oscBank.phase;
  phaseMod = oscBank.phaseMod;

  out = oscOut;
  shape = shapeOut;
  pmBus = pmModBus;
  pm = pmOut;


  for(i=0; i<WAVES_OSC_COUNT; ++i) {
    // calculate
    osc_bank_calc_phase();
    osc_bank_calc_value();
    osc_bank_advance_phase();
   
    // increment pointers
    out++;
    inc++;
    phase++;
    phaseMod++;
    shape++;
  }
}

