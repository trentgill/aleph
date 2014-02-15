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
	.ident	"GCC: (ADI/svn-5865) 4.3.5"
