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
  
  // obviously, these could be done in other ways ( easier to read/edit maybe. )
  // but neither code space nor speed is a big deal here.
  // this code can also be script-generated easily enough.

  /// input values have param type "Amp"
  // they range from 0 to the full positive value of fract32
  /// and will be displayed as dB by bees.
  strcpy(desc[eParam_gain0].label, "gain0");
  desc[eParam_gain0].type = eParamTypeAmp; 
  desc[eParam_gain0].min = 0x00000000; 
  desc[eParam_gain0].max = PARAM_AMP_MAX;
  desc[eParam_gain0].radix = 16; // doesn't matter

  strcpy(desc[eParam_gain1].label, "gain1");
  desc[eParam_gain1].type = eParamTypeAmp; 
  desc[eParam_gain1].min = 0x00000000; 
  desc[eParam_gain1].max = PARAM_AMP_MAX;
  desc[eParam_gain1].radix = 16; // doesn't matter

  strcpy(desc[eParam_gain2].label, "gain2");
  desc[eParam_gain2].type = eParamTypeAmp; 
  desc[eParam_gain2].min = 0x00000000; 
  desc[eParam_gain2].max = PARAM_AMP_MAX;
  desc[eParam_gain2].radix = 16; // doesn't matter

  strcpy(desc[eParam_gain3].label, "gain3");
  desc[eParam_gain3].type = eParamTypeAmp; 
  desc[eParam_gain3].min = 0x00000000; 
  desc[eParam_gain3].max = PARAM_AMP_MAX;
  desc[eParam_gain3].radix = 16; // doesn't matter


  // param type CUT for filter cutoff
  strcpy(desc[eParam_filter0].label, "filter0");
  desc[eParam_filter0].type = eParamTypeSvfFreq; 
  desc[eParam_filter0].min = 0x00000000; 
  desc[eParam_filter0].max = PARAM_CUT_MAX;
  desc[eParam_filter0].radix = 32;

  strcpy(desc[eParam_filter1].label, "filter1");
  desc[eParam_filter1].type = eParamTypeSvfFreq; 
  desc[eParam_filter1].min = 0x00000000; 
  desc[eParam_filter1].max = PARAM_CUT_MAX;
  desc[eParam_filter1].radix = 32;

  strcpy(desc[eParam_filter2].label, "filter2");
  desc[eParam_filter2].type = eParamTypeSvfFreq; 
  desc[eParam_filter2].min = 0x00000000; 
  desc[eParam_filter2].max = PARAM_CUT_MAX;
  desc[eParam_filter2].radix = 32;

  strcpy(desc[eParam_filter3].label, "filter3");
  desc[eParam_filter3].type = eParamTypeSvfFreq; 
  desc[eParam_filter3].min = 0x00000000; 
  desc[eParam_filter3].max = PARAM_CUT_MAX;
  desc[eParam_filter3].radix = 32;


  /// slew values have param type "Integrator"
  // they range from 0 to the full positive value of fract32
  /// and will be displayed as seconds-to-convergence by bees.

  strcpy(desc[eParam_slew0].label, "slew0");
  desc[eParam_slew0].type = eParamTypeIntegrator; 
  desc[eParam_slew0].min = 0x00000000; 
  desc[eParam_slew0].max = PARAM_SLEW_MAX;
  desc[eParam_slew0].radix = 16; // doesn't matter

  strcpy(desc[eParam_slew1].label, "slew1");
  desc[eParam_slew1].type = eParamTypeIntegrator; 
  desc[eParam_slew1].min = 0x00000000; 
  desc[eParam_slew1].max = PARAM_SLEW_MAX;
  desc[eParam_slew1].radix = 16; // doesn't matter

  strcpy(desc[eParam_slew2].label, "slew2");
  desc[eParam_slew2].type = eParamTypeIntegrator; 
  desc[eParam_slew2].min = 0x00000000; 
  desc[eParam_slew2].max = PARAM_SLEW_MAX; 
  desc[eParam_slew2].radix = 16; // doesn't matter

  strcpy(desc[eParam_slew3].label, "slew3");
  desc[eParam_slew3].type = eParamTypeIntegrator; 
  desc[eParam_slew3].min = 0x00000000; 
  desc[eParam_slew3].max = PARAM_SLEW_MAX;
  desc[eParam_slew3].radix = 16; // doesn't matter

