/* 
   keys.c
   aleph-bfin

   applies attenuation & lowpass filtering to each input signal.

   all 4 filtered inputs are also envelope followed w/ parameterised slew time.

   these envelopes can then be routed to any of the channels gain or filter.

   all 4 envelopes are routed to the 4 cv outs

*/

//-- blackfin toolchain headers
// 1.32 and 1.15 fixed-point arithmeti
#include "fract_math.h"

//-- aleph/common headers
#include "types.h"

//-- aleph/bfin-lib headers
// global variables
#include "bfin_core.h"
// cv output driver
#include "cv.h"
// gpio pin numbers
#include "gpio.h"

//-- dsp class headers
// simple 1-pole integrator
#include "filter_1p.h"
// 2pole svf
#include "filter_svf.h"
// global declarations for module data
#include "module.h"

//--- custom headers
// parameter lists and constants
#include "params.h"

// customized module data structure
// this will be located at the top of SDRAM (64MB)
// all other variables (code, stack, heap) are located in SRAM (64K)
typedef struct _keysData {
  //... here is where you would put other large data structures.

  // for example, a multichannel delay module would need an audio buffer:
  //  volatile fract32 audioBuffer[NUM_BUFS][FRAMES_PER_BUF];

  // bear in mind that access to SDRAM is significantly slower than SRAM (10-20 cycles!)
  // so don't put anything here that doesn't need the extra space.
} keysData;

//-------------------------
//----- extern vars (initialized here)


// global pointer to module descriptor.
// required by the aleph-bfin library!
ModuleData* gModuleData;

//-----------------------bfin_lib/src/
//------ static variables

/* 
   here's the actual memory for module descriptor and param data
   global pointers are to point at these here during module init.
   we do it in this indirect way, because
   a) modules have variable param count
   b) in an extreme case, might need to locate param data in SDRAM
      ( until recently, SDRAM contained full param descriptors.)
*/
static ModuleData super;
static ParamData mParamData[eParamNumParams];

// current channel to update - see below in process_cv() 
static u8 cvChan = 0;

// audio input bus & filters
static filter_1p_lo inSlew[4];
static filter_svf filt[4];

static fract32 inGain[4];
static fract32 inFilt[4];
static fract32 env[4];

static fract32 ef0_g[4];
static fract32 ef1_g[4];
static fract32 ef2_g[4];
static fract32 ef3_g[4];
static fract32 ef0_f[4];
static fract32 ef1_f[4];
static fract32 ef2_f[4];
static fract32 ef3_f[4];

//-----------------
//--- static function declaration

// update cv output
static void process_cv(void);

// small helper function to set parameter initial values
static inline void param_setup(u32 id, ParamValue v) {
  // set the input data so that bfin core will report it
  // back to the controller via SPI, when requested. 
  // (bees will make such a request of each param at launch to sync with network.)
  // this is also how a polled analysis parameter would work.
  gModuleData->paramData[id].value = v;
  module_set_param(id, v);
}


//----------------------
//----- external functions

