/* waves.c
 * aleph-bfin
 * 
 * wavetable synthesis module
 */

// std
#include <math.h>
#include <stdlib.h>
#include <string.h>
// aleph-common
#include "fix.h"
// audio lib
#include "filter_1p.h"
#include "filter_svf.h"
#include "table.h"
#include "conversion.h"
// bfin
#include "bfin_core.h"
#include "dac.h"
#include "fract_math.h"
#include <fract2float_conv.h>

#include "module.h"
#include "module_custom.h"
#include "params.h"
#include "types.h"

// waves
#include "mix.h"
#include "osc_waves.h"
#include "waves.h"

//-------- data types

// define a local data structure that subclasses moduleData.
// use this for all data that is large and/or not speed-critical.
// this structure should statically allocate all necessary memory 
// so it can simply be loaded at the start of SDRAM.
typedef struct _wavesData {
  ModuleData super;
  ParamData mParamData[eParamNumParams];
} wavesData;

//-------------------------
//----- extern vars (initialized here)
ModuleData * gModuleData; // module data
//-----------------------
//------ static variables

// pointer to local module data, initialize at top of SDRAM
static wavesData * data;

//-- static allocation (SRAM) for variables that are small and/or frequently accessed:

/// FIXME: wavetables are statically linked constants for now.
/// would like to have them in SDRAM and allow arbitrary asynchronous load.
 const fract32 wavtab[WAVE_TAB_NUM][WAVE_TAB_SIZE] = { 
#include "wavtab_data_inc.c" 
};

// 10v dac values (u16, but use fract32 and audio integrators, for now)
fract32 cvVal[4];
filter_1p_lo cvSlew[4];
u8 cvChan = 0;

//----------------------
//----- static function declaration
// frame calculation
static void calc_frame(void);

// initial param set
static inline void param_setup(u32 id, ParamValue v) {
  gModuleData->paramData[id].value = v;
  module_set_param(id, v);
}


// frame calculation
static void calc_frame(void) {
  static u8 i;
  static fract32 o;

  // calculate slewed parameters
  slew_bank_16_calc_frame();
  slew_bank_32_calc_frame();

  
  // mix modulation input
  //  mix_mod();

  for(i=0; i<WAVES_VOICE_COUNT; ++i) {

    // phase mod with fixed 1-frame delay
    //    osc_pm_in( &(osc[i]), pmIn[i] );

    // calculate oscillator output
    o = oscOut[i] = shr_fr1x32( osc_next( &(osc[i]) ), 1);    

    // process SVF param integrators
    //    slew_exp_calc_frame( cutSlew[i] );
    //    slew_exp_calc_frame( rqSlew[i] );

    // set svf params
    //    filter_svf_set_coeff( &(svf[i]), (cutSlew[i]).y );
    //    filter_svf_set_rq(	  &(svf[i]), (rqSlew[i]).y );

    filter_svf_set_coeff( &(svf[i]), *(svfCutOut[i]) );
    filter_svf_set_rq(	  &(svf[i]), *(svfRqOut[i]) );
    // calculate svf output
    svfOut[i] = filter_svf_next( &(svf[i]), shr_fr1x32(o, 1) );

    // process amp smoother
    //    slew_exp_calc_frame( ampSlew[i] );
    //    voiceAmp[i] = (ampSlew[i]).y;

    // apply wet/dry/amp
    //// TEST:
    /* voiceOut[i] = mult_fr1x32x32( 0x7fffffff, */
    /* 				  add_fr1x32(mult_fr1x32( oscOut[i], 0x7fff ), */
    /* 					     mult_fr1x32( svfOut[i], 0x7fff ) */
    /* 					     ) */
    /* 				  ); */
    
    voiceOut[i] = mult_fr1x32x32(
				 *(voiceAmpOut[i]),
				  add_fr1x32(
					     mult_fr1x32( 
							 trunc_fr1x32(oscOut[i]), 
							 *(svfDryOut[i]) 
							  ),
					     mult_fr1x32( 
							 trunc_fr1x32(svfOut[i]), 
							  *(svfWetOut[i]) 
							  )
					     )
				  );
  }

  // mix outputs
  out[0] = out[1] = out[2] = out[3] = 0;

  //  mix_voice(voiceOut, out, (const fract16**)mix_osc_dac);
  mix_voice(voiceOut, out, (const fract16*) &(mix_osc_dac[0]) );
  mix_adc(in, out, (const fract16*) &(mix_adc_dac[0]) );
  
}

