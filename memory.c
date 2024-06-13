#include "memory.h"

/**
 * Helper function
 * Converts a 16 bit (char) array to a real int
 * @param bits (char[16] *)
 */
int convertBitArrayToInt(char *bits)
{
  int out = 0;
  for (int i = 0; i < 16; i++)
  {
    out |= bits[i] << i;
  }
  return out;
}

/**
 * Helper function
 * Converts a real int to a 16 bit (char) array
 * @param in (int)
 * @param out (char[16] *)
 */
void convertIntToBitArray(int in, char *out)
{
  for (int i = 0; i < 16; i++)
  {
    out[i] = (in >> i) & 1;
  }
}

/**
 * Helper function
 * Converts an 8 bit (char) array to a real char
 * @param bits (char[8] *)
 */
char convertBitArrayToChar(char *bits)
{
  char out = 0;
  for (int i = 0; i < 8; i++)
  {
    out |= bits[i] << i;
  }
  return out;
}

/**
 * Reads a word (2B) from memory
 * @param memory (char[65536] *)
 * @param addr (char[16] *)
 * @param data_out (char[16] *)
 */
void memRead(char *memory, char *addr, char *data_out)
{
  int converted_addr = convertBitArrayToInt(addr);
  char fake_data[2] = {memory[converted_addr], memory[converted_addr + 1]};
  for (int i = 0; i < 8; i++)
  {
    data_out[i] = fake_data[0] >> i & 0b1;
  }
  for (int i = 0; i < 8; i++)
  {
    data_out[i + 8] = fake_data[1] >> i & 0b1;
  }
}

/**
 * Writes a word (2B) to memory
 * @param memory (char[65536] *)
 * @param addr (char[16] *)
 * @param data_in (char[16] *)
 */
void memWrite(char *memory, char *addr, char *data_in)
{
  int converted_addr = convertBitArrayToInt(addr);
  char converted_low_byte = convertBitArrayToChar(data_in);
  char converted_high_byte = convertBitArrayToChar(data_in + 8);
  memory[converted_addr] = converted_low_byte;
  memory[converted_addr + 1] = converted_high_byte;
}
