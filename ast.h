#ifndef AST_H
#define AST_H
#include "errors.h"

#define MAX_LABEL_LENGTH 32

typedef enum {
    LINE_EMPTY,
    LINE_COMMENT,
    LINE_DIRECTIVE,
    LINE_OPERATION
} LineType;

typedef struct OperandNode {
    char *operand;
    int adr_mode; /* Addressing mode */
    struct OperandNode *next;
} OperandNode;

typedef struct {
    LineType lineType;
    char *label;
    char *operation;
    OperandNode *operands;
    int numOperands;
    Location location;
} ASTNode;


ASTNode *create_empty_ASTNode(const char *file, int line);
void set_line_type(ASTNode *node, LineType lineType);
void set_label(ASTNode *node, char *label);
void set_operation(ASTNode *node, char *operation);
int add_operand(ASTNode *node, char *operand);
void set_operand_adr_mode(OperandNode *operand, int adr_mode);
void free_ASTNode(ASTNode *node);
void printASTNode(const ASTNode *node);


#endif /* AST_H */
