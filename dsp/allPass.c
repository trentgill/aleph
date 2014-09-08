/*
  allPass.c
  aleph - audio

  implement delay line using buffer and tap objects. non-interpolated version.
  
 */

#include "conversion.h"
#include "allPass.h"

// initialize with pointer to audio buffer
extern void allPass_init(allPass* dl, fract32* data, u32 frames) {
  buffer_init(&(dl->buffer), data, frames);
  buffer_tapN_init(&(dl->tapRd), &(dl->buffer));
  buffer_tapN_init(&(dl->tapWr), &(dl->buffer));

  dl->tapWr.idx = 0;
  dl->tapRd.idx = 0;

  dl->tapWr.loop = frames;
  dl->tapRd.loop = frames;

  dl->preLevel = 0;
  dl->write = 1;
}

extern fract32 allPass_next(allPass* dl, fract32 in) {
  fract32 readVal, fbMix;

  // get read value first.
  readVal = buffer_tapN_read( &(dl->tapRd) );

  if(dl->preLevel < 0) {
    // write mix
    fbMix = add_fr1x32( in, negate_fr1x32( readVal) );
    buffer_tapN_write(&(dl->tapWr), fbMix);
    readVal += in;
  }
  else {
    // write mix
    fbMix = add_fr1x32( in, negate_fr1x32( mult_fr1x32x32(readVal, dl->preLevel)) );
    buffer_tapN_write(&(dl->tapWr), fbMix);
    readVal += mult_fr1x32x32(in, dl->preLevel);  
  }
  
  // advance the read phasor 
  buffer_tapN_next( &(dl->tapRd) );

  // advance the write phasor
  buffer_tapN_next( &(dl->tapWr) );

  return readVal;
}

// set loop endpoint in seconds
extern void allPass_set_loop_sec(allPass* dl, fix16 sec) {
  u32 samps = sec_to_frames_trunc(sec);
  buffer_tapN_set_loop(&(dl->tapRd), samps - 1);
  buffer_tapN_set_loop(&(dl->tapWr), samps - 1);
}

// set loop endpoint in samples
extern void allPass_set_loop_samp(allPass* dl, u32 samps) {
  dl->tapRd.loop = samps - 1;
  dl->tapWr.loop = samps - 1;
}

// set allPass in seconds
extern void allPass_set_delay_sec(allPass* dl, fix16 sec) {
  u32 samp = sec_to_frames_trunc(sec);
  buffer_tapN_sync(&(dl->tapRd), &(dl->tapWr), samp);
}

// set allPass in samples
extern void allPass_set_delay_samp(allPass* dl, u32 samp) {
  buffer_tapN_sync(&(dl->tapRd), &(dl->tapWr), samp);
}

// set erase level
extern void allPass_set_pre(allPass* dl, fract32 pre) {
  dl->preLevel = pre;
}

// set write level
extern void allPass_set_write(allPass* dl, u8 write) {
  dl->write = write;
}

// set read pos in seconds
extern void allPass_set_pos_read_sec(allPass* dl, fix16 sec) {
  u32 samp = sec_to_frames_trunc(sec);
  buffer_tapN_set_pos(&(dl->tapRd), samp);
}
extern void allPass_set_pos_read_samp(allPass* dl, u32 samp) {
  buffer_tapN_set_pos(&(dl->tapRd), samp);
}

// set write pos in seconds
extern void allPass_set_pos_write_sec(allPass* dl, fix16 sec) {
  u32 samp = sec_to_frames_trunc(sec);
  buffer_tapN_set_pos(&(dl->tapWr), samp);
}

extern void allPass_set_pos_write_samp(allPass* dl, u32 samp) {
  buffer_tapN_set_pos(&(dl->tapWr), samp);
}

// set read run flag 
extern void allPass_set_run_read(allPass* dl, u8 val) {
  dl->runRd = val;
}

// set write run flag
extern void allPass_set_run_write(allPass* dl, u8 val) {
  dl->runWr = val;
}
/*
// set read-head rate multiplier
void allPass_set_mul(allPass* dl, u32 val, u8 id) {
  // different terms, dumb...
  buffer_tapN_set_inc( &(dl->tapRd[id]), val );

}

// set read-head rate divider
void allPass_set_div(allPass* dl, u32 val, u8 id) {
  buffer_tapN_set_div( &(dl->tapRd[id]), val );
}
*/