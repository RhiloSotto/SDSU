.data
	
Text: 		.asciiz "Rhilo Novuno Sotto - 130551574\n"

.text

	# print Name - RedID
	li 	$v0, 4
	la 	$a0, Text
	syscall
	
	# read int from user
	li 	$v0, 5
	syscall
	add 	$a0, $v0, $0
	
	# shift left 3 bits
	sll 	$a0, $a0, 3
	
	# print result
	li 	$v0, 1
	syscall
	
	# terminate execution
	li 	$v0, 10
	syscall