
#ifndef INC_20465_SYSTEM_PROGRAMMING_LABORATORY_FINAL_PROJECT_HARDWARE_H
#define INC_20465_SYSTEM_PROGRAMMING_LABORATORY_FINAL_PROJECT_HARDWARE_H


// Define a structure to hold command information
typedef struct Command {
    const char *name;
    int numParams;
} Command;

// Define the command table
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

const char* directive_table[] = {
        "string","data"
};

const char* regisrers[] = {
        "r0", "r1", "r2", "r3", "r4", "r5", "r6", "r7",
};

#endif /*INC_20465_SYSTEM_PROGRAMMING_LABORATORY_FINAL_PROJECT_HARDWARE_H*/
