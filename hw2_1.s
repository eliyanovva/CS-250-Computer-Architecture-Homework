
.text
main:
	li $v0, 4
	la $a0, integer_prompt
	syscall
	
	li $v0, 5
	move $t0, $a0
	syscall

	move $t0, $v0
	move $t1, $t0
	move $t2, $zero
	
	while:
	
	mult $t0, $t1
	mflo $t0
	
	addi $t2, $t2, 1
	bne $t2, $t1, exit_while
	j while
	
	exit_while:

	li $v0, 1
	move $a0, $t0
	syscall

	jr $ra




.data
	integer_prompt: .asciiz "Please enter an integer: "
