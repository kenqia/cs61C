.globl dot

.text
# =======================================================
# FUNCTION: Dot product of 2 int arrays
# Arguments:
#   a0 (int*) is the pointer to the start of arr0
#   a1 (int*) is the pointer to the start of arr1
#   a2 (int)  is the number of elements to use
#   a3 (int)  is the stride of arr0
#   a4 (int)  is the stride of arr1
# Returns:
#   a0 (int)  is the dot product of arr0 and arr1
# Exceptions:
#   - If the number of elements to use is less than 1,
#     this function terminates the program with error code 36
#   - If the stride of either array is less than 1,
#     this function terminates the program with error code 37
# =======================================================
dot:
    ble a2 x0 error36
    ble a3 x0 error37
    ble a4 x0 error37
    
    mv t0 a0
    mv t1 a1
    slli a3 a3 2
    slli a4 a4 2
    li t6 0
loop_start:
    lw t3 0(t0)
    lw t4 0(t1)
    mul t5 t3 t4
    add t6 t6 t5
    add t0 t0 a3
    add t1 t1 a4
    addi a2 a2 -1
    bgt a2 x0 loop_start
loop_end:
    mv a0 t6
    jr ra
    
error36:
    li a0 36
    j exit
error37:
    li a0 37
    j exit
