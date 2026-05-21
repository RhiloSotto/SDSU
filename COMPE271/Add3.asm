# Author:	Eric Walkingshaw
# Date:		Jan 25, 2013
# Description:	Add three numbers from memory and print the result

.data

# string to print before the result
resStr: .asciiz "Result: "

# numbers to add
nums: 	.word 	-7, 20, -5	# numbers to add
result: .word	0 		# a place to store the result

.text

	# print the initial string
	li 	$v0, 4		# set up print string syscall
	la 	$a0, resStr
	syscall

	# load three numbers into registers
	la	$t0, nums
	lw 	$t1, 0($t0)	# lw $t1, nums
	lw	$t2, 4($t0)	# lw $t2, nums + 4
	lw	$t3, 8($t0)	# lw $t3, nums + 8
	
	# add them up and store result in a0 (for easy printing)
	add	$a0, $t1, $t2	# add the first two numbers
	add	$a0, $a0, $t3	# add the third to the sum
	
	# save a0 to result location in memory
	sw 	$a0, result

	# now print the result
	li 	$v0, 1		# set up the print int syscall
	syscall
	
	# exit
	li 	$v0, 10		# set up exit syscall
	syscall
