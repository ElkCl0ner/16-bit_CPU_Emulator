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
| LDR      | Load             |   0110 | Rdst XXXX Addr | NO       |
| STR      | Store            |   0111 | XXXX RegA Addr | NO       |
| PUSH     | Push stack       |   1000 | XXXX RegA XXXX | NO       |
| POP      | Pop stack        |   1001 | XXXX RegA XXXX | NO       |
| BL       | Branch and link  |   1010 | Im12 """" """" | NO       |
| BLZ      | Branch if ZERO   |   1011 | Im12 """" """" | NO       |
| BX       | Jump to address  |   1100 | XXXX RegA XXXX | NO       |
| LSL\*    | Logic shift left |   1101 | Rdst RegA Imm4 | YES      |
| NAND     | Bitwise NAND     |   1110 | Rdst RegA RegB | YES      |
| DEC\*    | Decrement        |   1111 | Rdst RegA Imm4 | YES      |

LSL*: maybe turn into LSR (maybe remove Imm4) since LSL can be done with ADD Rdst RA RA
DEC*: maybe only allow DEC by 1

**Pseudo Instructions**

| Mnemonic | Full Name   | Resolution      |
| :------- | :---------- | --------------- |
| CMP      | Compare     | SUB R? RA RB    |
| NOT      | Bitwise NOT | NAND Rdst RA RA |
| AND      | Bitwise AND | NAND Rdst RA RB |
|          |             | NOT Rdst Rdst   |
| OR       | Bitwise OR  | NOT RA RA       |
|          |             | NOT RB RB       |
|          |             | NAND Rdst RA RB |
|          |             | NOT Rdst Rdst   |
| NOR      | Bitwise NOR | OR Rdst RA RB   |
|          |             | NOT Rdst Rdst   |

# TODO

- [ ] Consider bit orders (is `{LSB, ..., MSB}` good? should it be different for different circuits?)
