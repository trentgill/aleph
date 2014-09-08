/* 
    plates.c
    aleph-bfin

    a plate reverb simulator with patchable i/o.

    inputs:
      filters, PD, ER & diffusion
      pre-delay, ER and diffusion
      early reflections & diffusion
      tank diffusion only

    outputs:
      post predelay
      post ER
      post diffusion L&R
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
#include "buffer.h"
//#include "filter_svf.h"
//#include "filter_ramp.h"
//#include "delayFadeN.h"
#include "allPass.h"

// global declarations for module data
#include "module.h"

//--- custom headers
// parameter lists and constants
#include "params.h"


//-----------------------
//------ static variables

// total SDRAM is 64M
// each line 32767 frames = ~128kB ~600ms
      // changed to 0x4a07 = 379 samples * 50
#define PLATES_BUF_FRAMES 0x00004A07
#define NPLATES 18


// customized module data structure
// this will be located at the top of SDRAM (64MB)
// all other variables (code, stack, heap) are located in SRAM (64K)
typedef struct _platesData {
  //... here is where you would put other large data structures.

  // for example, a multichannel delay module would need an audio buffer:
  //  volatile fract32 audioBuffer[NUM_BUFS][FRAMES_PER_BUF];

  // bear in mind that access to SDRAM is significantly slower than SRAM (10-20 cycles!)
  // so don't put anything here that doesn't need the extra space.

  ModuleData super;
  //  ParamDesc mParamDesc[eParamNumParams];
  ParamData mParamData[eParamNumParams];
  volatile fract32 audioBuffer[NPLATES][PLATES_BUF_FRAMES];

} platesData;

//-------------------------
//----- extern vars (initialized here)


// global pointer to module descriptor.
// required by the aleph-bfin library!
ModuleData* gModuleData;

//-----------------------bfin_lib/src/
//------ static variables

// pointer to all external memory
platesData* pPlatesData;
// delay lines (each has buffer descriptor and read/write taps)
allPass plates[NPLATES];

fract32 wetMix = 0;
fract32 dryMix = 1;
fract32 decay = 0;


filter_1p_lo inDamp[4];

//-----------------
//--- static function declaration

// update cv output
//static void process_cv(void);

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
  u8 i;
  u32 j;
  // init module/params
  pPlatesData = (platesData*)SDRAM_ADDRESS;

  // initialize module superclass data
  // by setting global pointers to our own data
  gModuleData = &(pPlatesData->super);
  strcpy(gModuleData->name, "aleph-plates");

  gModuleData->paramData = (ParamData*)pPlatesData->mParamData;
  gModuleData->numParams = eParamNumParams;

  for(i=0; i<NPLATES; i++) { // 2 is number of delay buffers
    allPass_init(&(plates[i]), pPlatesData->audioBuffer[i], PLATES_BUF_FRAMES);

    /* // we really need to zero everything to avoid horrible noise at boot... */
    for(j=0; j<PLATES_BUF_FRAMES; ++j) {
      pPlatesData->audioBuffer[i][j] = 0;
    }

    //    memset(pLinesData->audioBuffer[i], 0, LINES_BUF_FRAMES * 4);

    // however, it is causing crashes or hangs here, for some damn reason.

    // at least zero the end of the buffer
    /* for(j=LINES_BUF_FRAMES - 100000; j < LINES_BUF_FRAMES - 1; ++j) { */
    /*   pLinesData->audioBuffer[i][j] = 0x00000000;  */
    /* } */
  }
  for(i=0; i<4; i++) {
    filter_1p_lo_init( &(inDamp[i]), 0xf );
  }

  allPass_set_pre(&(plates[10]), 0x4fffffff); // 0.875
  allPass_set_pre(&(plates[12]), 0x2fffffff); // 0.5
  allPass_set_pre(&(plates[14]), 0x4fffffff);
  allPass_set_pre(&(plates[16]), 0x2fffffff);

////////////////////////

