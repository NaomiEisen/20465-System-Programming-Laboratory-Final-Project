#include "mappings.h"
#include <stdio.h>
#include <string.h>

/* Directive table
DirectiveMapping directive_table[] = {
        {"string", STRING},
        {"data", DATA},
        {"entry", ENTRY},
        {"extern", EXTERN},
        {""}
};*/

/* Register table
const char* registers[] = {
        "r0", "r1", "r2", "r3", "r4", "r5",
        "r6", "r7", NULL
};*/

/* Representing a mapping from a function name to its corresponding function
InstructMapping instruct_table[] = {
        {"mov", 2, {1,1,1,1} , {0,1,1,1} },
        {"cmp", 2, {1,1,1,1} , {1,1,1,1}},
        {"add", 2, {1,1,1,1} , {0,1,1,1}},
        {"sub", 2, {1,1,1,1} , {0,1,1,1}},
        {"lea", 2, {0,1,0,0} , {0,1,1,1}},
        {"clr", 1,  {0,1,1,1}},
        {"not", 1,  {0,1,1,1}},
        {"inc", 1, {0,1,1,1}},
        {"dec", 1, {0,1,1,1}},
        {"jmp", 1, {0,1,1,0}},
        {"bne", 1, {0,1,1,0}},
        {"red", 1, {0,1,1,1}},
        {"prn", 1, {1,1,1,1}},
        {"jsr", 1, {0,1,1,0}},
        {"rts", 0},
        {"stop",  0},
        {"", 0, }
};*/

void init_command_table(InstructMapping *command_table) {
    command_table[0] = (InstructMapping){"mov", 2, {1, 1, 1, 1}, {0, 1, 1, 1}};
    command_table[1] = (InstructMapping){"cmp", 2, {1, 1, 1, 1}, {1, 1, 1, 1}};
    command_table[2] = (InstructMapping){"add", 2, {1, 1, 1, 1}, {0, 1, 1, 1}};
    command_table[3] = (InstructMapping){"sub", 2, {1, 1, 1, 1}, {0, 1, 1, 1}};
    command_table[4] = (InstructMapping){"lea", 2, {0, 1, 0, 0}, {0, 1, 1, 1}};
    command_table[5] = (InstructMapping){"clr", 1, {0, 1, 1, 1}};
    command_table[6] = (InstructMapping){"not", 1, {0, 1, 1, 1}};
    command_table[7] = (InstructMapping){"inc", 1, {0, 1, 1, 1}};
    command_table[8] = (InstructMapping){"dec", 1, {0, 1, 1, 1}};
    command_table[9] = (InstructMapping){"jmp", 1, {0, 1, 1, 0}};
    command_table[10] = (InstructMapping){"bne", 1, {0, 1, 1, 0}};
    command_table[11] = (InstructMapping){"red", 1, {0, 1, 1, 1}};
    command_table[12] = (InstructMapping){"prn", 1, {1, 1, 1, 1}};
    command_table[13] = (InstructMapping){"jsr", 1, {0, 1, 1, 0}};
    command_table[14] = (InstructMapping){"rts", 0};
    command_table[15] = (InstructMapping){"stop", 0};
}


void init_directive_table(DirectiveMapping *directive_table) {
    directive_table[0] = (DirectiveMapping){"string", STRING};
    directive_table[1] = (DirectiveMapping){"data", DATA};
    directive_table[2] = (DirectiveMapping){"entry", ENTRY};
    directive_table[3] = (DirectiveMapping){"extern", EXTERN};
}


void init_registers(const char **registers) {
    registers[0] = "r0";
    registers[1] = "r1";
    registers[2] = "r2";
    registers[3] = "r3";
    registers[4] = "r4";
    registers[5] = "r5";
    registers[6] = "r6";
    registers[7] = "r7";
}

void init_mappings(Mappings* mappings) {
    init_command_table(mappings->instruct_table);
    init_directive_table(mappings->directive_table);
    init_registers(mappings->registers);
}

boolean reserved_word(Mappings *mappings, const char *str) {
    int i; /* Index for iterating through the commandMappings array */

    /* check instructions */
    for (i = 0; i < NUM_INSTRUCTIONS ; i++) {
        if (strcmp(mappings->instruct_table[i].command_str, str) == 0) {
            return TRUE;
        }
    }

    /* check directive */
    for (i = 0; i < NUM_DIRECTIVES ; i++) {
        if (strcmp(mappings->instruct_table[i].command_str, str) == 0) {
            return TRUE;
        }
    }

    /* check register */
    for (i = 0; i < NUM_REGISTERS ; i++) {
        if (strcmp(mappings->registers[i], str) == 0) {
            return TRUE;
        }
    }

    return FALSE;
}