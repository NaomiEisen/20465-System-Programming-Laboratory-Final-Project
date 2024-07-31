/* ---------------------------------------------------------------------------------------
 *                                          Includes
 * --------------------------------------------------------------------------------------- */
#include <stdio.h>
#include <string.h>
#include "../headers/mappings.h"
/* ---------------------------------------------------------------------------------------
 *                                          Mappings
 * --------------------------------------------------------------------------------------- */

/* Directive table mapping directive names to their corresponding types */
static DirectiveMapping directive_table[] = {
        {"string", STRING},    /* Maps "string" to the STRING directive type */
        {"data", DATA},        /* Maps "data" to the DATA directive type */
        {"entry", ENTRY},      /* Maps "entry" to the ENTRY directive type */
        {"extern", EXTERN},    /* Maps "extern" to the EXTERN directive type */
        {""}                   /* Sentinel value to mark the end of the table */
};

/**
 * Mapping of instruction names to their corresponding function parameters and addressing modes
 * In the address mode array - 1 indicates a valid address mode ; 0 indicates invalid address mode.
 */
static InstructMapping instruct_table[] = {
        {"mov", 2, {1,1,1,1}, {0,1,1,1}}, /* MOV instruction with 2 parameters */
        {"cmp", 2, {1,1,1,1}, {1,1,1,1}}, /* CMP instruction with 2 parameters */
        {"add", 2, {1,1,1,1}, {0,1,1,1}}, /* ADD instruction with 2 parameters */
        {"sub", 2, {1,1,1,1}, {0,1,1,1}}, /* SUB instruction with 2 parameters */
        {"lea", 2, {0,1,0,0}, {0,1,1,1}}, /* LEA instruction with 2 parameters */
        {"clr", 1, {0,1,1,1}},             /* CLR instruction with 1 parameter */
        {"not", 1, {0,1,1,1}},             /* NOT instruction with 1 parameter */
        {"inc", 1, {0,1,1,1}},             /* INC instruction with 1 parameter */
        {"dec", 1, {0,1,1,1}},             /* DEC instruction with 1 parameter */
        {"jmp", 1, {0,1,1,0}},             /* JMP instruction with 1 parameter */
        {"bne", 1, {0,1,1,0}},             /* BNE instruction with 1 parameter */
        {"red", 1, {0,1,1,1}},             /* RED instruction with 1 parameter */
        {"prn", 1, {1,1,1,1}},             /* PRN instruction with 1 parameter */
        {"jsr", 1, {0,1,1,0}},             /* JSR instruction with 1 parameter */
        {"rts", 0},                        /* RTS instruction with no parameters */
        {"stop", 0},                       /* STOP instruction with no parameters */
        {"", 0}                           /* Sentinel value to mark the end of the table */
};

/* Register table listing the names of available registers. */
static const char* registers[] = {
        "r0", "r1", "r2", "r3", "r4", "r5",
        "r6", "r7", NULL       /* NULL marks the end of the array */
};

/* Table for macro commands - initialization and the end of initialization of a macro */
static const MacrReserved macr_reserved_table[] = {
        {"macr", MACR_START}, {"endmacr", MACR_END},
        {"",          0} /* Sentinel value to mark the end of the table */
};

/* ---------------------------------------------------------------------------------------
 *                                       Functions
 * --------------------------------------------------------------------------------------- */
/**
 * Retrieves the number of parameters required by an instruction.
 *
 * @param i The index of the instruction in the instruct_table.
 * @return The number of parameters required by the instruction.
 */
int get_num_param(int i) {
    return instruct_table[i].num_params;
}

/**
 * Retrieves the directive type based on its index.
 *
 * @param i The index of the directive in the directive_table.
 * @return The directive type.
 */
DirectiveType get_dir_command (int i) {
    return directive_table[i].dir_command;
}

