/* ---------------------------------------------------------------------------------------
 *                                          Includes
 * --------------------------------------------------------------------------------------- */
#include <stdio.h>
#include <string.h>
#include <ctype.h>
#include <stdlib.h>
#include "ast.h"
#include "utils.h"
#include "errors.h"
#include "boolean.h"
#include "mappings.h"
#include "parser.h"
/* ---------------------------------------------------------------------------------------
 *                                   Head Function Of Parser
 * --------------------------------------------------------------------------------------- */
ASTNode *parseLine(MacroTrie *macr_trie, const char *file_name, int line_num, const char *line) {
    ASTNode *node = create_empty_ASTnode(file_name, line_num);
    const char *line_ptr = line;

    /* Skip leading whitespace */
    trim_leading_spaces(&line_ptr);

    /* ------------------- 1. Check if line is empty or a comment ------------------- */
    if (check_empty_line(&line_ptr, node) == TRUE) {
        return node;
    }

    /* ----------------------------- 2. Check for label ----------------------------- */
    is_label(&line_ptr, node, macr_trie);


    /* ------------------------- 3. Read the operation name ------------------------- */

    if (*line_ptr == '.') {
        /* This is directive */
        set_ast_type(node, LINE_DIRECTIVE);
        line_ptr++; /* Skip dot */
    } else {
        set_ast_type(node, LINE_INSTRUCTION);
    }

    /* get operation name */
    if (parse_operation(&line_ptr, node) == FALSE) {
        return node;
    }

    /* get operands */
    if (parse_operands(&line_ptr, node) == FALSE) {
        return node;
    }

    return node;
}
/* ---------------------------------------------------------------------------------------
 *                                         Functions
 * --------------------------------------------------------------------------------------- */

Boolean check_empty_line (const char** line, ASTNode* node) {
    const char* ptr = *line;
    if (*ptr == '\0') {
        set_ast_type(node, LINE_EMPTY);
        return TRUE;
    }
    if (*ptr == ';') {
        set_ast_type(node, LINE_COMMENT);
        return TRUE;
    }
    return FALSE;
}


Boolean is_label(const char **line, ASTNode *node, MacroTrie *macr_trie) {
    const char *start = *line;
    const char *line_ptr = start;
    char *label;

    while (*line_ptr && *line_ptr != ':' && !isspace(*line_ptr)) line_ptr++;
    /* Label found */
    if (*line_ptr == ':') {
        /* Save label string */
        label = my_strndup(start, line_ptr - start);

        line_ptr++;  /* Skip the colon */
        trim_leading_spaces(&line_ptr); /* Skip any whitespace after the label */
        *line = line_ptr;

        if (label == NULL) {
            set_general_error(MEMORY_ALLOCATION_ERROR);
            return FALSE; /* Memory allocation failure */
        }
        /* Validate label name */
        if (validate_label(label, node, macr_trie) == FALSE) {
            free(label);
            return FALSE; /* Invalid name */
        }

        set_ast_label(node, label); /* Save label in AST node */
        free(label); /* Free duplicated label */

    }
    return TRUE; /* Function proceeded successfully */
}

Boolean validate_label(const char *label, ASTNode *node, MacroTrie *macr_trie) {
    /* Check if the label is NULL or does not start with an alphabetic character */
    if (!label || !isalpha(label[0])) {
        set_error(INVALID_LABEL_NAME, node->location);
        return FALSE; /* Return FALSE if the label is invalid */
    }

    /* Check if the label exceeds the maximum allowed length */
    if (strlen(label) > MAX_LABEL_LENGTH) {
        set_error(INVALID_LABEL_LENGTH, node->location);
        return FALSE; /* Return FALSE if the label is too long */
    }

    if (reserved_word(label) == TRUE) {
        set_error(LABEL_RESERVED_WORD, node->location);
        return FALSE;
    }

    if (find_macro(macr_trie,label) != NULL) {
        set_error(LABEL_MACR_COLLIDES, node->location);
        return FALSE;
    }

    return TRUE;
}


Boolean parse_operation(const char **line, ASTNode *node) {
    const char* start = *line;
    const char* line_ptr = start;
    char* operation = NULL;
    Boolean result;


    while (*line_ptr && !is_space(*line_ptr) && *line_ptr != ',') {
        if (*line_ptr == ',') {
            set_error(ILLEGAL_COMMA_ERROR, node->location);
            return FALSE;
        }
        line_ptr++;
    }

    operation = my_strndup(start, line_ptr - start);
    if (operation == NULL) {
        set_error(MEMORY_ALLOCATION_ERROR, node->location);
        return FALSE; /* Memory allocation failure */
    }

    trim_leading_spaces(&line_ptr); /* Skip any whitespace after the operation */
    *line = line_ptr;

    result = validate_operation(operation, node);
    free(operation);
    return result;
}

