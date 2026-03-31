.globl factorial

.data
n: .word 8

.text
main:
    la t0, n
    lw a0, 0(t0)
    jal ra, factorial

    addi a1, a0, 0
    addi a0, x0, 1
    ecall # Print Result

    addi a1, x0, '\n'
    addi a0, x0, 11
    ecall # Print newline

    addi a0, x0, 10
    ecall # Exit


# 栈空间操作
factorial:
    addi sp, sp, -8
    sw ra, 4(sp)
    sw a0, 0(sp)

    li t0, 1
    ble a0, t0, return_one

    addi a0, a0, -1
    jal factorial

    lw t1, 0(sp)
    mul a0, a0, t1

    j recover

return_one:
    li a0, 1

recover:
    lw ra, 4(sp)
    addi sp, sp, 8
    ret

# 纯寄存器操作
factorial_reg:
    # YOUR CODE HERE
    addi t0, x0, 1


loop:
    addi t1, x0, 1
    ble a0, t1, finish
    mul t0, t0, a0
    addi a0, a0, -1
    j loop


finish:
    mv a0, t0
    ret

