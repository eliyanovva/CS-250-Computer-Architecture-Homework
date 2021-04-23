
.text
main:
	li $v0, 4
	la $a0, integer_prompt
	syscall

	li $v0, 5
#	move $t4, $a0
	syscall
	move $t4, $v0

		
	move $s0, $ra
#	move $a0, $t0
	addi $a0, $t4, 0
	jal func
	move $t0, $v0
	
	li $v0, 1
	move $a0, $t0
	syscall

	move $ra, $s0
	jr $ra

func:
	addi $sp, $sp, -8
	sw $ra, 4($sp)
	sw $a0, 0($sp)

	bne $a0, $zero, REC
	
	lw $ra, 4($sp)
	addi $v0, $zero, 5
	addi $sp, $sp, 8
	jr $ra

REC:
	addi $a0, $a0, -1
	jal func
	lw $a0, 0($sp)
	lw $ra, 4($sp)
	addi $sp, $sp, 8

	addi $t0, $a0, 1
	addi $t1, $zero, 4
	mul $t0, $t0, $t1
	addi $t0, $t0, -2
	add $v0, $v0, $v0
	add $v0, $v0, $t0

	jr $ra

.data
        integer_prompt: .asciiz "Please enter an integer: "

