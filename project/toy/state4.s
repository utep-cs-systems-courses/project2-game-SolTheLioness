	.arch msp430g2553
	.p2align 1,0
	.text

	.extern buzzer_set_period
	.global state4AS
	.type state4, %function
	.extern P1OUT

state4AS:
	cmp #0, r12
	jz zeroCount
	jmp off
	
zeroCount:
	sub #2, r1		
	cmp #0, r13
	jz zeroState
	cmp #1, r13
	jz oneState
	jmp off

zeroState:
	bis #1, &P1OUT
	mov #5405.5515, 0(r1)
	call buzzer_set_period
	jmp off

oneState:
	bis #64, &P1OUT
	mov #7644.3833, 0(r1)
	call #buzzer_set_period
	jmp off

off:
	add #2, r1
	pop r0