//----------------------
//----- external functions

void module_init(void) {
  int i;

  // init module/param descriptor
  // intialize local data at start of SDRAM
  data = (wavesData * )SDRAM_ADDRESS;
  // initialize moduleData superclass for core routines
  gModuleData = &(data->super);
  strcpy(gModuleData->name, "aleph-waves");
  gModuleData->paramData = data->mParamData;
  gModuleData->numParams = eParamNumParams;

  // init parameter slew
  slew_bank_init();
  // init oscillators
  init_osc();
  // init additional param slew pointers
  init_param_slew();

  for(i=0; i<WAVES_VOICE_COUNT; i++) {
    //    fract32 tmp = FRACT32_MAX >> 2;

    filter_svf_init( &(svf[i]) );
    //    voiceAmp[i] = tmp;

    /* slew_exp_init(ampSlew[i], PARAM_AMP_12); */
    /* slew_exp_init(cutSlew[i], PARAM_CUT_DEFAULT); */
    /* slew_exp_init(rqSlew[i], PARAM_RQ_DEFAULT); */
    
  }



  // dac
  filter_1p_lo_init( &(cvSlew[0]), 0xf );
  filter_1p_lo_init( &(cvSlew[1]), 0xf );
  filter_1p_lo_init( &(cvSlew[2]), 0xf );
  filter_1p_lo_init( &(cvSlew[3]), 0xf );

  // set parameters to defaults
  param_setup(  eParamHz1, 	220 << 16 );
  param_setup(  eParamHz0, 	330 << 16 );
  param_setup(  eParamTune1, 	FIX16_ONE );
  param_setup(  eParamTune0, 	FIX16_ONE );
  param_setup(  eParamWave1, 	0 );
  param_setup(  eParamWave0, 	0 );
  param_setup(  eParamAmp1, 	PARAM_AMP_6 );
  param_setup(  eParamAmp0, 	PARAM_AMP_6 );
  param_setup(  eParamPm0, 	0 );
  param_setup(  eParamPm1, 	0 );
 
  param_setup(  eParam_cut1,	PARAM_CUT_DEFAULT);
  param_setup(  eParam_rq1,	PARAM_RQ_DEFAULT);
  param_setup(  eParam_low1,       PARAM_AMP_6 );
  param_setup(  eParam_high1,	0 );
  param_setup(  eParam_band1,	0 );
  param_setup(  eParam_notch1,	0 );
  param_setup(  eParam_fwet1,	PARAM_AMP_6 );
  param_setup(  eParam_fdry1,	PARAM_AMP_6 );
  param_setup(  eParam_cut0, 	PARAM_CUT_DEFAULT );
  param_setup(  eParam_rq0, 	PARAM_RQ_DEFAULT );
  param_setup(  eParam_low0,	FRACT32_MAX >> 1 );
  param_setup(  eParam_high0,	0 );
  param_setup(  eParam_band0,	0 );
  param_setup(  eParam_notch0,	0 );
  param_setup(  eParam_fwet0,	PARAM_AMP_6 );
  param_setup(  eParam_fdry0,	PARAM_AMP_6 );

  param_setup(  eParamHz1Slew, PARAM_SLEW_DEFAULT );
  param_setup(  eParamHz0Slew, PARAM_SLEW_DEFAULT );
  param_setup(  eParamPm0Slew, 	PARAM_SLEW_DEFAULT );
  param_setup(  eParamPm1Slew, 	PARAM_SLEW_DEFAULT );

  param_setup(  eParamWave1Slew, PARAM_SLEW_DEFAULT );
  param_setup(  eParamWave0Slew, PARAM_SLEW_DEFAULT );
  param_setup(  eParamAmp1Slew, 	PARAM_SLEW_DEFAULT );
  param_setup(  eParamAmp0Slew, PARAM_SLEW_DEFAULT );
 
  param_setup(  eParamCut0Slew, PARAM_SLEW_DEFAULT );
  param_setup(  eParamCut1Slew, PARAM_SLEW_DEFAULT );
  param_setup(  eParamRq0Slew, PARAM_SLEW_DEFAULT );
  param_setup(  eParamRq1Slew, PARAM_SLEW_DEFAULT );

  param_setup(  eParam_adc0_dac0, 	FRACT32_MAX );
  param_setup(  eParam_adc1_dac1,  	FRACT32_MAX );
  param_setup(  eParam_adc2_dac2, 	FRACT32_MAX );
  param_setup(  eParam_adc3_dac3, 	FRACT32_MAX );

  param_setup(  eParam_osc0_dac0, 	FRACT32_MAX );
  param_setup(  eParam_osc0_dac1,  	FRACT32_MAX );
  param_setup(  eParam_osc0_dac2, 	FRACT32_MAX );
  param_setup(  eParam_osc0_dac3, 	FRACT32_MAX );
  param_setup(  eParam_osc1_dac0, 	FRACT32_MAX );
  param_setup(  eParam_osc1_dac1,  	FRACT32_MAX );
  param_setup(  eParam_osc1_dac2, 	FRACT32_MAX );
  param_setup(  eParam_osc1_dac3, 	FRACT32_MAX );

  param_setup(  eParam_cvVal0, 	FRACT32_MAX >> 1 );
  param_setup(  eParam_cvVal1, 	FRACT32_MAX >> 1 );
  param_setup(  eParam_cvVal2, 	FRACT32_MAX >> 1 );
  param_setup(  eParam_cvVal3, 	FRACT32_MAX >> 1 );
  param_setup(  eParam_cvSlew0, 	PARAM_SLEW_DEFAULT );
  param_setup(  eParam_cvSlew1, 	PARAM_SLEW_DEFAULT );
  param_setup(  eParam_cvSlew2, 	PARAM_SLEW_DEFAULT );
  param_setup(  eParam_cvSlew3, 	PARAM_SLEW_DEFAULT );

  

}

