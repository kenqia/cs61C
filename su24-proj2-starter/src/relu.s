.globl relu

.text
# ==============================================================================
# FUNCTION: Performs an inplace element-wise ReLU on an array of ints
# Arguments:
#   a0 (int*) is the pointer to the array
#   a1 (int)  is the # of elements in the array
# Returns:
#   None
# Exceptions:
#   - If the length of the array is less than 1,
#     this function terminates the program with error code 36
# ==============================================================================
relu:
    li t0 1
    blt a1 t0 error
    mv t1 a0
loop_start:
    lw t2 0(t1)
    bge t2 x0 loop_continue
    sw x0 0(t1)
loop_continue:
    addi t1 t1 4
    addi a1 a1 -1
    bgt a1 x0 loop_start
loop_end:
    jr ra
error:
    li a0 36
    j exit