/// setup params with intial values

  param_setup(  eParam_preDel, 0x00000000 );
  param_setup(  eParam_time, 0x028F5c28 ); // equals 1x griesinger rates
  param_setup(  eParam_pre0, 0x3fffffff );
  param_setup(  eParam_dryMix, 0x3fffffff );
  param_setup(  eParam_wetMix, 0x3fffffff );
  param_setup(  eParam_inDamp, 0x0000000f );
  param_setup(  eParam_size, 0x1fffffff );
  param_setup(  eParam_decay, 0x1fffffff );

  //param_setup(  eParam_delay1,    0x4000 );
  //param_setup(  eParam_pre2,    0 );
  //param_setup(  eParam_delay2,    0x4000 );
  //param_setup(  eParam_pre3,    0 );
  //param_setup(  eParam_delay3,    0x4000 );
  //param_setup(  eParamCut0Slew, PARAM_SLEW_DEFAULT );

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
  u8 i;
  static fract32 tmpLeft, tmpRight, tankLeft, tankRight, ap1l, ap1r, ap2l, ap2r, d1l, d1r;

    // shift inputs right by 2 bit to add 12dB of headroom
  tmpLeft = shr_fr1x32( in[0], 2 );
  tmpRight = shr_fr1x32( in[1], 2 );

    // input damping
  filter_1p_lo_in( &(inDamp[0]), tmpLeft);
  filter_1p_lo_in( &(inDamp[1]), tmpRight);
  tmpLeft = filter_1p_lo_next( &(inDamp[0]));
  tmpRight = filter_1p_lo_next( &(inDamp[1]));

    // pre-delay
  tmpLeft = allPass_next( &(plates[8]), tmpLeft );
  tmpRight = allPass_next( &(plates[9]), tmpRight );


    // early reflections: left
  tmpLeft = allPass_next( &(plates[0]), tmpLeft );
  tmpLeft = allPass_next( &(plates[1]), tmpLeft );
  tmpLeft = allPass_next( &(plates[2]), tmpLeft );
  tmpLeft = allPass_next( &(plates[3]), tmpLeft );
    // early reflections: right
  tmpRight = allPass_next( &(plates[4]), tmpRight );
  tmpRight = allPass_next( &(plates[5]), tmpRight );
  tmpRight = allPass_next( &(plates[6]), tmpRight );
  tmpRight = allPass_next( &(plates[7]), tmpRight );


    // crossfed tank: left
  ap1l = allPass_next( &(plates[10]), add_fr1x32( tmpLeft, tankRight ) ); // allpass & mixer
  d1l = allPass_next( &(plates[11]), ap1l ); // delay
  filter_1p_lo_in( &inDamp[2], d1l ); // damping input
  tmpLeft = filter_1p_lo_next( &(inDamp[2]) ); //damping output
  tmpLeft = mult_fr1x32x32(tmpLeft, decay ); // decay scaling
  ap2l = allPass_next( &(plates[12]), tmpLeft ); // allpass
  tankLeft = allPass_next( &(plates[13]), ap2l ); // delay

    // crossfed tank: right
  ap1r = allPass_next( &(plates[14]), add_fr1x32( tmpRight, tankLeft ) ); // allpass & mixer
  d1r = allPass_next( &(plates[15]), ap1r ); // delay
  filter_1p_lo_in( &inDamp[3], d1r ); // damping input
  tmpRight = filter_1p_lo_next( &(inDamp[3]) ); //damping output
  tmpRight = mult_fr1x32x32(tmpRight, decay ); // decay scaling
  ap2r = allPass_next( &(plates[16]), tmpRight ); // allpass
  tankRight = allPass_next( &(plates[17]), ap2r ); // delay


    // output mixing & scaling up
  tmpLeft = add_fr1x32( tankLeft, add_fr1x32( ap1l, negate_fr1x32( add_fr1x32( ap2l, add_fr1x32( d1r, add_fr1x32( ap2r, tankRight ) ) ) ) ) );
  tmpRight = add_fr1x32( tankRight, add_fr1x32( ap1r, negate_fr1x32( add_fr1x32( ap2r, add_fr1x32( d1l, add_fr1x32( ap2l, tankLeft ) ) ) ) ) );

  tmpLeft = mult_fr1x32x32( tmpLeft, wetMix );
  tmpRight = mult_fr1x32x32( tmpRight, wetMix );

  in[0] = mult_fr1x32x32( in[0], dryMix );
  in[1] = mult_fr1x32x32( in[1], dryMix );

  out[0] = add_fr1x32( tmpLeft, in[0] );
  out[1] = add_fr1x32( tmpRight, in[1] );
}


