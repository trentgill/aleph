// bfin
#include <fract2float_conv.h>
#include "fract_math.h"

// aleph/dsp
#include "interpolate.h"
#include "table.h"

#include "osc_waves.h"

//----------------
//--- static vars


//------------------
//---- static functions

// calculate phase incremnet
static inline void osc_calc_inc( ComplexOsc* osc) {
  //  (osc->lpInc).x = fix16_mul(osc->ratio, fix16_mul(osc->hz, ips) ); 

  *(osc->incIn) = fix16_mul(osc->ratio, fix16_mul(osc->hz, OSC_IPS) );
  /// TEST:
  osc->inc = fix16_mul(osc->ratio, fix16_mul(osc->hz, OSC_IPS) ); 
}

// calculate phase
static inline void osc_calc_pm(ComplexOsc* osc) {


  /*
  osc->idxMod = fix16_add( osc->idx, 
			   fix16_mul( FRACT_FIX16( mult_fr1x32x32( osc->pmIn, 
								   osc->pmAmt ) ),
				      WAVE_TAB_MAX16
				      ) );
  */

  // 16b phase mod:
  /*
  osc->idxMod = fix16_add(osc->idx, 
			  fix16_mul( mult_fr1x32(osc->pmIn, osc->pmAmt),
				     WAVE_TAB_MAX16
				     )
			  );
*/
  // with param pointers:
  osc->idxMod = fix16_add(osc->idx, 
			  fix16_mul( mult_fr1x32(*(osc->pmBus), *(osc->pmAmt)),
				     WAVE_TAB_MAX16
				     )
			  );


  // wrap negative
  while (BIT_SIGN_32(osc->idxMod)) {
    osc->idxMod = fix16_add(osc->idxMod, WAVE_TAB_MAX16);
  }

  // wrap positive
  while(osc->idxMod > WAVE_TAB_MAX16) { 
    osc->idxMod = fix16_sub(osc->idxMod, WAVE_TAB_MAX16); 
  }
}

// lookup 
static inline fract32 osc_lookup(ComplexOsc* osc) {
  //  u32 idxA = (*(osc->shape)) >> WAVE_TAB_RSHIFT;
  /// TEST:
  u32 idxA = 0;
  u32 idxB = idxA + 1;
  
  //  fract32 mul = shr_fr1x32((fract32)( (*(osc->shape)) & WAVE_SHAPE_MASK), WAVE_TAB_LSHIFT);
  // TEST:
  fract32 mul = 0;
  fract32 mulInv = sub_fr1x32(FR32_MAX, mul);
  
  return add_fr1x32( 
		    mult_fr1x32x32(table_lookup_idx_mask( (fract32*)(*(osc->tab))[idxA], 
						     WAVE_TAB_SIZE_1, 
						     osc->idxMod
						     ), mulInv ),
		    mult_fr1x32x32(table_lookup_idx_mask( (fract32*)(*(osc->tab))[idxB],
						     WAVE_TAB_SIZE_1,
						     osc->idxMod 
						     ), mul 
				   ) );
}

// advance phase
static inline void osc_advance(ComplexOsc* osc) {
  osc->idx = fix16_add(osc->idx, *(osc->incOut));
  //// TEST:
  // osc->idx = fix16_add(osc->idx, osc->inc);

  while(osc->idx > WAVE_TAB_MAX16) { 
    osc->idx = fix16_sub(osc->idx, WAVE_TAB_MAX16);
  }
}

//----------------
//--- extern funcs

// initialize given table data and pointers to parameter smoothing i/o
void osc_init( ComplexOsc* osc, 
		      wavtab_t tab, 
		      ComplexOsc_params* params
		      ) {

  

  osc->tab = tab;

  osc->val = 0;
  osc->idx = 0;
  osc->ratio = FIX16_ONE;
  osc->shape = 0;

  osc->idx = 0;
  osc->idxMod = 0;

  osc->incIn = params->incIn;
  osc->incOut = params->incOut;

    
  osc->pmAmt = params->pmAmt;
  osc->shape = params->shape;
    

  osc->inc = 0x962fc;

}

// set waveshape (table)
/*
void osc_set_shape(ComplexOsc* osc, fract16 shape) {
  //  filter_1p_lo_in( &(osc->lpShape), shape );
  //  (osc->lpShape).x = shape;
  *(osc->shape) = shape;
}
*/

// set base frequency in hz
void osc_set_hz(ComplexOsc* osc, fix16 hz) {
  osc->hz = hz;
  osc_calc_inc(osc);
}

// set fine-tuning ratio
void osc_set_tune(ComplexOsc* osc, fix16 ratio) {
  osc->ratio = ratio;
  osc_calc_inc(osc);
}

/*
// phase modulation amount
void osc_set_pm(ComplexOsc* osc, fract16 amt) {
  //  filter_1p_lo_in( &(osc->lpPm), amt);
  //  (osc->lpPm).x = amt;
  *(osc->pmIn) = amt;
}
*/

// get next frame value
fract32 osc_next(ComplexOsc* osc) {

  /// update param smoothers
    
  /* osc->inc = filter_1p_lo_next( &(osc->lpInc) ); */
  /* osc->shape = filter_1p_lo_next( &(osc->lpShape) ); */
  /* osc->pmAmt = filter_1p_lo_next( &(osc->lpPm) ); */
  //  osc->wmAmt = filter_1p_lo_next( &(osc->lpWm) );

  /* slew_exp_calc_frame( osc->lpInc ); */
  /* slew_exp_calc_frame( osc->lpShape ); */
  /* slew_exp_calc_frame( osc->lpPm ); */

  /* osc->inc = (osc->lpInc).y; */
  /* osc->shape = (osc->lpShape).y; */
  /* osc->pmAmt = (osc->lpPm).y; */

  // calculate phase modulation
  osc_calc_pm(osc);

  // advance phase
  osc_advance(osc);
  
  // lookup 
  return osc_lookup(osc);
}
