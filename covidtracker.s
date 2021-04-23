
.data
.align 2
	first_name: .asciiz "Please enter patient:"
.align 2
	second_name: .asciiz "Please enter infecter:"
.align 2	
	stop: .asciiz "DONE\n"
.align 2
	buffer: .space 20
.align 2
	buffer_2: .space 20
.align 2
	root: .asciiz "BlueDevil\n"
.align 2
.text

main:

	la $t0, stop # put DONE in t0
	
	li $a0, 28 # allocate dynamically memory for the root
	li $v0, 9
	syscall
	
	move $s7, $v0 # store root in s7
	
	move $a0, $s7 # copy "BlueDevil" into the root buffer
	la $a1, root
	jal strcpy

	#move $s7, $v0 #store BlueDevil as root in register s7
	
	#li $v0, 4    this code was used for printing, I have 
	#lw $a0, 0($s7)   preserved it for further use and testing
	#li $a1, 20
	#syscall
	
loop: # loop section receives the input
	li $v0, 4 # print message to get first name
	la $a0, first_name
	syscall
	
	li $v0, 8 # get first name
	la $a0, buffer
	li $a1, 20
	syscall
	move $s0, $a0
	#syscall # move pointer to first name to s0
	
	la $a1, stop # move DONE to a1
	jal strcmp # compare first name with DONE
	beq $v0, $zero, after # exit loop if the first name is DONE
	
	li $v0, 4 # prompt for the second name
        la $a0, second_name
        syscall

	li $v0, 8 # get the second name
        la $a0, buffer_2
        li $a1, 20
        syscall
	move $s1, $a0 #store the second name in s1
       # syscall

	li $a0, 28 # dynamically allocate memory for a new node
	li $v0, 9
	syscall
	move $a0, $v0 #put pointer to new node in a0
	move $a1, $s0 #put  the first name (infected person) in a1
	jal strcpy # copy the name into the new node

	add $s2, $v0, $zero # put struct node with infected person in $s2
	sw $zero, 20($s2) # set left pointer in the new node to 0
	sw $zero, 24($s2) # set right pointer in the new node to 0

#	li $v0, 4
#        la $a0, 0($s2)
#        la $a1, 2
#        syscall

	move $a0, $s7 # pointer to root
	move $a1, $s2 # pointer to node with infected person
	move $a2, $s1 # pointer to string containing infector
	jal find_and_insert # find the location of the infector in the tree and insert the new root as his child	

	j loop

after: # this happens after we exit the loop
	move $a0, $s7 # put the root in a0
	addi $sp, $sp, -4
	sw $s7, 0($sp)
	jal print # print the tree. the print function does not work yet
	add $sp, $sp, 4
	jr $ra # return control

print: # doesn't work, I tried fixing find_and_insert first 

#	beq $a0, $zero, NULL_print
	
	addi $sp, $sp, -8 # store the return address and the input in the stack
	sw $ra, 0($sp)
	sw $s0, 4($sp)
	lw $a0, 8($sp)	
	
	j NULL_print

continue:

	addi $sp, $sp, -4
	lw $t1, 20($a0)
	sw $t1, 0($sp)
	jal print

	li $v0, 4 # print the string in the input
      #  lw $a0, 0($a0)
        li $a1, 20
        syscall

#	lw $a0, 4($sp) # recover  original input
	lw $a0, 24($s0) # set input to the left pointer 
       # beq $a0, $zero, NULL_print # if the pointer is empty, go to NULL_print
      #  jal print # recursive call

#	lw $a0, 4($sp) # repeat procedure for right pointer
        lw $a0, 24($s0)
       # beq $a0, $zero, NULL_print
        jal print

	lw $s0, 4($sp)
	lw $ra, 8($sp)
	addi $sp, $sp, 8
	jr $ra

NULL_print: # exit the function whwe have a pointer equals NULL
        bne $a0, $zero, continue
	lw $ra, 4($sp)
	lw $a0, 0($sp)
	add $sp, $sp, 12
#	lw $ra, 8($sp) # restore return address
#	addi $sp, $sp, 8 # clear stack
        jr $ra



find_and_insert: # a0 = root, a1 = name that has to be inserted, a2 = string with the name of the parent of the new node
	addi $sp, $sp, -20 # allocate space on the stack
	sw $ra, 8($sp) # store the return address 
	sw $a0, 0($sp) # store the pointer to the root
	sw $a1, 4($sp) # store the pointer to the new node

	lw $t1, 20($a0) # load the pointer to the left child of the root in t1
        sw $t1, 12($sp)	# store t1

	lw $t1, 24($a0) # repeat the procedure for the pointer to the right child
        sw $t1, 16($sp) #store right and left pointers of both nodes in the stack

	move $a1, $a2 # load the parent string into a1 
	jal strcmp # compare root string with parent string
	bne $v0, $zero, not_found # continue searching if the strings do no match

	lw $a1, 4($sp) # load back a1
	lw $a0, 0($sp) # load back a0
	lw $t0, 12($sp) # load pointer to the left child of the root in t0
        lw $t1, 16($sp) # load pointer to the right chilld of the root in t1

	lw $t1, 12($sp) # load the left pointer in the root in t1
	
	bne $t1, $zero, has_left # if the left pointer is full, go to has_left
	sw $a1, 20($a0) #set the left pointer of the root to the new node
	lw $ra, 8($sp)
	addi $sp, $sp, 20 # clear the stack
	jr $ra # return
	
has_left: # come here if the left pointer is not NULL. I have not implemented the case where the leftnode contains a string which is larger than the new string yet, I want to get the base case to work first.

        sw $a1, 24($a0) # set the right pointer of the root to the new node
       	lw $ra, 8($sp)
	addi $sp, $sp, 20 # clear the stack
        jr $ra #return


not_found: # the search function comes here if the string of the root and the parent don't match

	lw $a0, 20($a0) # set the root to is left child
	beq $a0, $zero, NULL # branch if the child is NULL
	jal find_and_insert # call the function with the new root

	lw $a0, 24($a0) # set the root to its right child
	beq $a0, $zero, NULL # branch if the child is NULL
	jal find_and_insert # call the function with the new root

NULL: # if the new root is NULL, exit the function by freeing the stack and returning
	lw $ra, 8($sp)
	addi $sp, $sp, 20
	jr $ra



# I copied the strcpy and strcmp functions directly from the document without changing them

strcpy:
	lb $t0, 0($a1)
	beq $t0, $zero,done_copying
	sb $t0, 0($a0)
	addi $a0, $a0, 1
	addi $a1, $a1, 1
	j strcpy

	done_copying:
	jr $ra

strcmp:
lb $t0, 0($a0)
lb $t1, 0($a1)
addi $t5, $zero, 0

bne $t0, $t1, done_with_loop
addi $a0, $a0, 1
addi $a1, $a1, 1
bnez $t0, strcmp
li $v0, 0
addi $t5, $zero, 1
jr $ra

done_with_loop:

sub $a0, $a0, $t5
sub $a1, $a1, $t5
sub $v0, $t0,$t1
jr $ra
