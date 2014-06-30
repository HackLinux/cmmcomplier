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

gcd:
	move $s0, $a0
	move $s1, $a1
	li $t9, 0
	bne $s1, $t9, label2
label1:
	move $v0, $s0
	jr $ra
	j label3
label2:
	div $s0, $s1
	mflo $t0
	mul $t1, $s1, $t0
	sub $t0, $s0, $t1
	move $a0, $s1
	move $a1, $t0
	addi $sp, $sp, -12
	sw $s0, 0($sp)
	sw $s1, 4($sp)
	sw $ra, 8($sp)
	jal gcd
	lw $s0, 0($sp)
	lw $s1, 4($sp)
	lw $ra, 8($sp)
	addi $sp, $sp, 12
	move $t0, $v0
	move $v0, $t0
	jr $ra
label3:
	li $t9, 0
	move $v0, $t9
	jr $ra

main:
	li $s0, 2013
	li $s1, 66
	move $a0, $s0
	move $a1, $s1
	addi $sp, $sp, -12
	sw $s0, 0($sp)
	sw $s1, 4($sp)
	sw $ra, 8($sp)
	jal gcd
	lw $s0, 0($sp)
	lw $s1, 4($sp)
	lw $ra, 8($sp)
	addi $sp, $sp, 12
	move $s2, $v0
	move $a0, $s2
	addi $sp, $sp, -4
	sw $ra, 0($sp)
	jal write
	lw $ra, 0($sp)
	addi $sp, $sp, 4
	li $t9, 0
	move $v0, $t9
	jr $ra
