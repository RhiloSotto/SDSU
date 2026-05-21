# Author:	Rhilo Sotto
# Date:		March 13, 2024
# Description:	HW #4: A 32 Bit Linear-feedback Shift Register

.data
	seedPrompt: 	.asciiz "Seed: "
	newline: 	.asciiz "\n"
	seed: 		.word 1437269760	# initial seed value 0x55AAFF00
	lfsr: 		.word	# memory location of lfsr
	
.text

main:
# s0 = lfsr
# s1 = period
	li 	$v0, 4	
	la 	$a0, seedPrompt
	syscall			# print "Seed: "
	
	lw 	$a0, seed
	jal 	lfsr32		# n = seed
	
	jal 	PrintHex	# print seed in hex
	
	li 	$v0, 4
	la 	$a0, newline
	syscall			# print newline
	
	addi 	$s1, $zero, 1	# period = 1
While:
	bgt 	$s1, 10, Done	# while (period <= 10)
	
	add 	$a0, $zero, $zero	# lfsr32(0)
	jal 	lfsr32
	
	add 	$a0, $v1, $zero
	jal 	PrintHex	# print generated number in hex

	li 	$v0, 4
	la 	$a0, newline
	syscall			# print newline
	
	addi 	$s1, $s1, 1	# ++period
	j 	While
Done:
	li 	$v0, 10
	syscall			# exit program


lfsr32: 
# a0 = n (seed)
# t0 = lfsr
# t1 = bit
# t2 = intermediate taps
# v1 = returned random number

	bne 	$a0, $zero, nNotZero	# if (n == 0)

	lw 	$t0, lfsr	# load lfsr from memory
	
	srl	$t1, $t0, 0	# lfsr >> 0, tap 32
	srl 	$t2, $t0, 10 	# lfsr >> 10, tap 22
	xor 	$t1, $t1, $t2	# (tap 32) ^ (tap 22)
	
	srl 	$t2, $t0, 30	# lfsr >> 30, tap 2
	xor 	$t1, $t1, $t2	# ((tap 32) ^ (tap 22)) ^ (tap 2)
	
	srl 	$t2, $t0, 31	# lfsr >> 31, tap 1
	xor 	$t1, $t1, $t2	# ((tap 32) ^ (tap 22) ^ (tap 2)) ^ (tap 1)

	andi 	$t1, $t1, 1	# bit = (taps xor) & 1
	
	sll 	$t1, $t1, 31	# bit << 31	
	srl 	$t0, $t0, 1		# lfsr >> 1
	or 	$t0, $t0, $t1	# lfsr = (lfsr >> 1) | (bit << 31)
	
	sw 	$t0, lfsr		# store new lfsr to memory
	add 	$v1, $t0, $zero	# return lfsr
	
	j 	return_lfsr32
nNotZero:				# else

	sw 	$a0, lfsr		# lfsr = n
	j 	return_lfsr32
return_lfsr32:
	jr 	$ra			


PrintHex:
# a0 = int to print in hex
	li 	$v0, 34
	syscall
	jr 	$ra