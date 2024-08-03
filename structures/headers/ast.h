#ifndef AST_H
#define AST_H
/* ----------------------------------------- Includes -----------------------------------------*/
#include "errors.h"
#include "../../utils/headers/boolean.h"
#include "../../assembler/headers/defines.h"

/* ---------------------------------------- Structures ----------------------------------------*/

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
    int adr_mode;              /* Address mode of the instruction */
    InstructionValue value; /* Value of the instruction's operand */
} InstructionOperand;

/* Structure representing an instruction */
typedef struct {
    int operation;               /* Operation image for the instruction */
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

/* ----------------------------------- Functions Prototypes ------------------------------------*/

/**
 * Creates and initializes an empty AST node.
 * @param file: The name of the file where the node is created.
 * @param line: The line number in the file where the node is created.
 * @return A pointer to the newly created AST node, or a null pointer if memory allocation failed.
 */
ASTNode *create_empty_ASTnode(const char *file, int line, char *line_content);

/**
 * Sets the label of an AST node.
 *
 * @param node: The AST node whose label is to be set.
 * @param label: The label to be set.
 */
void set_ast_label(ASTNode *node, const char *label);

/**
 * Sets the type of an AST node.
 *
 * @param node: The AST node whose type is to be set.
 * @param lineType: The type to be set.
 */
void set_ast_type(ASTNode *node, LineType lineType);

/**
 * Sets the operation for a directive in an AST node.
 *
 * @param node: The AST node whose directive operation is to be set.
 * @param operation: The directive operation to be set.
 */
void set_operation_for_directive(ASTNode *node, DirectiveType operation);

/**
 * Sets the operation for an instruction in an AST node.
 *
 * @param node: The AST node whose instruction operation is to be set.
 * @param operation: The instruction operation to be set.
 */
void set_operation_for_instruction(ASTNode *node, int operation);

/**
 * Retrieves the operand of an instruction in an AST node.
 *
 * @param node: The AST node from which to retrieve the operand.
 * @param num: The number of the operand to retrieve (1 or 2).
 * @return A pointer to the specified operand.
 */
InstructionOperand* get_operand(ASTNode *node, int num);
/**
 * Adds an operand to an instruction in an AST node.
 *
 * @param node: The AST node to which the operand is to be added.
 * @param adr_mode: The addressing mode of the operand.
 * @param value: The value of the operand.
 * @param num: The register number (if applicable) of the operand.
 * @return TRUE if the operand was added successfully, FALSE otherwise.
 */
Boolean add_instruct_operand(ASTNode *node, int adr_mode, const char *value, int num);

/**
 * Adds an operand to a directive.
 *
 * @param directive: The directive to which the operand is to be added.
 * @param operand: The operand to be added.
 * @return TRUE if the operand was added successfully, FALSE otherwise.
 */
Boolean add_directive_operand(Directive *directive, char *operand);

/**
 * Frees the memory allocated for an AST node.
 *
 * @param node: The AST node to be freed.
 */
void free_ast_node(ASTNode *node);

#endif /* AST_H */
