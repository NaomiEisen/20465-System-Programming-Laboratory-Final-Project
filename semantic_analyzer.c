#include "semantic_analyzer.h"
#include "ast.h"
#include "label_trie.h"
#include "hardware.h"
#include "boolean.h"
#include "code_convert.h"

boolean analyzeLine(ASTNode *node) {

    /* Check if line is empty or comment */
    if (node->lineType == LINE_EMPTY || node->lineType == LINE_COMMENT) {
        return TRUE;
    }


    /* Handle operation line */
    if (node->lineType == LINE_OPERATION) {
        return handle_opeartion(node);
    }


    /* Handle directive line */



    return TRUE;
}

boolean validate_label(ASTNode *node, int address) {
    switch (node->lineType) {
        case LINE_OPERATION: return insert_label(&trie_label, node->label, address, OPERATION);
        case LINE_DIRECTIVE: return insert_label(&trie_label, node->label, address, DIRECTIVE);
        default: return FALSE;

    }
}

boolean handle_opeartion(ASTNode *node) {
    int ic_start = code.count; /* remember starting address */
    int command;

    command = find_command(node->operation);

    if (command == -1){
        set_error(&global_error, COMMAND_NAME_ERROR, node->location);
        print_error(&global_error);
        return FALSE;
    }

    /* if no error occurred */
    if (command_table[command].num_params != node->numOperands) {
        set_error(&global_error, INVALID_PARAM_NUMBER, node->location);
        print_error(&global_error);
        return FALSE;
    }

    /* code the operation name */
    set_int_code(0, command, node->lineType);

    /* code the rest of the word */

    /* insert label if exists */
    if (node->label != NULL){
        return validate_label(node, ic_start);
    }

    return TRUE;
}

boolean handle_directive(ASTNode *node) {
return TRUE;
}

boolean code_operands(ASTNode *node) {

}

/**
 * Finds the corresponding function function for the inputted command name.
 *
 * @param command the user's inputted command name
 * @return a Function structure representing the corresponding function function,
 * or NULL if no matching command is found
 */
int find_command(const char *command) {
    int i; /*index for iterating through the commandMappings array */

    /*iterate through the commandMappings array until a NULL command is found */
    for (i = 0 ; i < command_table_size ; i++) {
        if (strcmp(command_table[i].command, command) == 0) {
            return i;
        }
    }
   /* while (command_table[i].command != NULL) {
        if (strcmp(command_table[i].command, command) == 0) {
            return i;
        }
        i++;
    }*/

    /* return -1 if the corresponding command has not been found */
    return -1;
}