// parameter set function
void module_set_param(u32 idx, ParamValue v) {
  // switch on the param index
  switch(idx) {

  case eParam_preDel :
    allPass_set_delay_samp(&(plates[8]), mult_fr1x32x32(v, 0x00004A06) );
    allPass_set_delay_samp(&(plates[9]), mult_fr1x32x32(v, 0x00004A06) );

  case eParam_time :
    allPass_set_delay_samp(&(plates[0]), mult_fr1x32x32(v, 0x00001BBC));
    allPass_set_delay_samp(&(plates[1]), mult_fr1x32x32(v, 0x000014EC));
    allPass_set_delay_samp(&(plates[2]), mult_fr1x32x32(v, 0x00004A06));
    allPass_set_delay_samp(&(plates[3]), mult_fr1x32x32(v, 0x0000361A));

    allPass_set_delay_samp(&(plates[4]), mult_fr1x32x32(v, 0x00001BBC));
    allPass_set_delay_samp(&(plates[5]), mult_fr1x32x32(v, 0x000014EC));
    allPass_set_delay_samp(&(plates[6]), mult_fr1x32x32(v, 0x00004A06));
    allPass_set_delay_samp(&(plates[7]), mult_fr1x32x32(v, 0x0000361A));
    break;

  case eParam_pre0 :
    if(v == FR32_MAX) {
      // negative == full
      allPass_set_pre(&(plates[0]), -1);
      allPass_set_pre(&(plates[1]), -1);
      allPass_set_pre(&(plates[4]), -1);
      allPass_set_pre(&(plates[5]), -1);
      allPass_set_pre(&(plates[2]), -1);
      allPass_set_pre(&(plates[3]), -1);
      allPass_set_pre(&(plates[6]), -1);
      allPass_set_pre(&(plates[7]), -1);      
    } else {
      allPass_set_pre(&(plates[0]), v);
      allPass_set_pre(&(plates[1]), v);
      allPass_set_pre(&(plates[4]), v);
      allPass_set_pre(&(plates[5]), v);
      v = mult_fr1x32x32( v, 0x5fffffff); // scale to 3/4
      allPass_set_pre(&(plates[2]), v );
      allPass_set_pre(&(plates[3]), v );
      allPass_set_pre(&(plates[6]), v );
      allPass_set_pre(&(plates[7]), v );
    }
    break;

  case eParam_dryMix :
    dryMix = v;
    break;
  case eParam_wetMix :
    wetMix = v;
    break;

    // param integrators
  case eParam_inDamp :
    filter_1p_lo_set_slew(&(inDamp[0]), v);
    filter_1p_lo_set_slew(&(inDamp[1]), v);
    filter_1p_lo_set_slew(&(inDamp[2]), v);
    filter_1p_lo_set_slew(&(inDamp[3]), v);
    break;

  case eParam_size :
    allPass_set_delay_samp(&(plates[10]), mult_fr1x32x32(v, 0x00000A80));
    allPass_set_delay_samp(&(plates[11]), mult_fr1x32x32(v, 0x00004594));
    allPass_set_delay_samp(&(plates[12]), mult_fr1x32x32(v, 0x00001C20));
    allPass_set_delay_samp(&(plates[13]), mult_fr1x32x32(v, 0x00003A20));

    allPass_set_delay_samp(&(plates[14]), mult_fr1x32x32(v, 0x00000E30));
    allPass_set_delay_samp(&(plates[15]), mult_fr1x32x32(v, 0x000041E4));
    allPass_set_delay_samp(&(plates[16]), mult_fr1x32x32(v, 0x00002980));
    allPass_set_delay_samp(&(plates[17]), mult_fr1x32x32(v, 0x0000316C));
    break;
  
  case eParam_decay :
    decay = v;
    break;

  default:
    break;
  }
}
