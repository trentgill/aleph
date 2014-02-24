.file "asm_test.c";
.global _globalCount;
	.section	.bss,"aw",@nobits
	.align 4
	.type	_globalCount, @object
	.size	_globalCount, 4
_globalCount:
	.zero	4
.global _globalBuf;
	.align 4
	.type	_globalBuf, @object
	.size	_globalBuf, 32
_globalBuf:
	.zero	32
	.local	_staticBuf
	.comm	_staticBuf,32,4
.text;
	.align 4
.global _mix32;
.type _mix32, STT_FUNC;
_mix32:
	LINK 16;
	[FP+8] = R0;
	[FP+12] = R1;
	[FP+16] = R2;
	P2 = [FP+12];
	R2 = [P2];
	P2 = [FP+8];
	R1 = [P2];
	P2 = [FP+16];
	R0 = [P2];
	[FP+-12] = R1;
	[FP+-16] = R0;
	R1 = [FP+-12];
	R0 = [FP+-16];
	A1 = R1.L * R0.L (FU);
	A1 = A1 >> 16;
	A0 = R1.H * R0.H, A1 += R1.H * R0.L (M);
	A1 += R0.H * R1.L (M);
	A1 = A1 >>> 15;
	R0 = (A0 += A1);
	[FP+-4] = R2;
	[FP+-8] = R0;
	R1 = [FP+-4];
	R0 = [FP+-8];
	R0 = R1 + R0 (S);
	P2 = [FP+12];
	[P2] = R0;
	UNLINK;
	rts;
	.size	_mix32, .-_mix32
	.align 4
.global _mix16;
.type _mix16, STT_FUNC;
_mix16:
	LINK 24;
	[FP+8] = R0;
	[FP+12] = R1;
	[FP+16] = R2;
	P2 = [FP+12];
	R3 = [P2];
	P2 = [FP+8];
	R0 = [P2];
	[FP+-24] = R0;
	R0 = [FP+-24];
	R0 >>= 16;
	[FP+-20] = R0;
	R0 = [FP+-20];
	R2 = R0.L (X);
	P2 = [FP+16];
	R0 = W [P2] (Z);
	R1 = R0.L (X);
	R0 = R2;
	W [FP+-12] = R0;
	R0 = R1;
	W [FP+-16] = R0;
	R1 = W [FP+-12] (X);
	R0 = W [FP+-16] (X);
	R0 = R1.L * R0.L ;
	[FP+-4] = R3;
	[FP+-8] = R0;
	R1 = [FP+-4];
	R0 = [FP+-8];
	R0 = R1 + R0 (S);
	P2 = [FP+12];
	[P2] = R0;
	UNLINK;
	rts;
	.size	_mix16, .-_mix16
	.align 4
.global _mul32x32;
.type _mul32x32, STT_FUNC;
_mul32x32:
	LINK 8;
	[FP+8] = R0;
	[FP+12] = R1;
	R0 = [FP+8];
	[FP+-4] = R0;
	R0 = [FP+12];
	[FP+-8] = R0;
	R1 = [FP+-4];
	R0 = [FP+-8];
	A1 = R1.L * R0.L (FU);
	A1 = A1 >> 16;
	A0 = R1.H * R0.H, A1 += R1.H * R0.L (M);
	A1 += R0.H * R1.L (M);
	A1 = A1 >>> 15;
	R0 = (A0 += A1);
	UNLINK;
	rts;
	.size	_mul32x32, .-_mul32x32
	.align 4
.global _mul16x16;
.type _mul16x16, STT_FUNC;
_mul16x16:
	LINK 8;
	[FP+8] = R0;
	[FP+12] = R1;
	[FP+16] = R2;
	P2 = [FP+12];
	R0 = W [P2] (Z);
	R2 = R0.L (X);
	P2 = [FP+16];
	R0 = W [P2] (Z);
	R1 = R0.L (X);
	R0 = R2;
	W [FP+-4] = R0;
	R0 = R1;
	W [FP+-8] = R0;
	R1 = W [FP+-4] (X);
	R0 = W [FP+-8] (X);
	R0 = R1.L * R0.L ;
	P2 = [FP+8];
	[P2] = R0;
	UNLINK;
	rts;
	.size	_mul16x16, .-_mul16x16
