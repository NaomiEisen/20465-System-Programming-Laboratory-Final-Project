#include "mappings.h"
#include <stdio.h>
#include <string.h>

/* Directive table */
static DirectiveMapping directive_table[] = {
        {"string", STRING},
        {"data", DATA},
        {"entry", ENTRY},
        {"extern", EXTERN},
        {""}
};

/* Register table */
static const char* registers[] = {
        "r0", "r1", "r2", "r3", "r4", "r5",
        "r6", "r7", NULL
};

/* Representing a mapping from a function name to its corresponding function */
static InstructMapping instruct_table[] = {
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
};



int get_num_param(int i) {
    return instruct_table[i].num_params;
}

int get_instruct_index(const char* str) {
    int i = 0; /* Index for iterating through the commandMappings array */

    /* Iterate through the commandMappings array until an empty string command_str is found */
    while (instruct_table[i].command_str[0] != '\0') {
        if (strcmp(instruct_table[i].command_str, str) == 0) {
            return i;
        }
        i++;
    }

    /* Return -1 if the corresponding command_str has not been found */
    return -1;
}

boolean valid_addr_mode(int command_index, int addr_mode, int param) {
    if (param == 1) {
        if (instruct_table[command_index].addr_mode_op1[addr_mode] == 1 ) {
            return TRUE;
        } else {
            return FALSE;
        }
    }

    if (param == 2 ) {
        if (instruct_table[command_index].addr_mode_op2[addr_mode] == 1 ) {
            return TRUE;
        }
    }
    return FALSE;
}

int get_dir_index(const char* str) {
    int i = 0; /* Index for iterating through the commandMappings array */

    /* Iterate through the commandMappings array until an empty string command_str is found */
    while (directive_table[i].command_str[0] != '\0') {
        if (strcmp(directive_table[i].command_str, str) == 0) {
            return i;
        }
        i++;
    }

    /* Return -1 if the corresponding command_str has not been found */
    return -1;
}

dirCommand get_dir_command (int i) {
    return directive_table[i].dir_command;
}

int get_register_index(const char* str) {
    int i = 0; /* Index for iterating through the commandMappings array */

    /* Iterate through the commandMappings array until an empty string command_str is found */
    while (registers[i] != NULL) {
        if (strcmp(registers[i] , str) == 0) {
            return i;
        }
        i++;
    }

    /* Return -1 if the corresponding command_str has not been found */
    return -1;
}

boolean reserved_word(const char *str) {
    int i; /* Index for iterating through the commandMappings array */

    /* check instructions */
    if (get_instruct_index(str) != -1) {
        return TRUE;
    }

    /* check directive */
    if (get_dir_index(str) != -1) {
        return TRUE;
    }

    /* check register */
    if (get_register_index(str) != -1) {
        return TRUE;
    }
    return FALSE;
}