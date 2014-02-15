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

typedef struct {
  int buf [8];
  char str[8];
} bigData_t;

extern bigData_t bigData;
bigData_t bigData = { 
  .buf = { 0, 0, 0, 0, 0, 0, 0, 0 },
    .str = "stringy"
};  

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

