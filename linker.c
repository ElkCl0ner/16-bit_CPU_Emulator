#include "linker.h"

#include <stdlib.h>
#include <stdio.h>

/**
 * Creates a linker
 * @param registers (char[256] *)
 * @param link (char *)
 */
Circuit *linker(char *registers, char *link)
{
  Circuit *c = createCircuit(49, 0);
  c->values = (char *)malloc(2 * sizeof(char));
  if (c->values == NULL)
  {
    perror("Malloc failed. Terminated.");
    exit(1);
  }

  setGate(c, 0, NOT, link, link, c->values);

  for (int i = 0; i < 16; i++)
  {
    setGate(c, 1 + i * 3, AND, registers + 14 + i * 16, c->values, c->values + 1);
    setGate(c, 2 + i * 3, AND, registers + 15 + i * 16, link, registers + 14 + i * 16);
    setGate(c, 3 + i * 3, OR, c->values + 1, registers + 14 + i * 16, registers + 14 + i * 16);
  }

  return c;
}
