#include "circuit.h"

#include <stdio.h>
#include <stdlib.h>

// AND Gate
char AND(char a, char b)
{
  return a && b;
}

// OR Gate
char OR(char a, char b)
{
  return a || b;
}

// NOT Gate
char NOT(char a, char b)
{
  return !a;
}

// NAND Gate
char NAND(char a, char b)
{
  return !(a && b);
}

// NOR Gate
char NOR(char a, char b)
{
  return !(a || b);
}

// XOR Gate
char XOR(char a, char b)
{
  return a ^ b;
}

// XNOR Gate
char XNOR(char a, char b)
{
  return !(a ^ b);
}

// Allocate circuit
Circuit *createCircuit(int gateCount, int subCircuitCount)
{
  if (gateCount + subCircuitCount == 0)
  {
    fprintf(stderr, "Tried to create an empty circuit. Terminating.");
    exit(1);
  }

  Circuit *circuit = (Circuit *)malloc(sizeof(Circuit));
  if (circuit == NULL)
  {
    perror("Malloc failed. Terminating.");
    exit(1);
  }

  circuit->gateCount = gateCount;
  circuit->subCircuitCount = subCircuitCount;

  if (gateCount > 0)
  {
    circuit->gates = (Gate *)malloc(gateCount * sizeof(Gate));
    if (circuit->gates == NULL)
    {
      perror("Malloc failed. Terminating.");
      exit(1);
    }
  }

  if (subCircuitCount > 0)
  {
    circuit->subCircuits = (Circuit **)malloc(subCircuitCount * sizeof(Circuit *));
    if (circuit->subCircuits == NULL)
    {
      perror("Malloc failed. Terminating.");
      exit(1);
    }
  }

  return circuit;
}

// Set a gate in the circuit
void setGate(Circuit *circuit, int index, char (*gateFunction)(char, char), char *input1, char *input2, char *output)
{
  circuit->gates[index].gateFunction = gateFunction;
  circuit->gates[index].input1 = input1;
  circuit->gates[index].input2 = input2;
  circuit->gates[index].output = output;
}

// Simulate the circuit
void simulateCircuit(Circuit *circuit)
{
  for (int i = 0; i < circuit->subCircuitCount; i++)
  {
    simulateCircuit(circuit->subCircuits[i]);
  }

  for (int i = 0; i < circuit->gateCount; i++)
  {
    Gate *gate = &circuit->gates[i];
    *gate->output = gate->gateFunction(*gate->input1, *gate->input2);
  }
}

// Free the memory allocated for the circuit
void freeCircuit(Circuit *circuit)
{
  for (int i = 0; i < circuit->subCircuitCount; i++)
  {
    freeCircuit(circuit->subCircuits[i]);
  }
  if (circuit->gateCount > 0)
    free(circuit->gates);
  if (circuit->values != NULL)
    free(circuit->values);
  if (circuit->subCircuitCount > 0)
    free(circuit->subCircuits);
}
