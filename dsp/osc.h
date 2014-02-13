/* osc.h
   dsp
   aleph

   a simple wavetable oscillator with phase and shape modulation.
*/

#ifndef _ALEPH_DSP_OSC_H_
#define _ALEPH_DSP_OSC_H_

//#include "filter_1p.h"
#include "fix.h"
#include "slew.h"

// base-frequency limits in fix16
#define OSC_HZ_MIN 0x00010000      // 1 hz
#define OSC_HZ_MAX 0x40000000    // 16384 hz
#define OSC_HZ_RADIX 15

//---- expected parameters for wavetable data
// how many wavetables
#define WAVE_TAB_NUM 5
// size of each table
#define WAVE_TAB_SIZE 	1024
#define WAVE_TAB_SIZE_1 (WAVE_TAB_SIZE - 1)
#define WAVE_TAB_MAX16 	(WAVE_TAB_SIZE * FIX16_ONE - 1)
// rshift from shape variable to get table index
#define WAVE_TAB_RSHIFT 29
// mask to get interpolation constant
#define WAVE_SHAPE_MASK 0x1fffffff
// lshift after mask to get multiplier 
#define WAVE_TAB_LSHIFT 2


// samplerate is fixed at compilation for now
// this is (table size) / samplerate in 16.16
#define OSC_IPS_FIX16 0x576

// wavetable type: pointer to 2d array
typedef const fract32 (*wavtab_t) [WAVE_TAB_NUM][WAVE_TAB_SIZE];

// temp variables for macro-ized oscillator functions
extern u32 idxA;
extern u32 idxB;
extern fract32 mul;
extern fract32 mulInv;


// class structure
typedef struct _ComplexOsc {
  // output value
  fract32 val;
  // wavetable data (pointer to table of tables)
  wavtab_t tab;
  // normalized waveshape
  fract32 shape;
  // smoothed and normalized waveshape
  //  fract32 shapeSmooth;
  // modulated and bandlimited waveshape
  fract32 shapeMod;
  // frequency in hz
  fix16 hz;
  // tuning ratio
  fix16 ratio;
  // base phase as fractional index
  fix16 idx;
  // modulated phase
  fix16 idxMod;
  // fixed-point index increment
  fix16 inc;
  // bandlimiting coefficient [0-1]
  //  fract32 bandLim;
  // phase modulation amount [0-1)
  fract32 pmAmt;
  // shape modulation amount [0-1)
  fract32 wmAmt;

  /// store last modulation input values for recalculation
  fract32 pmIn, wmIn;

  /// 1pole filters for smoothing phase increment, shape, modulation params
  /* filter_1p_lo lpInc; */
  /* filter_1p_lo lpShape; */
  /* filter_1p_lo lpPm; */
  /* filter_1p_lo lpWm; */
  SlewExp lpInc;
  SlewExp lpShape;
  SlewExp lpPm;
  SlewExp lpWm;
  
  //// TODO: can get more shapes, at slight expense, by invert+shift+sum
  // inversion amount 
  //  fract32 invAmp;
  // inversion phase
  //  fix16 invPhase;
} ComplexOsc;

/*
// initialize given table data and samplerate
extern void osc_init(ComplexOsc* osc, wavtab_t tab, u32 sr);

// set waveshape (table)
extern void osc_set_shape(ComplexOsc* osc, fract32 shape);
// set base frequency in hz
extern void osc_set_hz(ComplexOsc* osc, fix16 hz);
// set fine-tuning ratio
extern void osc_set_tune(ComplexOsc* osc, fix16 ratio);

// phase modulation amount
extern void osc_set_pm(ComplexOsc* osc, fract32 wmAmt);
// shape modulation amount
extern void osc_set_wm(ComplexOsc* osc, fract32 wmAmt);

// phase modulation input
extern void osc_pm_in(ComplexOsc* osc, fract32 pm);
// shape modulation input
extern void osc_wm_in(ComplexOsc* osc, fract32 wm);

// set bandlimiting coefficient
extern void osc_set_bl(ComplexOsc* osc, fract32 bl);

// compute next value
extern fract32 osc_next( ComplexOsc* osc);
*/

//=======================================

//====== macro-ized

