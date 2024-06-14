#ifndef REGISTER_WRITER_H
#define REGISTER_WRITER_H

#include "circuit.h"

Circuit *register_writer(char *registers, char *alu_output, char *mem_inter_data_out, char *Rdst, char *storeOutput, char *mem_load);

#endif
