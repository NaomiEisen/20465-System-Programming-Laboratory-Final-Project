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



int get_num_param(int i);
int get_instruct_index(const char* str);
int get_dir_index(const char* str);
int get_register_index(const char* str);
boolean reserved_word(const char *str);
boolean valid_addr_mode(int command_index, int addr_mode, int param);
dirCommand get_dir_command (int i);

#endif /*HARDWARE_H*/

/* ----------------------------------------------------------------------- */

