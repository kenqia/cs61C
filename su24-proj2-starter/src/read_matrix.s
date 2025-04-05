.globl read_matrix

.text
# ==============================================================================
# FUNCTION: Allocates memory and reads in a binary file as a matrix of integers
#
# FILE FORMAT:
#   The first 8 bytes are two 4 byte ints representing the # of rows and columns
#   in the matrix. Every 4 bytes afterwards is an element of the matrix in
#   row-major order.
# Arguments:
#   a0 (char*) is the pointer to string representing the filename
#   a1 (int*)  is a pointer to an integer, we will set it to the number of rows
#   a2 (int*)  is a pointer to an integer, we will set it to the number of columns
# Returns:
#   a0 (int*)  is the pointer to the matrix in memory
# Exceptions:
#   - If malloc returns an error,
#     this function terminates the program with error code 26
#   - If you receive an fopen error or eof,
#     this function terminates the program with error code 27
#   - If you receive an fclose error or eof,
#     this function terminates the program with error code 28
#   - If you receive an fread error or eof,
#     this function terminates the program with error code 29
# ==============================================================================
read_matrix:
    addi sp sp -24
    sw s0 0(sp)
    sw s1 4(sp)
    sw s2 8(sp)
    sw s3 12(sp)
    sw s4 16(sp)
    sw ra 20(sp)
    
    mv s0 a0
    mv s1 a1
    mv s2 a2
    
    li a1 0
    jal ra fopen
    li t6 -1
    beq a0 t6 fopen_error
    mv s0 a0
    
    li a0 8
    jal ra malloc
    beq a0 x0 malloc_error
    mv a1 a0
    mv s3 a1
    mv a0 s0
    li a2 8
    jal ra fread
    li t6 8
    bne a0 t6 fread_error
    
    lw t0 0(s3)
    lw t1 4(s3)
    mul t0 t0 t1
    slli a0 t0 2
    jal ra malloc
    beq a0 x0 malloc_error
    mv a1 a0
    mv s4 a1
    mv a0 s0
    lw t0 0(s3)
    lw t1 4(s3)
    mul t0 t0 t1
    slli a2 t0 2
    jal ra fread
    lw t0 0(s3)
    lw t1 4(s3)
    mul t0 t0 t1
    slli t0 t0 2
    bne a0 t0 fread_error
    
    mv a0 s0
    jal ra fclose
    bne a0 x0 fclose_error
    
    lw t0 0(s3)
    lw t1 4(s3)
    sw t0 0(s1)
    sw t1 0(s2)
    mv a0 s3
    jal ra free
    
    mv a0 s4
    
    lw s0 0(sp)
    lw s1 4(sp)
    lw s2 8(sp)
    lw s3 12(sp)
    lw s4 16(sp)
    lw ra 20(sp)
    addi sp sp 24
    jr ra

malloc_error:
    li a0 26
    j exit
fopen_error:
    li a0 27
    j exit
fclose_error:
    li a0 28
    j exit
fread_error:
    li a0 29
    j exit