// EF0 //
/// input values have param type "Amp"
  // they range from 0 to the full positive value of fract32
  /// and will be displayed as dB by bees.
  strcpy(desc[eParam_ef0_gain0].label, "ef0_gain0");
  desc[eParam_ef0_gain0].type = eParamTypeAmp; 
  desc[eParam_ef0_gain0].min = 0x00000000; 
  desc[eParam_ef0_gain0].max = PARAM_AMP_MAX;
  desc[eParam_ef0_gain0].radix = 16; // doesn't matter

  strcpy(desc[eParam_ef0_gain1].label, "ef0_gain1");
  desc[eParam_ef0_gain1].type = eParamTypeAmp; 
  desc[eParam_ef0_gain1].min = 0x00000000; 
  desc[eParam_ef0_gain1].max = PARAM_AMP_MAX;
  desc[eParam_ef0_gain1].radix = 16; // doesn't matter

  strcpy(desc[eParam_ef0_gain2].label, "ef0_gain2");
  desc[eParam_ef0_gain2].type = eParamTypeAmp; 
  desc[eParam_ef0_gain2].min = 0x00000000; 
  desc[eParam_ef0_gain2].max = PARAM_AMP_MAX;
  desc[eParam_ef0_gain2].radix = 16; // doesn't matter

  strcpy(desc[eParam_ef0_gain3].label, "ef0_gain3");
  desc[eParam_ef0_gain3].type = eParamTypeAmp; 
  desc[eParam_ef0_gain3].min = 0x00000000; 
  desc[eParam_ef0_gain3].max = PARAM_AMP_MAX;
  desc[eParam_ef0_gain3].radix = 16; // doesn't matter

/// input values have param type "Amp"
  // they range from 0 to the full positive value of fract32
  /// and will be displayed as dB by bees.
  strcpy(desc[eParam_ef0_filt0].label, "ef0_filt0");
  desc[eParam_ef0_filt0].type = eParamTypeAmp; 
  desc[eParam_ef0_filt0].min = 0x00000000; 
  desc[eParam_ef0_filt0].max = PARAM_AMP_MAX;
  desc[eParam_ef0_filt0].radix = 16; // doesn't matter

  strcpy(desc[eParam_ef0_filt1].label, "ef0_filt1");
  desc[eParam_ef0_filt1].type = eParamTypeAmp; 
  desc[eParam_ef0_filt1].min = 0x00000000; 
  desc[eParam_ef0_filt1].max = PARAM_AMP_MAX;
  desc[eParam_ef0_filt1].radix = 16; // doesn't matter

  strcpy(desc[eParam_ef0_filt2].label, "ef0_filt2");
  desc[eParam_ef0_filt2].type = eParamTypeAmp; 
  desc[eParam_ef0_filt2].min = 0x00000000; 
  desc[eParam_ef0_filt2].max = PARAM_AMP_MAX;
  desc[eParam_ef0_filt2].radix = 16; // doesn't matter

  strcpy(desc[eParam_ef0_filt3].label, "ef0_filt3");
  desc[eParam_ef0_filt3].type = eParamTypeAmp; 
  desc[eParam_ef0_filt3].min = 0x00000000; 
  desc[eParam_ef0_filt3].max = PARAM_AMP_MAX;
  desc[eParam_ef0_filt3].radix = 16; // doesn't matter


// EF1 //
/// input values have param type "Amp"
  // they range from 0 to the full positive value of fract32
  /// and will be displayed as dB by bees.
  strcpy(desc[eParam_ef1_gain0].label, "ef1_gain0");
  desc[eParam_ef1_gain0].type = eParamTypeAmp; 
  desc[eParam_ef1_gain0].min = 0x00000000; 
  desc[eParam_ef1_gain0].max = PARAM_AMP_MAX;
  desc[eParam_ef1_gain0].radix = 16; // doesn't matter

  strcpy(desc[eParam_ef1_gain1].label, "ef1_gain1");
  desc[eParam_ef1_gain1].type = eParamTypeAmp; 
  desc[eParam_ef1_gain1].min = 0x00000000; 
  desc[eParam_ef1_gain1].max = PARAM_AMP_MAX;
  desc[eParam_ef1_gain1].radix = 16; // doesn't matter

  strcpy(desc[eParam_ef1_gain2].label, "ef1_gain2");
  desc[eParam_ef1_gain2].type = eParamTypeAmp; 
  desc[eParam_ef1_gain2].min = 0x00000000; 
  desc[eParam_ef1_gain2].max = PARAM_AMP_MAX;
  desc[eParam_ef1_gain2].radix = 16; // doesn't matter

  strcpy(desc[eParam_ef1_gain3].label, "ef1_gain3");
  desc[eParam_ef1_gain3].type = eParamTypeAmp; 
  desc[eParam_ef1_gain3].min = 0x00000000; 
  desc[eParam_ef1_gain3].max = PARAM_AMP_MAX;
  desc[eParam_ef1_gain3].radix = 16; // doesn't matter

