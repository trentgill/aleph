/*
  asm_test.c

  template for producing test asm

 */

#include <fract_base.h>
#include <fract_math.h>

#define SIZE 8


extern fract32 globalBuf [SIZE];
extern int globalCount;


fract32 globalBuf [SIZE] = { 0, 0, 0, 0, 0, 0, 0, 0 };
int globalCount = 0;

static fract32 staticBuf [SIZE] = { 0, 0, 0, 0, 0, 0, 0, 0 };


// mix modulation busses
void copy(int n) {
  int i;
  for(i=0; i<n; i++) {
    globalCount = i;
    staticBuf[i] = globalBuf[i];
  }
} 
