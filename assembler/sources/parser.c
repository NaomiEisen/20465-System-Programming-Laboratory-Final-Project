/* ---------------------------------------------------------------------------------------
 *                                          Includes
 * --------------------------------------------------------------------------------------- */
#include <stdio.h>
#include <string.h>
#include <ctype.h>
#include <stdlib.h>
#include "../../structures/headers/macro_data.h"
#include "../../structures/headers/ast.h"
#include "../../utils/headers/utils.h"
#include "../../structures/headers/mappings.h"
#include "../headers/parser.h"
/* ---------------------------------------------------------------------------------------
 *                               Static Functions Prototypes
 * --------------------------------------------------------------------------------------- */
static Boolean check_empty_line (const char** line, ASTNode* node);
static Boolean is_label(const char **line, ASTNode *node, const MacroTrie *macr_trie);
static Boolean validate_label(const char *label, ASTNode *node, const MacroTrie *macr_trie);
static Boolean parse_operation(const char **line, ASTNode *node);
static Boolean validate_operation(const char *operation, ASTNode* node);
static Boolean parse_operands(const char **line, ASTNode *node, const MacroTrie *macr_trie);
static Boolean parse_string(const char **line, ASTNode *node);
static void parse_instruct_operand(ASTNode *node, const char *operand, const MacroTrie *macr_trie);
static void parse_int(ASTNode* node, const char *operand);
static void parse_reg(ASTNode* node, const char *operand, short addr_mode);
static void parse_label(ASTNode *node, const char *operand, const MacroTrie *macr_trie);
/* ---------------------------------------------------------------------------------------
 *                                   Head Function Of Parser
 * --------------------------------------------------------------------------------------- */
/**
 * Parses a line of assembly image and creates an ASTNode representing the line.
 * This function processes a single line of assembly image, determining its type
 * (empty/comment, label, directive, or instruction) and extracting the relevant
 * information into an ASTNode structure. It uses several helper functions to
 * handle different parts of the line.
 *
 * @param macr_trie A trie containing macro definitions to check for collisions with labels.
 * @param file_name The name of the file being parsed, used for error reporting.
 * @param line_num The line number in the file, used for error reporting.
 * @param line The line of assembly image to parse.
 *
 * @return A pointer to the newly created ASTNode representing the parsed line.
 */
ASTNode *parseLine(const MacroTrie *macr_trie, const char *file_name, int line_num, char *line) {
    ASTNode *node = NULL;
    const char *line_ptr = line;

    /* Skip leading whitespace */
    trim_leading_spaces(&line_ptr);

    /* Create empty ASTNode */
    node = create_empty_ASTnode(file_name, line_num, line);

    /* ------------------- 1. Check if line is empty or a comment ------------------- */
    if (check_empty_line(&line_ptr, node) == TRUE) {
        return node;
    }

    /* ----------------------------- 2. Check for label ----------------------------- */
    if (is_label(&line_ptr, node, macr_trie) == FALSE) {
        return node; /* Memory allocation failed */
    }

    /* ------------------------- 3. Read the operation name ------------------------- */

    if (*line_ptr == '.') {
        /* Directive line */
        set_ast_type(node, LINE_DIRECTIVE);
        line_ptr++; /* Skip dot */
    } else { /* Instruction line */
        set_ast_type(node, LINE_INSTRUCTION);
    }

    /* get operation name */
    if (parse_operation(&line_ptr, node) == FALSE) {
        return node;
    }

    /* get operands */
    if (parse_operands(&line_ptr, node, macr_trie) == FALSE) {
        return node;
    }

    return node;
}
/* ---------------------------------------------------------------------------------------
 *                                         Functions
 * --------------------------------------------------------------------------------------- */
/**
* Private function - checks if a line is empty or a comment.
* This function determines if a given line of text is empty or a comment
* and updates the ASTNode accordingly.
*
* @param line Pointer to the line of text.
* @param node The ASTNode to update.
* @return TRUE if the line is empty or a comment, FALSE otherwise.
*/
static Boolean check_empty_line(const char** line, ASTNode* node) {
    const char* ptr = *line;
    if (*ptr == '\0') { /* Empty line */
        set_ast_type(node, LINE_EMPTY);
        return TRUE;
    }
    if (*ptr == ';') { /* Comment line */
        set_ast_type(node, LINE_COMMENT);
        return TRUE;
    }
    /* If none of the conditions true, return FALSE */
    return FALSE;
}

/**
 * Private Function - identifies a label in the line, validates it, and updates the ASTNode.
 *
 * @param line Pointer to the line of text.
 * @param node The ASTNode to update.
 * @param macr_trie Trie structure for macro definitions.
 * @return TRUE if the label was processed successfully, FALSE otherwise.
 */
