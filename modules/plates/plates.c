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
#include "filter_svf.h"
#include "filter_ramp.h"
#include "delayFadeN.h"

// global declarations for module data
#include "module.h"

//--- custom headers
// parameter lists and constants
#include "params.h"


//-----------------------
//------ static variables

// total SDRAM is 64M
// each line about 10k frames ~220ms
#define PLATES_BUF_FRAMES 0x2710


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
  volatile fract32 audioBuffer[2][PLATES_BUF_FRAMES];

} platesData;

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

// pointer to all external memory
platesData* pPlatesData;
// delay lines (each has buffer descriptor and read/write taps)
static delayFadeN plates[2];
// state variable filters
static filter_svf svf[2];

// these are in SDRAM now
//static ModuleData super;
//static ParamData mParamData[eParamNumParams];

// input values
static fract32 adcVal[4];
static filter_1p_lo adcSlew[4];
static filter_1p_lo svfCutSlew[2]; // buffer filters
static filter_1p_lo svfRqSlew[2];

//--- crossfade stuff
/// which tap we are fading towards...
static u8 fadeTargetRd[2] = { 0, 0 };
static u8 fadeTargetWr[2] = { 0, 0 };

// crossfade integrators
/* filter_1p_lo lpFadeRd[2]; */
/* filter_1p_lo lpFadeWr[2]; */
static filter_ramp_tog lpFadeRd[2];
static filter_ramp_tog lpFadeWr[2];

// cv values (16 bits, but use fract32 and audio integrators)
static fract32 cvVal[4];
static filter_1p_lo cvSlew[4];
// current channel to update - see below in process_cv() 
static u8 cvChan = 0;

// audio output bus
static fract32 outBus = 0;

///////////////
// try this out
/// if set, param changes triggering fade will be ignored if a fade is in progress.
static const u8 fadeIgnore = 1;
////////////////


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
  u8 i;
  // init module/params
  pPlatesData = (platesData*)SDRAM_ADDRESS;

  // initialize module superclass data
  // by setting global pointers to our own data
  gModuleData = &(pPlatesData->super);
  strcpy(gModuleData->name, "aleph-plates");

  gModuleData->paramData = (ParamData*)pPlatesData->mParamData;
  gModuleData->numParams = eParamNumParams;

  for(i=0; i<2; i++) { // 2 is number of delay buffers
    delayFadeN_init(&(plates[i]), pPlatesData->audioBuffer[i], PLATES_BUF_FRAMES);
    filter_svf_init(&(svf[i]));

    filter_1p_lo_init(&(svfCutSlew[i]), 0x3fffffff);
    filter_1p_lo_init(&(svfRqSlew[i]), 0x3fffffff);

    filter_ramp_tog_init(&(lpFadeRd[i]), 0);
    filter_ramp_tog_init(&(lpFadeWr[i]), 0);

    /* // we really need to zero everything to avoid horrible noise at boot... */
    /* for(j=0; j<LINES_BUF_FRAMES; ++j) { */
    /*   pLinesData->audioBuffer[i][j] = 0; */
    /* } */

    //    memset(pLinesData->audioBuffer[i], 0, LINES_BUF_FRAMES * 4);

    // however, it is causing crashes or hangs here, for some damn reason.

    // at least zero the end of the buffer
    /* for(j=LINES_BUF_FRAMES - 100000; j < LINES_BUF_FRAMES - 1; ++j) { */
    /*   pLinesData->audioBuffer[i][j] = 0x00000000;  */
    /* } */
  }


////////////////////////

