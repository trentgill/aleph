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
  strcpy(desc[eParam_preDel].label, "preDelay");
  desc[eParam_preDel].type = eParamTypeFix; 
  desc[eParam_preDel].min = 0x00000000; 
  desc[eParam_preDel].max = 0x7fffffff; 
  desc[eParam_preDel].radix = 16;
  
  strcpy(desc[eParam_time].label, "scatter");
  desc[eParam_time].type = eParamTypeFix; 
  desc[eParam_time].min = 0x00000000; 
  desc[eParam_time].max = 0x7fffffff; 
  desc[eParam_time].radix = 16;

  strcpy(desc[eParam_pre0].label, "ringing");
  desc[eParam_pre0].type = eParamTypeAmp; 
  desc[eParam_pre0].min = 0x00000000; 
  desc[eParam_pre0].max = FR32_MAX;
  desc[eParam_pre0].radix = 1;

  strcpy(desc[eParam_inDamp].label, "damping");
  desc[eParam_inDamp].type = eParamTypeIntegrator;
  desc[eParam_inDamp].min = 0;
  desc[eParam_inDamp].max = FR32_MAX;
  desc[eParam_inDamp].radix = 32;

  strcpy(desc[eParam_size].label, "size");
  desc[eParam_size].type = eParamTypeFix; 
  desc[eParam_size].min = 0x00000000; 
  desc[eParam_size].max = 0x7fffffff; 
  desc[eParam_size].radix = 16;

  strcpy(desc[eParam_decay].label, "decay");
  desc[eParam_decay].type = eParamTypeAmp; 
  desc[eParam_decay].min = 0x00000000; 
  desc[eParam_decay].max = FR32_MAX;
  desc[eParam_decay].radix = 1;

  strcpy(desc[eParam_dryMix].label, "dryMix");
  desc[eParam_dryMix].type = eParamTypeAmp; 
  desc[eParam_dryMix].min = 0x00000000; 
  desc[eParam_dryMix].max = FR32_MAX;
  desc[eParam_dryMix].radix = 1;

  strcpy(desc[eParam_wetMix].label, "wetMix");
  desc[eParam_wetMix].type = eParamTypeAmp; 
  desc[eParam_wetMix].min = 0x00000000; 
  desc[eParam_wetMix].max = FR32_MAX;
  desc[eParam_wetMix].radix = 1;

}

// EOF