static Boolean is_label(const char **line, ASTNode *node, const MacroTrie *macr_trie) {
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

        if (!label) {
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

/**
 * Private function - checks if the label is valid, ensuring it meets the naming rules
 * and does not collide with reserved words or macros.
 *
 * @param label The label to validate.
 * @param node The ASTNode to update the error status if needed.
 * @param macr_trie Trie structure for macro names.
 * @return TRUE if the label is valid, FALSE otherwise.
 */
static Boolean validate_label(const char *label, ASTNode *node, const MacroTrie *macr_trie) {
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

    /* Check if the label collides with reserved words */
    if (reserved_word(label) == TRUE) {
        set_error(LABEL_RESERVED_WORD, node->location);
        return FALSE;
    }

    /* Check if the label collides with macro name */
    if (find_macro(macr_trie, label)) {
        set_error(LABEL_MACR_COLLIDES, node->location);
        return FALSE;
    }

    return TRUE; /* Label is valid */
}

/**
 * Private function - parses the operation from the line.
 * This function extracts the operation name from the line, validates it,
 * and updates the ASTNode.
 *
 * @param line Pointer to the line of text.
 * @param node The ASTNode to update.
 * @return TRUE if the operation was parsed successfully, FALSE otherwise.
 */
static Boolean parse_operation(const char **line, ASTNode *node) {
    const char* start = *line;    /* Pointer to the start of the line */
    const char* line_ptr = start; /* Pointer to move through the line */
    char* operation = NULL;   /* Variable to store the operation name */
    Boolean result; /* The status of the operation validation methods */


    /* Position the pointer to the last char before the comme - if exists */
    while (*line_ptr && !is_space(*line_ptr) && *line_ptr != ',') {
        if (*line_ptr == ',') {
            set_error(ILLEGAL_COMMA_ERROR, node->location);
            return FALSE;
        }
        line_ptr++;
    }

    /* Get the first word of the command */
    operation = my_strndup(start, line_ptr - start);

    if (!operation) { /* Memory allocation failure */
        set_error(MEMORY_ALLOCATION_ERROR, node->location);
        return FALSE;
    }

    /* Skip any whitespace after the operation */
    trim_leading_spaces(&line_ptr);

    /* Update the line to point the next char after this word */
    *line = line_ptr;

    /* Save and validate the founded operation name */
    result = validate_operation(operation, node);
    free(operation);
    return result;
}

/**
 * Private function - validates the operation.
 * This function checks if the operation is valid and updates the ASTNode
 * with the corresponding instruction or directive.
 *
 * @param operation The operation to validate.
 * @param node The ASTNode to update.
 * @return TRUE if the operation is valid, FALSE otherwise.
 */
static Boolean validate_operation(const char *operation, ASTNode* node) {
    short command_index; /* Index of the command in the command mapping */

    /* If this is an instruction line */
    if (node->lineType == LINE_INSTRUCTION) {
        /* find the corresponding instruction to the given name */
        command_index = get_instruct_index(operation);
        if (command_index != -1 ) {
            /* Found the corresponding instruction */
            set_operation_for_instruction(node, command_index);
            return TRUE;
        }
    }

    /* If this is directive line */
    if (node->lineType == LINE_DIRECTIVE) {
        /* find the corresponding directive command to the given name */
        command_index = get_dir_index(operation);
        if (command_index != -1) {
            /* Found the corresponding directive */
            set_operation_for_directive(node, get_dir_command(command_index) );
            return TRUE;
        }
    }

    /* Given command name is invalid */
    set_error(INSTRUCTION_NAME_ERROR, node->location);
    return FALSE;
}

/**
 * Private function - parses the operands from the line.
 * This function extracts the operands from the line, validates them,
 * and updates the ASTNode.
 *
 * @param line Pointer to the line of text.
 * @param node The ASTNode to update.
 * @return TRUE if the operands were parsed successfully, FALSE otherwise.
 */
static Boolean parse_operands(const char **line, ASTNode *node, const MacroTrie *macr_trie) {
    const char *start = *line;                      /* Pointer to the start of the word */
    const char *line_ptr = start;                   /* Pointer to move through the line */
    Boolean first_op = TRUE; /* Flag indicating if the first operand was already parsed */
    char* operand = NULL;                         /* Variable to store the operand text */
    int operand_counter = 0;                        /* Counts number of operands parsed */

    /* If this is a string directive - parse the string */
    if (node->lineType == LINE_DIRECTIVE) {
        if (node->specific.directive.operation == STRING) {
            return parse_string(line, node);
        }
    }

    while (*line_ptr) {
        if (first_op == TRUE) {
            /* First operand shouldn't start with comma */
            if (*line_ptr == ',') {
                set_error(ILLEGAL_COMMA_ERROR, node->location);
                return FALSE;
            }
        }

        if (first_op == FALSE) {
            /* All operand should be seperated by comma */
            if (*line_ptr != ',') {
                set_error(MISSING_COMMA_ERROR, node->location);
                return FALSE;
            }
            line_ptr++; /* Skip comma */
            trim_leading_spaces(&line_ptr); /* Skip spaces */
            if (*line_ptr == ',') { /* Consecutive comma error */
                set_error(CONSECUTIVE_COMMA_ERROR, node->location);
                return FALSE;
            }
        }

        start = line_ptr; /* Position the pointer to get the word */

        /* Move the pointer to the end of the word */
        while (*line_ptr && *line_ptr != ',' && !isspace(*line_ptr)) {
            if (is_space(*line_ptr)) { /* New operand without comma separation */
                set_error(MISSING_COMMA_ERROR, node->location);
                return FALSE;
            }
            line_ptr++;
        }

        /* If word is not empty */
        if (line_ptr > start) {
            /* Save the text */
            operand = my_strndup(start, line_ptr - start);
            if (!operand) { /* Memory allocation failed */
                set_general_error(MEMORY_ALLOCATION_ERROR);
                return FALSE;
            }

            /* Parse directive operands */
            if (node->lineType == LINE_DIRECTIVE) {
                add_directive_operand(&node->specific.directive, operand);
            } else { /* Parse instruction type operands */
                parse_instruct_operand(node, operand, macr_trie);
                operand_counter++;
                free(operand);
            }
            first_op = FALSE; /* Processed first operand already */
            trim_leading_spaces(&line_ptr);
        } else {
            set_error(EXTRA_TXT, node->location);
            return FALSE;
        }
    }
    return TRUE; /* Process executed successfully */
}

/**
 * Private function - parses a string operand.
 * This function extracts a quoted string from the provided line. The string
 * consists of all characters placed between the first and last quotes. If the string
 * is invalid, an error is printed, and the function returns FALSE.
 *
 * @param line Pointer to the line of text.
 * @param node The ASTNode to update.
 * @return TRUE if the string was parsed successfully, FALSE otherwise.
 */
static Boolean parse_string(const char **line, ASTNode *node) {
    const char *start = *line;  /* Pointer to the start of the string */
    const char *end = start;      /* Pointer to the end of the string */

    if (*start == '"') {  /* Start of a quoted string */
        start++;
        end = last_nonspace_char(end);
        if (*end != '"') { /* String does not end in with quotes */
            set_error(INVALID_END_STRING, node->location);
            return FALSE;
        }
        /* Add string to ASTNode and return this process status */
        return add_directive_operand(&node->specific.directive, my_strndup(start, end - start));
    } else {/* String does not start in with quotes */
        set_error(INVALID_START_STRING, node->location);
        return FALSE;
    }
}

/**
 * Private function - parses and sets the addressing modes for operands in an ASTNode.
 * This function parses using helper methods an operand and sets the appropriate addressing
 * mode in the ASTNode.
 *
 * @param node The ASTNode to update.
 * @param operand The operand to parse.
 */
static void parse_instruct_operand(ASTNode *node, const char *operand, const MacroTrie *macr_trie) {
    if (!operand) return; /* Null pointer */

    /* Integer */
    else if (operand[0] == '#') {
        parse_int(node, operand+1);
    }

    /* Indirect register */
    else if (operand[0] == '*') {
        /* Check if the rest of the string is a valid register */
        parse_reg(node, operand+1, ADDR_INDIRECT_REG);
    }

    /* Check for direct mode register */
    else if (get_register_index(operand) != -1) {
        parse_reg(node, operand, ADDR_DIRECT_REG);
    }

    else { /* label */
        parse_label(node, operand, macr_trie);
    }
}

/**
 * Private function - parses an integer operand.
 * This function validates if the operand is an integer and adds it to the ASTNode.
 *
 * @param node The ASTNode to update.
 * @param operand The operand to parse.
 */
static void parse_int(ASTNode* node, const char *operand) {
    /* Validate if the rest of the string is an integer */
    if (is_valid_integer(operand)) {
        if (add_instruct_operand(node, ADDR_MODE_IMMEDIATE, NULL, (short)my_atoi(operand)) == FALSE) {
            set_error(INVALID_PARAM_NUMBER, node->location);
        }
    } else {
        /* Invalid integer */
        set_error(NOT_INTEGER, node->location);
    }
}

/**
 * Private function - parses a register operand.
 * This function validates if the operand is a register and adds it to the ASTNode.
 *
 * @param node The ASTNode to update.
 * @param operand The operand to parse.
 * @param addr_mode The addressing mode to set.
 */
static void parse_reg(ASTNode* node, const char *operand, short addr_mode) {
    /* Get the corresponding register from the mappings */
    short index = get_register_index(operand);
    if (index != -1) {
        if (add_instruct_operand(node, addr_mode, NULL, index) == FALSE) {
            set_error(INVALID_PARAM_NUMBER, node->location);
        }
    } else {
        set_error(INVALID_REGISTER, node->location);
    }
}
/**
 * Private function - parses a label operand.
 * his function checks if the operand is a valid label and if so, adds it to the ASTNode.
 *
 * @param node The ASTNode to update.
 * @param operand The operand to parse.
 */
static void parse_label(ASTNode *node, const char *operand, const MacroTrie *macr_trie) {
    if (validate_label(operand, node, macr_trie) == TRUE) { /* Validate label */
        if (add_instruct_operand(node, ADDR_MODE_DIRECT, operand, 0) == FALSE) {
            set_error(INVALID_PARAM_NUMBER, node->location);
        }
    }
}