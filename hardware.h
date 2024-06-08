
#ifndef HARDWARE_H
#define HARDWARE_H
#include <corecrt.h>

#define MEMORY_CAPACITY 4096
#define WORD_SIZE 15

typedef struct Word {
    int address;
    unsigned short binaryCode; /* Array of 16 bits */
    struct Word *next;
} Word;

typedef struct MemoryImage {
    Word *head;
    int nodeCount;
} MemoryImage;

MemoryImage codeImage;
MemoryImage dataImage;

/* ----------------------------------------------------------------------- */
/* Define a structure to hold command information */
typedef struct Command {
    const char *name;
    int numParams;
} Command;

/* Define the command table */
Command command_table[] = {
        {"mov",  2},
        {"cmp",  1},
        {"add",  0},
        {"sub",  1},
        {"lea",  1},
        {"clr",  1},
        {"not", 1},
        {"inc",  1},
        {"dec", 1},
        {"jmp", 1},
        {"bne", 1},
        {"red", 1},
        {"prn", 1},
        {"jsr", 1},
        {"rts", 1},
        {"stop",  0},

        {"terminator", 0}
};
const size_t command_table_size = sizeof(command_table) / sizeof(command_table[0]);

const char* declaration_type[] = {
        "entry","extern"
};

const char* directive_table[] = {
        "string","data"
};

const char* regisrers[] = {
        "r0", "r1", "r2", "r3", "r4", "r5", "r6", "r7",
};

#endif /*HARDWARE_H*/
