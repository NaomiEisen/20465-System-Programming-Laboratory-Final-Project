#ifndef MAPPINGS_H
#define MAPPINGS_H
/* ----------------------------------------- Includes -----------------------------------------*/
#include "ast.h"
#include "../../utils/headers/boolean.h"
/* ------------------------------------------ Defines ------------------------------------------*/
#define MAX_COMMAND_CHAR 15
#define ADDRESS_MODES 4
/* ---------------------------------------- Structures ----------------------------------------*/

/* Structure representing one instruction in the instructions mapping */
typedef struct {
    char command_str[MAX_COMMAND_CHAR];                        /* Instruction name */
    int num_params;                     /* Number of parameters in the instruction */
    int addr_mode_op1[ADDRESS_MODES];  /* Valid address modes in the first operand */
    int addr_mode_op2[ADDRESS_MODES]; /* Valid address modes in the second operand */
} InstructMapping;

/* Structure representing one directive in the directives mapping */
typedef struct {
    char command_str[MAX_COMMAND_CHAR]; /* Directive name */
    DirectiveType dir_command;          /* Directive type */
} DirectiveMapping;

typedef enum {
    MACR_START,
    MACR_END
} MacrCommand;

typedef struct {
    char command_str[MAX_COMMAND_CHAR]; /* Macro command */
    MacrCommand macr_code;                /* The command */
} MacrReserved;

/* ----------------------------------- Functions Prototypes ---------------------------------- */
/**
 * Retrieves the number of parameters required by an instruction.
 *
 * @param i The index of the instruction in the instruct_table.
 * @return The number of parameters required by the instruction.
 */
int get_num_param(int i);

/**
 * Retrieves the directive type based on its index.
 *
 * @param i The index of the directive in the directive_table.
 * @return The directive type.
 */
DirectiveType get_dir_command (int i);

/**
 * Finds the index in the instruction table of an instruction based on its name.
 *
 * @param str The name of the instruction to find.
 * @return The index of the instruction in the instruct_table, or -1 if not found.
 */
int get_instruct_index(const char* str);

/**
 * Finds the corresponding index of the directive table of a directive based on its name.
 *
 * @param str The name of the directive to find.
 * @return The index of the directive in the directive_table, or -1 if not found.
 */
int get_dir_index(const char* str);

/**
 * Finds the index of a register based on its name.
 *
 * @param str The name of the register to find.
 * @return The index of the register in the registers array, or -1 if not found.
 */
int get_register_index(const char* str);

/**
 * Finds the index of a reserved macro word - one of the two words indicating
 * a macro initialization or the end of macro initialization.
 *
 * @param str The name of the reserved word to find.
 * @return The macro command image.
 */
MacrCommand find_macr_reserved(const char* str);

/**
 * Validates if a given addressing mode is valid for a specific instruction and parameter.
 *
 * @param command_index The index of the instruction in the instruct_table.
 * @param addr_mode The addressing mode to validate.
 * @param param The parameter index (1 or 2) to check.
 * @return TRUE if the addressing mode is valid for the specified parameter ; FALSE otherwise.
 */
Boolean valid_addr_mode(int command_index, int addr_mode, int param);

/**
 * Checks if a given string is a reserved word, including instructions, directives, or registers.
 *
 * @param str The string to check.
 * @return TRUE if the string matches an instruction, directive, or register ; FALSE otherwise.
 */
Boolean reserved_word(const char *str);

#endif /* MAPPINGS_H */