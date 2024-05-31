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

def parse_HLT(operands: list[str]) -> int:
  if len(operands) != 0:
    print('Error: invalid operands for HLT instruction.\n Terminating.')
  
  return 0b0000_0000_0000_0000

instructions = {
  'HLT': parse_HLT,
}

def parse_line(ln: int, line: list[str]) -> int:
  if len(line) == 0:
    print(f'Error: blank line passed to parse_line function.\nTerminating.')
    exit(3)
  
  if not line[0] in instructions:
    print(f'Error: invalid instruction at line {ln}')#: {''.join(line)}.\nTerminating.')
    exit(2)

  return instructions[line[0]](line[1:])

def main() -> None:
  if len(sys.argv) != 3:
    print('Usage: python3 assembler.py <src_file_path> <output_file_path>')
  
  try:
    with open(sys.argv[1], 'r') as f:
      for ln, line in enumerate(f, start=1):
        print(ln)
        print('ln {}:\t{}'.format(ln, bin(parse_line(ln, line.strip().split(' ')))[2:].zfill(16)))
  except OSError as e:
    print('Error: Source file not found.\nTerminating')
    exit(1)

if __name__ == '__main__':
  main()
  exit(0)
