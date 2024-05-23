#include "mux_4_1.h"
#include "mux_2_1.h"

#include <stdlib.h>
#include <stdio.h>

/**
 * Creates a 4:1 mux
 * @param input (char[4] *)
 * @param select (char[2] *)
 * @param output (char *)
 */
Circuit *mux_4_1(char *input, char *select, char *output)
{
  Circuit *c = createCircuit(0, 3);

  c->values = (char *)malloc(2 * sizeof(char));
  if (c->values == NULL)
  {
    perror("Malloc failed. Terminating.");
    exit(1);
  }

  c->subCircuits[0] = mux_2_1(input, select, c->values);
  c->subCircuits[1] = mux_2_1(input + 2, select, c->values + 1);
  c->subCircuits[2] = mux_2_1(c->values, select + 1, output);

  return c;
}
