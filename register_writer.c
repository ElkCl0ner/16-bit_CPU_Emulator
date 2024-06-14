#include "register_reader.h"
#include "decoder.h"

#include <stdlib.h>
#include <stdio.h>

/**
 * Creates a Register Writer circuit
 * @param registers (char[256] *)
 * @param alu_output (char[16] *)
 * @param mem_inter_data_out (char[16] *)
 * @param Rdst (char[4] *)
 * @param storeOutput (char *)
 * @param mem_load (char *)
 */
Circuit *register_writer(char *registers, char *alu_output, char *mem_inter_data_out, char *Rdst, char *storeOutput, char *mem_load)
{
  Circuit *c = createCircuit(2080, 1);

  c->values = (char *)malloc(38 * sizeof(char));
  if (c->values == NULL)
  {
    perror("Malloc failed. Terminating.");
    exit(1);
  }

  c->subCircuits[0] = decoder(Rdst, c->values);
  for (int i = 0; i < 16; i++)
  {
    setGate(c, i, AND, c->values + i, storeOutput, c->values + 20 + i); // replace register
  }

  for (int i = 0; i < 16; i++) // foreach register
  {
    setGate(c, 16 + i * 49, NOT, c->values + 20 + i, c->values + 20 + i, c->values + 16); // !(replace register)

    for (int j = 0; j < 16; j++) // foreach bit
    {
      setGate(c, 17 + i * 49 + j * 3, AND, registers + i + j * 16, c->values + 16, c->values + 17); // RX AND !(replace register)
      setGate(c, 18 + i * 49 + j * 3, AND, alu_output + j, c->values + 20 + i, c->values + 18);     // alu_output AND (replace register)
      setGate(c, 19 + i * 49 + j * 3, OR, c->values + 17, c->values + 18, registers + i + j * 16);  // this OR that
    }
  }

  // mem_load
  // setGate(c, 800, NOT, mem_load, mem_load, c->values + 36);
  for (int i = 0; i < 16; i++) // foreach register
  {
    for (int j = 0; j < 16; j++) // foreach bit
    {
      setGate(c, 800 + i * 80 + j * 5, NAND, mem_load, c->values + i, c->values + 37);
      setGate(c, 801 + i * 80 + j * 5, AND, registers + i + j * 16, c->values + 37, registers + i + j * 16);
      setGate(c, 802 + i * 80 + j * 5, AND, mem_load, mem_inter_data_out + j, c->values + 19);
      setGate(c, 803 + i * 80 + j * 5, AND, c->values + 19, c->values + i, c->values + 19);
      setGate(c, 804 + i * 80 + j * 5, OR, registers + i + j * 16, c->values + 19, registers + i + j * 16);
    }
  }

  return c;
}
