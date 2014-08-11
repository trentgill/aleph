/*

  params.c
  
  defines the module parameter descriptor fields.
   
  this is done in the descriptor helper program, 
  *not* in the module code itself!

  */

#include <string.h>
#include "module.h"
#include "params.h" 

// this function
extern void fill_param_desc(ParamDesc* desc) {
    strcpy(desc[eParam_delay0].label, "delay0");
  desc[eParam_delay0].type = eParamTypeFix; 
  desc[eParam_delay0].min = 0x00000000; 
  desc[eParam_delay0].max = PARAM_SECONDS_MAX; 
  desc[eParam_delay0].radix = PARAM_SECONDS_RADIX;

  strcpy(desc[eParam_loop0].label, "loop0");
  desc[eParam_loop0].type = eParamTypeFix; 
  desc[eParam_loop0].min = 0x00000000; 
  desc[eParam_loop0].max = PARAM_SECONDS_MAX; 
  desc[eParam_loop0].radix = PARAM_SECONDS_RADIX;

  strcpy(desc[eParam_write0].label, "write0");
  desc[eParam_write0].type = eParamTypeAmp; 
  desc[eParam_write0].min = 0x00000000; 
  desc[eParam_write0].max = FR32_MAX;
  desc[eParam_write0].radix = 1;

  strcpy(desc[eParam_pre0].label, "pre0");
  desc[eParam_pre0].type = eParamTypeAmp; 
  desc[eParam_pre0].min = 0x00000000; 
  desc[eParam_pre0].max = FR32_MAX;
  desc[eParam_pre0].radix = 1;

  strcpy(desc[eParam_pos_write0].label, "pos_write0");
  desc[eParam_pos_write0].type = eParamTypeFix; 
  desc[eParam_pos_write0].min = 0x00000000; 
  desc[eParam_pos_write0].max = PARAM_SECONDS_MAX; 
  desc[eParam_pos_write0].radix = PARAM_SECONDS_RADIX;

  strcpy(desc[eParam_pos_read0].label, "pos_read0");
  desc[eParam_pos_read0].type = eParamTypeFix; 
  desc[eParam_pos_read0].min = 0x00000000; 
  desc[eParam_pos_read0].max = PARAM_SECONDS_MAX; 
  desc[eParam_pos_read0].radix = PARAM_SECONDS_RADIX;

  strcpy(desc[eParam_run_write0].label, "run_write0");
  desc[eParam_run_write0].type = eParamTypeBool; 
  desc[eParam_run_write0].min = 0;
  desc[eParam_run_write0].max = 1; 
  desc[eParam_run_write0].radix = 2; 

  strcpy(desc[eParam_run_read0].label, "run_read0");
  desc[eParam_run_read0].type = eParamTypeBool; 
  desc[eParam_run_read0].min = 0;
  desc[eParam_run_read0].max = 1; 
  desc[eParam_run_read0].radix = 2; 

  strcpy(desc[eParam_delay1].label, "delay1");
  desc[eParam_delay1].type = eParamTypeFix; 
  desc[eParam_delay1].min = 0x00000000; 
  desc[eParam_delay1].max = PARAM_SECONDS_MAX; 
  desc[eParam_delay1].radix = PARAM_SECONDS_RADIX;

  strcpy(desc[eParam_loop1].label, "loop1");
  desc[eParam_loop1].type = eParamTypeFix; 
  desc[eParam_loop1].min = 0x00000000; 
  desc[eParam_loop1].max = PARAM_SECONDS_MAX; 
  desc[eParam_loop1].radix = PARAM_SECONDS_RADIX;

  strcpy(desc[eParam_write1].label, "write1");
  desc[eParam_write1].type = eParamTypeAmp; 
  desc[eParam_write1].min = 0x00000000; 
  desc[eParam_write1].max = FR32_MAX;
  desc[eParam_write1].radix = 1;

  strcpy(desc[eParam_pre1].label, "pre1");
  desc[eParam_pre1].type = eParamTypeAmp; 
  desc[eParam_pre1].min = 0x00000000; 
  desc[eParam_pre1].max = FR32_MAX;
  desc[eParam_pre1].radix = 1;

  strcpy(desc[eParam_pos_write1].label, "pos_write1");
  desc[eParam_pos_write1].type = eParamTypeFix; 
  desc[eParam_pos_write1].min = 0x00000000; 
  desc[eParam_pos_write1].max = PARAM_SECONDS_MAX; 
  desc[eParam_pos_write1].radix = PARAM_SECONDS_RADIX;

  strcpy(desc[eParam_pos_read1].label, "pos_read1");
  desc[eParam_pos_read1].type = eParamTypeFix; 
  desc[eParam_pos_read1].min = 0x00000000; 
  desc[eParam_pos_read1].max = PARAM_SECONDS_MAX; 
  desc[eParam_pos_read1].radix = PARAM_SECONDS_RADIX;

  strcpy(desc[eParam_run_write1].label, "run_write1");
  desc[eParam_run_write1].type = eParamTypeBool; 
  desc[eParam_run_write1].min = 0;
  desc[eParam_run_write1].max = 1;
  desc[eParam_run_write1].radix = 2;

  strcpy(desc[eParam_run_read1].label, "run_read1");
  desc[eParam_run_read1].type = eParamTypeBool; 
  desc[eParam_run_read1].min = 0;
  desc[eParam_run_read1].max = 1;
  desc[eParam_run_read1].radix = 2;

  strcpy(desc[eParam_freq0].label, "cut0");
  desc[eParam_freq0].type = eParamTypeSvfFreq; 
  desc[eParam_freq0].min = 0x00000000; 
  desc[eParam_freq0].max = PARAM_CUT_MAX;
  desc[eParam_freq0].radix = 32;

  strcpy(desc[eParam_rq0].label, "rq0");
  desc[eParam_rq0].type = eParamTypeFix; 
  desc[eParam_rq0].min = PARAM_RQ_MIN;
  desc[eParam_rq0].max = PARAM_RQ_MAX;
  desc[eParam_rq0].radix = 2;
  
  strcpy(desc[eParam_low0].label, "low0");
  desc[eParam_low0].type = eParamTypeAmp; 
  desc[eParam_low0].min = 0x00000000; 
  desc[eParam_low0].max = FR32_MAX;
  desc[eParam_low0].radix = 1;

  strcpy(desc[eParam_high0].label, "high0");
  desc[eParam_high0].type = eParamTypeAmp; 
  desc[eParam_high0].min = 0x00000000; 
  desc[eParam_high0].max = FR32_MAX;
  desc[eParam_high0].radix = 1;

  strcpy(desc[eParam_band0].label, "band0");
  desc[eParam_band0].type = eParamTypeAmp; 
  desc[eParam_band0].min = 0x00000000; 
  desc[eParam_band0].max = FR32_MAX;
  desc[eParam_band0].radix = 1;

  strcpy(desc[eParam_notch0].label, "notch0");
  desc[eParam_notch0].type = eParamTypeAmp; 
  desc[eParam_notch0].min = 0x00000000; 
  desc[eParam_notch0].max = 0x00010000;
  desc[eParam_notch0].radix = 32;

  strcpy(desc[eParam_freq1].label, "cut1");
  desc[eParam_freq1].type = eParamTypeSvfFreq; 
  desc[eParam_freq1].min = 0x00000000; 
  desc[eParam_freq1].max = PARAM_CUT_MAX; 
  desc[eParam_freq1].radix = 1;

  ///// FIXME
  strcpy(desc[eParam_rq1].label, "rq1");
  desc[eParam_rq1].type = eParamTypeFix; 
  desc[eParam_rq1].min = PARAM_RQ_MIN;
  desc[eParam_rq1].max = PARAM_RQ_MAX; 
  desc[eParam_rq1].radix = 2;
 
  strcpy(desc[eParam_low1].label, "low1");
  desc[eParam_low1].type = eParamTypeAmp; 
  desc[eParam_low1].min = 0x00000000; 
  desc[eParam_low1].max = FR32_MAX;
  desc[eParam_low1].radix = 1;

  strcpy(desc[eParam_high1].label, "high1");
  desc[eParam_high1].type = eParamTypeAmp; 
  desc[eParam_high1].min = 0x00000000; 
  desc[eParam_high1].max = FR32_MAX;
  desc[eParam_high1].radix = 1;

  strcpy(desc[eParam_band1].label, "band1");
  desc[eParam_band1].type = eParamTypeAmp; 
  desc[eParam_band1].min = 0x00000000; 
  desc[eParam_band1].max = FR32_MAX;
  desc[eParam_band1].radix = 1;

  strcpy(desc[eParam_notch1].label, "notch1");
  desc[eParam_notch1].type = eParamTypeAmp; 
  desc[eParam_notch1].min = 0x00000000; 
  desc[eParam_notch1].max = FR32_MAX;
  desc[eParam_notch1].radix = 1;

  // filter slew

  strcpy(desc[eParamCut0Slew].label, "cut0Slew");
  desc[eParamCut0Slew].type = eParamTypeIntegrator;
  desc[eParamCut0Slew].min = 0;
  desc[eParamCut0Slew].max = FR32_MAX;
  desc[eParamCut0Slew].radix = 32;

  strcpy(desc[eParamRq0Slew].label, "rq0Slew");
  desc[eParamRq0Slew].type = eParamTypeIntegrator;
  desc[eParamRq0Slew].min = 0;
  desc[eParamRq0Slew].max = FR32_MAX;
  desc[eParamRq0Slew].radix = 32;

  strcpy(desc[eParamLow0Slew].label, "low0Slew");
  desc[eParamLow0Slew].type = eParamTypeIntegrator;
  desc[eParamLow0Slew].min = 0;
  desc[eParamLow0Slew].max = FR32_MAX;
  desc[eParamLow0Slew].radix = 32;

  strcpy(desc[eParamHigh0Slew].label, "high0Slew");
  desc[eParamHigh0Slew].type = eParamTypeIntegrator;
  desc[eParamHigh0Slew].min = 0;
  desc[eParamHigh0Slew].max = FR32_MAX;
  desc[eParamHigh0Slew].radix = 32;

  strcpy(desc[eParamBand0Slew].label, "band0Slew");
  desc[eParamBand0Slew].type = eParamTypeIntegrator;
  desc[eParamBand0Slew].min = 0;
  desc[eParamBand0Slew].max = FR32_MAX;
  desc[eParamBand0Slew].radix = 32;

  strcpy(desc[eParamNotch0Slew].label, "notch0Slew");
  desc[eParamNotch0Slew].type = eParamTypeIntegrator;
  desc[eParamNotch0Slew].min = 0;
  desc[eParamNotch0Slew].max = FR32_MAX;
  desc[eParamNotch0Slew].radix = 32;


  strcpy(desc[eParamCut1Slew].label, "cut1Slew");
  desc[eParamCut1Slew].type = eParamTypeIntegrator;
  desc[eParamCut1Slew].min = 0;
  desc[eParamCut1Slew].max = FR32_MAX;
  desc[eParamCut1Slew].radix = 32;

  strcpy(desc[eParamRq1Slew].label, "rq1Slew");
  desc[eParamRq1Slew].type = eParamTypeIntegrator;
  desc[eParamRq1Slew].min = 0;
  desc[eParamRq1Slew].max = FR32_MAX;
  desc[eParamRq1Slew].radix = 32;

  strcpy(desc[eParamLow1Slew].label, "low1Slew");
  desc[eParamLow1Slew].type = eParamTypeIntegrator;
  desc[eParamLow1Slew].min = 0;
  desc[eParamLow1Slew].max = FR32_MAX;
  desc[eParamLow1Slew].radix = 32;

  strcpy(desc[eParamHigh1Slew].label, "high1Slew");
  desc[eParamHigh1Slew].type = eParamTypeIntegrator;
  desc[eParamHigh1Slew].min = 0;
  desc[eParamHigh1Slew].max = FR32_MAX;
  desc[eParamHigh1Slew].radix = 32;

  strcpy(desc[eParamBand1Slew].label, "band1Slew");
  desc[eParamBand1Slew].type = eParamTypeIntegrator;
  desc[eParamBand1Slew].min = 0;
  desc[eParamBand1Slew].max = FR32_MAX;
  desc[eParamBand1Slew].radix = 32;

  strcpy(desc[eParamNotch1Slew].label, "notch1Slew");
  desc[eParamNotch1Slew].type = eParamTypeIntegrator;
  desc[eParamNotch1Slew].min = 0;
  desc[eParamNotch1Slew].max = FR32_MAX;
  desc[eParamNotch1Slew].radix = 32;









  // obviously, these could be done in other ways ( easier to read/edit maybe. )
  // but neither code space nor speed is a big deal here.
  // this code can also be script-generated easily enough.

  /// input values have param type "Amp"
  // they range from 0 to the full positive value of fract32
  /// and will be displayed as dB by bees.
  strcpy(desc[eParam_adc0].label, "adc0");
  desc[eParam_adc0].type = eParamTypeAmp; 
  desc[eParam_adc0].min = 0x00000000; 
  desc[eParam_adc0].max = PARAM_AMP_MAX;
  desc[eParam_adc0].radix = 16; // doesn't matter

  strcpy(desc[eParam_adc1].label, "adc1");
  desc[eParam_adc1].type = eParamTypeAmp; 
  desc[eParam_adc1].min = 0x00000000; 
  desc[eParam_adc1].max = PARAM_AMP_MAX;
  desc[eParam_adc1].radix = 16; // doesn't matter

  strcpy(desc[eParam_adc2].label, "adc2");
  desc[eParam_adc2].type = eParamTypeAmp; 
  desc[eParam_adc2].min = 0x00000000; 
  desc[eParam_adc2].max = PARAM_AMP_MAX;
  desc[eParam_adc2].radix = 16; // doesn't matter

  strcpy(desc[eParam_adc3].label, "adc3");
  desc[eParam_adc3].type = eParamTypeAmp; 
  desc[eParam_adc3].min = 0x00000000; 
  desc[eParam_adc3].max = PARAM_AMP_MAX;
  desc[eParam_adc3].radix = 16; // doesn't matter

  /// slew values have param type "Integrator"
  // they range from 0 to the full positive value of fract32
  /// and will be displayed as seconds-to-convergence by bees.

  strcpy(desc[eParam_adcSlew0].label, "adcSlew0");
  desc[eParam_adcSlew0].type = eParamTypeIntegrator; 
  desc[eParam_adcSlew0].min = 0x00000000; 
  desc[eParam_adcSlew0].max = PARAM_SLEW_MAX;
  desc[eParam_adcSlew0].radix = 16; // doesn't matter

  strcpy(desc[eParam_adcSlew1].label, "adcSlew1");
  desc[eParam_adcSlew1].type = eParamTypeIntegrator; 
  desc[eParam_adcSlew1].min = 0x00000000; 
  desc[eParam_adcSlew1].max = PARAM_SLEW_MAX;
  desc[eParam_adcSlew1].radix = 16; // doesn't matter

  strcpy(desc[eParam_adcSlew2].label, "adcSlew2");
  desc[eParam_adcSlew2].type = eParamTypeIntegrator; 
  desc[eParam_adcSlew2].min = 0x00000000; 
  desc[eParam_adcSlew2].max = PARAM_SLEW_MAX; 
  desc[eParam_adcSlew2].radix = 16; // doesn't matter

  strcpy(desc[eParam_adcSlew3].label, "adcSlew3");
  desc[eParam_adcSlew3].type = eParamTypeIntegrator; 
  desc[eParam_adcSlew3].min = 0x00000000; 
  desc[eParam_adcSlew3].max = PARAM_SLEW_MAX;
  desc[eParam_adcSlew3].radix = 16; // doesn't matter


  /// CV values have param type "Fixed" (for now!)
  // they range from 0 to 32k, the full range of bees values, integers only

  strcpy(desc[eParam_cv0].label, "cv0");
  desc[eParam_cv0].type = eParamTypeFix; 
  desc[eParam_cv0].min = 0x00000000; 
  desc[eParam_cv0].max = PARAM_CV_MAX ;
  desc[eParam_cv0].radix = 16; // full-scale 16.16

  strcpy(desc[eParam_cv1].label, "cv1");
  desc[eParam_cv1].type = eParamTypeFix; 
  desc[eParam_cv1].min = 0x00000000; 
  desc[eParam_cv1].max = PARAM_CV_MAX ;
  desc[eParam_cv1].radix = 16; // full-scale 16.16

  strcpy(desc[eParam_cv2].label, "cv2");
  desc[eParam_cv2].type = eParamTypeFix; 
  desc[eParam_cv2].min = 0x00000000; 
  desc[eParam_cv2].max = PARAM_CV_MAX ;
  desc[eParam_cv2].radix = 16; // full-scale 16.16

  strcpy(desc[eParam_cv3].label, "cv3");
  desc[eParam_cv3].type = eParamTypeFix; 
  desc[eParam_cv3].min = 0x00000000; 
  desc[eParam_cv3].max = PARAM_CV_MAX ;
  desc[eParam_cv3].radix = 16; // full-scale 16.16


  /// slew values have param type "Integrator"
  // they range from 0 to the full positive value of fract32
  /// and will be displayed as seconds-to-convergence by bees.

  strcpy(desc[eParam_cvSlew0].label, "cvSlew0");
  desc[eParam_cvSlew0].type = eParamTypeIntegrator; 
  desc[eParam_cvSlew0].min = 0x00000000; 
  desc[eParam_cvSlew0].max = PARAM_SLEW_MAX;
  desc[eParam_cvSlew0].radix = 16; // doesn't matter

  strcpy(desc[eParam_cvSlew1].label, "cvSlew1");
  desc[eParam_cvSlew1].type = eParamTypeIntegrator; 
  desc[eParam_cvSlew1].min = 0x00000000; 
  desc[eParam_cvSlew1].max = PARAM_SLEW_MAX;
  desc[eParam_cvSlew1].radix = 16; // doesn't matter

  strcpy(desc[eParam_cvSlew2].label, "cvSlew2");
  desc[eParam_cvSlew2].type = eParamTypeIntegrator; 
  desc[eParam_cvSlew2].min = 0x00000000; 
  desc[eParam_cvSlew2].max = PARAM_SLEW_MAX; 
  desc[eParam_cvSlew2].radix = 16; // doesn't matter

  strcpy(desc[eParam_cvSlew3].label, "cvSlew3");
  desc[eParam_cvSlew3].type = eParamTypeIntegrator; 
  desc[eParam_cvSlew3].min = 0x00000000; 
  desc[eParam_cvSlew3].max = PARAM_SLEW_MAX;
  desc[eParam_cvSlew3].radix = 16; // doesn't matter

  
  strcpy(desc[eParamFade0].label, "fade0");
  desc[eParamFade0].type = eParamTypeFix;
  desc[eParamFade0].min = PARAM_FADE_MIN;
  desc[eParamFade0].max = PARAM_FADE_MAX;
  desc[eParamFade0].radix = PARAM_FADE_RADIX;

  strcpy(desc[eParamFade1].label, "fade1");
  desc[eParamFade1].type = eParamTypeFix;
  desc[eParamFade1].min = PARAM_FADE_MIN;
  desc[eParamFade1].max = PARAM_FADE_MAX;
  desc[eParamFade1].radix = PARAM_FADE_RADIX;

}

// EOF
