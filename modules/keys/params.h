#ifndef _ALEPH_KEYS_PARAMS_H_
#define _ALEPH_KEYS_PARAMS_H_

#include "param_common.h"

// define some constants here for the param descriptor code
// here, all the parameter ranges are pretty simple.
#define PARAM_AMP_MAX 	0x7fffffff
#define PARAM_CV_MAX 	0x7fffffff
#define PARAM_SLEW_MAX 	0x7fffffff

#define PARAM_CUT_MAX     0x7fffffff


// something pretty fast, but noticeable
#define PARAM_SLEW_DEFAULT 	0x7ffecccc
#define PARAM_CUT_DEFAULT 0x7fffffff

// enumerate parameters
// the order defined here must be matched in the descriptor !
enum params {
  // adc static gain level
  eParam_gain0,
  eParam_gain1,
  eParam_gain2,
  eParam_gain3,
  // adc filter cutoff
  eParam_filter0,
  eParam_filter1,
  eParam_filter2,
  eParam_filter3,
  // adc envelope slew time
  eParam_slew0,
  eParam_slew1,
  eParam_slew2,
  eParam_slew3,

  // envelopes to gain0
  eParam_ef0_gain0,
  eParam_ef1_gain0,
  eParam_ef2_gain0,
  eParam_ef3_gain0,
  // envelopes to filter0
  eParam_ef0_filt0,
  eParam_ef1_filt0,
  eParam_ef2_filt0,
  eParam_ef3_filt0,

  // envelopes to gain1
  eParam_ef0_gain1,
  eParam_ef1_gain1,
  eParam_ef2_gain1,
  eParam_ef3_gain1,
  // envelopes to filter1
  eParam_ef0_filt1,
  eParam_ef1_filt1,
  eParam_ef2_filt1,
  eParam_ef3_filt1,

  // envelopes to gain2
  eParam_ef0_gain2,
  eParam_ef1_gain2,
  eParam_ef2_gain2,
  eParam_ef3_gain2,
  // envelopes to filter2
  eParam_ef0_filt2,
  eParam_ef1_filt2,
  eParam_ef2_filt2,
  eParam_ef3_filt2,

  // envelopes to gain3
  eParam_ef0_gain3,
  eParam_ef1_gain3,
  eParam_ef2_gain3,
  eParam_ef3_gain3,
  // envelopes to filter3
  eParam_ef0_filt3,
  eParam_ef1_filt3,
  eParam_ef2_filt3,
  eParam_ef3_filt3,

  eParamNumParams
};

// this is only defined by the descriptor helper program.
extern void fill_param_desc(ParamDesc* desc);

#endif // header guard 
// EOF
