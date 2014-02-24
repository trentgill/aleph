/*
  asm_test.c

  template for producing test asm

 */

#include <fract_base.h>
#include <fract_math.h>

extern int globalCount;
int globalCount = 0;
extern int globalBuf[8];
int globalBuf[8] = { 0, 0, 0, 0, 0, 0, 0, 0 };
static int staticBuf[8] = { 0, 0, 0, 0, 0, 0, 0, 0 };

void mix32 (const fract32* src, fract32* dst, const fract32* mix) {
  *dst = add_fr1x32( *dst, mult_fr1x32x32(*src, *mix) );
}

void mix16 (const fract32* src, fract32* dst, const fract16* mix) {
  *dst = add_fr1x32( *dst, mult_fr1x32( trunc_fr1x32(*src), *mix) );
}

fract32 mul32x32(fract32 a, fract32 b) {
  return mult_fr1x32x32(a, b);
}


void mul16x16(fract32* o, fract16* a, fract16* b) {
  *o = mult_fr1x32(*a, *b);
}


typedef struct {
  int buf [8];
  char str[8];
} bigData_t;

extern bigData_t bigData;
bigData_t bigData = { 
  .buf = { 0, 0, 0, 0, 0, 0, 0, 0 },
    .str = "stringy"
};  


void mix (fract32* out, const fract32* in, const fract16* mix, int incount, int outcount) {
  int i, j;
  const fract32* pIn = in;
  fract32* pOut = out;
  const fract16* pMix = mix;
  for(i=0; i<incount; i++) {    
    for(i=0; i<outcount; i++) {
      *pOut = add_fr1x32(*pOut, mult_fr1x32(trunc_fr1x32(*pIn), *pMix++) );
      pOut++;
    }
    pIn++;
  }
}





void mix_voice (fract32* out, const fract32* in, const fract16* mix) {
  int i, j;
  const fract32* pIn = in;
  fract32* pOut = out;
  const fract16* pMix = mix;
  for(i=0; i < 2; i++) {    
    for(j=0; j < 4; j++) {
      *pOut = add_fr1x32(*pOut, mult_fr1x32(trunc_fr1x32(*pIn), *pMix++) );
      pOut++;
    }
    pIn++;
  }
}

void copy(int n) {
  int i;
  for(i=0; i<n; i++) {
    globalCount = i;
    staticBuf[i] = globalBuf[i];
  }
} 

void copy_args(int* a, const int* b, const int n) {
  int i;
  for(i=0; i<n; i++) {
    a[i] = b[i];
  }
} 

void copy_struct(int n) {
  int i;
  for(i=0; i<n; i++) {
    if(n < 8) {
      staticBuf[i] = bigData.buf[i];
    }
  }
} 

unsigned int  add_ns_s32(signed int a, signed int b) {
  return a + b;
}

