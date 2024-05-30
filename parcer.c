#include <stdio.h>
#include <ctype.h>
#include "ast.h"
#include "utils.h"
#include "errors.h"
#include "general.h"
#include "hardware.h"


int check_empty_line (const char** line, ASTNode* node) {
    const char* ptr = *line;
    if (*ptr == '\0') {
        setLineType(node, LINE_EMPTY);
        return 1;
    }
    if (*ptr == ';') {
        setLineType(node, LINE_COMMENT);
        return 1;
    }
    return 0;
}

void check_label(const char **line, ASTNode *node) {
    const char *start = *line;
    const char *line_ptr = start;

    while (*line_ptr && *line_ptr != ':' && !isspace(*line_ptr)) line_ptr++;
    /* Label found */
    if (*line_ptr == ':') {
        /* Save label in AST node */
        setLabel(node, my_strndup(start, line_ptr - start));
        line_ptr++;  /* Skip the colon */
        while (isspace(*line_ptr)) line_ptr++; /* Skip any whitespace after the label */
        *line = line_ptr;
    }
}

void parse_dir_opr(const char **line, ASTNode *node) {
    const char *start = *line;
    const char *line_ptr = start;

    /* ======================== Get operation name ======================== */
    trim_leading_spaces(&line_ptr);
    setOperation(node, my_strndup(start, line_ptr - start));
    trim_leading_spaces(&line_ptr); /* Skip any whitespace after the operation */

    /* ======================== Get operands ======================== */

    *line = line_ptr;
}


int parse_operation(const char **line, ASTNode *node) {
    const char *start = *line;
    const char *line_ptr = start;

    while (*line_ptr && !is_space(*line_ptr) && *line_ptr != ',') {
        if (*line_ptr == ',') {
            set_error(&global_error, ILLEGAL_COMMA_ERROR, NULL, 0);
            print_error(&global_error);
            return 0;
        }
        line_ptr++;
    }
    setOperation(node, my_strndup(start, line_ptr - start));

    trim_leading_spaces(&line_ptr); /* Skip any whitespace after the operation */
    *line = line_ptr;
    return 1;
}

int parse_operands(const char **line, ASTNode *node) {
    const char *start = *line;
    const char *line_ptr = start;
    bool first_op = true;

    while (*line_ptr) {
        if (first_op == false) {
            if (*line_ptr != ',') {
                set_error(&global_error, MISSING_COMMA_ERROR, NULL, 0);
                return 0;
            }
        }
        while (*line_ptr && *line_ptr != ',' && !isspace(*line_ptr)) {
            if (is_space(*line_ptr)) {
                set_error(&global_error, MISSING_COMMA_ERROR, NULL, 0);
                return 0;
            }
            line_ptr++;
        }
        if (line_ptr > start) {
            if ( addOperand(node, my_strndup(start, line_ptr - start)) == 0)
                return 0;
        }
        first_op = false;
        trim_leading_spaces(&line_ptr);
        start = line_ptr;
    }

    return 1;
}

ASTNode* parseLine(const char *line) {
    ASTNode *node = create_empty_ASTNode();
    const char *line_ptr = line;
    const char *start_ptr = NULL;
    int i = 0;

    if (global_error.code != NO_ERROR) {
        return NULL;
    }

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
    setLineType(node, LINE_OPERATION); /* default */
    if (*line_ptr == '.') {
        /* This is directive */
        setLineType(node, LINE_DIRECTIVE);
        line_ptr++; /* Skip dot */
    }

    /* get operation name */
    if (parse_operation(&line_ptr, node) == 0) {
        return node;
    }

    /* get operands */
    parse_operands(&line_ptr, node);

    return node;
}



/* ---------------------------------------------------------------------------------------
 *                                          Archive
 * --------------------------------------------------------------------------------------- */
/**
* if ((numParams == 2 && (!node->operand1 || !node->operand2)) ||
        (numParams == 1 && !node->operand1) ||
        (numParams == 0 && (node->operand1 || node->operand2))) {
        fprintf(stderr, "Error: invalid number of operands for operation %s.\n", node->operation);
        freeASTNode(node);
        return NULL;
*/

/*  while (*line_ptr && !is_space(*line_ptr)) line_ptr++;
setOperation(node, my_strndup(start_ptr, line_ptr - start_ptr));
while (isspace(*line_ptr)) line_ptr++;

*//*  Read the first operand *//*
start_ptr = line_ptr;
while (*line_ptr && *line_ptr != ',' && !is_space(*line_ptr)) {
if (*line_ptr == ',') {
set_error(&global_error, ILLEGAL_COMMA_ERROR, NULL, 0);
return NULL;
}
line_ptr++;
}
setOperand1(node, my_strndup(start_ptr, line_ptr - start_ptr));
trim_leading_spaces(&line_ptr);

*//* Read the second operand *//*
start_ptr = line_ptr;
if (*line_ptr != ',') {
set_error(&global_error, MISSING_COMMA_ERROR, NULL, 0);
return NULL;
}
while (*line_ptr &&  *line_ptr != ',' && !isspace(*line_ptr)) {
if (*line_ptr != ','){
set_error(&global_error, MULTIPULE_COMMA_ERROR, NULL, 0);
return NULL;
}
line_ptr++;
}
setOperand2(node, my_strndup(start_ptr, line_ptr - start_ptr));
trim_leading_spaces(&line_ptr);

if (*line_ptr != '\n') {
set_error(&global_error, EXTRA_TXT, NULL, 0);
return NULL;
}*/