/// setup params with intial values

  param_setup( eParamFade0 , 0x100000 );
  param_setup( eParamFade1 , 0x100000 );

  param_setup(  eParam_loop0,   PARAM_SECONDS_MAX );
  //param_setup(  eParam_rMul0,   0x10000 );
  //param_setup(  eParam_rDiv0,   0x10000 );
  param_setup(  eParam_write0,    FRACT32_MAX );
  param_setup(  eParam_pre0,    0 );
  param_setup(  eParam_pos_write0,    0 );
  param_setup(  eParam_pos_read0,   0 );

  param_setup(  eParam_delay0,    0x4000 );

  param_setup(  eParam_run_read0, 1 );
  param_setup(  eParam_run_write0, 1 );

  param_setup(  eParam_loop1,   PARAM_SECONDS_MAX );
  //param_setup(  eParam_rMul1,   0x10000 );
  //param_setup(  eParam_rDiv1,   0x10000 );
  param_setup(  eParam_write1,    FRACT32_MAX );
  param_setup(  eParam_pre1,    0 );
  param_setup(  eParam_pos_write1,    0 );
  param_setup(  eParam_pos_read1,   0 );

  param_setup(  eParam_delay1,    0x4000 );

  param_setup(  eParam_run_read1, 1 );
  param_setup(  eParam_run_write1, 1 );

  param_setup(  eParam_freq1, PARAM_CUT_DEFAULT);
  param_setup(  eParam_rq1, PARAM_RQ_DEFAULT);
  param_setup(  eParam_low1,       PARAM_AMP_6 );
  param_setup(  eParam_high1, 0 );
  param_setup(  eParam_band1, 0 );
  param_setup(  eParam_notch1,  0 );
  //param_setup(  eParam_fwet1, PARAM_AMP_6 );
  //param_setup(  eParam_fdry1, PARAM_AMP_6 );

  param_setup(  eParam_freq0,   PARAM_CUT_DEFAULT );
  param_setup(  eParam_rq0,   PARAM_RQ_DEFAULT );
  param_setup(  eParam_low0,  FRACT32_MAX >> 1 );
  param_setup(  eParam_high0, 0 );
  param_setup(  eParam_band0, 0 );
  param_setup(  eParam_notch0,  0 );
  //param_setup(  eParam_fwet0, PARAM_AMP_6 );
  //param_setup(  eParam_fdry0, PARAM_AMP_6 );

  param_setup(  eParamCut0Slew, PARAM_SLEW_DEFAULT );
  param_setup(  eParamCut1Slew, PARAM_SLEW_DEFAULT );
  param_setup(  eParamRq0Slew, PARAM_SLEW_DEFAULT );
  param_setup(  eParamRq1Slew, PARAM_SLEW_DEFAULT );








  ////////////////////////



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


  // set initial param values
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
  static fract32 tmpDel, tmpSvf;
  u8 i;

  tmpDel = 0;
  tmpSvf = 0;


  for(i=0; i<2; i++) { // 2 is number of buffers
    // process fade integrator
    //    lines[i].fadeWr = filter_ramp_tog_next(&(lpFadeWr[i]));
    plates[i].fadeRd = filter_ramp_tog_next(&(lpFadeRd[i]));

    // process delay line
    tmpDel = delayFadeN_next( &(plates[i]), in[i]);      
    // process filters
    // check integrators for filter params
    /*
    if( !(svfCutSlew[i].sync) ) {
      filter_svf_set_coeff( &(svf[i]), filter_1p_lo_next(&(svfCutSlew[i])) );
    }
    if( !(svfRqSlew[i].sync) ) {
      filter_svf_set_rq( &(svf[i]), filter_1p_lo_next(&(svfRqSlew[i])) );
    }
    */
    tmpSvf = filter_svf_next( &(svf[i]), tmpDel);  
    // mix
    //tmpDel = mult_fr1x32x32( tmpDel, mix_fdry[i] ); 
    //tmpDel = add_fr1x32(tmpDel, mult_fr1x32x32(tmpSvf, mix_fwet[i]) );

    //out_del[i] = tmpSvf; // fully wet output
    //out[i] = add_fr1x32(in[i], tmpSvf);
  } // end buffers loop 

  //--- mix outputs
  
