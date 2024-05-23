#include "mux_16_1.h"
#include "mux_4_1.h"

#include <stdlib.h>
#include <stdio.h>

Circuit *mux_16_1(char *input, char *select, char *output)
{
  Circuit *c = createCircuit(0, 5);

  c->values = (char *)malloc(4 * sizeof(char));
  if (c->values == NULL)
  {
    perror("Malloc failed. Terminating.");
    exit(1);
  }

  for (int i = 0; i < 4; i++)
  {
    c->subCircuits[i] = mux_4_1(input + i * 4, select, c->values + i);
  }
  c->subCircuits[4] = mux_4_1(c->values, select + 2, output);

  return c;
}
