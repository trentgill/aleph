#include <string.h>
#include "osc.h"
#include "param_common.h"
#include "params.h"

void fill_param_desc(ParamDesc* desc) {

  /// amp
  strcpy(desc[eParam_amp0].label, "amp0");
  desc[eParam_amp0].type = eParamTypeAmp;
  desc[eParam_amp0].min = 0;
  desc[eParam_amp0].max = FRACT32_MAX >> 1;
  desc[eParam_amp0].radix = 1;

  strcpy(desc[eParam_amp1].label, "amp1");
  desc[eParam_amp1].type = eParamTypeAmp;
  desc[eParam_amp1].min = 0;
  desc[eParam_amp1].max = FRACT32_MAX >> 1;
  desc[eParam_amp1].radix = 1;

  strcpy(desc[eParam_amp2].label, "amp2");
  desc[eParam_amp2].type = eParamTypeAmp;
  desc[eParam_amp2].min = 0;
  desc[eParam_amp2].max = FRACT32_MAX >> 1;
  desc[eParam_amp2].radix = 1;

  strcpy(desc[eParam_amp3].label, "amp3");
  desc[eParam_amp3].type = eParamTypeAmp;
  desc[eParam_amp3].min = 0;
  desc[eParam_amp3].max = FRACT32_MAX >> 1;
  desc[eParam_amp3].radix = 1;


  /// slew
  strcpy(desc[eParam_ampSlew0].label, "ampSlew0");
  desc[eParam_ampSlew0].type = eParamTypeIntegrator;
  desc[eParam_ampSlew0].min = SLEW_SECONDS_MIN;
  desc[eParam_ampSlew0].max = SLEW_SECONDS_MAX;
  desc[eParam_ampSlew0].radix = SLEW_SECONDS_RADIX;

  strcpy(desc[eParam_ampSlew1].label, "ampSlew1");
  desc[eParam_ampSlew1].type = eParamTypeIntegrator;
  desc[eParam_ampSlew1].min = SLEW_SECONDS_MIN;
  desc[eParam_ampSlew1].max = SLEW_SECONDS_MAX;
  desc[eParam_ampSlew1].radix = SLEW_SECONDS_RADIX;

  strcpy(desc[eParam_ampSlew2].label, "ampSlew2");
  desc[eParam_ampSlew2].type = eParamTypeIntegrator;
  desc[eParam_ampSlew2].min = SLEW_SECONDS_MIN;
  desc[eParam_ampSlew2].max = SLEW_SECONDS_MAX;
  desc[eParam_ampSlew2].radix = SLEW_SECONDS_RADIX;

  strcpy(desc[eParam_ampSlew3].label, "ampSlew3");
  desc[eParam_ampSlew3].type = eParamTypeIntegrator;
  desc[eParam_ampSlew3].min = SLEW_SECONDS_MIN;
  desc[eParam_ampSlew3].max = SLEW_SECONDS_MAX;
  desc[eParam_ampSlew3].radix = SLEW_SECONDS_RADIX;
}
