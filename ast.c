#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include "ast.h"
#include "utils.h"


ASTNode *create_empty_ASTnode(const char *file, int line) {
    ASTNode *node = (ASTNode*)malloc(sizeof(ASTNode));
    if (node) {
        node->lineType = LINE_EMPTY;
        node->label[0] = '\0';  /* Empty label */
        memset(&(node->instruction), 0, sizeof(Instruction));  /* Initialize instruction to 0 */
        node->location.file = file;
        node->location.line = line;
    }
    return node;
}
void set_ast_label(ASTNode *node, const char *label) {
    if (node) {
        strncpy(node->label, label, MAX_LABEL_LENGTH-2);  /* Copy label into node with a maximum length of 80 characters */
        node->label[MAX_LABEL_LENGTH-1] = '\0';  /* Null-terminate the label string */
    }
}

void set_ast_type(ASTNode *node, LineType lineType) {
    if (node) {
        node->lineType = lineType;
    }
}

void set_operation_for_directive(ASTNode *node, DirectiveType operation) {
    if (node && node->lineType == LINE_DIRECTIVE) {
        node->directive.operation = operation;
    }
}

void set_operation_for_instruction(ASTNode *node, int operation) {
    if (node && node->lineType == LINE_INSTRUCTION) {
        node->instruction.operation = operation;
    }
}

InstructionOperand* get_operand(ASTNode *node, int num) {
    if (num == 2) {
        return &node->instruction.operand2;
    }
    else {
        return &node->instruction.operand1;
    }
}

boolean add_instruct_operand(ASTNode *node, int adr_mode, const char *value, int reg) {
    InstructionOperand* instruct_op = get_operand(node, node->instruction.num_operands + 1);
    switch (adr_mode) {
        case 1:
            strcpy(instruct_op->value.char_val, value);
            instruct_op->adr_mode = adr_mode;
            node->instruction.num_operands++;
            break;
        case 0:
        case 2:
        case 3:
            instruct_op->value.int_val = reg;
            instruct_op->adr_mode = adr_mode;
            node->instruction.num_operands++;
            break;
        default:
            return FALSE;
    }
    return TRUE;
}

DirNode* create_dir_node(char *operand) {
    DirNode *new_node = (DirNode *)malloc(sizeof(DirNode));
    if (!new_node) {
        return NULL;
    }
    new_node->operand = operand;
    new_node->next = NULL;
    return new_node;
}

boolean add_directive_operand(Directive *directive, char *operand) {
    DirNode *new_node = create_dir_node(operand);
    if (!new_node) {
        return FALSE;
    }
    if (!directive->operands) {
        directive->operands = new_node;
    } else {
        DirNode *temp = directive->operands;
        while (temp->next) {
            temp = (DirNode *) temp->next;
        }
        temp->next = (struct DirNode *) new_node;
    }

    return TRUE;
}

void free_dir_nodes(DirNode *node) {
    DirNode *temp;
    while (node) {
        temp = node;
        node = (DirNode *) node->next;
        free(temp->operand);
        free(temp);
    }
}

void free_ast_node(ASTNode *node) {
    if (!node) {
        return;
    }
    if (node->lineType == LINE_DIRECTIVE) {
        free_dir_nodes(node->directive.operands);
    }
    free(node);
}

void print_instruction_node(ASTNode *node) {
    if (node->lineType != LINE_INSTRUCTION) {
        return;
    }
    printf("Instruction Node: label=%s, operation=%d, operand1(int)=%d, operand2(char)=%s\n",
           node->label, node->instruction.operation,
           node->instruction.operand1.value.int_val,
           node->instruction.operand2.value.char_val);
}

void print_directive_node(ASTNode *node) {
    DirNode *operand;
    if (node->lineType != LINE_DIRECTIVE) {
        return;
    }
    printf("Directive Node: label=%s, operation=%d\n", node->label, node->directive.operation);
    operand = node->directive.operands;
    while (operand) {
        printf("  Operand: %s\n", operand->operand);
        operand = (DirNode *) operand->next;
    }
}

/*void print_AST_node(ASTNode *node) {
    printf("Line Type: %d\n", node->lineType);
    printf("Label: %s\n", node->label);

    if (node->lineType == LINE_INSTRUCTION) {
        printf("Operation: %d\n", node->specifics.instruction.operation);
        printf("Operand 1: Adr Mode - %d, Value - ", node->specifics.instruction.operand1.adr_mode);
        if (node->specifics.instruction.operand1.adr_mode == 0) {
            printf("%d\n", node->specifics.instruction.operand1.value.int_val);
        } else if (node->specifics.instruction.operand1.adr_mode == 1) {
            printf("%s\n", node->specifics.instruction.operand1.value.char_val);
        }
        printf("Operand 2: Adr Mode - %d, Value - ", node->specifics.instruction.operand2.adr_mode);
        if (node->specifics.instruction.operand2.adr_mode == 0) {
            printf("%d\n", node->specifics.instruction.operand2.value.int_val);
        } else if (node->specifics.instruction.operand2.adr_mode == 1) {
            printf("%s\n", node->specifics.instruction.operand2.value.char_val);
        }
    } else if (node->lineType == LINE_DIRECTIVE) {
        printf("Directive Type: %d\n", node->specifics.directive.operation);
        printf("Operands:\n");
        struct OperandNode *current = node->specifics.directive.operands;
        while (current != NULL) {
            printf("%s\n", current->operand);
            current = current->next;
        }
    }
}*/