out[0] = in[0];
out[1] = in[1];



  //--- process slew
/*
  // 
  // update filters, calling "class method" on pointer to each
  adcVal[0] = filter_1p_lo_next( &(adcSlew[0]) );
  adcVal[1] = filter_1p_lo_next( &(adcSlew[1]) );
  adcVal[2] = filter_1p_lo_next( &(adcSlew[2]) );
  adcVal[3] = filter_1p_lo_next( &(adcSlew[3]) );
*/
  //--- mix

  // zero the output bus
  //outBus = 0;

  /* 
     note the use of fract32 arithmetic intrinsics!
     these are fast saturating multiplies/adds for 32bit signed fractions in [-1, 1)
     there are also intrinsics for fr16, mixed modes, and conversions.
     for details see:
     http://blackfin.uclinux.org/doku.php?id=toolchain:built-in_functions
  */
/*
  // scale each input and add it to the bus
  outBus = add_fr1x32( outBus, mult_fr1x32x32(in[0], adcVal[0]) );
  outBus = add_fr1x32( outBus, mult_fr1x32x32(in[1], adcVal[1]) );
  outBus = add_fr1x32( outBus, mult_fr1x32x32(in[2], adcVal[2]) );
  outBus = add_fr1x32( outBus, mult_fr1x32x32(in[3], adcVal[3]) );

  // copy the bus to all the outputs
  out[0] = outBus;
  out[1] = outBus;
  out[2] = outBus;
  out[3] = outBus;


  //--- cv
  process_cv();
*/
}




// check crossfade status of target
static u8 check_fade_rd(u8 id) {
  u8 newTarget, oldTarget = fadeTargetRd[id];
  //  u8 ret;

  if(lpFadeRd[id].sync) {
    // not fading right now, so pick different target and start crossfade
    newTarget =  oldTarget ^ 1;
    // copy all tap parameters to target
    buffer_tapN_copy( &(plates[id].tapRd[oldTarget]) ,  &(plates[id].tapRd[newTarget]) );
    // start the fade
    filter_ramp_tog_in(&(lpFadeRd[id]), newTarget);
    fadeTargetRd[id] = newTarget;
    return 1;
  } else {
    // fade is in progress.
    return !fadeIgnore;
  }
}


