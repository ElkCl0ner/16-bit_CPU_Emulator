#include "mux_alu_input2.h"

#include <stdlib.h>
#include <stdio.h>

Circuit *custom_mux_2_1(char *A, char *B, char *sel, char *out)
{
  Circuit *c = createCircuit(4, 0);

  c->values = (char *)malloc(1 * sizeof(char));
  if (c->values == NULL)
  {
    perror("Malloc failed. Terminating.");
    exit(1);
  }

  setGate(c, 0, NOT, sel, sel, c->values);     // !select
  setGate(c, 1, AND, A, c->values, c->values); // A AND !select
  setGate(c, 2, AND, B, sel, out);             // B AND select
  setGate(c, 3, OR, out, c->values, out);      // output

  return c;
}

/**
 * Creates the circuit of the MUX that selects the input 2 of the ALU
 * @param RB_val (char[16] *)
 * @param Imm (char[16] *)
 * @param useImm (char *)
 * @param alu_input2 (char[16] *)
 */
Circuit *mux_alu_input2(char *RB_val, char *Imm, char *useImm, char *alu_input2)
{
  Circuit *c = createCircuit(0, 16);

  for (int i = 0; i < 16; i++)
  {
    c->subCircuits[i] = custom_mux_2_1(RB_val + i, Imm + i, useImm, alu_input2 + i);
  }

  return c;
}
