#ifndef AST_H
#define AST_H
#include "errors.h"
#include "boolean.h"

#define MAX_LABEL_LENGTH 32
#define ADDR_MODE_IMMEDIATE 0
#define ADDR_MODE_DIRECT 1
#define ADDR_UNDIRECT_REG 2
#define ADDR_DIRECT_REG 3


typedef enum {
    LINE_EMPTY,
    LINE_COMMENT,
    LINE_DIRECTIVE,
    LINE_INSTRUCTION
} LineType;

typedef enum {
    STRING,
    DATA,
    ENTRY,
    EXTERN
} DirectiveType;

typedef struct OperandNode {
    char *operand;
    struct DirNode *next;
} DirNode;

typedef union {
    int int_val;
    char char_val[MAX_LABEL_LENGTH];
} InstructionValue;

typedef struct {
    int adr_mode;
    InstructionValue value;
} InstructionOperand;

typedef struct {
    int operation;
    InstructionOperand operand1;
    InstructionOperand operand2;
    int num_operands;
} Instruction;

typedef struct {
    DirectiveType operation;
    DirNode *operands;
} Directive;

typedef struct {
    LineType lineType;
    char label[MAX_LABEL_LENGTH];
    union {
        Instruction instruction;
        Directive directive;
    };
    Location location;
} ASTNode;






typedef union {
    Instruction instruction;
    Directive directive;
} LineTypeSpecific;
ASTNode *create_empty_ASTnode(const char *file, int line);
void set_ast_label(ASTNode *node, const char *label);
void set_ast_type(ASTNode *node, LineType lineType);
void set_operation_for_directive(ASTNode *node, DirectiveType operation);
boolean add_directive_operand(Directive *directive, char *operand);
void set_operation_for_instruction(ASTNode *node, int operation);
boolean add_instruct_operand(ASTNode *node, int adr_mode, const char *value, int reg);
InstructionOperand * get_operand(ASTNode *node, int num);
void free_dir_nodes(DirNode *node);
void free_ast_node(ASTNode *node);
void print_AST_node(ASTNode *node);


#endif /* AST_H */
