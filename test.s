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

fact:
	move $s0, $a0
	li $t9, 1
	bne $s0, $t9, label2
label1:
	move $v0, $s0
	jr $ra
	j label3
label2:
	addi $t0, $s0, -1
	move $a0, $t0
	addi $sp, $sp, -8
	sw $s0, 0($sp)
	sw $ra, 4($sp)
	jal fact
	lw $s0, 0($sp)
	lw $ra, 4($sp)
	addi $sp, $sp, 8
	move $t0, $v0
	mul $t1, $s0, $t0
	move $v0, $t1
	jr $ra
label3:
	li $t9, 0
	move $v0, $t9
	jr $ra

main:
	addi $sp, $sp, -4
	sw $ra, 0($sp)
	jal read
	lw $ra, 0($sp)
	addi $sp, $sp, 4
	move $s0, $v0
	li $t9, 1
	ble $s0, $t9, label5
label4:
	move $a0, $s0
	addi $sp, $sp, -8
	sw $s0, 0($sp)
	sw $ra, 4($sp)
	jal fact
	lw $s0, 0($sp)
	lw $ra, 4($sp)
	addi $sp, $sp, 8
	move $s1, $v0
	j label6
label5:
	li $s1, 1
label6:
	move $a0, $s1
	addi $sp, $sp, -4
	sw $ra, 0($sp)
	jal write
	lw $ra, 0($sp)
	addi $sp, $sp, 4
	li $t9, 0
	move $v0, $t9
	jr $ra


