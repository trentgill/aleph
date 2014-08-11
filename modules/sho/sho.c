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
static fract32 a1 = 0x1fffffff;
static fract32 a2 = 0x1fffffff;
static fract32 b0;
static fract32 b1;
static fract32 b2;

// time step
static fract32 k = 0x0000AEC3; // 1/48k represented as 32bit fract ~0.000021

// filter coefficient
static fract32 fc = 0x30000000; // the filter coefficient calculated on param change

// damping coefficients
static fract32 dcp = 0x20000000; // 1+sk: where sigma equals zero for infinite damping
static fract32 dcn = 0x20000000; // 1-sk

// runtime coefficients aka scale factors
static fract32 sf0 = 0x1fffffff;
static fract32 sf1 = 0x1fffffff;

// delay registers
static fract32 z[2] = { 0x26666680, 0x26666680 }; // initial state = 0.3

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
  static u32 ar1;
  static u32 br1;
  static u32 br2;
  static u32 calc;
  static fract32 coef;
  

  ar1 = shr_fr1x32(a1, 1);
  br1 = shr_fr1x32(a2, 1);
  br2 = shr_fr1x32(a2, 2);

  //coef = 0x7fffffff; // coef = 1 for around 7500Hz

  // coef = 2 - k^2 * (2 * pi * freq)^2

  // coef is really just accepting the _a1 between 0 and 1 where '1' is maximum acceptable frequency
  // then square it, and subtract it from 0.5
  // basically coef is everything inside the bracket that multiplies by u[n]
  //coef = sub_fr1x32( 0x40000000, mult_fr1x32x32( a1, a1 ));

  // out[0] = coef * u1 - u2
  // u2 = u1, u1 = u

  //out[0] = sub_fr1x32( mult_fr1x32x32( sf0 , z[0] ), mult_fr1x32x32( sf1, z[1] ) );
    
  //calc = mult_fr1x32x32( sub_fr1x32( 0x40000000, mult_fr1x32x32(ar1, ar1) ) / add_fr1x32( 0x40000000, br1 ), z[0] );
  //calc = sub_fr1x32(calc, mult_fr1x32x32( sub_fr1x32( 0x20000000, br2 ) / add_fr1x32( 0x40000000, br1 ), z[1] ) );


  // let s0k=0.01
  // 
  //sf0 = mult_fr1x32x32( sub_fr1x32( 0x40000000, mult_fr1x32x32( a1, a1 ) ), 0x40a3d700 );
  //out[0] = sub_fr1x32( mult_fr1x32x32( sf0 , z[0] ), mult_fr1x32x32( 0x3ebb9080, z[1] ) );

  // let s0k = 0
  sf1 = sub_fr1x32( 0x7fffffff, shr_fr1x32( a2, 4 ) ); // set (1-s)/(1+s) directly where 0 > s > 0.0625
  sf0 = mult_fr1x32x32( sub_fr1x32( 0x40000000, mult_fr1x32x32( a1, a1 ) ), sf1 );
  //out[0] = sub_fr1x32( mult_fr1x32x32( sf0 , z[0] ), mult_fr1x32x32( 0x20000000, z[1] ) );
  out[0] = sub_fr1x32( mult_fr1x32x32( sf0 , z[0] ), mult_fr1x32x32( shr_fr1x32( sf1, 2 ) , z[1] ) );


    // note that out[0] is actually only 1/2 of the proposed output
  out[0] = shl_fr1x32( out[0], 2 ); // multiplies out[0] by 2 for full scale output
  out[1] = out[0]; // copy to L & R

  // shift memory bits & scalers
  //z[1] = shr_fr1x32( z[0], 2 );
  z[1] = z[0];
  z[0] = out[0];


  // ∂tt*u = -w^2 * utt *u
  // ∂tt*u = -w^2 * ( a + (1 - a) ut•) * u
  // u{n+1} = (2 - a * w^2 * k^2) / (1 + (1 - a) * w^2 * k^2 / 2) * u{n} - u{n-1}










/*

  static fract32 w;

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
    //fc = sub_fr1x32( 0x40000000, mult_fr1x32x32( v, v ));

    // for testing, let s0k=0.1
    // therefore 
    //sf0 = 
    //calc_coeffs();
    a1 = v;
    break;
  case eParam_a2 :
    //dcp = add_fr1x32( 0x40000000, v );
    //dcn = sub_fr1x32( 0x40000000, v );
    //calc_coeffs();
    a2 = v;
    break;
  case eParam_b0 : // just forces the displacement to 0.3 
    z[0] = 0x26666680;
    z[1] = 0x26666680;
    //b0 = v;
    break;
  case eParam_b1 :
    b1 = v;
    break;
  case eParam_b2 :
    b2 = v;
    break;

  default:
    break;
  }
}

void calc_coeffs(void) {
  // here we perform the divisions to find the scaling factors for the process frame
  // do this separately to avoid expensive divisions in the runtime frame
  // could still cause process issue if called repetitively

  // stored in sf0 and sf1
  sf0 = fc / dcp;
  sf1 = shr_fr1x32( (dcn / dcp), 2 );

}