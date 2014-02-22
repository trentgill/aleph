#include <fract_math.h>

#include "dac.h"
#include "module.h"
#include "slew_bank.h"
#include "waves.h"

// set param values.

//-- hz
static inline void set_hz(const int id, const ParamValue v) {
  //  osc_set_hz ( &(osc[id]), v );
  osc_bank_set_hz(id, v);
}

//-- tune
static inline void set_tune(const int id, const ParamValue v) {
  // osc_set_tune ( &(osc[id]), v );
  osc_bank_set_tune(id, v);
}

//-- voice amp env
static inline void set_amp(const int id, const ParamValue v) {
  *(voiceAmpIn[id]) = v; //trunc_fr1x32(v);
}

// waveshape
static inline void set_shape(const int id, const ParamValue v) {
  // input is [0,1] in 16.16, so just take fract part and discard sign
  *(shapeIn[id]) = (fract16) ((v & 0x0000ffff) >> 1);
}

// phase mod
static inline void set_pm(const int id, const ParamValue v) {
  // input is [0,1] in 16.16, so just take fract part and discard sign
  *(pmIn[id]) = (fract16) ((v & 0x0000ffff) >> 1);
}

// filter cutoff
static inline void set_cut(const int id, const ParamValue v) {
  *(svfCutIn[id]) = v;
}

// filter rq
static inline void set_rq(const int id, const ParamValue v) {
  *(svfRqIn[id]) = v ;
}

// filter dry
static inline void set_dry(const int id, const ParamValue v) {
  *(svfDryIn[id]) = trunc_fr1x32(v);
}

// filter wet
static inline void set_wet(const int id, const ParamValue v) {
  *(svfWetIn[id]) = trunc_fr1x32(v);
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

  case eParam_cvVal0 :
    filter_1p_lo_in(&(cvSlew[0]), (v >> ( PARAM_DAC_RADIX - 1))  & DAC_VALUE_MASK);
    break;
  case eParam_cvVal1 :
    filter_1p_lo_in(&(cvSlew[1]), (v >> (PARAM_DAC_RADIX - 1)) & DAC_VALUE_MASK);
    break;
  case eParam_cvVal2 :
    filter_1p_lo_in(&(cvSlew[2]), (v >> (PARAM_DAC_RADIX - 1))  & DAC_VALUE_MASK);
    break;
  case eParam_cvVal3 :
    filter_1p_lo_in(&(cvSlew[3]), (v >> (PARAM_DAC_RADIX - 1))  & DAC_VALUE_MASK);
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
  /* case eParam_low0 : */
  /*   filter_svf_set_low(&(svf[0]), v); */
  /*   break; */
  /* case eParam_high1 : */
  /*   filter_svf_set_high(&(svf[1]), v); */
  /*   break; */
  /* case eParam_high0 : */
  /*   filter_svf_set_high(&(svf[0]), v); */
  /*   break; */
  /* case eParam_band1 : */
  /*   filter_svf_set_band(&(svf[1]), v); */
  /*   break; */
  /* case eParam_band0 : */
  /*   filter_svf_set_band(&(svf[0]), v); */
  /*   break; */
  /* case eParam_notch1 : */
  /*   filter_svf_set_notch(&(svf[1]), v); */
  /*   break; */
  /* case eParam_notch0 : */
  /*   filter_svf_set_notch(&(svf[0]), v); */
  /*   break; */
    ////////
    ////////

    // filter blend
  case eParam_fwet0 :
    set_wet(0, v);
    break;
  case eParam_fwet1 :
    set_wet(1, v);
    break;
  case eParam_fdry0 :
    set_dry(0, v);
    break;
  case eParam_fdry1 :
    set_dry(1, v);
    break;

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
