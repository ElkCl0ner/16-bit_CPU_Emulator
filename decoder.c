#include "decoder.h"

#include <stdlib.h>
#include <stdio.h>

/**
 * Creates a 4 to 16 bit decoder
 * @param input (int[4] *)
 * @param output (int[16] *)
*/
Circuit *decoder(int *input, int *output)
{
  Circuit *c = createCircuit(28, 0);

  c->values = (int *)malloc(4 * sizeof(int));
  if (c->values == NULL)
  {
    perror("Malloc Failed. Terminating.");
    exit(1);
  }

  // NOT inputs
  for (int i = 0; i < 4; i++)
  {
    setGate(c, i, NOT, input+i, input+i, c->values+i);
  }

  // AND gates
  //itermediate terms
  setGate(c, 5, );

  return c;
}
