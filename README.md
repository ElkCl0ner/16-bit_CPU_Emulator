# 16-bit CPU

Logic gate level emulation of a 16-bit cpu written in `C`

## Instruction Set

| Mnemonic | Full Name        | Opcode | Operands       | Set Flag |
| :------- | :--------------- | -----: | :------------- | -------- |
| HLT      | Halt             |   0000 | XXXX XXXX XXXX | NO       |
| ADD      | Add              |   0010 | Rdst RegA RegB | YES      |
| SUB      | Subtract         |   0011 | Rdst RegA RegB | YES      |
| MUL      | Multiply         |   0011 | Rdst RegA RegB | YES      |
| MOVI     | Move Immediate   |   0100 | Rdst Imm8 """" | NO       |
| MOVR     | Move Register    |   0101 | Rdst RegA XXXX | NO       |
| LDR      | Load             |   0110 | Rdst XXXX Dest | NO       |
| STR      | Store            |   0111 | XXXX RegA Dest | NO       |
| PUSH     | Push stack       |   1000 | XXXX RegA XXXX | NO       |
| POP      | Pop stack        |   1001 | XXXX RegA XXXX | NO       |
| BL       | Branch and link  |   1010 | Im12 """" """" | NO       |
| BLZ      | Branch if ZERO   |   1011 | Im12 """" """" | NO       |
| BX       | Jump to address  |   1100 | XXXX RegA XXXX | NO       |
| LSL      | Logic shift left |   1101 | Rdst RegA Imm4 | YES      |
| NAND     | Bitwise NAND     |   1110 | Rdst RegA RegB | YES      |
| DEC      | Decrement        |   1111 | Rdst RegA Imm4 | YES      |
