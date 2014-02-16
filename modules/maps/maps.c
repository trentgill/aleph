#include "string.h"

#include "bfin_core.h"
//#include "dac.h"
#include "fract_math.h"
#include "module.h"
#include "module_custom.h"
#include "types.h"

#include "maps.h"

// define a local data structure that subclasses moduleData.
// use this for all data that is large and/or not speed-critical.
// this structure should statically allocate all necessary memory 
// so it can simply be loaded at the start of SDRAM.
typedef struct _mapsData {
  ModuleData super;
  ParamData mParamData[eParamNumParams];
} mapsData;

//-------------------------
//----- extern vars (initialized here)
ModuleData * gModuleData; // module data


//// module state variables

// noise generator
lcprng rng[MAPS_NVOICES];
// amp smoother
SlewExp ampSlew[MAPS_NVOICES];

//-----------------------
//------ static variables

// pointer to local module data, initialize at top of SDRAM
static mapsData * data;


static inline void param_setup(u32 id, ParamValue v) {
  gModuleData->paramData[id].value = v;
  module_set_param(id, v);
}

// initialize module runtime and metadata
void module_init(void) {
  int i;

  // init module/param descriptor
  // intialize local data at start of SDRAM
  data = (mapsData * )SDRAM_ADDRESS;
  // initialize moduleData superclass for core routines
  gModuleData = &(data->super);
  strcpy(gModuleData->name, "aleph-maps");
  gModuleData->paramData = data->mParamData;
  gModuleData->numParams = eParamNumParams;

  /// intialize state variables
  for(i=0; i<MAPS_NVOICES; i++) {
    lcprng_reset(rng[i], 0);
  }

  for(i=0; i<MAPS_NVOICES; i++) {
    slew_exp_init(ampSlew[i]);
  }

  param_setup(eParam_amp0, PARAM_AMP_6);
  param_setup(eParam_amp1, PARAM_AMP_6);
  param_setup(eParam_amp2, PARAM_AMP_6);
  param_setup(eParam_amp3, PARAM_AMP_6);

  param_setup(eParam_ampSlew0, PARAM_SLEW_DEFAULT);
  param_setup(eParam_ampSlew1, PARAM_SLEW_DEFAULT);
  param_setup(eParam_ampSlew2, PARAM_SLEW_DEFAULT);
  param_setup(eParam_ampSlew3, PARAM_SLEW_DEFAULT);
  
}


// frame callback
void module_process_frame(void) {

  for(i=0; i<MAPS_NVOICES; i++) {
    lcprng_calc_frame( rng[i] );
  }

  for(i=0; i<MAPS_NVOICES; i++) {
    out[i] = mult_fr1x32x32( rng[i].x, ampSlew[i].y );
  }
  
}
