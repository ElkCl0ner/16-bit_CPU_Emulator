OBJECTS = main.o globals.o circuit.o half_adder.o full_adder.o adder.o twos_complement.o subtractor.o multiplier.o bitwise_nand.o alu.o decoder.o control_unit.o mux_2_1.o mux_4_1.o mux_16_1.o register_writer.o register_reader.o z_flag_writer.o mux_alu_input2.o memory.o linker.o cpu.o
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
