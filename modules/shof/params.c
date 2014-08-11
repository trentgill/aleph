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

  strcpy(desc[eParam_b2].label, "b2");
  desc[eParam_b2].type = eParamTypeFix; 
  desc[eParam_b2].min = PARAM_FIX_MIN; 
  desc[eParam_b2].max = PARAM_FIX_MAX;
  desc[eParam_b2].radix = 32; // -4 to 4

  strcpy(desc[eParam_b1].label, "b1");
  desc[eParam_b1].type = eParamTypeFix; 
  desc[eParam_b1].min = PARAM_FIX_MIN; 
  desc[eParam_b1].max = PARAM_FIX_MAX;
  desc[eParam_b1].radix = 32; // -4 to 4

  strcpy(desc[eParam_b0].label, "b0");
  desc[eParam_b0].type = eParamTypeFix; 
  desc[eParam_b0].min = PARAM_FIX_MIN; 
  desc[eParam_b0].max = PARAM_FIX_MAX;
  desc[eParam_b0].radix = 32; // -4 to 4

  strcpy(desc[eParam_a2].label, "a2");
  desc[eParam_a2].type = eParamTypeFix; 
  desc[eParam_a2].min = 0x00000000;  // could try -.5 as 0xC0000000
  desc[eParam_a2].max = 0x3fffffff; // max set to <0.5 to avoid overflow at 0.5
  desc[eParam_a2].radix = 32; // -4 to 4

  strcpy(desc[eParam_a1].label, "a1");
  desc[eParam_a1].type = eParamTypeFix; 
  desc[eParam_a1].min = 0x00000000; 
  desc[eParam_a1].max = PARAM_FIX_MAX;
  desc[eParam_a1].radix = 32; // -4 to 4



  // obviously, these could be done in other ways ( easier to read/edit maybe. )
  // but neither code space nor speed is a big deal here.
  // this code can also be script-generated easily enough.


/*
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

*/

