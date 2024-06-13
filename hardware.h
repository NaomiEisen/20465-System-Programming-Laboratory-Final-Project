#ifndef HARDWARE_H
#define HARDWARE_H
#include <corecrt.h>
#include "ast.h"
#include "semantic_analyzer.h"
#include "boolean.h"

#define MEMORY_CAPACITY 4096
#define MAX_COMMAND_CHAR 15
#define NUM_OF_BYTES 2
#define ADDRESS_MODES 4


typedef struct {
    char lines[MEMORY_CAPACITY][NUM_OF_BYTES];
    int count;
} MemoryImage;

/* Declare the external MemoryImage struct variables
extern MemoryImage code;
extern MemoryImage data;
*/

/* typedef for a function pointer representing a command function */
typedef boolean (*Function)(ASTNode*);

/* structure representing command mappings */
typedef struct {
    char command[MAX_COMMAND_CHAR]; /* command name */
    int num_params;
    int addr_mode_op1[ADDRESS_MODES];
    int addr_mode_op2[ADDRESS_MODES];
} CommandMapping;

size_t command_table_size;

/* Directive and register tables */
extern const char* directive_table[];
extern const char* registers[];
extern CommandMapping command_table[];


#endif /*HARDWARE_H*/

/* ----------------------------------------------------------------------- */

