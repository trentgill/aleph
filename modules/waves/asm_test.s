.file "asm_test.c";
.global _globalBuf;
	.section	.bss,"aw",@nobits
	.align 4
	.type	_globalBuf, @object
	.size	_globalBuf, 32
_globalBuf:
	.zero	32
.global _globalCount;
	.align 4
	.type	_globalCount, @object
	.size	_globalCount, 4
_globalCount:
	.zero	4
	.local	_staticBuf
	.comm	_staticBuf,32,4
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
.global _copy;
.type _copy, STT_FUNC;
_copy:
	LINK 4;
	[FP+8] = R0;
	R0 = 0 (X);
	[FP+-4] = R0;
	jump.s .L2;
.L3:
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
.L2:
	R1 = [FP+-4];
	R0 = [FP+8];
	cc =R1<R0;
	if cc jump .L3;
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
	jump.s .L6;
.L7:
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
.L6:
	R1 = [FP+-4];
	R0 = [FP+16];
	cc =R1<R0;
	if cc jump .L7;
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
	jump.s .L10;
.L12:
	R0 = [FP+8];
	R1 = 7 (X);
	cc =R0<=R1;
	if !cc jump .L11;
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
.L11:
	R0 = [FP+-4];
	R0 += 1;
	[FP+-4] = R0;
.L10:
	R1 = [FP+-4];
	R0 = [FP+8];
	cc =R1<R0;
	if cc jump .L12;
	UNLINK;
	rts;
	.size	_copy_struct, .-_copy_struct
	.ident	"GCC: (ADI-2010R1-RC4) 4.3.5"
