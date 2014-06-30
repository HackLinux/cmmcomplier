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

main:
	li $s0, 0
	li $s1, 4
label1:
	bge $s0, $s1, label3
label2:
	li $s2, 0
label4:
	bge $s2, $s0, label6
label5:
	move $a0, $s2
	addi $sp, $sp, -4
	sw $ra, 0($sp)
	jal write
	lw $ra, 0($sp)
	addi $sp, $sp, 4
	addi $s2, $s2, 1
	j label4
label6:
	addi $s0, $s0, 1
	j label1
label3:
	li $t9, 0
	move $v0, $t9
	jr $ra
