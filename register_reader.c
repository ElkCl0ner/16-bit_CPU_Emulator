#include "register_reader.h"
#include "mux_16_1.h"

/**
 * Creates a Register Reader circuit
 * @param registers (char[256] *)
 * @param RA_in (char[4] *)
 * @param RB_in (char[4] *)
 * @param RA_out (char[16] *)
 * @param RB_out (char[16] *)
 */
Circuit *register_reader(char *registers, char *RA_in, char *RB_in, char *RA_out, char *RB_out)
{
  Circuit *c = createCircuit(0, 32);

  for (int i = 0; i < 16; i++)
  {
    c->subCircuits[i * 2] = mux_16_1(registers + i * 16, RA_in, RA_out + i);
    c->subCircuits[1 + i * 2] = mux_16_1(registers + i * 16, RB_in, RB_out + i);
  }

  return c;
}
