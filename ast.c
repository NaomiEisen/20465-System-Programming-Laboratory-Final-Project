#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include "ast.h"
#include "errors.h"
#include "utils.h"

ASTNode* create_empty_ASTNode() {
    ASTNode *node = (ASTNode *)malloc(sizeof(ASTNode));
    if (node == NULL) {
        set_error(&global_error, MEMORY_ALLOCATION_ERROR,0,0);
        return NULL;
    }
    node->lineType = LINE_EMPTY;
    node->label = NULL;
    node->operation = NULL;
    node->operands = NULL;
    node->numOperands = 0;
    return node;
}

void setLineType(ASTNode *node, LineType lineType) {
    node->lineType = lineType;
}

void setLabel(ASTNode *node, const char *label) {
    if (node->label) free(node->label);
    node->label = label ? strdup(label) : NULL;
}

void setOperation(ASTNode *node, const char *operation) {
    if (node->operation) free(node->operation);
    node->operation = operation ? strdup(operation) : NULL;
}

int addOperand(ASTNode *node, const char *operand) {
    OperandNode *newOperand = (OperandNode *)malloc(sizeof(OperandNode));
    if (newOperand == NULL) {
        set_error(&global_error, MEMORY_ALLOCATION_ERROR,0,0);
        return 0;
    }
    newOperand->operand = my_strndup(operand, strlen(operand));
    newOperand->next = node->operands;
    node->operands = newOperand;
    node->numOperands++;
    return 1;
}

void freeASTNode(ASTNode *node) {
    if (node) {
        if (node->label) free(node->label);
        if (node->operation) free(node->operation);
        OperandNode *current = node->operands;
        while (current) {
            OperandNode *next = current->next;
            free(current->operand);
            free(current);
            current = next;
        }
        free(node);
    }
}