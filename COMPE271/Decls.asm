# Author:	Eric Walkingshaw
# Date:		Jan 25, 2013
# Description:	Some example data declarations

.data

# a null-terminated string
prompt:	.asciiz	"What is your favorite number?: "

# an integer variable
favnum:	.word	7

# reserve space for a 10 integer array
array:	.space	40

# a long, multi-line string
help:	.ascii  "The best tool ever. (v.1.0)\n"
	.ascii  "Options:\n"
	.asciiz "    --h   Print this help text.\n"
	
# a pre-initialized array
fibs:	.word	0, 1, 1, 2, 3, 5, 8, 13, 21, 35, 55, 89, 144
