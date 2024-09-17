IDIR=./function_library
CC=gcc
CFLAGS=-Wall -Wextra -I$(IDIR)

ODIR=./obj

_DEPS = check_verify.h generate_name_laptime.h print_to_file.h
DEPS = $(patsubst %,$(IDIR)/%,$(_DEPS))


_OBJ = check_verify.o generate_name_laptime.o print_to_file.o
OBJ = $(patsubst %,$(ODIR)/%,$(_OBJ))

_MAIN = formula_main.c

$(ODIR)/%.o: %.c $(DEPS)
		$(CC) -c -o $@ $< $(CFLAGS)

formula: $(OBJ)
		$(CC) -o $@ $^ $(_MAIN) $(CFLAGS)

.PHONY: clean

clean:
		rm -f $(ODIR)/*.o *~ core $(INCDIR)/*~ formula *.txt

