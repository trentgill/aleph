/* 
   SHO.c
   aleph-bfin

   finite difference scheme for a simple harmonic oscillator

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
typedef struct _shoData {
  //... here is where you would put other large data structures.

  // for example, a multichannel delay module would need an audio buffer:
  //  volatile fract32 audioBuffer[NUM_BUFS][FRAMES_PER_BUF];

  // bear in mind that access to SDRAM is significantly slower than SRAM (10-20 cycles!)
  // so don't put anything here that doesn't need the extra space.
} shoData;

//-------------------------
//----- extern vars (initialized here)


// global pointer to module descriptor.
// required by the aleph-bfin library!
ModuleData* gModuleData;

//-----------------------bfin_lib/src/
//------ static variables

static ModuleData super;
static ParamData mParamData[eParamNumParams];

// coefficients
static fract32 a1 = 0x1fffffff;
static fract32 a2 = 0x1fffffff;
static fract32 b0;
static fract32 b1;
static fract32 b2;

// runtime coefficients aka scale factors
static fract32 freqH = 0x10000000;
static fract32 freqM = 0x50000000;
static fract32 rM = 0x10000000;
//static fract32 sf0 = 0x1fffffff;
//static fract32 sf1 = 0x0fffffff;

// delay registers
static fract32 uM[3] = { 0, 0, 0 };
static fract32 uH[3] = { 0, 0xffe08af0, 0xfeb851ec };

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
  param_setup(eParam_a1, 0x30000000 );
  param_setup(eParam_a2, 0 );
  param_setup(eParam_b0, 0 ); // direct signal
  param_setup(eParam_b1, 0 );
  param_setup(eParam_b2, 0 );
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
  static fract32 diff, fn;

    // if diff is positive then uH and uM are in contact and interact -> fn is non-zero & positive
  //diff = sub_fr1x32( uH[1], uM[1] );
    // as wha1 is always positive, a negative diff means a negative fn.
    // in order to avoid if/else loop, we use a maximum function against 0.
    // for alpha=3 need to cube 'diff'
    // note that if alpha is even (eg. 2) fn will be positive and max function won't work.
  //fn = max_fr1x32( mult_fr1x32x32( freqH, diff ), 0 );

  diff = uH[1] - uM[1];
  if(uH[1] > uM[1] ) fn = mult_fr1x32x32( freqH, mult_fr1x32x32( mult_fr1x32x32( diff, diff ), diff ) );
  else fn = 0;
    // uH[0] = uH[1]/8 - uH[2]/16 - fn
  uH[0] = negate_fr1x32( sub_fr1x32( sub_fr1x32( shr_fr1x32( uH[1], 3 ), shr_fr1x32( uH[2], 4 ) ), fn ) ); // calc'd at 1/16 level // flipped sign
    // out[0] = uM[1]/8 - uM[2]/16 - freqM*uM[1]/16 + rM*fn
  out[0] = add_fr1x32( sub_fr1x32( sub_fr1x32( shr_fr1x32( uM[1], 3 ), shr_fr1x32( uM[2], 4 ) ), shr_fr1x32( mult_fr1x32x32( freqM, uM[1] ), 4 ) ), mult_fr1x32x32( rM, fn ) );

  out[0] = shl_fr1x32( out[0], 4 ); // multiplies out[0] by 16 for full scale output
  //out[1] = out[0]; // copy to L & R

  // shift memory bits for Mass and Hammer
  uM[2] = uM[2];
  uM[1] = out[0];

  uH[0] = shl_fr1x32( uH[0], 4 ); // *16 uH as calculation done at half level
  uH[1] = uH[2];
  uH[1] = uH[0];

  out[1] = uH[0];
}


// parameter set function
void module_set_param(u32 idx, ParamValue v) {
  
  // switch on the param index
  switch(idx) {

    // filter coefficients
  case eParam_a1 : // freqM
    freqM = v;
    //a1 = v;
    //calc_coeffs(); // only calculate new sf0 as sf1 stays unchanged
    break;
  case eParam_a2 : // freqH
    freqH = mult_fr1x32x32( mult_fr1x32x32( mult_fr1x32x32(v, v), v ), v );
    //a2 = v;
    //calc_coeffs(); // calculate both coefficients as they both rely on changes.
    break;
  case eParam_b0 : // mass ratio = rM
    rM = v;
    //b0 = v;
    break;
  case eParam_b1 :
    //b1 = v;
    break;
  case eParam_b2 : // reinstates initial condition (restart simulation)
    uM[0] = 0;
    uM[1] = 0; // Mass starts stationary
    uM[2] = 0;
    uH[0] = 0;
    uH[1] = 0xffe08af0; // init hammer position + k*hammer velocity ~ -0.001 + 2/48000 ~ 
    uH[2] = 0xfeb851ec; // init hammer position ~ -0.001

    //b2 = v;
    break;

  default:
    break;
  }
}
/*
void calc_coeff0(void) {
  sf0 = mult_fr1x32x32( sub_fr1x32( 0x40000000, mult_fr1x32x32( a1, a1 ) ), add_fr1x32( 0x40000000, shr_fr1x32( sf1, 1 ) ) );
}

void calc_coeffs(void) {
  sf1 = sub_fr1x32( 0x7fffffff, shr_fr1x32( a2, 6 ) ); // set (1-s)/(1+s) directly where 0 > s > 0.0625
  sf0 = mult_fr1x32x32( sub_fr1x32( 0x40000000, mult_fr1x32x32( a1, a1 ) ), add_fr1x32( 0x40000000, shr_fr1x32( sf1, 1 ) ) );
}



void calc_coeffs(void) {
    // 0 < wha1 < 1.
    // relates to: 0 < wha1 < 15278 Hz
    // aka hammer stiffness
    // wH ^ (1+alpha)
    // using alpha=1, 
  freqM = // aka w2k2
  freqH = // aka wha1
  rM = // aka MR
}

*/