.global _bigData;
.data;
	.align 4
	.type	_bigData, @object
	.size	_bigData, 40
_bigData:
	.long	0
	.long	0
	.long	0
	.long	0
	.long	0
	.long	0
	.long	0
	.long	0
	.string	"stringy"
.text;
	.align 4
.global _mix;
.type _mix, STT_FUNC;
_mix:
	LINK 44;
	[FP+8] = R0;
	[FP+12] = R1;
	[FP+16] = R2;
	R0 = [FP+12];
	[FP+-36] = R0;
	R0 = [FP+8];
	[FP+-32] = R0;
	R0 = [FP+16];
	[FP+-28] = R0;
	R0 = 0 (X);
	[FP+-44] = R0;
	jump.s .L10;
.L13:
	R0 = 0 (X);
	[FP+-44] = R0;
	jump.s .L11;
.L12:
	P2 = [FP+-32];
	R3 = [P2];
	P2 = [FP+-36];
	R0 = [P2];
	[FP+-24] = R0;
	R0 = [FP+-24];
	R0 >>= 16;
	[FP+-20] = R0;
	R0 = [FP+-20];
	R1 = R0.L (X);
	P2 = [FP+-28];
	R0 = W [P2] (Z);
	R2 = R0.L (X);
	R0 = [FP+-28];
	R0 += 2;
	[FP+-28] = R0;
	R0 = R1;
	W [FP+-12] = R0;
	R0 = R2;
	W [FP+-16] = R0;
	R1 = W [FP+-12] (X);
	R0 = W [FP+-16] (X);
	R0 = R1.L * R0.L ;
	[FP+-4] = R3;
	[FP+-8] = R0;
	R1 = [FP+-4];
	R0 = [FP+-8];
	R0 = R1 + R0 (S);
	P2 = [FP+-32];
	[P2] = R0;
	R0 = [FP+-32];
	R0 += 4;
	[FP+-32] = R0;
	R0 = [FP+-44];
	R0 += 1;
	[FP+-44] = R0;
.L11:
	R1 = [FP+-44];
	R0 = [FP+24];
	cc =R1<R0;
	if cc jump .L12;
	R0 = [FP+-36];
	R0 += 4;
	[FP+-36] = R0;
	R0 = [FP+-44];
	R0 += 1;
	[FP+-44] = R0;
.L10:
	R1 = [FP+-44];
	R0 = [FP+20];
	cc =R1<R0;
	if cc jump .L13;
	UNLINK;
	rts;
	.size	_mix, .-_mix
	.align 4
.global _mix_voice;
.type _mix_voice, STT_FUNC;
_mix_voice:
	LINK 44;
	[FP+8] = R0;
	[FP+12] = R1;
	[FP+16] = R2;
	R0 = [FP+12];
	[FP+-36] = R0;
	R0 = [FP+8];
	[FP+-32] = R0;
	R0 = [FP+16];
	[FP+-28] = R0;
	R0 = 0 (X);
	[FP+-44] = R0;
	jump.s .L16;
.L19:
	R0 = 0 (X);
	[FP+-40] = R0;
	jump.s .L17;
.L18:
	P2 = [FP+-32];
	R3 = [P2];
	P2 = [FP+-36];
	R0 = [P2];
	[FP+-24] = R0;
	R0 = [FP+-24];
	R0 >>= 16;
	[FP+-20] = R0;
	R0 = [FP+-20];
	R1 = R0.L (X);
	P2 = [FP+-28];
	R0 = W [P2] (Z);
	R2 = R0.L (X);
	R0 = [FP+-28];
	R0 += 2;
	[FP+-28] = R0;
	R0 = R1;
	W [FP+-12] = R0;
	R0 = R2;
	W [FP+-16] = R0;
	R1 = W [FP+-12] (X);
	R0 = W [FP+-16] (X);
	R0 = R1.L * R0.L ;
	[FP+-4] = R3;
	[FP+-8] = R0;
	R1 = [FP+-4];
	R0 = [FP+-8];
	R0 = R1 + R0 (S);
	P2 = [FP+-32];
	[P2] = R0;
	R0 = [FP+-32];
	R0 += 4;
	[FP+-32] = R0;
	R0 = [FP+-40];
	R0 += 1;
	[FP+-40] = R0;
