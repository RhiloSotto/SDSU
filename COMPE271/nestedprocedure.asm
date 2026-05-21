#source:https://www.youtube.com/watch?v=E0PHijf0P7g

#Instead of writing the instructions for printing the value every time,
#a procedure named printthevalue is made and called whenever needed


# main calls increaseRegvalue
# increaseRegvalue calls printthevalue
# so increaseRegvalue must save the return address to main before calling the printthvalue

.data
	newline: .asciiz "\n"
.text
	main:
		addi $s0,$zero,10 #$s0 = 10
		
		jal increaseRegvalue
	
		li $v0,4
		la $a0,newline
		syscall
	
		jal printthevalue
		
		li $v0,10
		syscall
	
	increaseRegvalue:
		addi $sp,$sp,-8
		sw $s0, 0($sp)
		sw $ra, 4($sp) #since we are using nested procedure, the return address must be saved
		
		addi $s0,$s0,30
		
		jal printthevalue
		
		lw $s0, 0($sp)
		lw $ra, 4($sp)
		addi $sp, $sp, 8
		
		jr $ra
		
		#this procedure prints the value
	printthevalue:
		li $v0,1
		move $a0, $s0
		syscall
		
		jr $ra
		
