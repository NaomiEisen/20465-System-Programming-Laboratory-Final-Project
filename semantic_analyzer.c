#include "semantic_analyzer.h"
#include "ast.h"
#include "cmp_data.h"
#include "hardware.h"
#include "boolean.h"
#include "code_convert.h"
#include "utils.h"



boolean validate_label(ASTNode *node, int address) {
    switch (node->lineType) {
        case LINE_OPERATION: return insert_label(&trie_label, node->label, address, OPERATION);
        case LINE_DIRECTIVE: return insert_label(&trie_label, node->label, address, DIRECTIVE);
        default: return FALSE;

    }
}


void code_operands(ASTNode *node, CmpData *cmp_data) {
    OperandNode *current = node->operands;
    boolean reg = FALSE;
    int counter = 0;
    int reg_pos;

    while (current != NULL) {
        switch (current->adr_mode) {
            case 0: code_immediate_addr_mode(current, &cmp_data->code);
                    break;
            case 1: code_direct_addr_mode(current,cmp_data);
                break;
            case 2:
            case 3:
                if (reg == TRUE) {
                    cmp_data->code.count--; /* write on the previous word */
                }
                reg_pos = REGISTER_POS + (3 * counter); /** todo fix this number */
                code_register_addr_mode(current, &cmp_data->code,reg_pos) ;
                reg = TRUE;
                break;

        }
        counter++;
        cmp_data->code.count++;
    }
}

boolean handle_directive(ASTNode *node) {
return TRUE;
}

boolean first_word(ASTNode *node, int command_index, MemoryImage *code_img) {
    OperandNode *current = node->operands;

    /* Code the operation name */
    set_int_code(0, 3, command_index, code_img);

    /* Code first operand's address mode */
    if (command_table[command_index].addr_mode_op1[current->adr_mode] == 1) {

        set_bit(7-current->adr_mode, code_img); /** Todo: define number **/
        current = current->next;

        if (command_table[command_index].addr_mode_op1[current->adr_mode] == 1) {
            set_bit(11-current->adr_mode, code_img); /** Todo: define number **/

            /* ARE default first word's field */
            set_bit(A, code_img);

            code_img->count++;
            return TRUE;

        }
    }
    return FALSE;
}



/**
 * Finds the corresponding function function for the inputted command name.
 *
 * @param command the user's inputted command name
 * @return a Function structure representing the corresponding function function,
 * or NULL if no matching command is found
 */
int find_command(const char *command) {
    int i = 0; /*index for iterating through the commandMappings array */

    /*iterate through the commandMappings array until a NULL command is found */
    while ((strcmp(command_table[i].command, "terminator") == 0)) {
        if (strcmp(command_table[i].command, command) == 0) {
            return i;
        }
        i++;
    }
    /*for (i = 0 ; i < command_table_size ; i++) {
        if (strcmp(command_table[i].command, command) == 0) {
            return i;
        }
    }*/


    /* return -1 if the corresponding command has not been found */
    return -1;
}


boolean handle_operation(ASTNode *node, CmpData *cmp_data) {
    int ic_start = cmp_data->code.count; /* remember starting address */
    int command_index;

    /* Find corresponding command_index */
    command_index = find_command(node->operation);
    if (command_index == -1){ /* Invalid command_index name */
        set_error(&global_error, COMMAND_NAME_ERROR, node->location);
        print_error(&global_error);
        return FALSE;
    }

    /* Validate number of parameters */
    if (command_table[command_index].num_params != node->numOperands) {
        set_error(&global_error, INVALID_PARAM_NUMBER, node->location);
        print_error(&global_error);
        return FALSE;
    }

    /* Code first word */
    if (first_word(node, command_index, &cmp_data->code) == FALSE) {
        set_error(&global_error, INVALID_PARAM_NUMBER, node->location);
        print_error(&global_error);
        return FALSE;
    }

    /* Code the second/third word */
    code_operands(node, cmp_data);

    /* insert label if exists */
    if (node->label != NULL){
        return validate_label(node, ic_start);
    }

    return TRUE;
}

boolean analyzeLine(ASTNode *node, CmpData *cmp_data) {

    /* Check if line is empty or comment */
    if (node->lineType == LINE_EMPTY || node->lineType == LINE_COMMENT) {
        return TRUE;
    }

    /* Handle operation line */
    if (node->lineType == LINE_OPERATION) {
        return handle_operation(node, cmp_data);
    }

    /* Handle directive line */


    return TRUE;
}