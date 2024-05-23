#include "mux_2_1.h"

#include <stdlib.h>
#include <stdio.h>

/**
 * Creates a 2:1 mux
 * @param input (char[2] *)
 * @param select (char *)
 * @param output (char *)
 */
Circuit *mux_2_1(char *input, char *select, char *output)
{
  Circuit *c = createCircuit(4, 0);

  c->values = (char *)malloc(1 * sizeof(char));
  if (c->values == NULL)
  {
    perror("Malloc failed. Terminating.");
    exit(1);
  }

  setGate(c, 0, NOT, select, select, c->values);   // !select
  setGate(c, 1, AND, input, c->values, c->values); // input[0] AND !select
  setGate(c, 2, AND, input + 1, select, output);   // input[1] AND select
  setGate(c, 3, OR, output, c->values, output);    // output

  return c;
}