/// input values have param type "Amp"
  // they range from 0 to the full positive value of fract32
  /// and will be displayed as dB by bees.
  strcpy(desc[eParam_ef1_filt0].label, "ef1_filt0");
  desc[eParam_ef1_filt0].type = eParamTypeAmp; 
  desc[eParam_ef1_filt0].min = 0x00000000; 
  desc[eParam_ef1_filt0].max = PARAM_AMP_MAX;
  desc[eParam_ef1_filt0].radix = 16; // doesn't matter

  strcpy(desc[eParam_ef1_filt1].label, "ef1_filt1");
  desc[eParam_ef1_filt1].type = eParamTypeAmp; 
  desc[eParam_ef1_filt1].min = 0x00000000; 
  desc[eParam_ef1_filt1].max = PARAM_AMP_MAX;
  desc[eParam_ef1_filt1].radix = 16; // doesn't matter

  strcpy(desc[eParam_ef1_filt2].label, "ef1_filt2");
  desc[eParam_ef1_filt2].type = eParamTypeAmp; 
  desc[eParam_ef1_filt2].min = 0x00000000; 
  desc[eParam_ef1_filt2].max = PARAM_AMP_MAX;
  desc[eParam_ef1_filt2].radix = 16; // doesn't matter

  strcpy(desc[eParam_ef1_filt3].label, "ef1_filt3");
  desc[eParam_ef1_filt3].type = eParamTypeAmp; 
  desc[eParam_ef1_filt3].min = 0x00000000; 
  desc[eParam_ef1_filt3].max = PARAM_AMP_MAX;
  desc[eParam_ef1_filt3].radix = 16; // doesn't matter

// EF2 //
/// input values have param type "Amp"
  // they range from 0 to the full positive value of fract32
  /// and will be displayed as dB by bees.
  strcpy(desc[eParam_ef2_gain0].label, "ef2_gain0");
  desc[eParam_ef2_gain0].type = eParamTypeAmp; 
  desc[eParam_ef2_gain0].min = 0x00000000; 
  desc[eParam_ef2_gain0].max = PARAM_AMP_MAX;
  desc[eParam_ef2_gain0].radix = 16; // doesn't matter

  strcpy(desc[eParam_ef2_gain1].label, "ef2_gain1");
  desc[eParam_ef2_gain1].type = eParamTypeAmp; 
  desc[eParam_ef2_gain1].min = 0x00000000; 
  desc[eParam_ef2_gain1].max = PARAM_AMP_MAX;
  desc[eParam_ef2_gain1].radix = 16; // doesn't matter

  strcpy(desc[eParam_ef2_gain2].label, "ef2_gain2");
  desc[eParam_ef2_gain2].type = eParamTypeAmp; 
  desc[eParam_ef2_gain2].min = 0x00000000; 
  desc[eParam_ef2_gain2].max = PARAM_AMP_MAX;
  desc[eParam_ef2_gain2].radix = 16; // doesn't matter

  strcpy(desc[eParam_ef2_gain3].label, "ef2_gain3");
  desc[eParam_ef2_gain3].type = eParamTypeAmp; 
  desc[eParam_ef2_gain3].min = 0x00000000; 
  desc[eParam_ef2_gain3].max = PARAM_AMP_MAX;
  desc[eParam_ef2_gain3].radix = 16; // doesn't matter

