#ifndef AST_H
#define AST_H

typedef enum {
    LINE_EMPTY,
    LINE_COMMENT,
    LINE_DIRECTIVE,
    LINE_OPERATION
} LineType;

typedef struct OperandNode {
    char *operand;
    struct OperandNode *next;
} OperandNode;

typedef struct {
    LineType lineType;
    char *label;
    char *operation;
    OperandNode *operands;
    int numOperands;
} ASTNode;


ASTNode* create_empty_ASTNode();
void setLineType(ASTNode *node, LineType lineType);
void setLabel(ASTNode *node, const char *label);
void setOperation(ASTNode *node, const char *operation);
int addOperand(ASTNode *node, const char *operand);
void freeASTNode(ASTNode *node);


#endif /* AST_H */