#include "fix16.h"

// optimized 16.16 multiply in blackfin ASM
fix16_t fix16_mul(fix16_t a, fix16_t b) {
  fix16_t res;
  asm( "R0 = %1; " 						"\n\t"
       "R1 = %2; "						"\n\t"
       "R5.h = (A1 = R0.h * R1.h)(is); "			"\n\t"
       "A1 = R1.h * R0.l(m), R5.l = (A0 = R1.l * R0.l)(fu); "	"\n\t"
       "R3 = (A1 += R0.h * R1.l)(m); "				"\n\t"
       "R3 = R3 + R5(s); "					"\n\t"
       "%0 = R3; "						"\n\t"
       : "=m"(res) : "m"(a) , "m"(b) : "R0","R1","R3","R5","A0","A1" );
  return res;
}
