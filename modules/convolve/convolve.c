/* 
   convolve.c
   aleph-bfin

   convolve the input signal with an 8 sample IR set by params.

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

// convolution values
static fract32 h[32];

// stored input values (buffer)
static fract32 hist[32] = { 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0 };

// input values
//static fract32 adcVal[4];
//static filter_1p_lo adcSlew[4];


// cv values (16 bits, but use fract32 and audio integrators)
//static fract32 cvVal[4];
//static filter_1p_lo cvSlew[4];
// current channel to update - see below in process_cv() 
//static u8 cvChan = 0;

// audio output bus
//static fract32 outBus = 0;

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
  param_setup(eParam_h0, PARAM_FIX_DEFAULT );
  param_setup(eParam_h1, 0 );
  param_setup(eParam_h2, 0 );
  param_setup(eParam_h3, 0 );
  param_setup(eParam_h4, 0 );
  param_setup(eParam_h5, 0 );
  param_setup(eParam_h6, 0 );
  param_setup(eParam_h7, 0 );

  param_setup(eParam_h8, PARAM_FIX_DEFAULT );
  param_setup(eParam_h9, 0 );
  param_setup(eParam_h10, 0 );
  param_setup(eParam_h11, 0 );
  param_setup(eParam_h12, 0 );
  param_setup(eParam_h13, 0 );
  param_setup(eParam_h14, 0 );
  param_setup(eParam_h15, 0 );

  param_setup(eParam_h16, PARAM_FIX_DEFAULT );
  param_setup(eParam_h17, 0 );
  param_setup(eParam_h18, 0 );
  param_setup(eParam_h19, 0 );
  param_setup(eParam_h20, 0 );
  param_setup(eParam_h21, 0 );
  param_setup(eParam_h22, 0 );
  param_setup(eParam_h23, 0 );

  param_setup(eParam_h24, PARAM_FIX_DEFAULT );
  param_setup(eParam_h25, 0 );
  param_setup(eParam_h26, 0 );
  param_setup(eParam_h27, 0 );
  param_setup(eParam_h28, 0 );
  param_setup(eParam_h29, 0 );
  param_setup(eParam_h30, 0 );
  param_setup(eParam_h31, 0 );

/*
  // initialize 1pole filters for input attenuation slew
  filter_1p_lo_init( &(adcSlew[0]), 0 );
  filter_1p_lo_init( &(adcSlew[1]), 0 );
  filter_1p_lo_init( &(adcSlew[2]), 0 );
  filter_1p_lo_init( &(adcSlew[3]), 0 );

  // initialize 1pole filters for cv output slew 
  filter_1p_lo_init( &(cvSlew[0]), 0 );
  filter_1p_lo_init( &(cvSlew[1]), 0 );
  filter_1p_lo_init( &(cvSlew[2]), 0 );
  filter_1p_lo_init( &(cvSlew[3]), 0 );


  // constants are from params.h
  param_setup(eParam_cv0, 0 );
  param_setup(eParam_cv1, 0 );
  param_setup(eParam_cv2, 0 );
  param_setup(eParam_cv3, 0 );

  // set amp to 1/4 (-12db) with right-shift intrinsic
  param_setup(eParam_adc0, PARAM_AMP_MAX >> 2 );
  param_setup(eParam_adc1, PARAM_AMP_MAX >> 2 );
  param_setup(eParam_adc2, PARAM_AMP_MAX >> 2 );
  param_setup(eParam_adc3, PARAM_AMP_MAX >> 2 );

  // set slew defaults. the value is pretty arbitrary
  param_setup(eParam_adcSlew0, PARAM_SLEW_DEFAULT);
  param_setup(eParam_adcSlew1, PARAM_SLEW_DEFAULT);
  param_setup(eParam_adcSlew2, PARAM_SLEW_DEFAULT);
  param_setup(eParam_adcSlew3, PARAM_SLEW_DEFAULT);
  param_setup(eParam_cvSlew0, PARAM_SLEW_DEFAULT);
  param_setup(eParam_cvSlew1, PARAM_SLEW_DEFAULT);
  param_setup(eParam_cvSlew2, PARAM_SLEW_DEFAULT);
  param_setup(eParam_cvSlew3, PARAM_SLEW_DEFAULT);
*/


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
  static fract32 cSig;
  u8 i;

  // convolution routine //

  // initial calculation uses input value
  cSig = mult_fr1x32x32(in[0], h[0] ); // coeff 0 to right

  // then subsequent calculations are based on hist[] buffer
  // this is a straight up MAC so could drastically accellerate in ASM?
  for(i=1; i<32; i++) {
    cSig = add_fr1x32(cSig, mult_fr1x32x32( hist[i], h[i] ) ); // add previous plus second convolution
  }

  // copy output to both sides
  out[1] = out[0] = cSig;

  // increment buffer storage
  for(i=31; i>1; i--) {
    hist[i] = hist[i-1]; // 7=6, 6=5 etc.
  }
  hist[1] = in[0]; // finally shift the current value into front of the buffer

}


