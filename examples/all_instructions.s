MOVI R0 #13   ; MOVI and init some test values
MOVI R1 #7

ADD R2 R0 R1  ; ADD result=#20
SUB R3 R0 R1  ; SUB result=#6
MUL R4 R0 R1  ; MUL result=#91

MOVI R5 0b11111110  ; addr (#254) for LDR and STR tests
STR R2 R5     ; STR
LDR R6 R5     ; LDR

PUSH R5       ; PUSH
POP R7        ; POP

BL #4         ; BL, PC_this_instruction=22, PC_destination=30

; put next instruction at #30
SUB R8 R0 R1  ; setup for BLZ
BLZ #8        ; BLZ with Z=false so skip instruction (target is #50)
MOVI R8 #13
SUB R8 R0 R8
BLZ #5       ; BLZ with Z=true so branch (target = #50)

; put next instruction at #50
MOVI R9 0b01000000    ; addr (#64 for BX)
BX R9                 ; BX

; put next instruction at #64
MOVI R10 0b01111111   ; setup for LSL
LSL R10 R10 #1        ; LSL result=0b11111110

MOVI R11 0b10101010   ; setup for NAND
NAND R11 R10 R11      ; NAND result=01010100

MOVI R12 #2           ; setup for DEC
DEC R12 R12 #3        ; DEC result=#-1

HLT                   ; HTL
