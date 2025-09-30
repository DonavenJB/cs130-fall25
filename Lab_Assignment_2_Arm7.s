@ CS130 Spring 2024 Section 27623 Lab Assignment 2 - Bruce, Donaven
@ Emulator: ARM DE1-SoC (CPUlator)

    .syntax unified
    .cpu cortex-a9
    .text
    .align 2
    .global _start

_start:
    @ local test: uncomment next line to preload R1; 
    @ ldr  r1, =0x5751

    uxth r0, r1            @ keep bits 15..0
    cmp  r0, #0            @ zero date?
    bne  parse

    mov  r2, #0            @ year=0
    mov  r3, #1            @ month=1
    mov  r4, #1            @ day=1
    b    stop

parse:
    ubfx r4, r0, #0, #5    @ day
    ubfx r3, r0, #5, #4    @ month
    ubfx r2, r0, #9, #7    @ year offset
    movw r12, #1980
    add  r2,  r2, r12      @ year = 1980 + offset

    cmp  r3, #1            @ month in 1..12?
    blo  bad_month
    cmp  r3, #12
    bhi  bad_month
    b    stop

bad_month:
    mov  r3, #15           @ invalid month
    b    stop

stop:
    b    stop              @ stop here
