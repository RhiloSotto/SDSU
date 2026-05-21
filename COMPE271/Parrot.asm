# Author:	Eric Walkingshaw
# Date:		Jan 25, 2013
# Description:	A simple parroting program.

.data

	# string constants
intro:	.asciiz "Say something to the parrot and it'll say it back!\n\n"
prompt:	.asciiz "You say: "
parrot:	.asciiz "Parrot says: "

	# save space for 255 character input, plus the null character
text:	.space	256

.text

# Pseudocode:
#
#   print(intro)
#   while(true) {
#     print(prompt)
#     text = readString()
#     print(parrot)
#     print(text)
#   }
	
	li	$v0, 4		# print the intro
	la	$a0, intro
	syscall
	
	li 	$a1, 256	# size of input buffer	
	
	# infinite parrot loop
loop:	la	$a0, prompt	# print prompt
	syscall
	
	li	$v0, 8		# read text to parrot
	la	$a0, text	# address to store read text
	syscall
	
	li	$v0, 4		# print parrot lead
	la 	$a0, parrot
	syscall
	
	la 	$a0, text	# print parroted text
	syscall
	
	j	loop		# repeat
