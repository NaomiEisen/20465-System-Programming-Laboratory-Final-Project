#ifndef AST_H
#define AST_H
/* ---------------------------------------------------------------------------------------
 *                                          Includes
 * --------------------------------------------------------------------------------------- */
#include "errors.h"
#include "boolean.h"
/* ---------------------------------------------------------------------------------------
 *                                          Defines
 * --------------------------------------------------------------------------------------- */
#define MAX_LABEL_LENGTH 32
#define ADDR_MODE_IMMEDIATE 0
#define ADDR_MODE_DIRECT 1
#define ADDR_UNDIRECT_REG 2
#define ADDR_DIRECT_REG 3
/* ---------------------------------------------------------------------------------------
 *                                          Structures
 * --------------------------------------------------------------------------------------- */

/* Enum representing different types of lines */
typedef enum {
    LINE_EMPTY,      /* Represents an empty line */
    LINE_COMMENT,    /* Represents a comment line */
    LINE_DIRECTIVE,  /* Represents a directive line */
    LINE_INSTRUCTION /* Represents an instruction line */
} LineType;

/* Enum representing different types of directive operations*/
typedef enum {
    STRING, /* Represents a string directive */
    DATA,   /* Represents a data directive */
    ENTRY,  /* Represents an entry directive */
    EXTERN  /* Represents an extern directive */
} DirectiveType;

/* Node structure for operands in a directive operation */
typedef struct OperandNode {
    char *operand;        /* Operand string */
    struct DirNode *next; /* Pointer to the next operand node in a directive */
} DirNode;

/* Structure representing a directive operation */
typedef struct {
    DirectiveType operation; /* Type of directive operation (STRING, DATA, ENTRY, EXTERN) */
    DirNode *operands;       /* Linked list of operands for the directive */
} Directive;

/* Union holding either an integer or a string of instruction operation's operands */
typedef union {
    int int_val;                     /* Integer value for instructions */
    char char_val[MAX_LABEL_LENGTH]; /* Character array for labels in instructions */
} InstructionValue;

/* Structure representing an operand in an instruction operation */
typedef struct {
    int adr_mode;           /* Addressing mode for the instruction */
    InstructionValue value; /* Value of the instruction's operand */
} InstructionOperand;

/* Structure representing an instruction */
typedef struct {
    int operation;               /* Operation code for the instruction */
    InstructionOperand operand1; /* First operand of the instruction */
    InstructionOperand operand2; /* Second operand of the instruction */
    int num_operands;            /* Number of operands for the instruction */
} Instruction;

/* Union holding either an instruction or a directive */
typedef union {
    Instruction instruction; /* Union holding an instruction */
    Directive directive;     /* Union holding a directive */
} LineTypeSpecific;

/* Structure representing a node in an abstract syntax tree (AST) */
typedef struct {
    LineType lineType;            /* Type of the line (empty, comment, directive, instruction) */
    char label[MAX_LABEL_LENGTH]; /* Label associated with the line (if any) */
    LineTypeSpecific specific;    /* Specific details depending on lineType (instruction or directive) */
    Location location;            /* Location information of the line */
} ASTNode;

/* ---------------------------------------------------------------------------------------
 *                                         Prototypes
 * --------------------------------------------------------------------------------------- */

ASTNode *create_empty_ASTnode(const char *file, int line);
void set_ast_label(ASTNode *node, const char *label);
void set_ast_type(ASTNode *node, LineType lineType);
void set_operation_for_directive(ASTNode *node, DirectiveType operation);
Boolean add_directive_operand(Directive *directive, char *operand);
void set_operation_for_instruction(ASTNode *node, int operation);
Boolean add_instruct_operand(ASTNode *node, int adr_mode, const char *value, int reg);
InstructionOperand * get_operand(ASTNode *node, int num);
void free_dir_nodes(DirNode *node);
void free_ast_node(ASTNode *node);
void print_AST_node(ASTNode *node);


#endif /* AST_H */
