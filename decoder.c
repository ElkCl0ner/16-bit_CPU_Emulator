#include "decoder.h"

#include <stdlib.h>
#include <stdio.h>

/**
 * Creates a 4 to 16 bit decoder
 * @param input (char[4] *)
 * @param output (char[16] *)
 */
Circuit *decoder(char *input, char *output)
{
  Circuit *c = createCircuit(28, 0);
  c->values = (char *)malloc(12 * sizeof(char)); // 4 + 8
  if (c->values == NULL)
  {
    perror("Malloc Failed. Terminating.");
    exit(1);
  }

  // NOT inputs
  for (int i = 0; i < 4; i++)
  {
    setGate(c, i, NOT, input + i, input + i, c->values + i);
  }

  // AND gates
  // itermediate terms
  setGate(c, 4, AND, c->values, c->values + 1, c->values + 4); // XX00
  setGate(c, 5, AND, input, c->values + 1, c->values + 5);     // XX01
  setGate(c, 6, AND, c->values, input + 1, c->values + 6);     // XX10
  setGate(c, 7, AND, input, input + 1, c->values + 7);         // XX11

  setGate(c, 8, AND, c->values + 2, c->values + 3, c->values + 8); // 00XX
  setGate(c, 9, AND, input + 2, c->values + 3, c->values + 9);     // 01XX
  setGate(c, 10, AND, c->values + 2, input + 3, c->values + 10);   // 10XX
  setGate(c, 11, AND, input + 2, input + 3, c->values + 11);       // 11XX

  // final outputs
  setGate(c, 12, AND, c->values + 8, c->values + 4, output);     // 0000
  setGate(c, 13, AND, c->values + 8, c->values + 5, output + 1); // 0001
  setGate(c, 14, AND, c->values + 8, c->values + 6, output + 2); // 0010
  setGate(c, 15, AND, c->values + 8, c->values + 7, output + 3); // 0011

  setGate(c, 16, AND, c->values + 9, c->values + 4, output + 4); // 0100
  setGate(c, 17, AND, c->values + 9, c->values + 5, output + 5); // 0101
  setGate(c, 18, AND, c->values + 9, c->values + 6, output + 6); // 0110
  setGate(c, 19, AND, c->values + 9, c->values + 7, output + 7); // 0111

  setGate(c, 20, AND, c->values + 10, c->values + 4, output + 8);  // 1000
  setGate(c, 21, AND, c->values + 10, c->values + 5, output + 9);  // 1001
  setGate(c, 22, AND, c->values + 10, c->values + 6, output + 10); // 1010
  setGate(c, 23, AND, c->values + 10, c->values + 7, output + 11); // 1011

  setGate(c, 24, AND, c->values + 11, c->values + 4, output + 12); // 1100
  setGate(c, 25, AND, c->values + 11, c->values + 5, output + 13); // 1101
  setGate(c, 26, AND, c->values + 11, c->values + 6, output + 14); // 1110
  setGate(c, 27, AND, c->values + 11, c->values + 7, output + 15); // 1111

  return c;
}
