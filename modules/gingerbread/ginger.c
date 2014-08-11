/* 
   filters.c
   aleph-bfin

   a basic implementation of a biquad filter

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
// global declarations for module data
#include "module.h"

//--- custom headers
// parameter lists and constants
#include "params.h"

// customized module data structure
// this will be located at the top of SDRAM (64MB)
// all other variables (code, stack, heap) are located in SRAM (64K)
typedef struct _convolveData {
  //... here is where you would put other large data structures.

  // for example, a multichannel delay module would need an audio buffer:
  //  volatile fract32 audioBuffer[NUM_BUFS][FRAMES_PER_BUF];

  // bear in mind that access to SDRAM is significantly slower than SRAM (10-20 cycles!)
  // so don't put anything here that doesn't need the extra space.
} convolveData;

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

// coefficients
static fract32 a1;
static fract32 a2;
static fract32 b0;
static fract32 b1;
static fract32 b2;

// delay registers
static fract32 z[2] = { 0, 0 };

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

  // set initial param values
  // constants are from params.h
  param_setup(eParam_a1, PARAM_FIX_MAX );
  param_setup(eParam_a2, PARAM_FIX_MAX );
  param_setup(eParam_b0, PARAM_FIX_DEFAULT ); // direct signal
  //param_setup(eParam_b1, 0 );
  //param_setup(eParam_b2, 0 );
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
  static fract32 w;

  // computes gingerbread man chaotic map in piece-wise linear fashion
  // initialises at 0,0 so need to modify the coefficients to push it out of stability
  // x[n] = 1 - a1*y[n-1] + a2*abs(x[n-1])
  // y[n] = x[n-1]
  // where z[0] is x[n-1]
  // and z[1] is y[n-1]

  out[0] = add_fr1x32( sub_fr1x32( b0, mult_fr1x32x32( a1, z[1] ) ), mult_fr1x32x32( a2, abs_fr1x32(z[0])) );
  out[1] = z[0];

  // modification for x[n-2] and y[n-2]
  z[0] = out[0];
  z[1] = out[1];


  
/*

  // computes direct form 2 of a biquad filter:
  // w[n] = x[n] - a1*w[n-1] - a2*w[n-2]
  // y[n] = b0*w[n] + b1*w[n-1] + b2*w[n-2]

  // should make this the same arrangement as max and build a BIQUAD operator to generate coeffs from mode, freq, rq

  w = sub_fr1x32( sub_fr1x32( in[0], mult_fr1x32x32( a1, z[0] ) ), mult_fr1x32x32( a2, z[1] ) );
  out[0] = add_fr1x32( add_fr1x32( mult_fr1x32x32( b0, w ), mult_fr1x32x32( b1, z[0] ) ), mult_fr1x32x32( b2, z[1] ) );
  out[1] = out[0];

  // shift delay buffer
  z[1] = z[0];
  z[0] = w;
*/
}


// parameter set function
void module_set_param(u32 idx, ParamValue v) {
  // switch on the param index
  switch(idx) {

    // filter coefficients
  case eParam_a1 :
    a1 = v;
    break;
  case eParam_a2 :
    a2 = v;
    break;
  case eParam_b0 :
    b0 = v;
    break;
/*  case eParam_b1 :
    b1 = v;
    break;
  case eParam_b2 :
    b2 = v;
    break;
*/
  default:
    break;
  }
}
