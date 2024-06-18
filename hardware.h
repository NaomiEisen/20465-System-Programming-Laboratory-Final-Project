#ifndef HARDWARE_H
#define HARDWARE_H
#include "ast.h"
#include "boolean.h"

/*#define MEMORY_CAPACITY 4096*/
#define MAX_COMMAND_CHAR 15
#define ADDRESS_MODES 4


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


/* Directive and register tables */
extern DirectiveMapping directive_table[];
extern const char* registers[];
extern InstructMapping command_table[];


#endif /*HARDWARE_H*/

/* ----------------------------------------------------------------------- */

