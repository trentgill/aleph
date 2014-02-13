// set param values.
// this is a separate file for convenience only.

void module_set_param(u32 idx, ParamValue v) {
  switch(idx) {

  case eParamHz1:
    complex_osc_set_hz( osc[1], v );
    break;
  case eParamHz0:
    complex_osc_set_hz( osc[0], v );
    break;

  case eParamTune1:
    complex_osc_set_tune( (osc[1]), v );
    break;
  case eParamTune0:
    complex_osc_set_tune( (osc[0]), v );
    break;

  case eParamWave1:
    complex_osc_set_shape( (osc[1]), BIT_ABS_32(FIX16_FRACT_TRUNC(v)) );
    break;
  case eParamWave0:
    complex_osc_set_shape( (osc[0]), BIT_ABS_32(FIX16_FRACT_TRUNC(v)) );
    break;

    // FIXME: this is assuming simple fixed x-modulation with 2 voices.
    /// should have a matrix of modulation mix points.
  case eParamPm10:
    complex_osc_set_pm ( (osc[0]), BIT_ABS_32(FIX16_FRACT_TRUNC(v)) );
    break;
  case eParamPm01:
    complex_osc_set_pm ( (osc[1]), BIT_ABS_32(FIX16_FRACT_TRUNC(v)) );
    break;

  case eParamWm10:
    complex_osc_set_wm ( (osc[0]), BIT_ABS_32(FIX16_FRACT_TRUNC(v)) );
    break;
  case eParamWm01:
    complex_osc_set_wm ( (osc[1]), BIT_ABS_32(FIX16_FRACT_TRUNC(v)) );
    break;

  case eParamAmp1:
    //     filter_1p_lo_in(amp1Lp, v);
    //    filter_1p_lo_in(&(voice[1].ampSlew), v);
    ampSlew[1].x = v;
    
    break;
  case eParamAmp0:
    //    filter_1p_lo_in(amp0Lp, v);
    //    filter_1p_lo_in(&(voice[0].ampSlew), v);
    ampSlew[0].x = v;
    break;

  case eParamBl1 :
    //    complex_osc_set_bl( (osc[1]), BIT_ABS_32(FIX16_FRACT_TRUNC(v)) );
    break;
  case eParamBl0 :
    //    complex_osc_set_bl( (osc[0]), BIT_ABS_32(FIX16_FRACT_TRUNC(v)) );
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
    fWet[0] = v;
    break;

  case eParam_fwet1 :
    fWet[1] = v;
    break;

  case eParam_fdry0 :
    fDry[0] = v;
    break;

  case eParam_fdry1 :
    fDry[1] = v;
    break;

    ///// slews
  case eParamHz1Slew:
    osc[1].lpInc.c = v;
    break;
  case eParamHz0Slew:
    osc[0].lpInc.c = v;
    break;

  case eParamPm01Slew:
    osc[1].lpPm.c = v;
    break;
  case eParamPm10Slew:
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
    mix_adc_dac[0][0] = v;
    //    ioAmp0 = v;
    break;
  case eParam_adc0_dac1 :
    mix_adc_dac[0][1] = v;
    break;
  case eParam_adc0_dac2 :
    mix_adc_dac[0][2] = v;
    break;
  case eParam_adc0_dac3 :
    mix_adc_dac[0][3] = v;
    break;
  case eParam_adc1_dac0 :
    mix_adc_dac[1][0] = v;
    break;
  case eParam_adc1_dac1 :
    mix_adc_dac[1][1] = v;
    //    ioAmp1 = v;
    break;
  case eParam_adc1_dac2 :
    mix_adc_dac[1][2] = v;
    break;
  case eParam_adc1_dac3 :
    mix_adc_dac[1][3] = v;
    break;
  case eParam_adc2_dac0 :
    mix_adc_dac[2][0] = v;
    break;
  case eParam_adc2_dac1 :
    mix_adc_dac[2][1] = v;
    break;
  case eParam_adc2_dac2 :
    mix_adc_dac[2][2] = v;
    //    ioAmp2 = v;
    break;
  case eParam_adc2_dac3 :
    mix_adc_dac[2][3] = v;
    break;
  case eParam_adc3_dac0 :
    mix_adc_dac[3][0] = v;
    break;
  case eParam_adc3_dac1 :
    mix_adc_dac[3][1] = v;
    break;
  case eParam_adc3_dac2 :
    mix_adc_dac[3][2] = v;
    break;
  case eParam_adc3_dac3 :
    mix_adc_dac[3][3] = v;
    //    ioAmp3 = v;
    break;
    // osc mix:
  case eParam_osc0_dac0 :
    mix_osc_dac[0][0] = v;
    break;
  case eParam_osc0_dac1 :
    mix_osc_dac[0][1] = v;
    break;
  case eParam_osc0_dac2 :
    mix_osc_dac[0][2] = v;
    break;
  case eParam_osc0_dac3 :
    mix_osc_dac[0][3] = v;
    break;
  case eParam_osc1_dac0 :
    mix_osc_dac[1][0] = v;
    break;
  case eParam_osc1_dac1 :
    mix_osc_dac[1][1] = v;
    break;
  case eParam_osc1_dac2 :
    mix_osc_dac[1][2] = v;
    break;
  case eParam_osc1_dac3 :
    mix_osc_dac[1][3] = v;
    break;


  default:
    break;
  }

}