/**
 * Finds the index in the instruction table of an instruction based on its name.
 *
 * @param str The name of the instruction to find.
 * @return The index of the instruction in the instruct_table, or -1 if not found.
 */
int get_instruct_index(const char* str) {
    int i = 0; /* Index for iterating through the instruct_table array */

    /* Iterate through the instruct_table array */
    while (instruct_table[i].command_str[0] != '\0') {
        if (strcmp(instruct_table[i].command_str, str) == 0) {
            return i; /* Found corresponding instruction */
        }
        i++;
    }

    /* Return -1 if the corresponding instruction has not been found */
    return -1;
}

/**
 * Finds the corresponding index of the directive table of a directive based on its name.
 *
 * @param str The name of the directive to find.
 * @return The index of the directive in the directive_table, or -1 if not found.
 */
int get_dir_index(const char* str) {
    int i = 0; /* Index for iterating through the directive_table array */

    /* Iterate through the directive_table array */
    while (directive_table[i].command_str[0] != '\0') {
        if (strcmp(directive_table[i].command_str, str) == 0) {
            return i; /* found corresponding directive */
        }
        i++;
    }

    /* Return -1 if the corresponding directive has not been found */
    return -1;
}

/**
 * Finds the index of a register based on its name.
 *
 * @param str The name of the register to find.
 * @return The index of the register in the registers array, or -1 if not found.
 */
int get_register_index(const char* str) {
    int i = 0; /* Index for iterating through the registers array */

    /* Iterate through the registers array */
    while (registers[i] != NULL) {
        if (strcmp(registers[i] , str) == 0) {
            return i; /* Found register */
        }
        i++;
    }

    /* Return -1 if the corresponding register has not been found */
    return -1;
}

/**
 * Finds the index of a reserved macro word - one of the two words indicating
 * a macro initialization or the end of macro initialization.
 *
 * @param str The name of the reserved word to find.
 * @return The macro command code.
 */
MacrCommand find_macr_reserved(const char* str) {
    int i = 0; /* Index for iterating through the macr_reserved_table array */

    /* Iterate through the macr_reserved_table array */
    while (macr_reserved_table[i].command_str[0] != '\0') {
        if (strcmp(macr_reserved_table[i].command_str, str) == 0) {
            return macr_reserved_table[i].macr_code; /* Found macr_code reserved word */
        }
        i++;
    }

    /* Return -1 if the corresponding macr_code reserved word has not been found */
    return -1;
}


/**
 * Validates if a given addressing mode is valid for a specific instruction and parameter.
 *
 * @param command_index The index of the instruction in the instruct_table.
 * @param addr_mode The addressing mode to validate.
 * @param param The parameter index (1 or 2) to check.
 * @return TRUE if the addressing mode is valid for the specified parameter ; FALSE otherwise.
 */
Boolean valid_addr_mode(int command_index, int addr_mode, int param) {
    /* Check accordingly to the parameter index */
    if (param == 1) {
        if (instruct_table[command_index].addr_mode_op1[addr_mode] == 1 ) return TRUE;
        else return FALSE;
    }
    if (param == 2 ) {
        if (instruct_table[command_index].addr_mode_op2[addr_mode] == 1 ) return TRUE;
    }
    return FALSE;
}

/**
 * Checks if a given string is a reserved word, including instructions, directives, or registers.
 *
 * @param str The string to check.
 * @return TRUE if the string matches an instruction, directive, or register ; FALSE otherwise.
 */
Boolean reserved_word(const char *str) {
    /* Check instructions */
    if (get_instruct_index(str) != -1) {
        return TRUE;
    }

    /* Check directive */
    if (get_dir_index(str) != -1) {
        return TRUE;
    }

    /* Check register */
    if (get_register_index(str) != -1) {
        return TRUE;
    }

    /* Check macr_code definition reserved words */
    if (find_macr_reserved(str) != -1) {
        return TRUE;
    }
    return FALSE;
}