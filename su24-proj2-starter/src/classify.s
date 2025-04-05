.globl classify

.text
# =====================================
# COMMAND LINE ARGUMENTS
# =====================================
# Args:
#   a0 (int)        argc
#   a1 (char**)     argv
#   a1[1] (char*)   pointer to the filepath string of m0
#   a1[2] (char*)   pointer to the filepath string of m1
#   a1[3] (char*)   pointer to the filepath string of input matrix
#   a1[4] (char*)   pointer to the filepath string of output file
#   a2 (int)        silent mode, if this is 1, you should not print
#                   anything. Otherwise, you should print the
#                   classification and a newline.
# Returns:
#   a0 (int)        Classification
# Exceptions:
#   - If there are an incorrect number of command line args,
#     this function terminates the program with exit code 31
#   - If malloc fails, this function terminates the program with exit code 26
#
# Usage:
#   main.s <M0_PATH> <M1_PATH> <INPUT_PATH> <OUTPUT_PATH>
classify:
    li t0 5
    bne a0 t0 wronginput_error
    
    addi sp sp -52
    sw s0 0(sp)
    sw s1 4(sp)
    sw s2 8(sp)
    sw s3 12(sp)
    sw s4 16(sp)
    sw s5 20(sp)
    sw s6 24(sp)
    sw s7 28(sp)
    sw s8 32(sp)
    sw s9 36(sp)
    sw s10 40(sp)
    sw s11 44(sp)
    sw ra 48(sp)

    mv s0 a0
    mv s1 a1
    mv s2 a2
    
    li a0 8
    jal ra malloc
    beq a0 x0 malloc_error
    mv s4 a0
    mv a1 a0
    addi a2 a1 4
    lw a0 4(s1)
    jal ra read_matrix
    mv s3 a0
    
    li a0 8
    jal ra malloc
    beq a0 x0 malloc_error
    mv s6 a0
    mv a1 a0
    addi a2 a1 4
    lw a0 8(s1)
    jal ra read_matrix
    mv s5 a0
    
    li a0 8
    jal ra malloc
    beq a0 x0 malloc_error
    mv s8 a0
    mv a1 a0
    addi a2 a1 4
    lw a0 12(s1)
    jal ra read_matrix
    mv s7 a0
    
    lw t0 0(s4)
    lw t1 4(s8)
    mul a0 t0 t1
    slli a0 a0 2
    jal ra malloc
    beq a0 x0 malloc_error
    mv s9 a0
    mv a6 a0
    mv a0 s3
    lw a1 0(s4)
    lw a2 4(s4)
    mv a3 s7
    lw a4 0(s8)
    lw a5 4(s8)
    jal ra matmul
    
    mv a0 s9
    lw t0 0(s4)
    lw t1 4(s8)
    mul a1 t0 t1
    jal ra relu

    lw t0 0(s6)
    lw t1 4(s8)
    mul a0 t0 t1
    slli a0 a0 2
    jal ra malloc
    beq a0 x0 malloc_error
    mv s10 a0
    mv a6 a0
    mv a0 s5
    lw a1 0(s6)
    lw a2 4(s6)
    mv a3 s9
    lw a4 0(s4)
    lw a5 4(s8)
    jal ra matmul
    
    lw a0 16(s1)
    mv a1 s10
    lw a2 0(s6)
    lw a3 4(s8)
    jal ra write_matrix
    
    mv a0 s10
    lw t0 0(s6)
    lw t1 4(s8)
    mul a1 t0 t1
    jal ra argmax
    mv s11 a0
    li t0 1
    beq s2 t0 next
    jal ra print_int
    li a0 '\n'
    jal ra print_char
next:
    mv a0 s3
    jal ra free
    mv a0 s4
    jal ra free
    mv a0 s5
    jal ra free
    mv a0 s6
    jal ra free
    mv a0 s7
    jal ra free
    mv a0 s8
    jal ra free
    mv a0 s9
    jal ra free
    mv a0 s10
    jal ra free
    
    mv a0 s11
    
    lw s0 0(sp)
    lw s1 4(sp)
    lw s2 8(sp)
    lw s3 12(sp)
    lw s4 16(sp)
    lw s5 20(sp)
    lw s6 24(sp)
    lw s7 28(sp)
    lw s8 32(sp)
    lw s9 36(sp)
    lw s10 40(sp)
    lw s11 44(sp)
    lw ra 48(sp)
    addi sp sp 52
    jr ra

malloc_error:
    li a0 26
    j exit
wronginput_error:
    li a0 31
    j exit