// initialize with wavetable data
// samplerate is fixed at compilation for now
#define complex_osc_init(o)			\
  slew_exp_init( (o).lpInc , FIX16_ONE);	\
  slew_exp_init( (o).lpShape , 0);		\
  slew_exp_init( (o).lpPm , 0);			\
  slew_exp_init( (o).lpWm , 0);			\
  (o).val = 0;					\
  (o).idx = 0;					\
  (o).ratio = FIX16_ONE;			\
  (o).shape = 0;				\
     (o).shapeMod = 0;				\
     (o).idx = 0;				\
     (o).idxMod = 0;				\
     (o).pmAmt = 0;				\
     (o).wmAmt = 0			

// calculate 16.16 phase increment (index per sample)
#define complex_osc_calc_inc( o )					\
  ((o).lpInc).x = fix16_mul((o).ratio, fix16_mul((o).hz, OSC_IPS_FIX16) )

// apply phase modulation to current phase 
#define complex_osc_calc_pm( o )					\
  (o).idxMod = fix16_add( (o).idx,					\
			  fix16_mul( FRACT_FIX16( mult_fr1x32x32( (o).pmIn, \
								  (o).pmAmt ) ), \
				     WAVE_TAB_MAX16			\
				     ) );				\
     while (BIT_SIGN_32((o).idxMod)) {					\
       (o).idxMod = fix16_add((o).idxMod, WAVE_TAB_MAX16);		\
     }									\
     while((o).idxMod > WAVE_TAB_MAX16) {				\
       (o).idxMod = fix16_sub((o).idxMod, WAVE_TAB_MAX16);		\
     }								

// use phase and shape to lookup current output value
// macro takes osc data, destination var, and 4 temp vars
#define complex_osc_lookup(o, dst)					\
  idxA = (o).shapeMod >> WAVE_TAB_RSHIFT;				\
  idxB = idxA + 1;							\
  mul = ((o).shapeMod & WAVE_SHAPE_MASK) << WAVE_TAB_LSHIFT;		\
  mulInv = sub_fr1x32(FR32_MAX, mul);					\
  dst = add_fr1x32(							\
		   mult_fr1x32x32(table_lookup_idx_mask( (fract32*)(*((o).tab))[idxA], \
							 WAVE_TAB_SIZE_1, \
							 (o).idxMod	\
							 ), mulInv ),	\
		   mult_fr1x32x32(table_lookup_idx_mask( (fract32*)(*((o).tab))[idxB], \
							 WAVE_TAB_SIZE_1, \
							 (o).idxMod	\
							 ), mul		\
				  ) )			

// advance phase
#define complex_osc_advance( o )			\
  (o).idx = fix16_add((o).idx, (o).inc);		\
     while((o).idx > WAVE_TAB_MAX16) {			\
       (o).idx = fix16_sub((o).idx, WAVE_TAB_MAX16);	\
     }							


// get next frame value
#define complex_osc_next( o, dst )		\
  slew_exp_calc_frame( (o).lpInc );		\
  slew_exp_calc_frame( (o).lpShape );		\
  slew_exp_calc_frame( (o).lpPm );		\
  (o).inc = ((o).lpInc).y;			\
  (o).shape = ((o).lpShape).y;			\
     (o).pmAmt = ((o).lpPm).y;			\
	(o).shapeMod = (o).shape;		\
	   complex_osc_calc_pm(o);		\
	   complex_osc_advance(o);		\
	   complex_osc_lookup(o, dst)

//--- "setter" macros

/// set base waveshape						
#define complex_osc_set_shape(o, v)		\
  ((o).lpShape).x = v

// set base frequency in hz
#define complex_osc_set_hz( o, v )		\
  (o).hz = v;					\
     complex_osc_calc_inc( o )

// set fine-tuning ratio
#define complex_osc_set_tune( o, v )		\
  (o).ratio = v;				\
     complex_osc_calc_inc( o )

// phase modulation amount
#define complex_osc_set_pm( o, v )		\
  ((o).lpPm).x = v	

// shape modulation amount
#define complex_osc_set_wm( o, v )		\
  ((o).lpWm).x = v

// phase modulation input
#define complex_osc_pm_in( o, v )		\
  (o).pmIn = v

// shape modulation input
#define complex_osc_wm_in( o, v )		\
  (o).wmIn = v

/* // set bandlimiting */
/* #define complex_osc_set_bl( o, v )		\ */
/*   (o).bandLim = v */



#endif
