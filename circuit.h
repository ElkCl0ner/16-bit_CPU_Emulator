#ifndef CIRCUIT_H
#define CIRCUIT_H

typedef struct Gate
{
  int (*gateFunction)(int, int);  // Pointer to the gate function
  int *input1;                    // Pointer to the first input
  int *input2;                    // Pointer to the second input
  int *output;                    // Pointer to the output
} Gate;

typedef struct Circuit
{
  Gate *gates;
  int gateCount;
  int *values;                    // Array to hold intermediate values
  struct Circuit **subCircuits;   // Array of pointers to subCircuits
  int subCircuitCount;            // Number of subCircuits
} Circuit;

int AND(int a, int b);
int OR(int a, int b);
int NOT(int a, int b);
int NAND(int a, int b);
int NOR(int a, int b);
int XOR(int a, int b);
int XNOR(int a, int b);

Circuit *createCircuit(int gateCount, int subCircuitCount);
void setGate(Circuit *circuit, int index, int (*gateFunction)(int, int), int *input1, int *input2, int *output);
void simulateCircuit(Circuit *circuit);
void freeCircuit(Circuit *circuit);

#endif
