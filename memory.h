#ifndef MEMORY_H
#define MEMORY_H

int convertBitArrayToInt(char *bits);
char convertBitArrayToChar(char *bits);

void memRead(char *memory, char *addr, char *data_out);
void memWrite(char *memory, char *addr, char *data_in);

#endif
