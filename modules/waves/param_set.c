#include <fract_math.h>

#include "dac.h"
#include "module.h"
#include "slew_bank.h"
#include "waves.h"

// set param values.

//-- hz
static inline void set_hz(const int id, const ParamValue v) {
  osc_set_hz ( &(osc[id]), v );
}

//-- tune
static inline void set_tune(const int id, const ParamValue v) {
  osc_set_tune ( &(osc[id]), v );
}

//-- voice amp env
static inline void set_amp(const int id, const ParamValue v) {
  //  *(voiceAmpIn[id]) = v; //trunc_fr1x32(v);
}

// waveshape
static inline void set_shape(const int id, const ParamValue v) {
  // input is [0,1] in 16.16, so just take fract part and discard sign
  //  *(shapeIn[id]) = (fract16) ((v & 0x0000ffff) >> 1);
}

// phase mod
static inline void set_pm(const int id, const ParamValue v) {
  // input is [0,1] in 16.16, so just take fract part and discard sign
  //  *(pmIn[id]) = (fract16) ((v & 0x0000ffff) >> 1);
}

// filter cutoff
static inline void set_cut(const int id, const ParamValue v) {
  //  *(svfCutIn[id]) =  v;
}

// filter rq
static inline void set_rq(const int id, const ParamValue v) {
  //  *(svfRqIn[id]) = v ;
}

// filter dry
static inline void set_dry(const int id, const ParamValue v) {
  //  *(svfDryIn[id]) = trunc_fr1x32(v);
}

// filter wet
static inline void set_wet(const int id, const ParamValue v) {
  //  *(svfWetIn[id]) = trunc_fr1x32(v);
}

// 16b slew param
static inline void set_slew16(const int voice, const int param, const ParamValue v) {
  //// FIXME:
  /// integrator coefficients in linear time are such that
  // we are discarding most of the param data this way.
  // need to add an additional class for 16b audio
  slew16.c[voice * WAVES_SLEW16_PER_VOICE + param] = trunc_fr1x32( v );
}

// 32b slew param
static inline void set_slew32(const int voice, const int param, const ParamValue v) {
  slew32.c[ (voice * WAVES_SLEW32_PER_VOICE) + param] = v;
}

