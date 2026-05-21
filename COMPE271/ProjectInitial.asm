# Author:	Rhilo Sotto
# Description:	Initial Project in MIPS

# macros
.eqv	X_SPACE	23
.eqv	Y_SPACE	20


# stack push
.macro	push (%x)
	add	$sp, $sp, -4
	sw	%x, 0($sp)
.end_macro

# stack pop
.macro	pop (%x)
	sw	$sp, 0(%x)
	add	$sp, $sp, 4
.end_macro

# lfsr
.macro randomNumber (%x)
lfsr32: 
# %x = n (seed)
# t0 = lfsr
# t1 = bit
# t2 = intermediate taps
# v1 = returned random number

	bne 	%x, $zero, lfsr32_nNotZero	# if (n == 0)

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
lfsr32_nNotZero:				# else

	sw 	%x, lfsr		# lfsr = n
	j 	return_lfsr32
return_lfsr32:
	jr 	$ra			

.end_macro


.data
	lfsr:	.word	# memory location of lfsr

# 23x20 = 460 bytes
gridSpace:	.space	460
newLine:	.asciiz	"\n"

# sprites
empty:	.byte	'.'
player:		.byte	'P'
playerProjectile:	.byte	'^'

enemy1:	.byte	'V'
	#enemy2:	.byte	'W'
enemyDead:	.byte	'x'
enemyProjectile:	.byte	'v'


.text


# s0 - gameStatus: 1 = title screen, 2 = game screen, 3 = lose screen, 4 = win screen, 5 = quit game
# s1 - player key pressed: 'a' = left, 's' = right, ' ' = shoot, 'p' = quit
# s2 - 
# s3 - 

main:
    	
    	titleScreen:
    	
    	jal	makeSpace
    	
    	j	gameLoop
    	
    	# polling
	gameLoop:
	lw	$s1, 0xffff0004
	
	
	
	
	beq	$s0, 3, loseScreen
	beq	$s0, 4, winScreen
	beq	$s0, 5, programFinished
    	
    	
    	jal	drawSpace
    	
    	jal	initPlayer
    	
    	
    	j gameLoop
    	
    	
    	
    	
    	loseScreen:
    	
    	j	programFinished
    	
    	
    	winScreen:
    	
    	j	programFinished
    	
    	programFinished:
    	li 	$v0, 10
    	syscall				# Exits program	


arrayAddress:
	# Memory address formula (for chars) = base address + (row * number_of_columns + column)
	# $a0 - row, $a1 - col, $a2 - ncolumns, $a3 - base address
	# $v0 - address/return value

	mul	$v0, $a0, $a2 		# v0 = row * number_of_columns
	add 	$v0, $v0, $a1		# v0 = (row * number_of_columns) + column
	add 	$v0, $v0, $a3		# v0 =  base address + ((row * number_of_columns + column) * 4)
	
	jr 	$ra			# Return to main
    	
# Initialization of game space
# t0 - index i, $t1 - index j, $t2 - gridSpace address
makeSpace:
	add	$t0, $zero, $zero	# i = 0
	makeSpaceWhileX:
	bge	$t0, X_SPACE, makeSpaceDoneX	# i < X_SPACE
	
	add	$t1, $zero, $zero	# j = 0
	makeSpaceWhileY:
	bge	$t1, Y_SPACE, makeSpaceDoneY	# j < Y_SPACE
	
	# initialize gridSpace with '.'
	add	$t2, $zero, $zero
	mul	$t2, $t1, X_SPACE 		# v0 = row * number_of_columns
	
	add 	$t2, $t2, $t0		# v0 = (row * number_of_columns) + column
	
	la	$t3, empty
	lb	$t4, 0($t3)
	sb	$t4, gridSpace($t2)
	# initialize gridSpace with '.'
	
	addi	$t1, $t1, 1	# ++j 
	j 	makeSpaceWhileY
	makeSpaceDoneY:
	
	addi	$t0, $t0, 1	# ++i
	j	makeSpaceWhileX
	makeSpaceDoneX:
	jr	$ra
	
drawSpace:
# t0 - index i, $t1 - index j, $t2 - gridSpace address, 
	add	$t1, $zero, $zero	# j = 0
	drawSpaceWhileY:
	bge	$t1, Y_SPACE, drawSpaceDoneY	# j < Y_SPACE
	
	add	$t0, $zero, $zero	# i = 0
	drawSpaceWhileX:
	bge	$t0, X_SPACE, drawSpaceDoneX	# i < X_SPACE
	

	# print out gridSpace
	add	$t2, $zero, $zero
	mul	$t2, $t1, X_SPACE 		# t2 = row * number_of_columns
	add 	$t2, $t2, $t0		# t2 = (row * number_of_columns) + column
	
	lb	$a0, gridSpace($t2)	
	li 	$v0, 11
	syscall
	lb	$t4, 0($t3)
	# print out gridSpace
	
	
	addi	$t0, $t0, 1	# ++i
	j	drawSpaceWhileX
	drawSpaceDoneX:
	
	# print new line
	la	$a0, newLine
    	li	$v0, 4
    	syscall	

	addi	$t1, $t1, 1	# ++j 
	j 	drawSpaceWhileY
	drawSpaceDoneY:

	jr	$ra
	
	
# Initialization
initPlayer:
	
	
	
	jr	$ra
initEnemies:



	jr	$ra
	
# Movement
moveLogic:
movePlayer:
movePlayerProjectile:
moveEnemy:
removeEnemy:
moveEnemyProjectile:

collision:
