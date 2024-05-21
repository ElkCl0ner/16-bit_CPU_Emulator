#ifndef CIRCUIT_H
#define CIRCUIT_H

typedef struct Gate
{
  char (*gateFunction)(char, char); // Pointer to the gate function
  char *input1;                     // Pointer to the first input
  char *input2;                     // Pointer to the second input
  char *output;                     // Pointer to the output
} Gate;

typedef struct Circuit
{
  Gate *gates;
  int gateCount;
  char *values;                 // Array to hold intermediate values
  struct Circuit **subCircuits; // Array of pointers to subCircuits
  int subCircuitCount;          // Number of subCircuits
} Circuit;

char AND(char a, char b);
char OR(char a, char b);
char NOT(char a, char b);
char NAND(char a, char b);
char NOR(char a, char b);
char XOR(char a, char b);
char XNOR(char a, char b);

Circuit *createCircuit(int gateCount, int subCircuitCount);
void setGate(Circuit *circuit, int index, char (*gateFunction)(char, char), char *input1, char *input2, char *output);
void simulateCircuit(Circuit *circuit);
void freeCircuit(Circuit *circuit);

#endif
