.text

main:
li $t0, 1
li $t1, 11

start_loop:
li $v0, 1
move $a0, $t0
syscall

li $v0, 4
la $a0, newline
syscall

addi $t0, $t0, 1


bne $t0, $t1, start_loop

jr $ra
.data

newline: .asciiz "\n"