void module_init(void) {
  // initialize module superclass data
  // by setting global pointers to our own data
  gModuleData = &super;
  gModuleData->paramData = mParamData;
  gModuleData->numParams = eParamNumParams;

  // initialize integrators & svf filters for audio processing
  filter_1p_lo_init( &(inSlew[0]), 0 );
  filter_1p_lo_init( &(inSlew[1]), 0 );
  filter_1p_lo_init( &(inSlew[2]), 0 );
  filter_1p_lo_init( &(inSlew[3]), 0 );

  filter_svf_init(&(filt[0]));
  filter_svf_init(&(filt[1]));
  filter_svf_init(&(filt[2]));
  filter_svf_init(&(filt[3]));

  // set rq and lowpass levels to static points
  filter_svf_set_rq(&(filt[0]), 0x7fffffff);
  filter_svf_set_rq(&(filt[1]), 0x7fffffff);
  filter_svf_set_rq(&(filt[2]), 0x7fffffff);
  filter_svf_set_rq(&(filt[3]), 0x7fffffff);

  filter_svf_set_low(&(filt[0]), 0x7fffffff);
  filter_svf_set_low(&(filt[1]), 0x7fffffff);
  filter_svf_set_low(&(filt[2]), 0x7fffffff);
  filter_svf_set_low(&(filt[3]), 0x7fffffff);

  // set amp to 1/4 (-12db) with right-shift intrinsic
  param_setup( eParam_gain0, PARAM_AMP_MAX >> 2 );
  param_setup( eParam_gain1, PARAM_AMP_MAX >> 2 );
  param_setup( eParam_gain2, PARAM_AMP_MAX >> 2 );
  param_setup( eParam_gain3, PARAM_AMP_MAX >> 2 );

  // ef0 to gain
  param_setup( eParam_ef0_gain0, 0 );
  param_setup( eParam_ef0_gain1, 0 );
  param_setup( eParam_ef0_gain2, 0 );
  param_setup( eParam_ef0_gain3, 0 );

  // ef1 to gain
  param_setup( eParam_ef1_gain0, 0 );
  param_setup( eParam_ef1_gain1, 0 );
  param_setup( eParam_ef1_gain2, 0 );
  param_setup( eParam_ef1_gain3, 0 );

  // ef2 to gain
  param_setup( eParam_ef2_gain0, 0 );
  param_setup( eParam_ef2_gain1, 0 );
  param_setup( eParam_ef2_gain2, 0 );
  param_setup( eParam_ef2_gain3, 0 );

  // ef3 to gain
  param_setup( eParam_ef3_gain0, 0 );
  param_setup( eParam_ef3_gain1, 0 );
  param_setup( eParam_ef3_gain2, 0 );
  param_setup( eParam_ef3_gain3, 0 );

  // ef0 to filt
  param_setup( eParam_ef0_filt0, 0 );
  param_setup( eParam_ef0_filt1, 0 );
  param_setup( eParam_ef0_filt2, 0 );
  param_setup( eParam_ef0_filt3, 0 );

  // ef1 to filt
  param_setup( eParam_ef1_filt0, 0 );
  param_setup( eParam_ef1_filt1, 0 );
  param_setup( eParam_ef1_filt2, 0 );
  param_setup( eParam_ef1_filt3, 0 );

  // ef2 to filt
  param_setup( eParam_ef2_filt0, 0 );
  param_setup( eParam_ef2_filt1, 0 );
  param_setup( eParam_ef2_filt2, 0 );
  param_setup( eParam_ef2_filt3, 0 );

  // ef3 to filt
  param_setup( eParam_ef3_filt0, 0 );
  param_setup( eParam_ef3_filt1, 0 );
  param_setup( eParam_ef3_filt2, 0 );
  param_setup( eParam_ef3_filt3, 0 );


  // adc filter cutoff
  param_setup( eParam_filter0, PARAM_CUT_MAX );
  param_setup( eParam_filter1, PARAM_CUT_MAX );
  param_setup( eParam_filter2, PARAM_CUT_MAX );
  param_setup( eParam_filter3, PARAM_CUT_MAX );

  // adc envelope slew time
  param_setup( eParam_slew0, PARAM_SLEW_DEFAULT );
  param_setup( eParam_slew1, PARAM_SLEW_DEFAULT );
  param_setup( eParam_slew2, PARAM_SLEW_DEFAULT );
  param_setup( eParam_slew3, PARAM_SLEW_DEFAULT );
}

// de-init (never actually used on blackfin, but maybe by emulator)
void module_deinit(void) {
  ;;
}

// get number of parameters
u32 module_get_num_params(void) {
  return eParamNumParams;
}



// frame process function! 
// called each audio frame from codec interrupt handler
// ( bad, i know, see github issues list )
void module_process_frame(void) { 
  static fract32 tmpGain, tmpSvf, toFilt;
  u8 i;

  //--- process envelope generators

  // take absolute values
  env[0] = abs_fr1x32( in[0] );
  env[1] = abs_fr1x32( in[1] );
  env[2] = abs_fr1x32( in[2] );
  env[3] = abs_fr1x32( in[3] );

  // pass absolute values to integrators
  filter_1p_lo_in( &(inSlew[0]), env[0] );
  filter_1p_lo_in( &(inSlew[1]), env[1] );
  filter_1p_lo_in( &(inSlew[2]), env[2] );
  filter_1p_lo_in( &(inSlew[3]), env[3] );

  // update integrators to find envelopes
  env[0] = filter_1p_lo_next( &(inSlew[0]) );
  env[1] = filter_1p_lo_next( &(inSlew[1]) );
  env[2] = filter_1p_lo_next( &(inSlew[2]) );
  env[3] = filter_1p_lo_next( &(inSlew[3]) );
  
  //--- mix
  // calculate gain and filter envelope levels then process volume and filters
  for(i=0; i<4; i++) {
    // multiply scalers by envelopes and add all 4 for gain
    tmpGain = inGain[i];
    tmpGain = add_fr1x32( tmpGain, mult_fr1x32x32( env[0], ef0_g[i] ) );
    tmpGain = add_fr1x32( tmpGain, mult_fr1x32x32( env[1], ef1_g[i] ) );
    tmpGain = add_fr1x32( tmpGain, mult_fr1x32x32( env[2], ef2_g[i] ) );
    tmpGain = add_fr1x32( tmpGain, mult_fr1x32x32( env[3], ef3_g[i] ) );

    // start with the static filter setting
    // add all four scaled envelopes for filter modulation
    tmpSvf = inFilt[i];
    tmpSvf = add_fr1x32( tmpSvf, mult_fr1x32x32( env[0], ef0_f[i] ) );
    tmpSvf = add_fr1x32( tmpSvf, mult_fr1x32x32( env[1], ef1_f[i] ) );
    tmpSvf = add_fr1x32( tmpSvf, mult_fr1x32x32( env[2], ef2_f[i] ) );
    tmpSvf = add_fr1x32( tmpSvf, mult_fr1x32x32( env[3], ef3_f[i] ) );

    // then set the svf coeff with the sum
    filter_svf_set_coeff( &(filt[i]), tmpSvf );

    // multiply adc in by sidechain mix
    toFilt = mult_fr1x32x32( tmpGain, in[i] );

    // send gained output through filter
    out[i] = filter_svf_next( &(filt[i]), toFilt );

  }

  //--- cv
  process_cv();
}


// parameter set function
void module_set_param(u32 idx, ParamValue v) {
  // switch on the param index
  switch(idx) {

    // set static gain levels
  case eParam_gain0 :
    inGain[0] = v;
    break;
  case eParam_gain1 :
    inGain[1] = v;
    break;
  case eParam_gain2 :
    inGain[2] = v;
    break;
  case eParam_gain3 :
    inGain[3] = v;
    break;

    // set static filter frequency
  case eParam_filter0 :
    inFilt[0] = v;
    break;
  case eParam_filter1 :
    inFilt[1] = v;
    break;
  case eParam_filter2 :
    inFilt[2] = v;
    break;
  case eParam_filter3 :
    inFilt[3] = v;
    break;

    // set envelope times
  case eParam_slew0 :
   filter_1p_lo_set_slew(&(inSlew[0]), v);
    break;
  case eParam_slew1 :
    filter_1p_lo_set_slew(&(inSlew[1]), v);
    break;
  case eParam_slew2 :
    filter_1p_lo_set_slew(&(inSlew[2]), v);
    break;
  case eParam_slew3 :
    filter_1p_lo_set_slew(&(inSlew[3]), v);
    break;


    // destination0
  case eParam_ef0_gain0 :
    ef0_g[0] = v;
    break;
  case eParam_ef1_gain0 :
    ef1_g[0] = v;
    break;
  case eParam_ef2_gain0 :
    ef2_g[0] = v;
    break;
  case eParam_ef3_gain0 :
    ef3_g[0] = v;
    break;
  case eParam_ef0_filt0 :
    ef0_f[0] = v;
    break;
  case eParam_ef1_filt0 :
    ef1_f[0] = v;
    break;
  case eParam_ef2_filt0 :
    ef2_f[0] = v;
    break;
  case eParam_ef3_filt0 :
    ef3_f[0] = v;
    break;

    // destination1
  case eParam_ef0_gain1 :
    ef0_g[1] = v;
    break;
  case eParam_ef1_gain1 :
    ef1_g[1] = v;
    break;
  case eParam_ef2_gain1 :
    ef2_g[1] = v;
    break;
  case eParam_ef3_gain1 :
    ef3_g[1] = v;
    break;
  case eParam_ef0_filt1 :
    ef0_f[1] = v;
    break;
  case eParam_ef1_filt1 :
    ef1_f[1] = v;
    break;
  case eParam_ef2_filt1 :
    ef2_f[1] = v;
    break;
  case eParam_ef3_filt1 :
    ef3_f[1] = v;
    break;

    // destination2
  case eParam_ef0_gain2 :
    ef0_g[2] = v;
    break;
  case eParam_ef1_gain2 :
    ef1_g[2] = v;
    break;
  case eParam_ef2_gain2 :
    ef2_g[2] = v;
    break;
  case eParam_ef3_gain2 :
    ef3_g[2] = v;
    break;
  case eParam_ef0_filt2 :
    ef0_f[2] = v;
    break;
  case eParam_ef1_filt2 :
    ef1_f[2] = v;
    break;
  case eParam_ef2_filt2 :
    ef2_f[2] = v;
    break;
  case eParam_ef3_filt2 :
    ef3_f[2] = v;
    break;

    // destination3
  case eParam_ef0_gain3 :
    ef0_g[3] = v;
    break;
  case eParam_ef1_gain3 :
    ef1_g[3] = v;
    break;
  case eParam_ef2_gain3 :
    ef2_g[3] = v;
    break;
  case eParam_ef3_gain3 :
    ef3_g[3] = v;
    break;
  case eParam_ef0_filt3 :
    ef0_f[3] = v;
    break;
  case eParam_ef1_filt3 :
    ef1_f[3] = v;
    break;
  case eParam_ef2_filt3 :
    ef2_f[3] = v;
    break;
  case eParam_ef3_filt3 :
    ef3_f[3] = v;
    break;

  default:
    break;
  }
}

//----- static function definitions

// update cv output
void process_cv(void) {  
  // send the current value to the cv driver
  cv_update( cvChan, env[cvChan] );

  // update the channel to be processed
  if(++cvChan == 4) {
    cvChan = 0;
  }
}