// parameter set function
void module_set_param(u32 idx, ParamValue v) {
  // switch on the param index
  switch(idx) {

    // convolution coefficients
  case eParam_h0 :
    h[0] = v;
    break;
  case eParam_h1 :
    h[1] = v;
    break;
  case eParam_h2 :
    h[2] = v;
    break;
  case eParam_h3 :
    h[3] = v;
    break;
  case eParam_h4 :
    h[4] = v;
    break;
  case eParam_h5 :
    h[5] = v;
    break;
  case eParam_h6 :
    h[6] = v;
    break;
  case eParam_h7 :
    h[7] = v;
    break;


  case eParam_h8 :
    h[8] = v;
    break;
  case eParam_h9 :
    h[9] = v;
    break;
  case eParam_h10 :
    h[10] = v;
    break;
  case eParam_h11 :
    h[11] = v;
    break;
  case eParam_h12 :
    h[12] = v;
    break;
  case eParam_h13 :
    h[13] = v;
    break;
  case eParam_h14 :
    h[14] = v;
    break;
  case eParam_h15 :
    h[15] = v;
    break;


  case eParam_h16 :
    h[16] = v;
    break;
  case eParam_h17 :
    h[17] = v;
    break;
  case eParam_h18 :
    h[18] = v;
    break;
  case eParam_h19 :
    h[19] = v;
    break;
  case eParam_h20 :
    h[20] = v;
    break;
  case eParam_h21 :
    h[21] = v;
    break;
  case eParam_h22 :
    h[22] = v;
    break;
  case eParam_h23 :
    h[23] = v;
    break;
  
  case eParam_h24 :
    h[24] = v;
    break;
  case eParam_h25 :
    h[25] = v;
    break;
  case eParam_h26 :
    h[26] = v;
    break;
  case eParam_h27 :
    h[27] = v;
    break;
  case eParam_h28 :
    h[28] = v;
    break;
  case eParam_h29 :
    h[29] = v;
    break;
  case eParam_h30 :
    h[30] = v;
    break;
  case eParam_h31 :
    h[31] = v;
    break;

/*
    // cv output values
  case eParam_cv0 :
    filter_1p_lo_in( &(cvSlew[0]), v );
    break;
  case eParam_cv1 :
    filter_1p_lo_in( &(cvSlew[1]), v );
    break;
  case eParam_cv2 :
    filter_1p_lo_in( &(cvSlew[2]), v );
    break;
  case eParam_cv3 :
    filter_1p_lo_in( &(cvSlew[3]), v );

    // cv slew values
    break;
  case eParam_cvSlew0 :
   filter_1p_lo_set_slew(&(cvSlew[0]), v);
    break;
  case eParam_cvSlew1 :
    filter_1p_lo_set_slew(&(cvSlew[1]), v);
    break;
  case eParam_cvSlew2 :
    filter_1p_lo_set_slew(&(cvSlew[2]), v);
    break;
  case eParam_cvSlew3 :
    filter_1p_lo_set_slew(&(cvSlew[3]), v);
    break;

    // input attenuation values
  case eParam_adc0 :
    filter_1p_lo_in( &(adcSlew[0]), v );
    break;
  case eParam_adc1 :
    filter_1p_lo_in( &(adcSlew[1]), v );
    break;
  case eParam_adc2 :
    filter_1p_lo_in( &(adcSlew[2]), v );
    break;
  case eParam_adc3 :
    filter_1p_lo_in( &(adcSlew[3]), v );

    // input attenuation slew values
    break;
  case eParam_adcSlew0 :
   filter_1p_lo_set_slew(&(adcSlew[0]), v);
    break;
  case eParam_adcSlew1 :
    filter_1p_lo_set_slew(&(adcSlew[1]), v);
    break;
  case eParam_adcSlew2 :
    filter_1p_lo_set_slew(&(adcSlew[2]), v);
    break;
  case eParam_adcSlew3 :
    filter_1p_lo_set_slew(&(adcSlew[3]), v);
    break;
*/
  default:
    break;
  }
}

//----- static function definitions

//// NOTE / FIXME:
/* CV updates are staggered, each channel on a separate audio frame. 
   
   the reason for this is that each channel takes some time to update.
   for now, we just wait a full frame between channels,
   and effectively reduce CV output sampling rate by a factor of 4.
   (as well as changing the effecticve slew time, which is  not great.)

   the more proper way to do this would be:
   - enable DMA tx interrupt
   - each ISR calls the next channel to be loaded
   - last thing audio ISR does is call the first DAC channel to be loaded
   - cv_update writes to 4x16 volatile buffer

   this could give rise to its own problems:
   audio frame processing is currently interrupt-driven per frame,
   so CV tx interrupt could be masked by the next audio frame if the schedule is tight.
*/

/*
// update cv output
void process_cv(void) {
  // process the current channel
  // do nothing if the value is stable
  if( filter_1p_sync( &(cvSlew[cvChan]) ) ) {
    ;;
  } else {
    // update the slew filter and store the value
      cvVal[cvChan] = filter_1p_lo_next(&(cvSlew[cvChan]));
      // send the value to the cv driver
      cv_update( cvChan, cvVal[cvChan] );
  }

  // update the channel to be processed
  if(++cvChan == 4) {
    cvChan = 0;
  }
}

*/