.L17:
	R0 = [FP+-40];
	cc =R0<=3;
	if cc jump .L18;
	R0 = [FP+-36];
	R0 += 4;
	[FP+-36] = R0;
	R0 = [FP+-44];
	R0 += 1;
	[FP+-44] = R0;
.L16:
	R0 = [FP+-44];
	cc =R0<=1;
	if cc jump .L19;
	UNLINK;
	rts;
	.size	_mix_voice, .-_mix_voice
	.align 4
.global _copy;
.type _copy, STT_FUNC;
_copy:
	LINK 4;
	[FP+8] = R0;
	R0 = 0 (X);
	[FP+-4] = R0;
	jump.s .L22;
.L23:
	P2.H = _globalCount;
	P2.L = _globalCount;
	R0 = [FP+-4];
	[P2] = R0;
	R3 = [FP+-4];
	R0 = [FP+-4];
	R1.H = _globalBuf;
	R1.L = _globalBuf;
	R0 <<= 2;
	R0 = R1 + R0;
	P2 = R0;
	R2 = [P2];
	R1.H = _staticBuf;
	R1.L = _staticBuf;
	R0 = R3 << 2;
	R0 = R1 + R0;
	P2 = R0;
	[P2] = R2;
	R0 = [FP+-4];
	R0 += 1;
	[FP+-4] = R0;
.L22:
	R1 = [FP+-4];
	R0 = [FP+8];
	cc =R1<R0;
	if cc jump .L23;
	UNLINK;
	rts;
	.size	_copy, .-_copy
	.align 4
.global _copy_args;
.type _copy_args, STT_FUNC;
_copy_args:
	LINK 4;
	[FP+8] = R0;
	[FP+12] = R1;
	[FP+16] = R2;
	R0 = 0 (X);
	[FP+-4] = R0;
	jump.s .L26;
.L27:
	R0 = [FP+-4];
	R1 = R0 << 2;
	R0 = [FP+8];
	R0 = R0 + R1;
	P1 = R0;
	R0 = [FP+-4];
	R1 = R0 << 2;
	R0 = [FP+12];
	R0 = R0 + R1;
	P2 = R0;
	R0 = [P2];
	[P1] = R0;
	R0 = [FP+-4];
	R0 += 1;
	[FP+-4] = R0;
.L26:
	R1 = [FP+-4];
	R0 = [FP+16];
	cc =R1<R0;
	if cc jump .L27;
	UNLINK;
	rts;
	.size	_copy_args, .-_copy_args
	.align 4
.global _copy_struct;
.type _copy_struct, STT_FUNC;
_copy_struct:
	LINK 4;
	[FP+8] = R0;
	R0 = 0 (X);
	[FP+-4] = R0;
	jump.s .L30;
.L32:
	R0 = [FP+8];
	R1 = 7 (X);
	cc =R0<=R1;
	if !cc jump .L31;
	R3 = [FP+-4];
	R0 = [FP+-4];
	R1.H = _bigData;
	R1.L = _bigData;
	R0 <<= 2;
	R0 = R1 + R0;
	P2 = R0;
	R2 = [P2];
	R1.H = _staticBuf;
	R1.L = _staticBuf;
	R0 = R3 << 2;
	R0 = R1 + R0;
	P2 = R0;
	[P2] = R2;
.L31:
	R0 = [FP+-4];
	R0 += 1;
	[FP+-4] = R0;
.L30:
	R1 = [FP+-4];
	R0 = [FP+8];
	cc =R1<R0;
	if cc jump .L32;
	UNLINK;
	rts;
	.size	_copy_struct, .-_copy_struct
	.align 4
.global _add_ns_s32;
.type _add_ns_s32, STT_FUNC;
_add_ns_s32:
	LINK 0;
	[FP+8] = R0;
	[FP+12] = R1;
	R1 = [FP+8];
	R0 = [FP+12];
	R0 = R1 + R0;
	UNLINK;
	rts;
	.size	_add_ns_s32, .-_add_ns_s32
	.ident	"GCC: (ADI/svn-5865) 4.3.5"
