#ifndef CONTROL_UNIT_H
#define CONTROL_UNIT_H

#include "circuit.h"

Circuit *control_unit(
    char *instruction,
    char *Zin,
    char *Rdst,
    char *RA,
    char *RB,
    char *Addr,
    char *Imm,
    char *zeroRA,
    char *LSL,
    char *useImm,
    char *add,
    char *sub,
    char *mul,
    char *nand,
    char *memLoad,
    char *memStore,
    char *setZ,
    char *link,
    char *storeOutputToRdst,
    char *halt);

#endif
