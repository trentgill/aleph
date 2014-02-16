#ifndef    _ALEPH_DSP_SLEW_H_
#define    _ALEPH_DSP_SLEW_H_

// macro-ized slew functions

typedef struct _SlewExp { 
  fract32 c;
  fract32 x;
  fract32 y;
} SlewExp;


/* // set slew coefficient */
/* #define slew_exp_set_slew( s, v ) (s).c = v; */

/* // set input */
/* #define slew_exp_in( s, v ) (s).x = v; */

// initialize
#define slew_exp_init( s, v )			\
  (s).x = (s).y = v; \
     (s).c = 0;

// compute next slew value
#define slew_exp_calc_frame( s )						\
  (s).y = add_fr1x32( (s).x, mult_fr1x32x32( (s).c, sub_fr1x32((s).y, (s).x) ) );

#endif
