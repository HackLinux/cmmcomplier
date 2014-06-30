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
	li $s0, 5
label1:
	li $t9, 0
	ble $s0, $t9, label3
label2:
	li $t9, 2
	beq $s0, $t9, label5
label6:
	li $t9, 4
	beq $s0, $t9, label5
label4:
	move $a0, $s0
	addi $sp, $sp, -4
	sw $ra, 0($sp)
	jal write
	lw $ra, 0($sp)
	addi $sp, $sp, 4
label5:
	addi $s0, $s0, -1
	j label1
label3:
	li $t9, 0
	move $v0, $t9
	jr $ra
