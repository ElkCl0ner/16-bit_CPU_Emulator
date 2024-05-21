OBJECTS = main.o globals.o circuit.o half_adder.o full_adder.o adder.o twos_complement.o subtractor.o multiplier.o bitwise_nand.o decoder.o
CC = gcc
CFLAGS = -Wall -g
APPLICATION = cpu

all: $(APPLICATION)

$(APPLICATION): $(OBJECTS)
	$(CC) $(CFLAGS) -o $(APPLICATION) $(OBJECTS)

%.o: %.c
	$(CC) $(CFLAGS) -D DEBUG=$(debug) -c $<

clean:
	rm -f *.o $(APPLICATION)
