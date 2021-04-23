
.text
main:
	li $v0, 5
	syscall
	move $t0, $v0
	move $t1, $t0
	move $t2, $zero
	
	while:
	
	move $t3, $zero

	for:
	$add $t0, $t1, $t0
	$addi $t3, $t3, 1
	
	bne $t3, $t1, exit_for
	j for

	exit_for:
	
	$addi $t2, $t2, 1
	bne $t2, $t1, exit_while
	j while
	
	exit_while:

	li $v0, 1
	la $a0, $t0
	syscall

	jr $ra




.data
	integer_prompt: .asciiz "Please enter an integer: "
	age_prompt: .asciiz "Please enter your age: "
	my_string: .asciiz " will turn 50 in the year "
	name: .space 16
