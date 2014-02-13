// global variables for all voices components.
// declare everything statically for macro-ized loops over each component array.


#define MAPS_NVOICES 4

#include "noise.h"
#include "slew.h"

// noise generator
extern lcprng rng[MAPS_NVOICES];
// amp smoother
extern SlewExp ampSlew[MAPS_NVOICES];
