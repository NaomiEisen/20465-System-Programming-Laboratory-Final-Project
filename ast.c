/* ---------------------------------------------------------------------------------------
 *                                          Includes
 * --------------------------------------------------------------------------------------- */
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include "ast.h"
#include "utils.h"
#include "defines.h"
#include "errors.h"
/* ---------------------------------------------------------------------------------------
 *                                         Functions
 * --------------------------------------------------------------------------------------- */
/**
 * Creates and initializes an empty AST node.
 * @param file: The name of the file where the node is created.
 * @param line: The line number in the file where the node is created.
 * @return A pointer to the newly created AST node, or a null pointer if memory allocation failed.
 */
ASTNode *create_empty_ASTnode(const char *file, int line, char *line_content) {
    /* Allocate memory for ASTNode struct */
    ASTNode *node = (ASTNode*)malloc(sizeof(ASTNode));
    if (node) { /* If node is not null */
        node->lineType = LINE_EMPTY; /* Default line type */
        node->label[0] = '\0';  /* Empty label */
        memset(&(node->specific.instruction), 0, sizeof(Instruction));  /* Initialize instruction to 0 */
        node->location.file = file;
        node->location.line = line;
        save_line_content(&node->location, line_content);
    }
    return node; /* Return the node or null if the memory allocation failed */
}

/**
 * Sets the label of an AST node.
 *
 * @param node: The AST node whose label is to be set.
 * @param label: The label to be set.
 */
void set_ast_label(ASTNode *node, const char *label) {
    if (node) {
        /* Copy label into node with a maximum length of 80 characters */
        strncpy(node->label, label, MAX_LABEL_LENGTH-2);
        node->label[MAX_LABEL_LENGTH-1] = '\0';  /* Null-terminate the label string */
    }
}

/**
 * Sets the type of an AST node.
 *
 * @param node: The AST node whose type is to be set.
 * @param lineType: The type to be set.
 */
void set_ast_type(ASTNode *node, LineType lineType) {
    if (node) {
        node->lineType = lineType;
    }
}

/**
 * Sets the operation for a directive in an AST node.
 *
 * @param node: The AST node whose directive operation is to be set.
 * @param operation: The directive operation to be set.
 */
void set_operation_for_directive(ASTNode *node, DirectiveType operation) {
    /* First check if node exists and if it's type is directive */
    if (node && node->lineType == LINE_DIRECTIVE) {
        node->specific.directive.operation = operation;
    }
}

/**
 * Sets the operation for an instruction in an AST node.
 *
 * @param node: The AST node whose instruction operation is to be set.
 * @param operation: The instruction operation to be set.
 */
void set_operation_for_instruction(ASTNode *node, int operation) {
    /* First check if node exists and if it's type is instruction */
    if (node && node->lineType == LINE_INSTRUCTION) {
        node->specific.instruction.operation = operation;
    }
}

/**
 * Retrieves the operand of an instruction in an AST node.
 *
 * @param node: The AST node from which to retrieve the operand.
 * @param num: The number of the operand to retrieve (1 or 2).
 * @return A pointer to the specified operand.
 */
InstructionOperand* get_operand(ASTNode *node, int num) {
    if (num == 2) { /* Second operand */
        return &node->specific.instruction.operand2;
    }
    else { /* First operand */
        return &node->specific.instruction.operand1;
    }
}

/**
 * Adds an operand to an instruction in an AST node.
 *
 * @param node: The AST node to which the operand is to be added.
 * @param adr_mode: The addressing mode of the operand.
 * @param value: The value of the operand.
 * @param num: The register number (if applicable) of the operand.
 * @return TRUE if the operand was added successfully, FALSE otherwise.
 */
Boolean add_instruct_operand(ASTNode *node, int adr_mode, const char *value, int num) {
    InstructionOperand* instruct_op = get_operand(node, node->specific.instruction.num_operands + 1);
    switch (adr_mode) {
        /* Direct address mode - label operand */
        case 1:
            strcpy(instruct_op->value.char_val, value);
            instruct_op->adr_mode = adr_mode;
            node->specific.instruction.num_operands++;
            break;
        /* Integer operand */
        case 0: /* Immediate address mode */
        case 2: /* Register Indirect Addressing Mode */
        case 3: /* Register Direct Addressing Mode */
            instruct_op->value.int_val = num;
            instruct_op->adr_mode = adr_mode;
            node->specific.instruction.num_operands++;
            break;
        /* Invalid address mode */
        default:
            return FALSE;
    }
    /* The process completed successfully */
    return TRUE;
}

/**
 * Private function - creates a new directive node.
 *
 * @param operand: The operand for the directive node.
 * @return A pointer to the newly created directive node. Or a null pointer if the
 * memory allocation failed.
 */
static DirNode* create_dir_node(char *operand) {
    /* Allocate memory for new DirNode */
    DirNode *new_node = (DirNode *)malloc(sizeof(DirNode));
    if (new_node) {
        new_node->operand = operand;
        new_node->next = NULL;
    }

    /* Return the new node, or a null pointer if memory allocation failed */
    return new_node;
}

/**
 * Adds an operand to a directive.
 *
 * @param directive: The directive to which the operand is to be added.
 * @param operand: The operand to be added.
 * @return TRUE if the operand was added successfully, FALSE otherwise.
 */
Boolean add_directive_operand(Directive *directive, char *operand) {
    /* Create new empty DirNode */
    DirNode *new_node = create_dir_node(operand);

    /* Return false if memory allocation failed */
    if (!new_node) {
        return FALSE;
    }

    /* Add the new node to the end of the operand list */
    if (!directive->operands) {
        directive->operands = new_node;
    } else {
        DirNode *temp = directive->operands;
        while (temp->next) {
            temp = (DirNode *) temp->next;
        }
        temp->next = (struct DirNode *) new_node;
    }

    /* The process completed successfully */
    return TRUE;
}

/**
 * Private function - Frees the memory allocated for a list of directive nodes.
 *
 * @param node: The head of the list of directive nodes to be freed.
 */
static void free_dir_nodes(DirNode *node) {
    DirNode *temp;
    /* Iterate through list and free all the nodes */
    while (node) {
        temp = node;
        node = (DirNode *) node->next;
        free(temp->operand);
        free(temp);
    }
}

/**
 * Frees the memory allocated for an AST node.
 *
 * @param node: The AST node to be freed.
 */
void free_ast_node(ASTNode *node) {
    if (!node) { /* Check for null pointer */
        return;
    }
    /* If this is directive node - free the list of directive nodes first */
    if (node->lineType == LINE_DIRECTIVE) {
        free_dir_nodes(node->specific.directive.operands);
    }
    /* Free location content */
    free_location(&node->location);

    /* Free the node struct */
    free(node);
}


/* ------------------------------- for meeeee ------------------------------------------------------*/

void print_instruction_node(ASTNode *node) {
    if (node->lineType != LINE_INSTRUCTION) {
        return;
    }
    printf("Instruction Node: label=%s, operation=%d, operand1(int)=%d, operand2(char)=%s\n",
           node->label, node->specific.instruction.operation,
           node->specific.instruction.operand1.value.int_val,
           node->specific.instruction.operand2.value.char_val);
}

void print_directive_node(ASTNode *node) {
    DirNode *operand;
    if (node->lineType != LINE_DIRECTIVE) {
        return;
    }
    printf("Directive Node: label=%s, operation=%d\n", node->label, node->specific.directive.operation);
    operand = node->specific.directive.operands;
    while (operand) {
        printf("  Operand: %s\n", operand->operand);
        operand = (DirNode *) operand->next;
    }
}