/// input values have param type "Amp"
  // they range from 0 to the full positive value of fract32
  /// and will be displayed as dB by bees.
  strcpy(desc[eParam_ef2_filt0].label, "ef2_filt0");
  desc[eParam_ef2_filt0].type = eParamTypeAmp; 
  desc[eParam_ef2_filt0].min = 0x00000000; 
  desc[eParam_ef2_filt0].max = PARAM_AMP_MAX;
  desc[eParam_ef2_filt0].radix = 16; // doesn't matter

  strcpy(desc[eParam_ef2_filt1].label, "ef2_filt1");
  desc[eParam_ef2_filt1].type = eParamTypeAmp; 
  desc[eParam_ef2_filt1].min = 0x00000000; 
  desc[eParam_ef2_filt1].max = PARAM_AMP_MAX;
  desc[eParam_ef2_filt1].radix = 16; // doesn't matter

  strcpy(desc[eParam_ef2_filt2].label, "ef2_filt2");
  desc[eParam_ef2_filt2].type = eParamTypeAmp; 
  desc[eParam_ef2_filt2].min = 0x00000000; 
  desc[eParam_ef2_filt2].max = PARAM_AMP_MAX;
  desc[eParam_ef2_filt2].radix = 16; // doesn't matter

  strcpy(desc[eParam_ef2_filt3].label, "ef2_filt3");
  desc[eParam_ef2_filt3].type = eParamTypeAmp; 
  desc[eParam_ef2_filt3].min = 0x00000000; 
  desc[eParam_ef2_filt3].max = PARAM_AMP_MAX;
  desc[eParam_ef2_filt3].radix = 16; // doesn't matter

// EF3 //
/// input values have param type "Amp"
  // they range from 0 to the full positive value of fract32
  /// and will be displayed as dB by bees.
  strcpy(desc[eParam_ef3_gain0].label, "ef3_gain0");
  desc[eParam_ef3_gain0].type = eParamTypeAmp; 
  desc[eParam_ef3_gain0].min = 0x00000000; 
  desc[eParam_ef3_gain0].max = PARAM_AMP_MAX;
  desc[eParam_ef3_gain0].radix = 16; // doesn't matter

  strcpy(desc[eParam_ef3_gain1].label, "ef3_gain1");
  desc[eParam_ef3_gain1].type = eParamTypeAmp; 
  desc[eParam_ef3_gain1].min = 0x00000000; 
  desc[eParam_ef3_gain1].max = PARAM_AMP_MAX;
  desc[eParam_ef3_gain1].radix = 16; // doesn't matter

  strcpy(desc[eParam_ef3_gain2].label, "ef3_gain2");
  desc[eParam_ef3_gain2].type = eParamTypeAmp; 
  desc[eParam_ef3_gain2].min = 0x00000000; 
  desc[eParam_ef3_gain2].max = PARAM_AMP_MAX;
  desc[eParam_ef3_gain2].radix = 16; // doesn't matter

  strcpy(desc[eParam_ef3_gain3].label, "ef3_gain3");
  desc[eParam_ef3_gain3].type = eParamTypeAmp; 
  desc[eParam_ef3_gain3].min = 0x00000000; 
  desc[eParam_ef3_gain3].max = PARAM_AMP_MAX;
  desc[eParam_ef3_gain3].radix = 16; // doesn't matter

/// input values have param type "Amp"
  // they range from 0 to the full positive value of fract32
  /// and will be displayed as dB by bees.
  strcpy(desc[eParam_ef3_filt0].label, "ef3_filt0");
  desc[eParam_ef3_filt0].type = eParamTypeAmp; 
  desc[eParam_ef3_filt0].min = 0x00000000; 
  desc[eParam_ef3_filt0].max = PARAM_AMP_MAX;
  desc[eParam_ef3_filt0].radix = 16; // doesn't matter

  strcpy(desc[eParam_ef3_filt1].label, "ef3_filt1");
  desc[eParam_ef3_filt1].type = eParamTypeAmp; 
  desc[eParam_ef3_filt1].min = 0x00000000; 
  desc[eParam_ef3_filt1].max = PARAM_AMP_MAX;
  desc[eParam_ef3_filt1].radix = 16; // doesn't matter

  strcpy(desc[eParam_ef3_filt2].label, "ef3_filt2");
  desc[eParam_ef3_filt2].type = eParamTypeAmp; 
  desc[eParam_ef3_filt2].min = 0x00000000; 
  desc[eParam_ef3_filt2].max = PARAM_AMP_MAX;
  desc[eParam_ef3_filt2].radix = 16; // doesn't matter

  strcpy(desc[eParam_ef3_filt3].label, "ef3_filt3");
  desc[eParam_ef3_filt3].type = eParamTypeAmp; 
  desc[eParam_ef3_filt3].min = 0x00000000; 
  desc[eParam_ef3_filt3].max = PARAM_AMP_MAX;
  desc[eParam_ef3_filt3].radix = 16; // doesn't matter

}

// EOF
