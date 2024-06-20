#ifndef HARDWARE_H
#define HARDWARE_H
#include "ast.h"
#include "boolean.h"

/*#define MEMORY_CAPACITY 4096*/
#define MAX_COMMAND_CHAR 15
#define ADDRESS_MODES 4
#define NUM_INSTRUCTIONS 16
#define NUM_DIRECTIVES 4
#define NUM_REGISTERS 8

/* structure representing command_str mappings */
typedef struct {
    char command_str[MAX_COMMAND_CHAR]; /* command_str name */
    int num_params;
    int addr_mode_op1[ADDRESS_MODES];
    int addr_mode_op2[ADDRESS_MODES];
} InstructMapping;

typedef enum { DATA, STRING, ENTRY, EXTERN} dirCommand;

typedef struct {
    char command_str[MAX_COMMAND_CHAR]; /* command_str name */
    dirCommand dir_command;
} DirectiveMapping;

typedef struct {
    InstructMapping instruct_table[NUM_INSTRUCTIONS];
    DirectiveMapping directive_table[NUM_DIRECTIVES];
    const char *registers[NUM_REGISTERS];
} Mappings;

/* Directive and register tables
extern DirectiveMapping directive_table[];
extern const char* registers[];
extern InstructMapping instruct_table[];*/

void init_mappings(Mappings* mappings);
boolean reserved_word(Mappings *mappings, const char *str);

#endif /*HARDWARE_H*/

/* ----------------------------------------------------------------------- */

