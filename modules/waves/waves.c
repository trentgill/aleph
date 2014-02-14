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
#include "osc.h"
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

#include "waves.h"


#define WAVES_NVOICES 2

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
static const fract32 wavtab[WAVE_TAB_NUM][WAVE_TAB_SIZE] = { 
#include "wavtab_data_inc.c" 
};

/// FIXME
// static fract32 frameVal;

// 10v dac values (u16, but use fract32 and audio integrators, for now)
static fract32 cvVal[4];
static filter_1p_lo cvSlew[4];
static u8 cvChan = 0;

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

#if 1

  // mix modulation input
  mix_mod();

  for(i=0; i<WAVES_NVOICES; ++i) {

    //////// FIXME: macroize osc ?

    // phase mod with fixed 1-frame delay
    osc_pm_in( &(osc[i]), pmIn[i] );

    // TODO: shape mod
    // TODO: amp mod
    // TODO: cutoff mod

    // calculate oscillator output
    o = oscOut[i] = shr_fr1x32( osc_next( &(osc[i]) ), 1);    

    // process SVF param integrators
    slew_exp_calc_frame( cutSlew[i] );
    slew_exp_calc_frame( rqSlew[i] );

    //////// FIXME: macroize SVF ?
    // set svf params
    filter_svf_set_coeff( &(svf[i]), (cutSlew[i]).y );
    filter_svf_set_rq(	  &(svf[i]), (rqSlew[i]).y );

    // calculate svf output
    svfOut[i] = filter_svf_next( &(svf[i]), shr_fr1x32(o, 1) );

    // process amp smoother
    slew_exp_calc_frame( ampSlew[i] );
    voiceAmp[i] = (ampSlew[i]).y;

    // apply wet/dry/amp
    //// TEST:
    /* voiceOut[i] = mult_fr1x32x32( 0x7fffffff, */
    /* 				  add_fr1x32(mult_fr1x32( oscOut[i], 0x7fff ), */
    /* 					     mult_fr1x32( svfOut[i], 0x7fff ) */
    /* 					     ) */
    /* 				  ); */
    
    voiceOut[i] = mult_fr1x32x32( voiceAmp[i],
				  add_fr1x32(mult_fr1x32( trunc_fr1x32(oscOut[i]), fDry[i] ),
					     mult_fr1x32( trunc_fr1x32(svfOut[i]), fWet[i] )
					     )
				  );
  }

  // mix outputs
  out[0] = out[1] = out[2] = out[3] = 0;
  mix_voice();
  mix_adc();	

#else
  /* wavesVoice* v; */

  /* for(i=0; i<WAVES_NVOICES; i++) { */
  /*   v = &(voice[i]); */
  /*   // oscillator class includes hz and mod integrators */
  /*   v->oscOut = shr_fr1x32( osc_next( &(v->osc) ), 2); */

  /*   // phase mod with fixed 1-frame delay */
  /*   osc_pm_in( &(v->osc), v->pmIn ); */
  /*   // shape mod with fixed 1-frame delay */
  /*   osc_wm_in( &(v->osc), v->wmIn ); */
  /*   // process filter integrators and set  */

  /*   slew_exp_calc_frame( v->cutSlew ); */
  /*   slew_exp_calc_frame( v->rqSlew ); */

  /*   filter_svf_set_coeff( &(v->svf), (v->cutSlew).y ); */
  /*   filter_svf_set_rq( &(v->svf), (v->rqSlew).y ); */

  /*   // process filter */
  /*   v->svfOut = filter_svf_next( &(v->svf), shr_fr1x32(v->oscOut, 1) ); */
  /*   // process amp smoother */
  /*   //    v->amp  = filter_1p_lo_next( &(v->ampSlew) ); */
  /*   slew_exp_calc_frame( v->ampSlew ); */
  /*   v->amp = (v->ampSlew).y; */

  /*   // mix to output bus */
  /*   v->out = mult_fr1x32x32(v->amp, */
  /* 			    add_fr1x32(mult_fr1x32x32( v->oscOut, v->fDry), */
  /* 				       mult_fr1x32x32( v->svfOut, v->fWet) */
  /* 				       ) */
  /* 			    ); */
  /* } // end voice loop */

  /* /// FIXME: later, more voices, mod matrix, arbitrary mod delay. */
  /* /// for now, simple direct mod feedback routing and 1-frame delay. */
  /* voice[0].pmIn = voice[1].oscOut; */
  /* //  voice[0].wmIn = voice[1].oscOut << 1; */
  /* voice[1].pmIn = voice[0].oscOut; */
  /* //  voice[1].wmIn = voice[0].oscOut << 1; */
  
  /* // mix outputs using matrix */
  /* mix_outputs(); */
#endif
  
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

  /* // fill param values with minima as default */
  /* for(i=0; i<eParamNumParams; ++i) { */
  /*   gModuleData->paramData[i].value = gModuleData->paramDesc[i].min; */
  /* } */

  for(i=0; i<WAVES_NVOICES; i++) {
    fract32 tmp = FRACT32_MAX >> 2;
    /* osc_init( &(voice[i].osc), &wavtab, SAMPLERATE ); */
    /* filter_svf_init( &(voice[i].svf) ); */
    /* voice[i].amp = tmp; */

    osc_init( &(osc[i]), &wavtab, SAMPLERATE );
    filter_svf_init( &(svf[i]) );
    voiceAmp[i] = tmp;

    /* filter_1p_lo_init(&(voice[i].ampSlew), 0xf); */
    /* filter_1p_lo_init(&(voice[i].cutSlew), 0xf); */
    /* filter_1p_lo_init(&(voice[i].rqSlew), 0xf); */
    slew_exp_init(ampSlew[i], PARAM_AMP_12);
    slew_exp_init(cutSlew[i], PARAM_CUT_DEFAULT);
    slew_exp_init(rqSlew[i], PARAM_RQ_DEFAULT);
    
  }

  //  oscAmp1 = oscAmp0 = INT32_MAX >> 2;

  // filters
  /* filter_svf_init(&(svf1)); */
  /* filter_svf_init(&(svf0));     */

  // allocate smoothers
  //  amp1Lp = (filter_1p_lo*)malloc(sizeof(filter_1p_lo));
  //  filter_1p_lo_init( amp1Lp, oscAmp1 );

  /* amp0Lp = (filter_1p_lo*)malloc(sizeof(filter_1p_lo)); */
  /* filter_1p_lo_init( amp0Lp, oscAmp0 ); */

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
  param_setup(  eParamPm10, 	0 );
  param_setup(  eParamPm01, 	0 );
  param_setup(  eParamWm10, 	0 );
  param_setup(  eParamWm01, 	0 );
  param_setup(  eParamBl1,  	0 );
  param_setup(  eParamBl0,  	0 );

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
  param_setup(  eParamPm10Slew, 	PARAM_SLEW_DEFAULT );
  param_setup(  eParamPm01Slew, 	PARAM_SLEW_DEFAULT );
  param_setup(  eParamWm10Slew, 	PARAM_SLEW_DEFAULT );
  param_setup(  eParamWm01Slew, 	PARAM_SLEW_DEFAULT );
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

  cvVal[cvChan] = filter_1p_lo_next(&(cvSlew[cvChan]));
  dac_update(cvChan, cvVal[cvChan]);
  cvChan = (cvChan + 1) & 0x3;
  
}

#include "param_set.c"
