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

void setLabel(ASTNode *node, char* label) {
    node->label = label;
}

void setOperation(ASTNode *node, char* operation) {
    node->operation = operation;
}

int addOperand(ASTNode *node, char *operand) {
    OperandNode *newOperand = (OperandNode *)malloc(sizeof(OperandNode));
    if (newOperand == NULL) {
        set_error(&global_error, MEMORY_ALLOCATION_ERROR,0,0);
        return 0;
    }
    newOperand->operand = operand;
    newOperand->next = NULL;

    /* If the operand list is empty, add the new operand as the first node */
    if (node->operands == NULL) {
        node->operands = newOperand;
    } else {
        /* Traverse to the end of the list */
        OperandNode *current = node->operands;
        while (current->next != NULL) {
            current = current->next;
        }
        /* Append the new operand at the end */
        current->next = newOperand;
    }

    /* Update the number of operands in AST node */
    node->numOperands++;
    return 1;
}

void freeASTNode(ASTNode *node) {
    OperandNode* current = NULL;
    if (node) {
        if (node->label) free(node->label);
        if (node->operation) free(node->operation);
        current = node->operands;
        while (current) {
            OperandNode *next = current->next;
            free(current->operand);
            free(current);
            current = next;
        }
        free(node);
    }
}

void printASTNode(const ASTNode *node) {
    const char *lineTypeStr = NULL;
    OperandNode *current = NULL;
    switch (node->lineType) {
        case LINE_EMPTY: lineTypeStr = "Empty"; break;
        case LINE_COMMENT: lineTypeStr = "Comment"; break;
        case LINE_DIRECTIVE: lineTypeStr = "Directive"; break;
        case LINE_OPERATION: lineTypeStr = "Operation"; break;
    }

    printf("Line Type: %s\n", lineTypeStr);
    if (node->label) printf("Label: %s\n", node->label);
    if (node->operation) printf("Operation: %s\n", node->operation);
    printf("Operands: ");
    current = node->operands;
    while (current) {
        printf("%s ", current->operand);
        current = current->next;
    }
    printf("\nNumber of Operands: %d\n", node->numOperands);
    printf("\n");
}