CC = gcc
CFLAGS = -Wall -g
DEPS = BldgGen.h
OBJ = windows_init.o car_functions.o initialised_vars.o main.o map_gen.o file_handling.o linked_list.o
EXEC = CarProgram.exe
BINDIR = ../bin

all: $(BINDIR)/$(EXEC)

$(BINDIR)/$(EXEC): $(OBJ)
	@if not exist "$(BINDIR)" mkdir "$(BINDIR)"
	$(CC) -o $@ $^ $(CFLAGS)

%.o: %.c $(DEPS)
	$(CC) -c -o $@ $< $(CFLAGS)

.PHONY: clean

clean:
	rm -f $(OBJ) $(BINDIR)/$(EXEC)
