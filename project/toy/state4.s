	.arch msp430g2553
	.p2align 1,0
	.text

	.extern buzzer_set_period
	.global state4AS
	.extern P1OUT

state4AS:
	cmp #0, r12
	jz zeroCount
	jmp off
	
zeroCount:
	cmp #0, r13
	jz zeroState
	cmp #1, r13
	jz oneState
	jmp off

zeroState:
	bis #1, &P1OUT
	mov #5405, r12
	call #buzzer_set_period
	jmp off

oneState:
	bis #64, &P1OUT
	mov #7644, r12
	call #buzzer_set_period
	jmp off

off:
	pop r0
