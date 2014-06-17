.data
_prompt: .asciiz "Enter an integer:"
_ret: .asciiz "\n"
.globl main
.text

read:
	li $v0, 4
	la $a0, _prompt
	syscall
	li $v0, 5
	syscall
	jr $ra

write:
	li $v0, 1
	syscall
	li $v0, 4
	la $a0, _ret
	syscall
	move $v0, $0
	jr $ra

fib:
	move $s0, $a0
	li $t9, 2
	bgt $s0, $t9, label2
label1:
	li $t9, 1
	move $v0, $t9
	jr $ra
label2:
	addi $t0, $s0, -1
	move $a0, $t0
	addi $sp, $sp, -8
	sw $s0, 0($sp)
	sw $ra, 4($sp)
	jal fib
	lw $s0, 0($sp)
	lw $ra, 4($sp)
	addi $sp, $sp, 8
	move $t0, $v0
	addi $t1, $s0, -2
	move $a0, $t1
	addi $sp, $sp, -12
	sw $s0, 0($sp)
	sw $t0, 4($sp)
	sw $ra, 8($sp)
	jal fib
	lw $s0, 0($sp)
	lw $t0, 4($sp)
	lw $ra, 8($sp)
	addi $sp, $sp, 12
	move $t1, $v0
	add $t2, $t0, $t1
	move $v0, $t2
	jr $ra

main:
	li $t9, 15
	move $a0, $t9
	addi $sp, $sp, -4
	sw $ra, 0($sp)
	jal fib
	lw $ra, 0($sp)
	addi $sp, $sp, 4
	move $t0, $v0
	move $a0, $t0
	addi $sp, $sp, -4
	sw $ra, 0($sp)
	jal write
	lw $ra, 0($sp)
	addi $sp, $sp, 4
	li $t9, 0
	move $v0, $t9
	jr $ra
