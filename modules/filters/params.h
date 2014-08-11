#ifndef _ALEPH_FILTERS_PARAMS_H_
#define _ALEPH_FILTERS_PARAMS_H_

#include "param_common.h"


#define PARAM_FIX_MAX   0x7fffffff
#define PARAM_FIX_DEFAULT   0x20000000
#define PARAM_FIX_MIN  0x80000000

// define some constants here for the param descriptor code
// here, all the parameter ranges are pretty simple.
#define PARAM_AMP_MAX 	0x7fffffff
#define PARAM_CV_MAX 	0x7fffffff
#define PARAM_SLEW_MAX 	0x7fffffff

// something pretty fast, but noticeable
#define PARAM_SLEW_DEFAULT 	0x7ffecccc

// enumerate parameters
// the order defined here must be matched in the descriptor !
enum params {

  eParam_b2,
  eParam_b1,
  eParam_b0,
  eParam_a2,
  eParam_a1,


/*
  // cv slew
  eParam_cvSlew0,
  eParam_cvSlew1,
  eParam_cvSlew2,
  eParam_cvSlew3,
  // adc multiplier slew
  eParam_adcSlew0,
  eParam_adcSlew1,
  eParam_adcSlew2,
  eParam_adcSlew3,
  // cv values
  eParam_cv0,
  eParam_cv1,
  eParam_cv2,
  eParam_cv3,
  // adc multiplier values
  eParam_adc0,
  eParam_adc1,
  eParam_adc2,
  eParam_adc3,
*/
/*
  // convolution coefficients
  eParam_h31,
  eParam_h30,
  eParam_h29,
  eParam_h28,
  eParam_h27,
  eParam_h26,
  eParam_h25,
  eParam_h24,

  eParam_h23,
  eParam_h22,
  eParam_h21,
  eParam_h20,
  eParam_h19,
  eParam_h18,
  eParam_h17,
  eParam_h16,

  eParam_h15,
  eParam_h14,
  eParam_h13,
  eParam_h12,
  eParam_h11,
  eParam_h10,
  eParam_h9,
  eParam_h8,

  eParam_h7,
  eParam_h6,
  eParam_h5,
  eParam_h4,
  eParam_h3,
  eParam_h2,
  eParam_h1,
  eParam_h0,
*/

  eParamNumParams
};

// this is only defined by the descriptor helper program.
extern void fill_param_desc(ParamDesc* desc);

#endif // header guard 
// EOF
