#include <stdio.h>
#include <stdlib.h>
#include "ast.h"
#include "errors.h"


ASTNode *create_empty_ASTNode(const char* file, int line) {
    ASTNode *node = (ASTNode *)malloc(sizeof(ASTNode));
    if (node == NULL) {
        set_general_error(&global_error, MEMORY_ALLOCATION_ERROR);
        return NULL;
    }
    node->lineType = LINE_EMPTY;
    node->label = NULL;
    node->operation = NULL;
    node->operands = NULL;
    node->numOperands = 0;
    node->location.file = file;
    node->location.line = line;
    return node;
}

void set_line_type(ASTNode *node, LineType lineType) {
    node->lineType = lineType;
}

void set_label(ASTNode *node, char* label) {
    node->label = label;
}

void set_operation(ASTNode *node, char* operation) {
    node->operation = operation;
}

int add_operand(ASTNode *node, char *operand) {
    OperandNode *newOperand = (OperandNode *)malloc(sizeof(OperandNode));
    if (newOperand == NULL) {
        set_general_error(&global_error, MEMORY_ALLOCATION_ERROR);
        return 0;
    }
    newOperand->operand = operand;
    newOperand->adr_mode = -1; /* Default addressing mode */
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

/* Function to set the addressing mode for a specific operand */
void set_operand_adr_mode(OperandNode *operand, int adr_mode) {
    operand->adr_mode = adr_mode;
}

void free_ASTNode(ASTNode *node) {
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

    /* Determine the line type as a string */
    switch (node->lineType) {
        case LINE_EMPTY: lineTypeStr = "Empty"; break;
        case LINE_COMMENT: lineTypeStr = "Comment"; break;
        case LINE_DIRECTIVE: lineTypeStr = "Directive"; break;
        case LINE_OPERATION: lineTypeStr = "Operation"; break;
    }

    /* Print the AST node details */
    printf("Line Type: %s\n", lineTypeStr);
    if (node->label) {
        printf("Label: %s\n", node->label);
    }
    if (node->operation) {
        printf("Operation: %s\n", node->operation);
    }

    /* Print the operands and their addressing modes */
    printf("Operands: ");
    current = node->operands;
    while (current) {
        printf("%s (adr_mode: %d) ", current->operand, current->adr_mode);
        current = current->next;
    }
    printf("\n");

    /* Print the number of operands */
    printf("Number of Operands: %d\n", node->numOperands);
    printf("\n");
}

