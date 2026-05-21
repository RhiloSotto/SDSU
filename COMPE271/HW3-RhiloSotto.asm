# Author:	Rhilo Sotto
# Date:		February 26, 2024
# Description:	HW #3: Find memory address of a 2D array

.data
rowPrompt: 	.asciiz 	"Enter row from [0,2]: "
colPrompt: 	.asciiz 	"Enter column from [0,4]: "
address:	.asciiz 	"Address of Array[row][col]: "
value:		.asciiz 	"\nValue: "

# initializates array, 3 rows and 5 columns
array:
	.word	1, 2, 3, 4, 5
	.word	6, 7, 8, 9, 10
   	.word	11, 12, 13, 14, 15

.text

main:
	li	$v0, 4 
	la	$a0, rowPrompt
	syscall				# Prompts user for row input

    	li	$v0, 5
    	syscall				# Read row input from user    	
    	add	$t0, $v0, $zero 	# storing row in $t0

    	li	$v0, 4
    	la	$a0, colPrompt
    	syscall				# Prompt user for column input

	li	$v0, 5
	syscall				# Read column input from user
	add	$t1, $v0, $zero 	# storing column in $t1
    
    	# Getting array value
    	mul 	$t2, $t0, 5		# row * ncolumns
    	add	$t2, $t2, $t1		# row * ncolumns + columns
    	addi	$t2, $t2, 1 		# storing array value in t2

	# Arguments for calculateAddress
    	add	$a0, $t0, $zero		# row
    	add	$a1, $t1, $zero		# col
    	li	$a2, 5			# ncols
    	la	$a3, array		# base address

    	# Calculates memory address of a given row & column of array
    	jal	arrayAddress
    
    	add	$t0, $v0, $zero 	# store address found in t0
    	
    	li 	$v0, 4
    	la 	$a0, address
    	syscall				# Prints address  
 
    	add 	$a0, $t0, $zero
    	li 	$v0, 34
    	syscall				# Prints array address in hexadecimal

    	li 	$v0, 4
    	la 	$a0, value
    	syscall				# Prints value

    	add 	$a0, $t2, $zero
    	li 	$v0, 1
    	syscall				# Prints array value 

    	li 	$v0, 10
    	syscall				# Exits program

arrayAddress:
	# Memory address formula (for ints) = base address + (row * number_of_columns + column) * 4
	# $a0 - row, $a1 - col, $a2 - ncolumns, $a3 - base address
	# $v0 - address/return value

	mul	$v0, $a0, $a2 		# v0 = row * number_of_columns
	add 	$v0, $v0, $a1		# v0 = (row * number_of_columns) + column
	sll 	$v0, $v0, 2		# v0 = ((row * number_of_columns + column)) * 4
	add 	$v0, $v0, $a3		# v0 =  base address + ((row * number_of_columns + column) * 4)
	
	jr 	$ra			# Return to main















   



