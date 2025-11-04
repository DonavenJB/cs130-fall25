@ CS130 Spring 2024 Lab Assignment 4 - Bruce, Donaven - Section 15046
@ Emulator used: CPUlator

.equ maxI, 61   @ max index

.section .text
@ Test Harness
_start:
    LDR R0, =90321
    BL findZips

Stop:
    B Stop

@ Function
findZips:
    PUSH {R1-R5, LR}    @ Save registers and return address at start of function

    CMP R0, #1
    BLT ret_zero        @ Return 0 if ZIP is less than 1 (invalid input)

    LDR R5, =100000
    CMP R0, R5
    BGE ret_zero

    LDR R1, =Zips
    MOV R2, #maxI     @ load symbolic constant value

loop:
    CMP R2, #0
    BEQ not_found      @ Exit loop if all table entries checked

    LDR R3, [R1, #4]
    LDR R4, [R1, #8]

    CMP R0, R3
    BLT next
    CMP R0, R4
    BGT next        @ Skip entry if ZIP is outside this table range

    LDR R0, [R1, #0]        @ Found: load corresponding state code into R0
    B done

next:
    ADD R1, R1, #12
    SUB R2, R2, #1
    B loop

not_found:
    MOV R0, #0
    B done

ret_zero:
    MOV R0, #0

done:
    POP {R1-R5, LR}
    BX LR   @ Safe return


.section .data
@ Data Section
Zips:
Z2:   .word 2,99500,99999   @ AK
Z1:   .word 1,35000,36999   @ AL
Z5:   .word 5,71600,72999   @ AR
Z4:   .word 4,85000,86999   @ AZ
Z6:   .word 6,90000,96199   @ CA
Z8:   .word 8,80000,81999   @ CO
Z9:   .word 9,6000,6389     @ CT
Z9B:  .word 9,6391,6999     @ CT
Z11A: .word 11,20000,20099  @ DC
Z11B: .word 11,20200,20599  @ DC
Z11C: .word 11,56900,56999  @ DC
Z10:  .word 10,19700,19999  @ DE
Z12:  .word 12,32000,34999  @ FL
Z13A: .word 13,30000,31999  @ GA
Z13B: .word 13,39901,39901  @ GA
Z15:  .word 15,96700,96899  @ HI
Z19:  .word 19,50000,52999  @ IA
Z16:  .word 16,83200,83999  @ ID
Z17:  .word 17,60000,62999  @ IL
Z18:  .word 18,46000,47999  @ IN
Z20:  .word 20,66000,67999  @ KS
Z21:  .word 21,40000,42799  @ KY
Z22:  .word 22,70000,71599  @ LA
Z25:  .word 25,1000,2799    @ MA
Z24:  .word 24,20600,21999  @ MD
Z23:  .word 23,3900,4999    @ ME
Z26:  .word 26,48000,49999  @ MI
Z27:  .word 27,55000,56999  @ MN
Z29:  .word 29,63000,65999  @ MO
Z28:  .word 28,38600,39799  @ MS  
Z28B: .word 28,39902,39999  @ MS
Z3:   .word 3,59000,59999   @ MT
Z37:  .word 37,27000,28999  @ NC
Z38:  .word 38,58000,58999  @ ND
Z31:  .word 31,68000,69999  @ NE
Z33:  .word 33,3000,3899    @ NH
Z34:  .word 34,7000,8999    @ NJ
Z35:  .word 35,87000,88999  @ NM
Z32:  .word 32,89000,89999  @ NV
Z36A: .word 36,501,501      @ NY
Z36B: .word 36,544,544      @ NY
Z36C: .word 36,6390,6390    @ NY
Z36D: .word 36,10000,14999  @ NY
Z39:  .word 39,43000,45999  @ OH
Z40:  .word 40,73000,74999  @ OK
Z41:  .word 41,97000,97999  @ OR
Z42:  .word 42,15000,19699  @ PA
Z44:  .word 44,2800,2999    @ RI
Z45:  .word 45,29000,29999  @ SC
Z46:  .word 46,57000,57999  @ SD
Z47:  .word 47,37000,38599  @ TN
Z48A: .word 48,75000,79999  @ TX
Z48B: .word 48,88500,88599  @ TX
Z49:  .word 49,84000,84999  @ UT
Z51A: .word 51,20100,20199  @ VA
Z51B: .word 51,22000,24699  @ VA
Z50:  .word 50,5000,5999    @ VT
Z53:  .word 53,98000,99499  @ WA
Z55:  .word 55,53000,54999  @ WI
Z54:  .word 54,24700,26999  @ WV
Z56:  .word 56,82000,83199  @ WY
@ End of Zips table

.end