/*

  strcpy(desc[eParam_h31].label, "h31");
  desc[eParam_h31].type = eParamTypeFix; 
  desc[eParam_h31].min = PARAM_FIX_MIN; 
  desc[eParam_h31].max = PARAM_FIX_MAX;
  desc[eParam_h31].radix = 32; // -4 to 4

  strcpy(desc[eParam_h30].label, "h30");
  desc[eParam_h30].type = eParamTypeFix; 
  desc[eParam_h30].min = PARAM_FIX_MIN; 
  desc[eParam_h30].max = PARAM_FIX_MAX;
  desc[eParam_h30].radix = 32; // -4 to 4

  strcpy(desc[eParam_h29].label, "h29");
  desc[eParam_h29].type = eParamTypeFix; 
  desc[eParam_h29].min = PARAM_FIX_MIN; 
  desc[eParam_h29].max = PARAM_FIX_MAX;
  desc[eParam_h29].radix = 32; // -4 to 4

  strcpy(desc[eParam_h28].label, "h28");
  desc[eParam_h28].type = eParamTypeFix; 
  desc[eParam_h28].min = PARAM_FIX_MIN; 
  desc[eParam_h28].max = PARAM_FIX_MAX;
  desc[eParam_h28].radix = 32; // -4 to 4

/// input values have param type "Fix"
  strcpy(desc[eParam_h27].label, "h27");
  desc[eParam_h27].type = eParamTypeFix; 
  desc[eParam_h27].min = PARAM_FIX_MIN; 
  desc[eParam_h27].max = PARAM_FIX_MAX;
  desc[eParam_h27].radix = 32; // -4 to 4

  strcpy(desc[eParam_h26].label, "h26");
  desc[eParam_h26].type = eParamTypeFix; 
  desc[eParam_h26].min = PARAM_FIX_MIN; 
  desc[eParam_h26].max = PARAM_FIX_MAX;
  desc[eParam_h26].radix = 32; // -4 to 4

  strcpy(desc[eParam_h25].label, "h25");
  desc[eParam_h25].type = eParamTypeFix; 
  desc[eParam_h25].min = PARAM_FIX_MIN; 
  desc[eParam_h25].max = PARAM_FIX_MAX;
  desc[eParam_h25].radix = 32; // -4 to 4
  
  /// input values have param type "Fix"
  strcpy(desc[eParam_h24].label, "h24");
  desc[eParam_h24].type = eParamTypeFix; 
  desc[eParam_h24].min = PARAM_FIX_MIN; 
  desc[eParam_h24].max = PARAM_FIX_MAX;
  desc[eParam_h24].radix = 32; // -4 to 4



  strcpy(desc[eParam_h23].label, "h23");
  desc[eParam_h23].type = eParamTypeFix; 
  desc[eParam_h23].min = PARAM_FIX_MIN; 
  desc[eParam_h23].max = PARAM_FIX_MAX;
  desc[eParam_h23].radix = 32; // -4 to 4

  strcpy(desc[eParam_h22].label, "h22");
  desc[eParam_h22].type = eParamTypeFix; 
  desc[eParam_h22].min = PARAM_FIX_MIN; 
  desc[eParam_h22].max = PARAM_FIX_MAX;
  desc[eParam_h22].radix = 32; // -4 to 4

  strcpy(desc[eParam_h21].label, "h21");
  desc[eParam_h21].type = eParamTypeFix; 
  desc[eParam_h21].min = PARAM_FIX_MIN; 
  desc[eParam_h21].max = PARAM_FIX_MAX;
  desc[eParam_h21].radix = 32; // -4 to 4

  strcpy(desc[eParam_h20].label, "h20");
  desc[eParam_h20].type = eParamTypeFix; 
  desc[eParam_h20].min = PARAM_FIX_MIN; 
  desc[eParam_h20].max = PARAM_FIX_MAX;
  desc[eParam_h20].radix = 32; // -4 to 4

  strcpy(desc[eParam_h19].label, "h19");
  desc[eParam_h19].type = eParamTypeFix; 
  desc[eParam_h19].min = PARAM_FIX_MIN; 
  desc[eParam_h19].max = PARAM_FIX_MAX;
  desc[eParam_h19].radix = 32; // -4 to 4

  strcpy(desc[eParam_h18].label, "h18");
  desc[eParam_h18].type = eParamTypeFix; 
  desc[eParam_h18].min = PARAM_FIX_MIN; 
  desc[eParam_h18].max = PARAM_FIX_MAX;
  desc[eParam_h18].radix = 32; // -4 to 4
  
  strcpy(desc[eParam_h17].label, "h17");
  desc[eParam_h17].type = eParamTypeFix; 
  desc[eParam_h17].min = PARAM_FIX_MIN; 
  desc[eParam_h17].max = PARAM_FIX_MAX;
  desc[eParam_h17].radix = 32; // -4 to 4
  
  strcpy(desc[eParam_h16].label, "h16");
  desc[eParam_h16].type = eParamTypeFix; 
  desc[eParam_h16].min = PARAM_FIX_MIN; 
  desc[eParam_h16].max = PARAM_FIX_MAX;
  desc[eParam_h16].radix = 32; // -4 to 4



  strcpy(desc[eParam_h15].label, "h15");
  desc[eParam_h15].type = eParamTypeFix; 
  desc[eParam_h15].min = PARAM_FIX_MIN; 
  desc[eParam_h15].max = PARAM_FIX_MAX;
  desc[eParam_h15].radix = 32; // -4 to 4

  strcpy(desc[eParam_h14].label, "h14");
  desc[eParam_h14].type = eParamTypeFix; 
  desc[eParam_h14].min = PARAM_FIX_MIN; 
  desc[eParam_h14].max = PARAM_FIX_MAX;
  desc[eParam_h14].radix = 32; // -4 to 4

  strcpy(desc[eParam_h13].label, "h13");
  desc[eParam_h13].type = eParamTypeFix; 
  desc[eParam_h13].min = PARAM_FIX_MIN; 
  desc[eParam_h13].max = PARAM_FIX_MAX;
  desc[eParam_h13].radix = 32; // -4 to 4

  strcpy(desc[eParam_h12].label, "h12");
  desc[eParam_h12].type = eParamTypeFix; 
  desc[eParam_h12].min = PARAM_FIX_MIN; 
  desc[eParam_h12].max = PARAM_FIX_MAX;
  desc[eParam_h12].radix = 32; // -4 to 4

/// input values have param type "Fix"
  strcpy(desc[eParam_h11].label, "h11");
  desc[eParam_h11].type = eParamTypeFix; 
  desc[eParam_h11].min = PARAM_FIX_MIN; 
  desc[eParam_h11].max = PARAM_FIX_MAX;
  desc[eParam_h11].radix = 32; // -4 to 4

  strcpy(desc[eParam_h10].label, "h10");
  desc[eParam_h10].type = eParamTypeFix; 
  desc[eParam_h10].min = PARAM_FIX_MIN; 
  desc[eParam_h10].max = PARAM_FIX_MAX;
  desc[eParam_h10].radix = 32; // -4 to 4

  strcpy(desc[eParam_h9].label, "h9");
  desc[eParam_h9].type = eParamTypeFix; 
  desc[eParam_h9].min = PARAM_FIX_MIN; 
  desc[eParam_h9].max = PARAM_FIX_MAX;
  desc[eParam_h9].radix = 32; // -4 to 4
  
  /// input values have param type "Fix"
  strcpy(desc[eParam_h8].label, "h8");
  desc[eParam_h8].type = eParamTypeFix; 
  desc[eParam_h8].min = PARAM_FIX_MIN; 
  desc[eParam_h8].max = PARAM_FIX_MAX;
  desc[eParam_h8].radix = 32; // -4 to 4



  strcpy(desc[eParam_h7].label, "h7");
  desc[eParam_h7].type = eParamTypeFix; 
  desc[eParam_h7].min = PARAM_FIX_MIN; 
  desc[eParam_h7].max = PARAM_FIX_MAX;
  desc[eParam_h7].radix = 32; // -4 to 4

  strcpy(desc[eParam_h6].label, "h6");
  desc[eParam_h6].type = eParamTypeFix; 
  desc[eParam_h6].min = PARAM_FIX_MIN; 
  desc[eParam_h6].max = PARAM_FIX_MAX;
  desc[eParam_h6].radix = 32; // -4 to 4

  strcpy(desc[eParam_h5].label, "h5");
  desc[eParam_h5].type = eParamTypeFix; 
  desc[eParam_h5].min = PARAM_FIX_MIN; 
  desc[eParam_h5].max = PARAM_FIX_MAX;
  desc[eParam_h5].radix = 32; // -4 to 4

  strcpy(desc[eParam_h4].label, "h4");
  desc[eParam_h4].type = eParamTypeFix; 
  desc[eParam_h4].min = PARAM_FIX_MIN; 
  desc[eParam_h4].max = PARAM_FIX_MAX;
  desc[eParam_h4].radix = 32; // -4 to 4

/// input values have param type "Fix"
  strcpy(desc[eParam_h3].label, "h3");
  desc[eParam_h3].type = eParamTypeFix; 
  desc[eParam_h3].min = PARAM_FIX_MIN; 
  desc[eParam_h3].max = PARAM_FIX_MAX;
  desc[eParam_h3].radix = 32; // -4 to 4

  strcpy(desc[eParam_h2].label, "h2");
  desc[eParam_h2].type = eParamTypeFix; 
  desc[eParam_h2].min = PARAM_FIX_MIN; 
  desc[eParam_h2].max = PARAM_FIX_MAX;
  desc[eParam_h2].radix = 32; // -4 to 4

  strcpy(desc[eParam_h1].label, "h1");
  desc[eParam_h1].type = eParamTypeFix; 
  desc[eParam_h1].min = PARAM_FIX_MIN; 
  desc[eParam_h1].max = PARAM_FIX_MAX;
  desc[eParam_h1].radix = 32; // -4 to 4
  
  /// input values have param type "Fix"
  strcpy(desc[eParam_h0].label, "h0");
  desc[eParam_h0].type = eParamTypeFix; 
  desc[eParam_h0].min = PARAM_FIX_MIN; 
  desc[eParam_h0].max = PARAM_FIX_MAX;
  desc[eParam_h0].radix = 32; // -4 to 4
*/

}

// EOF
