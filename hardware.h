#ifndef HARDWARE_H
#define HARDWARE_H
#include "ast.h"
#include "semantic_analyzer.h"
#include "boolean.h"

/*#define MEMORY_CAPACITY 4096*/
#define MAX_COMMAND_CHAR 15
#define ADDRESS_MODES 4


/*typedef struct MemoryImage {
    char lines[MEMORY_CAPACITY][NUM_OF_BYTES];
    int count;
} MemoryImage;*/

/* typedef for a function pointer representing a command function
typedef boolean (*Function)(ASTNode*);*/

/* structure representing command mappings */
typedef struct {
    char command[MAX_COMMAND_CHAR]; /* command name */
    int num_params;
    int addr_mode_op1[ADDRESS_MODES];
    int addr_mode_op2[ADDRESS_MODES];
} CommandMapping;


/* Directive and register tables */
extern const char* directive_table[];
extern const char* registers[];
extern CommandMapping command_table[];


#endif /*HARDWARE_H*/

/* ----------------------------------------------------------------------- */

