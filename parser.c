#include <stdio.h>
#include <string.h>
#include <ctype.h>
#include "ast.h"
#include "utils.h"
#include "errors.h"
#include "boolean.h"
#include "hardware.h"


int check_empty_line (const char** line, ASTNode* node) {
    const char* ptr = *line;
    if (*ptr == '\0') {
        set_line_type(node, LINE_EMPTY);
        return 1;
    }
    if (*ptr == ';') {
        set_line_type(node, LINE_COMMENT);
        return 1;
    }
    return 0;
}

int check_label(const char **line, ASTNode *node) {
    const char *start = *line;
    const char *line_ptr = start;
    char *label;

    while (*line_ptr && *line_ptr != ':' && !isspace(*line_ptr)) line_ptr++;
    /* Label found */
    if (*line_ptr == ':') {
        /* Save label in AST node */
        label = my_strndup(start, line_ptr - start);
        if (label == NULL) {
            set_general_error(&error, MEMORY_ALLOCATION_ERROR);
            print_error(&error);
            return 0; /* Memory allocation failure */
        }
        set_label(node, label);
        line_ptr++;  /* Skip the colon */
        trim_leading_spaces(&line_ptr); /* Skip any whitespace after the label */
        *line = line_ptr;
    }
    return 1; /* Function proceeded successfully */
}

int parse_operation(const char **line, ASTNode *node) {
    const char* start = *line;
    const char* line_ptr = start;
    char* operation = NULL;

    while (*line_ptr && !is_space(*line_ptr) && *line_ptr != ',') {
        if (*line_ptr == ',') {
            set_error(&error, ILLEGAL_COMMA_ERROR, node->location);
            print_error(&error);
            return 0;
        }
        line_ptr++;
    }

    operation = my_strndup(start, line_ptr - start);
    if (operation == NULL) {
        set_error(&error, MEMORY_ALLOCATION_ERROR, node->location);
        print_error(&error);
        return 0; /* Memory allocation failure */
    }
    set_operation(node, operation);

    trim_leading_spaces(&line_ptr); /* Skip any whitespace after the operation */
    *line = line_ptr;
    return 1;
}

int parse_operands(const char **line, ASTNode *node) {
    const char *start = *line;
    const char *line_ptr = start;
    boolean first_op = TRUE;

    while (*line_ptr) {
        if (first_op == TRUE) {
            if (*line_ptr == ',') {
                set_error(&error, ILLEGAL_COMMA_ERROR, node->location);
                print_error(&error);
                return 0;
            }
        }

        if (first_op == FALSE) {
            if (*line_ptr != ',') {
                set_error(&error, MISSING_COMMA_ERROR, node->location);
                print_error(&error);
                return 0;
            }
            line_ptr++; /* Skip comma */
            trim_leading_spaces(&line_ptr); /* Skip spaces */
        }

        start = line_ptr;
        while (*line_ptr && *line_ptr != ',' && !isspace(*line_ptr)) {
            if (is_space(*line_ptr)) {
                set_error(&error, MISSING_COMMA_ERROR, node->location);
                print_error(&error);
                return 0;
            }
            line_ptr++;
        }

        if (line_ptr > start) {
            if (add_operand(node, my_strndup(start, line_ptr - start)) == 0)
                return 0;

            first_op = FALSE;
            trim_leading_spaces(&line_ptr);
        }
        else {
            set_error(&error, EXTRA_TXT, node->location);
            print_error(&error);
            return 0;
        }
    }
    return 1;
}

/* Function to check if a string is a valid register */
boolean is_valid_register(char *str) {
    int i = 0;

    /* iterate through the commandMappings array until a NULL command_str is found */
    while (registers[i] != NULL) {
        if (strcmp(str, registers[i]) == 0) {
            return TRUE;
        }
        i++;
    }
    return FALSE;
}

/* Function to determine and set addressing modes for operands in an ASTNode */
void determine_operand_adr_modes(ASTNode *node) {
    OperandNode *current = node->operands;
    while (current) {
        if (current->operand[0] == '#') {
            /* Validate if the rest of the string is an integer */
            if (is_valid_integer(current->operand + 1)) {
                if (strip_first_chars(&current->operand, 1) == FALSE) {
                    set_error(&error, MEMORY_ALLOCATION_ERROR, node->location);
                    print_error(&error);
                }
                current->adr_mode = 0;
            }
            else {
                set_error(&error, NOT_INTEGER, node->location);
                print_error(&error);
            }
        } else if (current->operand[0] == '*') {
            /* Check if the rest of the string is a valid register */
            if (is_valid_register(current->operand + 1)) {
                if (strip_first_chars(&current->operand, 2) == FALSE) {
                    set_error(&error, MEMORY_ALLOCATION_ERROR, node->location);
                    print_error(&error);
                }
                current->adr_mode = 2;
            }
            else {
                set_error(&error, INVALID_REGISTER, node->location);
                print_error(&error);
            }
        } else if (is_valid_register(current->operand)) {
            if (strip_first_chars(&current->operand, 1) == FALSE) {
                set_error(&error, MEMORY_ALLOCATION_ERROR, node->location);
                print_error(&error);
            }
            current->adr_mode = 3;
        } else {
            current->adr_mode = 2;
        }
        current = current->next;
    }
}

ASTNode *parseLine(const char *line, const char* file_name, int line_num) {
    ASTNode *node = create_empty_ASTNode(file_name, line_num);
    const char *line_ptr = line;

    /* Skip leading whitespace */
    trim_leading_spaces(&line_ptr);

    /* =============== 1. Check if line is empty or a comment =============== */
    if (check_empty_line(&line_ptr, node) == 1) {
        return node;
    }

    /* ======================== 2. Check for label ======================== */
    check_label(&line_ptr, node);

    /* ====================== 3. Read the operation ====================== */

    /* determine line type */
    trim_leading_spaces(&line_ptr);
    set_line_type(node, LINE_OPERATION); /* default */
    if (*line_ptr == '.') {
        /* This is directive */
        set_line_type(node, LINE_DIRECTIVE);
        line_ptr++; /* Skip dot */
    }

    /* get operation name */
    if (parse_operation(&line_ptr, node) == 0) {
        return node;
    }

    /* get operands */
    if (parse_operands(&line_ptr, node) == 0) {
        return node;
    }

    /*  determine operands address mode */
    if (node->lineType == LINE_OPERATION) {
        determine_operand_adr_modes(node);
    }

    return node;
}