// parameter set function
void module_set_param(u32 idx, ParamValue v) {
  // switch on the param index
  switch(idx) {
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

//////////////////////

        // delay line params
  case eParam_delay0 :
    if( check_fade_rd(0) ) {
      delayFadeN_set_delay_sec(&(plates[0]), v,  fadeTargetRd[0]);
    }
    break;
  case eParam_delay1 :
    if(check_fade_rd(1)) {
      delayFadeN_set_delay_sec(&(plates[1]), v,  fadeTargetRd[1]);
    }
    break;
  case eParam_loop0 :
    delayFadeN_set_loop_sec(&(plates[0]), v, 0);
    delayFadeN_set_loop_sec(&(plates[0]), v, 1);
    break;
  case eParam_loop1 :
    delayFadeN_set_loop_sec(&(plates[1]), v , 0);
    delayFadeN_set_loop_sec(&(plates[1]), v , 1);
    break;
  case eParam_pos_write0 :
    // check_fade_wr(0);
    delayFadeN_set_pos_write_sec(&(plates[0]), v,  fadeTargetWr[0]);
    break;
  case eParam_pos_write1 :
    // check_fade_wr(1);
    delayFadeN_set_pos_write_sec(&(plates[1]), v,  fadeTargetWr[1] );
    break;
  case eParam_pos_read0 :
    if (check_fade_rd(0) ) {
      delayFadeN_set_pos_read_sec(&(plates[0]), v,  fadeTargetRd[0]);
    }
    break;
  case eParam_pos_read1 :
    if( check_fade_rd(1) ) {
      delayFadeN_set_pos_read_sec(&(plates[1]), v,  fadeTargetRd[1]);
    }
    break;
  case eParam_run_write0 :
    delayFadeN_set_run_write(&(plates[0]), v);
    break;
  case eParam_run_write1 :
    delayFadeN_set_run_write(&(plates[1]), v);
    break;
  case eParam_run_read0 :
    delayFadeN_set_run_read(&(plates[0]), v);
    break;
  case eParam_run_read1 :
    delayFadeN_set_run_read(&(plates[1]), v);
    break;





  case eParam_write0 :
    /// FIXME: need write level...
    delayFadeN_set_write(&(plates[0]), v > 0);
    break;
  case eParam_write1 :
    /// FIXME: need write level...gs
    delayFadeN_set_write(&(plates[1]), v > 0);
    break;
  case eParam_pre0 :
    if(v == FR32_MAX) {
      // negative == full
      delayFadeN_set_pre(&(plates[0]), -1);
    } else {
      delayFadeN_set_pre(&(plates[0]), v);
    }
    break;
  case eParam_pre1 :
    if(v == FR32_MAX) {
      // negative == full
      delayFadeN_set_pre(&(plates[1]), -1);
    } else {
      delayFadeN_set_pre(&(plates[1]), v);
    }
    break;
    // filter params
  case eParam_freq0 :
    //    filter_svf_set_coeff(&(svf[0]), v );
    filter_1p_lo_in(&(svfCutSlew[0]), v);
    break;
  case eParam_freq1 :
    //    filter_svf_set_coeff(&(svf[1]), v );
    filter_1p_lo_in(&(svfCutSlew[1]), v);
    break;
  case eParam_rq0 :
    //    filter_svf_set_rq(&(svf[0]), v);
    // incoming param value is 16.16
    // target is 2.30xs
    //    filter_svf_set_rq(&(svf[0]), v << 14);
    filter_1p_lo_in(&(svfRqSlew[0]), v << 14);
    break;
  case eParam_rq1 :
    //    filter_svf_set_rq(&(svf[1]), v);
    //    filter_svf_set_rq(&(svf[1]), v << 14);
    filter_1p_lo_in(&(svfRqSlew[1]), v << 14);
    break;
  case eParam_low0 :
    filter_svf_set_low(&(svf[0]), v);
    break;
  case eParam_low1 :
    filter_svf_set_low(&(svf[1]), v);
    break;
  case eParam_high0 :
    filter_svf_set_high(&(svf[0]), v);
    break;
  case eParam_high1 :
    filter_svf_set_high(&(svf[1]), v);
    break;
  case eParam_band0 :
    filter_svf_set_band(&(svf[0]), v);
    break;
  case eParam_band1 :
    filter_svf_set_band(&(svf[1]), v);
    break;
  case eParam_notch0 :
    filter_svf_set_notch(&(svf[0]), v);
    break;
  case eParam_notch1 :
    filter_svf_set_notch(&(svf[1]), v);
    break;



    // param integrators
  case eParamCut0Slew :
    filter_1p_lo_set_slew(&(svfCutSlew[0]), v);
    break;
  case eParamCut1Slew :
    filter_1p_lo_set_slew(&(svfCutSlew[1]), v);
    break;

  case eParamRq0Slew :
    filter_1p_lo_set_slew(&(svfRqSlew[0]), v);
    break;
  case eParamRq1Slew :
    filter_1p_lo_set_slew(&(svfRqSlew[1]), v);
    break;



    // fade times
    // FIXME: value is raw ramp increment per sample...
  case eParamFade0 :
    filter_ramp_tog_set_inc(&(lpFadeRd[0]), v );
    //    filter_ramp_tog_set_inc(&(lpFadeWr[0]), v );
    break;
  case eParamFade1 :
    filter_ramp_tog_set_inc(&(lpFadeRd[1]), v );
    //    filter_ramp_tog_set_inc(&(lpFadeWr[1]), v);
    break;






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