Boolean validate_operation(const char *operation, ASTNode* node) {
    int command_index;

    if (node->lineType == LINE_INSTRUCTION) {
        command_index = get_instruct_index(operation);
        if (command_index != -1 ) {
            set_operation_for_instruction(node, command_index);
            return TRUE;
        }
    }
    if (node->lineType == LINE_DIRECTIVE) {
        command_index = get_dir_index(operation);
        if (command_index != -1) {
            set_operation_for_directive(node, get_dir_command(command_index) );
            return TRUE;
        }
    }

    set_error(INSTRUCTION_NAME_ERROR, node->location);
    return FALSE;
}



Boolean parse_operands(const char **line, ASTNode *node) {
    const char *start = *line;
    const char *line_ptr = start;
    Boolean first_op = TRUE;
    char* operand = NULL;
    int operand_counter = 0;

    if (node->lineType == LINE_DIRECTIVE) {
        if (node->specific.directive.operation == STRING) {
            return parse_string(line, node);
        }
    }

    while (*line_ptr) {
        if (first_op == TRUE) {
            if (*line_ptr == ',') {
                set_error(ILLEGAL_COMMA_ERROR, node->location);
                return FALSE;
            }
        }

        if (first_op == FALSE) {
            if (*line_ptr != ',') {
                set_error(MISSING_COMMA_ERROR, node->location);
                return FALSE;
            }
            line_ptr++; /* Skip comma */
            trim_leading_spaces(&line_ptr); /* Skip spaces */
            if (*line_ptr == ',') {
                set_error(CONSECUTIVE_COMMA_ERROR, node->location);
                return FALSE;
            }
        }

        start = line_ptr;
        while (*line_ptr && *line_ptr != ',' && !isspace(*line_ptr)) {
            if (is_space(*line_ptr)) {
                set_error(MISSING_COMMA_ERROR, node->location);
                return FALSE;
            }
            line_ptr++;
        }

        if (line_ptr > start) {
            operand = my_strndup(start, line_ptr - start);
            if (operand == NULL) {
                set_general_error(MEMORY_ALLOCATION_ERROR);
                return FALSE;
            }
            if (node->lineType == LINE_DIRECTIVE) {
                add_directive_operand(&node->specific.directive, operand);

            } else { /* instruct type */
                if (operand_counter >= 2) {
                    set_general_error(INVALID_PARAM_NUMBER);
                    free(operand);
                    return FALSE;
                }
                parse_instruct_operand(node, operand);
                operand_counter++;
                free(operand);
            }
            first_op = FALSE;
            trim_leading_spaces(&line_ptr);
        }
        else {
            set_error(EXTRA_TXT, node->location);
            return FALSE;
        }
    }
    return TRUE;
}

Boolean parse_string(const char **line, ASTNode *node) {
    const char *start = *line;
    const char *line_ptr = start;

    if (*line_ptr == '"') {  /* Start of a quoted string */
        start++;
        line_ptr = trim_trailing_spaces(line_ptr);
        if (*line_ptr != '"') {
            set_error(INVALID_STRING, node->location);
            return FALSE;
        }
        return add_directive_operand(&node->specific.directive, my_strndup(start, line_ptr - start));
    }
    return FALSE;
}

/* Function to determine and set addressing modes for operands in an ASTNode */
void parse_instruct_operand(ASTNode *node, const char *operand) {

    if (operand == NULL) {
        return;
    }

    else if (operand[0] == '#') {
        /* Is integer */
        parse_int(node, operand+1);
    }
    else if (operand[0] == '*') {
        /* Check if the rest of the string is a valid register */
        parse_reg(node, operand+1, ADDR_INDIRECT_REG);
    }

    else if (get_register_index(operand) != -1) {
        parse_reg(node, operand, ADDR_DIRECT_REG);
    }

    else { /* label */
        parse_label(node, operand);
    }
}

void parse_int(ASTNode* node, const char *operand) {
    /* Validate if the rest of the string is an integer */
    if (is_valid_integer(operand)) {
        if (add_instruct_operand(node, ADDR_MODE_IMMEDIATE, NULL, my_atoi(operand)) == FALSE) {
            set_error(INVALID_PARAM_NUMBER, node->location);
        }
    } else {
        set_error(NOT_INTEGER, node->location);
    }
}


void parse_reg(ASTNode* node, const char *operand, int addr_mode) {
    int index = get_register_index(operand);
    if (index != -1) {
        if (add_instruct_operand(node, addr_mode, NULL, index) == FALSE) {
            set_error(INVALID_PARAM_NUMBER, node->location);
            print_error();
        }
    } else {
        set_error(INVALID_REGISTER, node->location);
    }
}

void parse_label (ASTNode* node, const char *operand) {
    if (validate_label(operand, node, NULL) == TRUE) {
        if (add_instruct_operand(node, ADDR_MODE_DIRECT, operand, 0) == FALSE) {
            set_error(INVALID_PARAM_NUMBER, node->location);
        }
    }
}