.file "asm_test.c";
.text;
	.align 4
.global _mul16x16;
.type _mul16x16, STT_FUNC;
_mul16x16:
	P2 = R1;
	P1 = R2;
	P0 = R0;
	LINK 0;
	UNLINK;
	R0 = W [P2] (X);
	R1 = W [P1] (X);
	R0 = R0.L * R1.L ;
	[P0] = R0;
	rts;
	.size	_mul16x16, .-_mul16x16
	.align 4
.global _mix_voice;
.type _mix_voice, STT_FUNC;
_mix_voice:
	LINK 0;
	P2 = R0;
	P1 = R2;
	P0 = R1;
	R3 = 0 (X);
.L4:
	R0 = W [P0+2] (X);
	R1 = W [P1] (X);
	R2 = [P2];
	R0 = R0.L * R1.L ;
	R2 = R2 + R0 (S) ||
	R1 = W [P1+2] (X) ||
	nop;
	[P2] = R2;
	R0 = W [P0+2] (X);
	R2 = [P2+4];
	R0 = R0.L * R1.L ;
	R2 = R2 + R0 (S) ||
	R1 = W [P1+4] (X) ||
	nop;
	[P2+4] = R2;
	R0 = W [P0+2] (X);
	R2 = [P2+8];
	R0 = R0.L * R1.L ;
	R2 = R2 + R0 (S) ||
	R1 = W [P1+6] (X) ||
	nop;
	[P2+8] = R2;
	R0 = W [P0+2] (X);
	R2 = [P2+12];
	R0 = R0.L * R1.L ;
	R2 = R2 + R0 (S);
	[P2+12] = R2;
	R3 += 4;
	R0 = 8 (X);
	P2 += 16;
	P1 += 8;
	cc =R3==R0;
	if !cc jump .L5 (bp);
	UNLINK;
	rts;
.L5:
	P0 += 4;
	jump.s .L4;
	.size	_mix_voice, .-_mix_voice
	.align 4
.global _mix;
.type _mix, STT_FUNC;
_mix:
	[--sp] = ( r7:4, p5:3 );

	LINK 0;
	P3 = [FP+48];
	R7 = [FP+52];
	cc =P3<=0;
	if cc jump .L14;
	cc =R7<=0;
	R5 = R0;
	R0 = CC;
	I0 = R0;
	P2 = R7;
	R4 = R7;
	R6 = R2;
	P0 = R1;
	R0 = I0;
	P4 = P2 << 2;
	P5 = P2 + P2;
	R4 += 1;
	CC = R0;
	if cc jump .L16;
.L12:
	P2 = R5;
	P1 = R6;
	R3 = 0 (X);
.L11:
	R3 += 1;
	R1 = W [P0+2] (X);
	R2 = W [P1++] (X);
	R0 = [P2];
	R1 = R1.L * R2.L ;
	R0 = R0 + R1 (S);
	[P2++] = R0;
	cc =R7<=R3;
	if !cc jump .L11 (bp);
	R0 = P4;
	R1 = P5;
	R5 = R5 + R0;
	R6 = R6 + R1;
	R0 = R4;
	R1 = P3;
	cc =R0<R1;
	if !cc jump .L14;
.L17:
	R0 = I0;
	P0 += 4;
	CC = R0;
	if !cc jump .L12 (bp);
.L16:
	R0 = 1 (X);
	R1 = P3;
	cc =R0<R1;
	if cc jump .L17 (bp);
.L14:
	UNLINK;
	( r7:4, p5:3 ) = [sp++];

	rts;
	.size	_mix, .-_mix
	.align 4
.global _mix16;
.type _mix16, STT_FUNC;
_mix16:
	P2 = R2;
	P1 = R0;
	P0 = R1;
	LINK 0;
	UNLINK;
	R1 = W [P2] (X);
	R0 = W [P1+2] (X);
	R2 = [P0];
	R0 = R0.L * R1.L ;
	R2 = R2 + R0 (S);
	[P0] = R2;
	rts;
	.size	_mix16, .-_mix16
	.align 4
.global _mul32x32;
.type _mul32x32, STT_FUNC;
_mul32x32:
	A1 = R0.L * R1.L (FU);
	A1 = A1 >> 16;
	A0 = R0.H * R1.H, A1 += R0.H * R1.L (M);
	A1 += R1.H * R0.L (M);
	LINK 0;
	A1 = A1 >>> 15;
	R0 = (A0 += A1);
	UNLINK;
	rts;
	.size	_mul32x32, .-_mul32x32
	.align 4
.global _mix32;
.type _mix32, STT_FUNC;
_mix32:
	P1 = R0;
	P2 = R2;
	P0 = R1;
	LINK 0;
	UNLINK;
	R1 = [P1];
	R0 = [P2];
	A1 = R1.L * R0.L (FU) ||
	R2 = [P0] ||
	nop;
	A1 = A1 >> 16;
	A0 = R1.H * R0.H, A1 += R1.H * R0.L (M);
	A1 += R0.H * R1.L (M);
	A1 = A1 >>> 15;
	R0 = (A0 += A1);
	R2 = R2 + R0 (S);
	[P0] = R2;
	rts;
	.size	_mix32, .-_mix32
	.ident	"GCC: (ADI/svn-5865) 4.3.5"
