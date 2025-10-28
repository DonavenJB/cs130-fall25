@ Bruce, Donaven     CS130 Section 27623   2025-10-27
@ Third Laboratory Assignment - Zeckendorf Number
@ CPUlator: https://cpulator.01xz.net/?sys=arm-de1soc

    .syntax unified
    .cpu cortex-a9

    .text
    .align 2
    .global _start
    .global zeck
    .global fib

_start:                         @ set k, call zeck, park
    mov     r0, #33            @ sample test 
    @ ldr   r0, =1836311902    @ boundary test: expect R0=22
    @ ldr   r0, =1836311903    @ boundary test: expect R0=1
    @ ldr   r0, =1836311904    @ boundary test: expect R0=-1
    @ ldr   r0, =-7            @ negative test: expect R0=-1
	@ mov r0, #0
    bl      zeck
stop: b       stop

zeck:                           @ R0=k -> R0=Zeckendorf count
    push    {r1-r4, lr}
    mov     r1, r0             @ r1 = remaining
    cmp     r1, #0
    beq     Z_ret0
    bmi     Z_bad
    mov     r2, #0             @ r2 = count
Z_loop:
    cmp     r1, #0
    beq     Z_done
    mov     r0, r1
    bl      fib                @ r0 = largest Fib <= r1  or -1
    cmp     r0, #-1
    beq     Z_bad
    subs    r1, r1, r0         @ remaining -= Fib
    add     r2, r2, #1         @ count++
    b       Z_loop
Z_done:
    mov     r0, r2
    pop     {r1-r4, pc}
Z_ret0:
    mov     r0, #0
    pop     {r1-r4, pc}
Z_bad:
    mov     r0, #-1
    pop     {r1-r4, pc}

fib:                            @ R0=k -> R0=largest Fib <= k  (or -1/0)
    push    {r1-r5, lr}
    mov     r1, r0
    cmp     r1, #0
    beq     F_ret0
    bmi     F_bad
    ldr     r2, =FIB_MAX_31    @ r2 = &FIB_MAX_31
    ldr     r2, [r2]           @ r2 = 1836311903 (value)  <<< important
    cmp     r1, r2
    bgt     F_bad              @ k > max
    ldr     r3, =fib_table_end
    sub     r3, r3, #4         @ r3 -> last entry
    ldr     r5, =fib_table     @ r5 -> first entry
F_scan:
    ldr     r4, [r3]           @ r4 = Fib
    cmp     r4, r1
    ble     F_found
    cmp     r3, r5
    beq     F_bad              @ guard (should not happen)
    sub     r3, r3, #4
    b       F_scan
F_found:
    mov     r0, r4
    pop     {r1-r5, pc}
F_ret0:
    mov     r0, #0
    pop     {r1-r5, pc}
F_bad:
    mov     r0, #-1
    pop     {r1-r5, pc}

    .data
    .align 2
FIB_MAX_31:    .word   1836311903         @ largest 31-bit Fibonacci

fib_table:                              @ ascending Fibonacci table
    .word   0
    .word   1
    .word   2
    .word   3
    .word   5
    .word   8
    .word   13
    .word   21
    .word   34
    .word   55
    .word   89
    .word   144
    .word   233
    .word   377
    .word   610
    .word   987
    .word   1597
    .word   2584
    .word   4181
    .word   6765
    .word   10946
    .word   17711
    .word   28657
    .word   46368
    .word   75025
    .word   121393
    .word   196418
    .word   317811
    .word   514229
    .word   832040
    .word   1346269
    .word   2178309
    .word   3524578
    .word   5702887
    .word   9227465
    .word   14930352
    .word   24157817
    .word   39088169
    .word   63245986
    .word   102334155
    .word   165580141
    .word   267914296
    .word   433494437
    .word   701408733
    .word   1134903170
    .word   1836311903
fib_table_end:
