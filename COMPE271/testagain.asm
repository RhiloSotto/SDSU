# $s0 = array base address, $s1 = i

add $s1, $s1, $0 # i = 0
WhileInit: 
blt $s1, 1000, DoneInit

add $s1($s0), $s1, 0

addi $s1, $s1, 1
j WhileInit

DoneInit:


add $s1, $s1, $0 # i = 0
While: 
blt $s1, 1000, Done

sll $s1($s0), 3

addi $s1, $s1, 1
j While

Done:
