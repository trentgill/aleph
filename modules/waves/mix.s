.file "mix.s";
.text;
	.align 4
	
.global _mix_mod;
.type _mix_mod, STT_FUNC;
_mix_mod:
	LINK 0;
	P2 = R0			;; // pointer to input array
	P1 = R1			;; // pointer to output array
	UNLINK;
	rts;
	.size	_mix_mod, .-_mix_mod
	.align 4
	
.global _mix_voice;
.type _mix_voice, STT_FUNC;	
_mix_voice:
	LINK 0
	[--sp] = (r7:6) 	;
	// input and output arrays
	p2 = r0			; 
	p1 = r1 		; 
	// sum the 2 input values
	r7 = [p2++]		;
	r6 = [p2]
	r7 = r7 + r6 (S) 	; // add 2nd value
	// store in all outputs
	[p1++] = r7		; 
	[p1++] = r7		; 
	[p1++] = r7		; 
	[p1] = r7		;
	(r7:6) = [sp++] 	;
	UNLINK;
	rts;
	.size	_mix_voice, .-_mix_voice
	.align 4
	
.global _mix_adc;
.type _mix_adc, STT_FUNC;
_mix_adc:
	LINK 32;

	UNLINK;
	rts;
	.size	_mix_adc, .-_mix_adc
	.local	_i
	.comm	_i,4,4
	.local	_j
	.comm	_j,4,4
	.ident	"GCC: (ADI/svn-5865) 4.3.5"
