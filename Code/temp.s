.data
_prompt: .asciiz "Enter an integer: "
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
	addi $sp, $sp, -4
	sw $ra, 0($sp)
	addi $sp, $sp, -4
	sw $fp, 0($sp)
	move $fp, $sp
	li $v0, 0
	move $s0, $v0
	li $v0, 1
	move $s1, $v0
	li $v0, 0
	move $s2, $v0
	li $v0, 4
	move $s3, $v0
	addi $sp, $sp, -4
	sw $s0, -4($fp)
	addi $sp, $sp, -4
	sw $s1, -8($fp)
	addi $sp, $sp, -4
	sw $s2, -12($fp)
	addi $sp, $sp, -4
	sw $s3, -16($fp)
label0:
	lw $s0, -12($fp)
	lw $s1, -16($fp)
	blt $s0, $s1, label1
	j label2
label1:
	lw $s0, -4($fp)
	lw $s1, -8($fp)
	add $s2, $s0, $s1
	move $s0, $s2
	move $a0, $s1
	jal write
	move $s3, $s1
	move $s1, $s0
	lw $s4, -12($fp)
	li $v1, 1
	add $s5, $s4, $v1
	move $s4, $s5
	j label0
	addi $sp, $sp, -4
	sw $s0, -20($fp)
	sw $s1, -8($fp)
	addi $sp, $sp, -4
	sw $s2, -24($fp)
	sw $s3, -4($fp)
	sw $s4, -12($fp)
	addi $sp, $sp, -4
	sw $s5, -28($fp)
label2:
	li $v0, 0
	addi $sp, $sp, 28
	move $v0, $v0
	lw $fp, 0($sp)
	addi $sp, $sp, 4
	lw $ra, 0($sp)
	addi $sp, $sp, 4
	jr $ra
