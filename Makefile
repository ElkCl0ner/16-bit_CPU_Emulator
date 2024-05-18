OBJECTS = main.o globals.o circuit.o full_adder.o eight_bit_adder.o
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
