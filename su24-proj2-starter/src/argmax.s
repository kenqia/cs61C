.globl argmax

.text
# =================================================================
# FUNCTION: Given a int array, return the index of the largest
#   element. If there are multiple, return the one
#   with the smallest index.
# Arguments:
#   a0 (int*) is the pointer to the start of the array
#   a1 (int)  is the # of elements in the array
# Returns:
#   a0 (int)  is the first index of the largest element
# Exceptions:
#   - If the length of the array is less than 1,
#     this function terminates the program with error code 36
# =================================================================
argmax:
    # Prologue
    ble a1 x0 error
    mv t0 a0
    mv t1 a1
    lw t2 0(a0)
    li t4 0
loop_start:
    lw t3 0(t0)
    ble t3 t2 loop_continue
    mv t2 t3
    sub t4 t0 a0
loop_continue:
    addi t0 t0 4
    addi t1 t1 -1
    bgt t1 x0 loop_start
loop_end:
    srli a0 t4 2
    jr ra
error:
    li a0 36
    j exit