void module_set_param(u32 idx, ParamValue v) {
  /// TEST:
  // don't.
  /*
  if( (idx != eParamHz0) 
      && (idx != eParamHz1) 
      && (idx != eParamHz0Slew) 
      && (idx != eParamHz1Slew) 
      ) {
    return;
  }
  */
  /// 


  switch(idx) {

  /// smoothers have to be processed first!
  case  eParamAmp0Slew :
    set_slew32(0, eSlew32_voiceAmp , v );
    break;
  case  eParamAmp1Slew :
    set_slew32(1, eSlew32_voiceAmp , v );
    break;

  case  eParamHz0Slew :
    set_slew32(0, eSlew32_oscInc , v );
    break;
  case  eParamHz1Slew :
    set_slew32(1, eSlew32_oscInc , v );
    break;

  case  eParamWave0Slew :
    set_slew16(0, eSlew16_oscShape , v );
    break;
  case  eParamWave1Slew :
    set_slew16(1, eSlew16_oscShape , v );
    break;

  case  eParamPm0Slew :
    set_slew16(0, eSlew16_oscPm , v );
    break;
  case  eParamPm1Slew :
    set_slew16(1, eSlew16_oscPm , v );
    break;

  case  eParamCut0Slew :
    set_slew32(0, eSlew32_svfCut , v );
    break;
  case  eParamCut1Slew :
    set_slew32(1, eSlew32_svfCut , v );
    break;


  case  eParamRq0Slew :
    set_slew32(0, eSlew32_svfRq , v );
    break;
  case  eParamRq1Slew :
    set_slew32(1, eSlew32_svfRq , v );
    break;

  case  eParamDry0Slew :
    set_slew16(0, eSlew16_svfDry , v );
    break;
  case  eParamDry1Slew :
    set_slew16(1, eSlew16_svfDry , v );
    break;

  case  eParamWet0Slew :
    set_slew16(0, eSlew16_svfWet , v );
    break;
  case  eParamWet1Slew :
    set_slew16(1, eSlew16_svfWet , v );
    break;

  // smoothing parameter for ALL mix values!
  case  eParamMixSlew :
    //... TODO
    break;

  /// osc out mix
  case  eParam_osc0_dac0 :
    mix_osc_dac[0][0] = v;
    break;
  case  eParam_osc0_dac1 :
    mix_osc_dac[0][1] = v;
    break;
  case  eParam_osc0_dac2 :
    mix_osc_dac[0][2] = v;
    break;
  case  eParam_osc0_dac3 :
    mix_osc_dac[0][3] = v;
    break;

  case  eParam_osc1_dac0 :
    mix_osc_dac[1][0] = v;
    break;
  case  eParam_osc1_dac1 :
    mix_osc_dac[1][1] = v;
    break;
  case  eParam_osc1_dac2 :
    mix_osc_dac[1][2] = v;
    break;
  case  eParam_osc1_dac3 :
    mix_osc_dac[1][3] = v;
    break;

  case  eParam_adc0_dac0		 :
    mix_adc_dac[0][0] = v;
    break;
  case  eParam_adc0_dac1		 :
    mix_adc_dac[0][1] = v;
    break;
  case  eParam_adc0_dac2		 :
    mix_adc_dac[0][2] = v;
    break;
  case  eParam_adc0_dac3		 :
    mix_adc_dac[0][3] = v;
    break;

  case  eParam_adc1_dac0		 :
    mix_adc_dac[1][0] = v;
    break;
  case  eParam_adc1_dac1		 :
    mix_adc_dac[1][1] = v;
    break;
  case  eParam_adc1_dac2		 :
    mix_adc_dac[1][2] = v;
    break;
  case  eParam_adc1_dac3		 :
    mix_adc_dac[1][3] = v;
    break;


  case  eParam_adc2_dac0		 :
    mix_adc_dac[2][0] = v;
    break;
  case  eParam_adc2_dac1		 :
    mix_adc_dac[2][1] = v;
    break;
  case  eParam_adc2_dac2		 :
    mix_adc_dac[2][2] = v;
    break;
  case  eParam_adc2_dac3		 :
    mix_adc_dac[2][3] = v;
    break;


  case  eParam_adc3_dac0		 :
    mix_adc_dac[3][0] = v;
    break;
  case  eParam_adc3_dac1		 :
    mix_adc_dac[3][1] = v;
    break;
  case  eParam_adc3_dac2		 :
    mix_adc_dac[3][2] = v;
    break;
  case  eParam_adc3_dac3		 :
    mix_adc_dac[3][3] = v;
    break;


  case eParam_cvVal0 :
    filter_1p_lo_in(&(cvSlew[0]), (v >> ( PARAM_DAC_RADIX - 1))  & DAC_VALUE_MASK);
    //cv_update(0, v >> (PARAM_CV_RADIX - 1));
    break;
  case eParam_cvVal1 :
    filter_1p_lo_in(&(cvSlew[1]), (v >> (PARAM_DAC_RADIX - 1)) & DAC_VALUE_MASK);
    //cv_update(1, v >> (PARAM_CV_RADIX - 1));
    break;
  case eParam_cvVal2 :
    filter_1p_lo_in(&(cvSlew[2]), (v >> (PARAM_DAC_RADIX - 1))  & DAC_VALUE_MASK);
    //cv_update(2, v >> (PARAM_CV_RADIX - 1));
    break;
  case eParam_cvVal3 :
    filter_1p_lo_in(&(cvSlew[3]), (v >> (PARAM_DAC_RADIX - 1))  & DAC_VALUE_MASK);
    //cv_update(3, v >> (PARAM_CV_RADIX - 1));
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


  // filter 0
  case  eParam_cut0 :
    set_cut(0, v);
    break;
  case  eParam_rq0 :
    set_rq(0, v);
    break;

  // filter 1
  case  eParam_cut1		 :
    set_cut(1, v);
    break;
  case  eParam_rq1		 :
    set_rq(1, v);
    break;

    ///////
    ///// FIXME: filter mode blend is inefficient and maybe unnecessary altogether.
  case eParam_low0 :
    filter_svf_set_low(&(svf[0]), v);
    break;
  case eParam_high1 :
    filter_svf_set_high(&(svf[1]), v);
    break;
  case eParam_high0 :
    filter_svf_set_high(&(svf[0]), v);
    break;
  case eParam_band1 :
    filter_svf_set_band(&(svf[1]), v);
    break;
  case eParam_band0 :
    filter_svf_set_band(&(svf[0]), v);
    break;
  case eParam_notch1 :
    filter_svf_set_notch(&(svf[1]), v);
    break;
  case eParam_notch0 :
    filter_svf_set_notch(&(svf[0]), v);
    break;
    ////////
    ////////

    // filter blend
  case eParam_fwet0 :
    set_wet(0, v);
  case eParam_fwet1 :
    set_wet(1, v);
  case eParam_fdry0 :
    set_dry(0, v);
  case eParam_fdry1 :
    set_dry(1, v);

  // osc parameters
  case  eParamPm0 :
    set_pm(0, v);
    break;
  case  eParamPm1 :
    set_pm(1, v);
    break;

  case  eParamWave0 :
    set_shape(0, v);
    break;
  case  eParamWave1 :
    set_shape(1, v);
    break;

  case  eParamAmp0 :
    set_amp(0, v);
    break;
  case  eParamAmp1 :
    set_amp(1, v);
    break;

  case eParamTune0:
    set_tune(0, v);
    break;
  case eParamTune1:
    set_tune(1, v);
     break;

  case eParamHz0:
    set_hz(0, v);
    break;
  case eParamHz1:
    set_hz(1, v);
    break;
  default:
    break;
  }
}

