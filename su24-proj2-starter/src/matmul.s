.globl matmul

.text
# =======================================================
# FUNCTION: Matrix Multiplication of 2 integer matrices
#   d = matmul(m0, m1)
# Arguments:
#   a0 (int*)  is the pointer to the start of m0
#   a1 (int)   is the # of rows (height) of m0
#   a2 (int)   is the # of columns (width) of m0
#   a3 (int*)  is the pointer to the start of m1
#   a4 (int)   is the # of rows (height) of m1
#   a5 (int)   is the # of columns (width) of m1
#   a6 (int*)  is the pointer to the the start of d
# Returns:
#   None (void), sets d = matmul(m0, m1)
# Exceptions:
#   Make sure to check in top to bottom order!
#   - If the dimensions of m0 do not make sense,
#     this function terminates the program with exit code 38
#   - If the dimensions of m1 do not make sense,
#     this function terminates the program with exit code 38
#   - If the dimensions of m0 and m1 don't match,
#     this function terminates the program with exit code 38
# =======================================================
matmul:
    ebreak
    li t0 1
    blt a1 t0 length_error
    blt a2 t0 length_error
    blt a4 t0 length_error
    blt a5 t0 length_error
    bne a2 a4 wrongformal_error
    
    addi sp sp -4
    sw ra 0(sp)
    li t0 0
    add t4 a6 x0
outer_loop_start:
    slli t2 t0 2
    mul t2 t2 a2
    add t2 a0 t2
    li t1 0
inner_loop_start:
    slli t3 t1 2
    add t3 a3 t3
    
    addi sp sp -40
    sw a0 0(sp)
    sw a1 4(sp)
    sw a2 8(sp)
    sw a3 12(sp)
    sw a4 16(sp)
    sw a5 20(sp)
    sw t0 24(sp)
    sw t1 28(sp)
    sw t2 32(sp)
    sw t4 36(sp)
    mv a0 t2
    mv a1 t3
    li a3 1
    mv a4 a5
    jal ra dot
    
    lw t4 36(sp)
    sw a0 0(t4)
    addi t4 t4 4
    
    lw a0 0(sp)
    lw a1 4(sp)
    lw a2 8(sp)
    lw a3 12(sp)
    lw a4 16(sp)
    lw a5 20(sp)
    lw t0 24(sp)
    lw t1 28(sp)
    lw t2 32(sp)
    addi sp sp 40
    
    addi t1 t1 1
    blt t1 a5 inner_loop_start
inner_loop_end:
    addi t0 t0 1
    blt t0 a1 outer_loop_start
outer_loop_end:
    lw ra 0(sp)
    addi sp sp 4
    jr ra

length_error:
wrongformal_error:
    li a0 38
    j exit