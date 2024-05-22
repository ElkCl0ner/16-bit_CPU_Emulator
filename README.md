# 16-bit CPU

Logic gate level emulation of a 16-bit cpu written in `C`

## Architecture

- 16 Registers (latches are not emulated, values are stored in your machine's real memory)
  - 13 General Purpose (R0-R12)
  - SP
  - LR
  - PC
- Instruction Fetcher
- Control Unit
- Register Reader
- LSL Module
- Z Flag Bit
- ALU
- Memory Interface (RAM chips are not emulated, memory is stored in your machine's real memory)
- Register Writer
- Z Flag Writter

```
 REGISTERS
▄▄▄▄▄▄▄▄▄▄▄
█   R 0   █       ▄▄▄▄▄▄▄▄▄▄▄▄▄▄▄▄▄        ▄▄▄▄▄▄▄▄▄▄▄▄
█   R 1   █       █               █        █          █
█   R 2   █  ┌───>█  Instruction  █───────>█   Main   █
█   R 3   █  │    █    Fetcher    █<───────█  Memory  █
█   R 4   █  │    █               █        █          █
█   R 5   █  │    ▀▀▀▀▀▀▀▀▀▀▀▀▀▀▀▀▀        ▀▀▀▀▀▀▀▀▀▀▀▀
█   R 6   █  │                │
█   R 7   █  │                │     ▄▄▄▄▄▄▄▄▄▄▄▄▄                                      ▄▄▄▄▄▄▄▄
█   R 8   █  │                └────>█           █<─────────────────────────────────────█  Z   █
█   R 9   █  │ ┌────────────────────█  Control  █───────────────────────┐              █ Flag █
█   R10   █  │ │  ▄▄▄▄▄▄▄▄▄▄▄▄▄▄    █   Unit    █─────────────────────┐ │              ▀▀▀▀▀▀▀▀
█   R11   █  │ │  █            █<───█           █──────┐              │ │                ^
█   R12   █──┼─┼─>█  Register  █    ▀▀▀▀▀▀▀▀▀▀▀▀▀      v              │ │                │
█   S P   █  │ │  █   Reader   █           │  │      ▄▄▄▄▄▄▄▄▄▄▄▄▄▄▄  │ │                │
█   L R   █  │ │  █            █           │  │      █             █  │ │                │
█   P C───█──┘ │  █ RA      RB █─────┬─────┼──┼──┐   █   Memory    █  │ │  ▄▄▄▄▄▄▄▄▄▄▄▄  │
▀▀▀▀▀▀▀▀▀▀▀    │  ▀▀▀▀▀▀▀▀▀▀▀▀▀▀     v     │  │  │   █  Interface  █  │ │  █          █  │
     ^         v    │             ▄▄▄▄▄▄▄  │  │  │   █             █──┼─┼─>█   Main   █  │
     │  ▄▄▄▄▄▄▄▄▄▄  │             █ MUX █<─┘  │  └──>█Addr         █<─┼─┼──█  Memory  █  │
     │  █  LSL   █<─┴─────┐       ▀▀▀▀▀▀▀     │      █             █  │ │  █          █  │
     │  █ Module █──┐     └──────────┼────────┼─────>█DataIn       █  │ │  ▀▀▀▀▀▀▀▀▀▀▀▀  │
     │  ▀▀▀▀▀▀▀▀▀▀  v                v        │      █   DataOut   █  │ │                │
     │       ▄▄▄▄▄▄▄▄▄▄▄▄▄▄   ▄▄▄▄▄▄▄▄▄▄▄▄▄▄▄ │      ▀▀▀▀▀▀▀▀▀▀▀▀▀▀▀  │ │                │
     │       █     In1     █ █      In2     █ │             │         │ │                │
     │        █             █       Control█<─┘             │         │ │  ▄▄▄▄▄▄▄▄▄▄▄   │
     │         █           ALU            █                 │         │ └─>█ Z Flag  █───┘
     │          █  Zout      Output      █                  │         │ ┌─>█ Writter █
     │           ▀▀▀▀▀▀▀▀▀▀▀▀▀▀▀▀▀▀▀▀▀▀▀▀                   │         │ │  ▀▀▀▀▀▀▀▀▀▀▀
     │              │           │                           │         │ │
     │              └───────────┼───────────────────────────┼─────────┼─┘
     │                          │                           │         │
     │                          │                           │         │
     │                          │                           │         │
     │                          v                           │         │
     │                  ▄▄▄▄▄▄▄▄▄▄▄▄▄▄                      │         │
     │                  █            █<─────────────────────┘         │
     └──────────────────█  Register  █                                │
                        █  Writter   █                                │
                        █            █<───────────────────────────────┘
                        ▀▀▀▀▀▀▀▀▀▀▀▀▀▀
```

## Instruction Set

| Mnemonic | Full Name        | Opcode | Operands       | Set Z | ALU Rdst RA RB  |
| :------- | :--------------- | -----: | :------------- | ----- | --------------- |
| HLT      | Halt             |   0000 | XXXX XXXX XXXX | NO    | NOP             |
| ADD      | Add              |   0001 | Rdst RegA RegB | YES   | ADD Rdst RA RB  |
| SUB      | Subtract         |   0010 | Rdst RegA RegB | YES   | SUB Rdst RA RB  |
| MUL      | Multiply         |   0011 | Rdst RegA RegB | YES   | MUL Rdst RA RB  |
| MOVI     | Move Immediate   |   0100 | Rdst Imm8 """" | NO    | ADD Rdst 0 Imm  |
| MOVR     | Move Register    |   0101 | Rdst RegA XXXX | NO    | ADD Rdst RA 0   |
| LDR      | Load             |   0110 | Rdst XXXX Addr | NO    | NOP             |
| STR      | Store            |   0111 | XXXX RegA Addr | NO    | NOP             |
| PUSH     | Push stack       |   1000 | XXXX RegA XXXX | NO    | SUB SP SP -1    |
| POP      | Pop stack        |   1001 | XXXX RegA XXXX | NO    | ADD SP SP 1     |
| BL       | Branch and link  |   1010 | Im12 """" """" | NO    | ADD PC PC Imm\* |
| BLZ      | Branch if ZERO   |   1011 | Im12 """" """" | NO    | ADD PC PC Imm\* |
| BX       | Jump to address  |   1100 | XXXX RegA XXXX | NO    | ADD PC RA 0     |
| LSL      | Logic shift left |   1101 | Rdst RegA Imm4 | YES   | ADD Rdst RA 0   |
| NAND     | Bitwise NAND     |   1110 | Rdst RegA RegB | YES   | NAND Rdst RA RB |
| DEC      | Decrement        |   1111 | Rdst RegA Imm4 | YES   | SUB Rdst RA Imm |

(1) Imm\*: the immediate is added directly, it is not multiplied by the word size
(2) Imm\*: the immediate is added directly, it is not multiplied by the word size

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
| CLR      | Clear       | LSL RA RA #16   |

## Memory Mappings

Memory is **Big-Endian**
Memory access must be word aligned

```markdown
# Initial Register Values

R0-R12 = 0x0000
SP = 0x
LR = 0x0000
PC = 0x0000
```

| IO Device           | [Start | End)   | %      | cumul% |
| ------------------- | -----: | :----- | ------ | ------ |
| Screen Pixel Buffer | 0xE2B4 | 0x0000 | 22.89% | 22.89% |
| Screen Control      | 0xE2B2 | 0xE2B4 | 0.003% | 22.89% |
| Screen Char Buffer  |        | 0xE2B2 | %      |        |
| Keyboard IO         |        |        |        |
| Pushbuttons         |        |        |
| Toggle Switches     |        |        |
| LEDs                |        |        |
| 7-Segment Displays  |        |        |

### Screen Pixel Buffer

```markdown
# Screen Size

width = 100
height = 75
=> 7500 pixels
=> 15'000 B
=> ~22.89% of total memory

# Pixel Data

size = 2B = 1 word

|7654 3210 7654 3210|
|RRRR GGGG BBBB AAAA|

RRRR = red
GGGG = green
BBBB = blue
AAAA = alpha
```

### Screen Control

```markdown
# Control Flags

|7654 3210 7654 3210|
|XXXX XXXX XXXX XXXD|

D = draw pixel buffer to screen
```

### Screen Char Buffer

## TODO