/*
void module_set_param(u32 idx, ParamValue v) {

#if 0
  switch(idx) {

  case eParamHzg1:
    osc_set_hz( &(osc[1]), v );
    break;
  case eParamHz0:
    osc_set_hz( &(osc[0]), v );
    break;

  case eParamTune1:
    osc_set_tune( &(osc[1]), v );
    break;
  case eParamTune0:
    osc_set_tune( &(osc[0]), v );
    break;

  case eParamWave1:
    osc_set_shape( &(osc[1]), BIT_ABS_32(FIX16_FRACT_TRUNC(v)) );
    break;
  case eParamWave0:
    osc_set_shape( &(osc[0]), BIT_ABS_32(FIX16_FRACT_TRUNC(v)) );
    break;

    // FIXME: this is assuming simple fixed x-modulation with 2 voices.
    /// should have a matrix of modulation mix points.
  case eParamPm10:
    osc_set_pm ( &(osc[0]), BIT_ABS_32(FIX16_FRACT_TRUNC(v)) );
    break;
  case eParamPm01:
    osc_set_pm ( &(osc[1]), BIT_ABS_32(FIX16_FRACT_TRUNC(v)) );
    break;

  case eParamWm10:
    //    osc_set_wm ( &(osc[0]), BIT_ABS_32(FIX16_FRACT_TRUNC(v)) );
    break;
  case eParamWm01:
    osc_set_wm ( &(osc[1]), BIT_ABS_32(FIX16_FRACT_TRUNC(v)) );
    break;

  case eParamAmp1:
    ampSlew[1].x = v;
    break;
  case eParamAmp0:
    ampSlew[0].x = v;
    break;

  case eParamBl1 :
    osc_set_bl( &(osc[1]), BIT_ABS_32(FIX16_FRACT_TRUNC(v)) );
    break;
  case eParamBl0 :
    osc_set_bl( &(osc[0]), BIT_ABS_32(FIX16_FRACT_TRUNC(v)) );
    break;

    //// filter params:
  case eParam_cut1 :
    cutSlew[1].x = v;
    break;
  case eParam_cut0 :
    cutSlew[0].x = v;
    break;

  case eParam_rq1 :
    // incoming param value is 16.16
    // target is 2.30
    rqSlew[1].x = v << 14;
    break;

  case eParam_rq0 :
    rqSlew[0].x = v << 14;
    break;

  case eParam_low1 :
    filter_svf_set_low(&(svf[1]), v);
    break;

  case eParam_low0 :
    filter_svf_set_low(&(svf[0]), v);
    break;
  case eParam_high1 :
    filter_svf_set_high(&(svf[1]), v);
    break;
  case eParam_high0 :
    filter_svf_set_high(&(svf[0]), v);
    break;
  case eParam_band1 :
    filter_svf_set_band(&(svf[1]), v);
    break;
  case eParam_band0 :
    filter_svf_set_band(&(svf[0]), v);
    break;
  case eParam_notch1 :
    filter_svf_set_notch(&(svf[1]), v);
    break;
  case eParam_notch0 :
    filter_svf_set_notch(&(svf[0]), v);
    break;

    // filter balance
  case eParam_fwet0 :
    fWet[0] = trunc_fr1x32(v);
    break;

  case eParam_fwet1 :
    fWet[1] = trunc_fr1x32(v);
    break;

  case eParam_fdry0 :
    fDry[0] = trunc_fr1x32( v );
    break;

  case eParam_fdry1 :
    fDry[1] = trunc_fr1x32( v );
    break;

    ///// slews
  case eParamHz1Slew:
    //    filter_1p_lo_set_slew( &(osc[1].lpInc), v); 
    osc[1].lpInc.c = v;
    break;
  case eParamHz0Slew:
    //    filter_1p_lo_set_slew( &(osc[0].lpInc), v); 
    osc[0].lpInc.c = v;
    break;

  case eParamPm01Slew:
    //    filter_1p_lo_set_slew( &(osc[1].lpPm), v); 
    osc[1].lpPm.c = v;
    break;
  case eParamPm10Slew:
    //    filter_1p_lo_set_slew( &(osc[0].lpPm), v); 
    osc[0].lpPm.c = v;
    break;

  case eParamWm01Slew:
    /// not using wave mod
    break;
  case eParamWm10Slew:
    /// not using wave mod
    break;

  case eParamWave1Slew:
    osc[1].lpShape.c = v;
    break;
  case eParamWave0Slew:
    osc[0].lpShape.c = v;
    break;

    //---- voice-param integrators
  case eParamAmp0Slew:
    ampSlew[0].c = v;
    break;
  case eParamAmp1Slew:
    ampSlew[1].c = v;
    break;
  case eParamCut0Slew :
    cutSlew[0].c = v;
    break;
  case eParamCut1Slew :
    cutSlew[1].c = v;
    break;
  case eParamRq0Slew :
    rqSlew[0].c = v;
    break;
  case eParamRq1Slew :
    rqSlew[1].c = v;
    break;

  case eParam_cvVal0 :
    filter_1p_lo_in(&(cvSlew[0]), (v >> ( PARAM_DAC_RADIX - 1))  & DAC_VALUE_MASK);
    //cv_update(0, v >> (PARAM_CV_RADIX - 1));
    break;
  case eParam_cvVal1 :
    filter_1p_lo_in(&(cvSlew[1]), (v >> (PARAM_DAC_RADIX - 1)) & DAC_VALUE_MASK);
    //cv_update(1, v >> (PARAM_CV_RADIX - 1));
    break;
  case eParam_cvVal2 :
    filter_1p_lo_in(&(cvSlew[2]), (v >> (PARAM_DAC_RADIX - 1))  & DAC_VALUE_MASK);
    //cv_update(2, v >> (PARAM_CV_RADIX - 1));
    break;
  case eParam_cvVal3 :
    filter_1p_lo_in(&(cvSlew[3]), (v >> (PARAM_DAC_RADIX - 1))  & DAC_VALUE_MASK);
    //cv_update(3, v >> (PARAM_CV_RADIX - 1));
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

   // i/o mix:
  case eParam_adc0_dac0 :
    mix_adc_dac[0][0] = trunc_fr1x32( v );
    //    ioAmp0 = trunc_fr1x32( v );
    break;
  case eParam_adc0_dac1 :
    mix_adc_dac[0][1] = trunc_fr1x32( v );
    break;
  case eParam_adc0_dac2 :
    mix_adc_dac[0][2] = trunc_fr1x32( v );
    break;
  case eParam_adc0_dac3 :
    mix_adc_dac[0][3] = trunc_fr1x32( v );
    break;
  case eParam_adc1_dac0 :
    mix_adc_dac[1][0] = trunc_fr1x32( v );
    break;
  case eParam_adc1_dac1 :
    mix_adc_dac[1][1] = trunc_fr1x32( v );
    //    ioAmp1 = trunc_fr1x32( v );
    break;
  case eParam_adc1_dac2 :
    mix_adc_dac[1][2] = trunc_fr1x32( v );
    break;
  case eParam_adc1_dac3 :
    mix_adc_dac[1][3] = trunc_fr1x32( v );
    break;
  case eParam_adc2_dac0 :
    mix_adc_dac[2][0] = trunc_fr1x32( v );
    break;
  case eParam_adc2_dac1 :
    mix_adc_dac[2][1] = trunc_fr1x32( v );
    break;
  case eParam_adc2_dac2 :
    mix_adc_dac[2][2] = trunc_fr1x32( v );
    //    ioAmp2 = trunc_fr1x32( v );
    break;
  case eParam_adc2_dac3 :
    mix_adc_dac[2][3] = trunc_fr1x32( v );
    break;
  case eParam_adc3_dac0 :
    mix_adc_dac[3][0] = trunc_fr1x32( v );
    break;
  case eParam_adc3_dac1 :
    mix_adc_dac[3][1] = trunc_fr1x32( v );
    break;
  case eParam_adc3_dac2 :
    mix_adc_dac[3][2] = trunc_fr1x32( v );
    break;
  case eParam_adc3_dac3 :
    mix_adc_dac[3][3] = trunc_fr1x32( v );
    //    ioAmp3 = trunc_fr1x32( v );
    break;
    // osc mix:
  case eParam_osc0_dac0 :
    mix_osc_dac[0][0] = trunc_fr1x32( v );
    break;
  case eParam_osc0_dac1 :
    mix_osc_dac[0][1] = trunc_fr1x32( v );
    break;
  case eParam_osc0_dac2 :
    mix_osc_dac[0][2] = trunc_fr1x32( v );
    break;
  case eParam_osc0_dac3 :
    mix_osc_dac[0][3] = trunc_fr1x32( v );
    break;
  case eParam_osc1_dac0 :
    mix_osc_dac[1][0] = trunc_fr1x32( v );
    break;
  case eParam_osc1_dac1 :
    mix_osc_dac[1][1] = trunc_fr1x32( v );
    break;
  case eParam_osc1_dac2 :
    mix_osc_dac[1][2] = trunc_fr1x32( v );
    break;
  case eParam_osc1_dac3 :
    mix_osc_dac[1][3] = trunc_fr1x32( v );
    break;


  default:
    break;
  }

#endif
}

*/
