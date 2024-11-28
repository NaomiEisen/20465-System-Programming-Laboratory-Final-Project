CC = gcc
CFLAGS = -ansi -Wall -pedantic -g -std=c90
SRC = assembler/sources/assembler.c \
      assembler/sources/assembler_controller.c \
      assembler/sources/code_convert.c \
      assembler/sources/first_phase.c \
      assembler/sources/parser.c \
      assembler/sources/phase_controller.c \
      assembler/sources/preprocessor.c \
      assembler/sources/second_phase.c \
      structures/sources/ast.c \
      structures/sources/cmp_data.c \
      structures/sources/errors.c \
      structures/sources/label_data.c \
      structures/sources/macro_data.c \
      structures/sources/mappings.c \
      structures/sources/trie.c \
      utils/sources/output_files.c \
      utils/sources/utils.c
OBJ = $(SRC:.c=.o)
EXEC = assembler_exe

all: $(EXEC)

$(EXEC): $(OBJ)
	$(CC) $(OBJ) -o $(EXEC)

%.o: %.c
	$(CC) $(CFLAGS) -c $< -o $@

clean:
	rm -f $(OBJ) $(EXEC)
