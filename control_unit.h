#ifndef CONTROL_UNIT_H
#define CONTROL_UNIT_H

#include "circuit.h"

Circuit *control_unit(
    char *instruction,
    char *Zin,
    char *halt,
    char *add,
    char *sub,
    char *mul,
    char *nand,
    char *memLoad,
    char *memStore,
    char *zeroRA,
    char *LSL,
    char *useImm,
    char *setZ,
    char *link,
    char *storeALUOutputToRdst,
    char *Rdst,
    char *RA,
    char *RB,
    char *Imm);

#endif
