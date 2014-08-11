#ifndef _ALEPH_PLATES_PARAMS_H_
#define _ALEPH_PLATES_PARAMS_H_

#include "param_common.h"

#define PARAM_HZ_MIN OSC_FREQ_MIN
#define PARAM_HZ_MAX OSC_FREQ_MIN
#define PARAM_HZ_DEFAULT (OSC_FREQ_MIN * 16)

#define PARAM_DAC_MIN 0
#define PARAM_DAC_MAX 0x7fffffff
#define PARAM_DAC_RADIX 16

#define PARAM_RATE_MIN 0x2000 // 1/8
#define PARAM_RATE_MAX 0x80000 // 8
#define PARAM_RATE_RADIX 3

#define SMOOTH_FREQ_MIN 0x2000 // 1/8s
#define SMOOTH_FREQ_MAX 0x400000 // 64s
#define SMOOTH_FREQ_RADIX 7

/// FIXME: 
// right now, "fade" is the raw increment of a linear ramp from 0 to 0x7fffffff.
// not very intuitive,
// for 1s, inc = 0x7fffffff / 48k
// let's say max = 8s and min = 1ms
#define PARAM_FADE_MIN  0x2000    // ~4s
#define PARAM_FADE_MAX  0x2000000   // ~1ms
#define PARAM_FADE_RADIX 12 

// svf cutoff
#define PARAM_CUT_MAX     0x7fffffff
#define PARAM_CUT_DEFAULT 0x43D0A8EC // ~8k

// rq
#define PARAM_RQ_MIN 0x00000000
#define PARAM_RQ_MAX 0x0000ffff
#define PARAM_RQ_DEFAULT 0x0000FFF0

#define PARAM_AMP_0 (FRACT32_MAX)
#define PARAM_AMP_6 (FRACT32_MAX >> 1)
#define PARAM_AMP_12 (FRACT32_MAX >> 2)
#define PARAM_AMP_MAX   0x7fffffff

// max time in seconds, 16.16
// 256 seconds, minus 1 bit
// #define PARAM_SECONDS_MAX 0xffffff
// #define PARAM_SECONDS_RADIX 9

// max time in seconds, 16.16
//// revert until shit gets figured out
#define PARAM_SECONDS_MAX 0x003c0000
#define PARAM_SECONDS_RADIX 7

/// smoother default
// about 1ms?
//#define PARAM_SLEW_DEFAULT  0x77000000
/// ehh, try longer
#define PARAM_SLEW_DEFAULT 0x77000000
#define PARAM_SLEW_MAX  0x7fffffff

// cv output
#define PARAM_CV_VAL_DEFAULT PARAM_AMP_6
#define PARAM_CV_MAX 	0x7fffffff


// enumerate parameters
// the order defined here must be matched in the descriptor !
enum params {


  // xfade time
  eParamFade0,
  eParamFade1,

  /// smoothers have to be processed first!
  ////////////////////////////////
  //// FIXME: not using these
  eParamCut0Slew,
  eParamRq0Slew,
  eParamLow0Slew,
  eParamHigh0Slew,
  eParamBand0Slew,
  eParamNotch0Slew,

  eParamCut1Slew,
  eParamRq1Slew,
  eParamLow1Slew,
  eParamHigh1Slew,
  eParamBand1Slew,
  eParamNotch1Slew,
  //////////////////


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


  // line 1
  eParam_freq1,   
  eParam_rq1,     
  eParam_low1,      
  eParam_high1,     
  eParam_band1,     
  eParam_notch1,    
  eParam_pos_read1,        
  eParam_pos_write1,         
  eParam_run_read1,   
  eParam_run_write1,      
  eParam_loop1,     
  eParam_pre1,           
  eParam_write1,         
  eParam_delay1,     

  // line 0
  eParam_freq0,   
  eParam_rq0,     
  eParam_low0,      
  eParam_high0,     
  eParam_band0,     
  eParam_notch0,    
  eParam_pos_read0,        
  eParam_pos_write0,         
  eParam_run_read0,   
  eParam_run_write0,      
  eParam_loop0,     
  eParam_pre0,           
  eParam_write0,         
  eParam_delay0,     

  eParamNumParams
};

// this is only defined by the descriptor helper program.
extern void fill_param_desc(ParamDesc* desc);

#endif // header guard 
// EOF
