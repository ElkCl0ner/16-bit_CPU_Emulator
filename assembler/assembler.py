import sys

registers = {
  'R0': 0,
  'R1': 1,
  'R2': 2,
  'R3': 3,
  'R4': 4,
  'R5': 5,
  'R6': 6,
  'R7': 7,
  'R8': 8,
  'R9': 9,
  'R10': 10,
  'R11': 11,
  'R12': 12,
  'SP': 13,
  'LR': 14,
  'PC': 15,
}

def parse_immediate(imm: str, limit: int) -> int:
  if imm[0] == '#': # BUG: cannot use "#" for negative numbers, must explicitely write bin of hex representation
    out = int(imm[1:])
  elif imm[0:2] == '0b':
    out = int(imm[2:], 2)
  elif imm[0:2] == '0x':
    out = int(imm[2:], 16)
  else:
    print('Error: Invalid immediate.\nTerminating.')
    exit(99)

  if out > 2 ** limit:
    print('Error: Immediate too big.\nTerminating.')
    exit(99)

  return out

def print_operands_error(operation: str, operands: list[str]) -> None:
  print(f'Error: invalid operands for {operation} instruction: {operands}.\n Terminating.')
  exit(3)

def validate_number_operands(operation: str, operands: list[str], expected_len: int) -> None:
  if len(operands) != expected_len:
    print_operands_error(operation, operands)

def parse_HLT(operands: list[str]) -> int:
  validate_number_operands('HLT', operands, 0)
  
  return 0b0000_0000_0000_0000

def parse_ADD(operands: list[str]) -> int:
  validate_number_operands('ADD', operands, 3)

  out = 0b0001_0000_0000_0000
  out |= registers[operands[0]] << 8
  out |= registers[operands[1]] << 4
  out |= registers[operands[2]]

  return out

def parse_SUB(operands: list[str]) -> int:
  validate_number_operands('SUB', operands, 3)

  out = 0b0010_0000_0000_0000
  out |= registers[operands[0]] << 8
  out |= registers[operands[1]] << 4
  out |= registers[operands[2]]

  return out

def parse_MUL(operands: list[str]) -> int:
  validate_number_operands('MUL', operands, 3)

  out = 0b0011_0000_0000_0000
  out |= registers[operands[0]] << 8
  out |= registers[operands[1]] << 4
  out |= registers[operands[2]]

  return out

def parse_MOVI(operands: list[str]) -> int:
  validate_number_operands('MOVI', operands, 2)

  out = 0b0100_0000_0000_0000
  out |= registers[operands[0]] << 8
  out |= parse_immediate(operands[1], 8)

  return out

def parse_MOVR(operands: list[str]) -> int:
  validate_number_operands('MOVR', operands, 2)

  out = 0b0101_0000_0000_0000
  out |= registers[operands[0]] << 8
  out |= registers[operands[1]] << 4

  return out

def parse_LDR(operands: list[str]) -> int:
  validate_number_operands('LDR', operands, 2)

  out = 0b0110_0000_0000_0000
  out |= registers[operands[0]] << 8
  out |= registers[operands[1]]

  return out

def parse_STR(operands: list[str]) -> int:
  validate_number_operands('STR', operands, 2)

  out = 0b0111_0000_0000_0000
  out |= registers[operands[0]] << 4
  out |= registers[operands[1]]

  return out

def parse_PUSH(operands: list[str]) -> int:
  validate_number_operands('PUSH', operands, 1)

  out = 0b1000_0000_0000_0000
  out |= registers[operands[0]] << 4

  return out

def parse_POP(operands: list[str]) -> int:
  validate_number_operands('POP', operands, 1)

  out = 0b1001_0000_0000_0000
  out |= registers[operands[0]] << 8

  return out

def parse_BL(operands: list[str]) -> int:
  validate_number_operands('BL', operands, 1)

  out = 0b1010_0000_0000_0000
  out |= parse_immediate(operands[0], 12)

  return out

def parse_BLZ(operands: list[str]) -> int:
  validate_number_operands('BLZ', operands, 1)

  out = 0b1011_0000_0000_0000
  out |= parse_immediate(operands[0], 12)

  return out

def parse_BX(operands: list[str]) -> int:
  validate_number_operands('BX', operands, 1)

  out = 0b1100_0000_0000_0000
  out |= registers[operands[0]] << 4

  return out

def parse_LSL(operands: list[str]) -> int:
  validate_number_operands('LSL', operands, 3)

  out = 0b1101_0000_0000_0000
  out |= registers[operands[0]] << 8
  out |= registers[operands[1]] << 4
  out |= parse_immediate(operands[2], 4)

  return out

def parse_NAND(operands: list[str]) -> int:
  validate_number_operands('NAND', operands, 3)

  out = 0b1110_0000_0000_0000
  out |= registers[operands[0]] << 8
  out |= registers[operands[1]] << 4
  out |= registers[operands[2]]

  return out

def parse_DEC(operands: list[str]) -> int:
  validate_number_operands('DEC', operands, 3)

  out = 0b1111_0000_0000_0000
  out |= registers[operands[0]] << 8
  out |= registers[operands[1]] << 4
  out |= parse_immediate(operands[2], 4)

  return out

instructions = {
  'HLT': parse_HLT,
  'ADD': parse_ADD,
  'SUB': parse_SUB,
  'MUL': parse_MUL,
  'MOVI': parse_MOVI,
  'MOVR': parse_MOVR,
  'LDR': parse_LDR,
  'STR': parse_STR,
  'PUSH': parse_PUSH,
  'POP': parse_POP,
  'BL': parse_BL,
  'BLZ': parse_BLZ,
  'BX': parse_BX,
  'LSL': parse_LSL,
  'NAND': parse_NAND,
  'DEC': parse_DEC,
}

def parse_line(ln: int, line: list[str]) -> int:
  if len(line) == 0:
    print(f'Error: blank line passed to parse_line function.\nTerminating.')
    exit(3)
  
  if not line[0] in instructions:
    print(f'Error: invalid instruction at line {ln}: {" ".join(line)}')
    exit(2)

  return instructions[line[0]](line[1:])

def main() -> None:
  if len(sys.argv) != 3:
    print('Usage: python3 assembler.py <src_file_path> <output_file_path>')
  
  try:
    with open(sys.argv[1], 'r') as f:
      for ln, line in enumerate(f, start=1):
        line_split = line.strip().split(' ')
        if len(line_split) == 0 or line[0] == '\n':  # Skip blank lines
          continue
        if line[0:2] == '//':   # Skip comments
          continue
        line_parsed = parse_line(ln, line.strip().split(' '))
        print('ln {}:\t{}'.format(ln, bin(line_parsed)[2:].zfill(16)))
  except OSError as e:
    print('Error: Source file not found.\nTerminating')
    exit(1)

if __name__ == '__main__':
  main()
  exit(0)
