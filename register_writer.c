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
  Circuit *c = createCircuit(1312, 1);

  c->values = (char *)malloc(19 * sizeof(char));
  if (c->values == NULL)
  {
    perror("Malloc failed. Terminating.");
    exit(1);
  }

  c->subCircuits[0] = decoder(Rdst, c->values);
  for (int i = 0; i < 16; i++)
  {
    setGate(c, i, AND, c->values + i, storeOutput, c->values + i); // replace register
  }

  for (int i = 0; i < 16; i++) // foreach register
  {
    setGate(c, 16 + i * 49, NOT, c->values + i, c->values + i, c->values + 16); // !(replace register)

    for (int j = 0; j < 16; j++) // foreach bit
    {
      setGate(c, 17 + i * 49 + j * 3, AND, registers + i + j * 16, c->values + 16, c->values + 17); // RX AND !(replace register)
      setGate(c, 18 + i * 49 + j * 3, AND, alu_output + j, c->values + i, c->values + 18);          // alu_output AND (replace register)
      setGate(c, 19 + i * 49 + j * 3, OR, c->values + 17, c->values + 18, registers + i + j * 16);  // this OR that
    }
  }

  // mem_load
  for (int i = 0; i < 16; i++) // foreach register
  {
    for (int j = 0; j < 16; j++) // foreach bit
    {
      setGate(c, 800 + i * 32 + j * 2, AND, mem_load, mem_inter_data_out + j, c->values + 19);
      setGate(c, 801 + i * 32 + j * 2, OR, registers + i * 16 + j, c->values + 19, registers + i * 16 + j);
    }
  }

  return c;
}
