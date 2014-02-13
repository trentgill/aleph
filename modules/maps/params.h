
#ifndef _ALEPH_MODULE_MAPS_PARAMS_H_
#define _ALEPH_MODULE_MAPS_PARAMS_H_


#include "param_common.h"

#define PARAM_AMP_6 (FRACT32_MAX >> 1)
#define PARAM_AMP_12 (FRACT32_MAX >> 2)


#define SLEW_SECONDS_MIN 0x2000 // 1/8
#define SLEW_SECONDS_MAX 0x400000 // 64
#define SLEW_SECONDS_RADIX 7
/// smoother default
// about 1ms?
#define PARAM_SLEW_DEFAULT  0x76000000


enum params {

  eParam_ampSlew0,  
  eParam_ampSlew1,  
  eParam_ampSlew2,  
  eParam_ampSlew3,  

  eParam_amp0,  
  eParam_amp1,  
  eParam_amp2,  
  eParam_amp3,  

  eParamNumParams

};

#endif