// de-init
void module_deinit(void) {
  /// why bother, it never happens!
  /* free(amp1Lp); */
  /* free(amp0Lp); */
}


// get number of parameters
extern u32 module_get_num_params(void) {
  return eParamNumParams;
}

// frame callback
void module_process_frame(void) {

  calc_frame();

  /* out[0] = add_fr1x32(frameVal, mult_fr1x32x32(in[0], ioAmp0)); */
  /* out[1] = add_fr1x32(frameVal, mult_fr1x32x32(in[1], ioAmp1)); */
  /* out[2] = add_fr1x32(frameVal, mult_fr1x32x32(in[2], ioAmp2)); */
  /* out[3] = add_fr1x32(frameVal, mult_fr1x32x32(in[3], ioAmp3)); */
  
  //  if(cvSlew[cvChan].sync) { ;; } else {
  //    cvVal[cvChan] = filter_1p_lo_next(&(cvSlew[cvChan]));
  //    dac_update(cvChan, cvVal[cvChan]);
  //  }

  // update one channel per sample... so, effective SR is 12khz...
  cvVal[cvChan] = filter_1p_lo_next(&(cvSlew[cvChan]));
  dac_update(cvChan, cvVal[cvChan]);
  cvChan = (cvChan + 1) & 0x3;
  
}

//#include "param_set.c"
