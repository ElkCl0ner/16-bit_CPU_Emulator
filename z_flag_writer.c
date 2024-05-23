#include "z_flag_writer.h"

#include <stdlib.h>
#include <stdio.h>

/**
 * Creates a Z Flag Writer circuit
 * @param z_flag (char *) current Z flag value (in & out)
 * @param alu_zero (char *) ALU zero output
 * @param cu_setZ (char *) CU setZ flag
 */
Circuit *z_flag_writer(char *z_flag, char *alu_zero, char *cu_setZ)
{
  Circuit *c = createCircuit(4, 0);

  c->values = (char *)malloc(3 * sizeof(char));
  if (c->values == NULL)
  {
    perror("Malloc failed. Terminating.");
    exit(1);
  }

  setGate(c, 0, NOT, cu_setZ, cu_setZ, c->values);         // !cu_setZ
  setGate(c, 1, AND, z_flag, c->values, c->values + 1);    // z_flag AND !cu_setZ
  setGate(c, 2, AND, alu_zero, cu_setZ, c->values + 2);    // alu_zero AND cu_setZ
  setGate(c, 3, OR, c->values + 1, c->values + 2, z_flag); // this OR that

  return c;
}
