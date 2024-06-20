#include "ast.h"
#include "cmp_data.h"
#include "mappings.h"
#include "boolean.h"
#include "code_convert.h"
#include "semantic.h"


boolean analyzeLine(ASTNode *node, CmpData *cmp_data, Mappings *mappings) {

    /* Check if line is empty or comment */
    if (node->lineType == LINE_EMPTY || node->lineType == LINE_COMMENT) {
        return TRUE;
    }

    /* Handle operation line */
    if (node->lineType == LINE_OPERATION) {
        return handle_operation(node, cmp_data, mappings);
    }

    /* Handle directive line */
    return handle_directive(node, cmp_data, mappings);
}

boolean handle_operation(ASTNode *node, CmpData *cmp_data, Mappings *mappings) {
    int ic_start = cmp_data->code.count; /* remember starting address */
    int command_index;

    /* Find corresponding command_index */
    command_index = find_command(node->operation, mappings->instruct_table);
    if (command_index == -1){ /* Invalid command_index name */
        set_error(&error, COMMAND_NAME_ERROR, node->location);
        print_error(&error);
        return FALSE;
    }

    /* Validate number of parameters */
    if (mappings->instruct_table[command_index].num_params != node->numOperands) {
        set_error(&error, INVALID_PARAM_NUMBER, node->location);
        print_error(&error);
        return FALSE;
    }

    /* Code first word */
    if (first_word(node, command_index, &cmp_data->code, mappings) == FALSE) {
        set_error(&error, INVALID_PARAM_TYPE, node->location);
        print_error(&error);
        return FALSE;
    }

    /* Code the second/third word */
    code_operands(node, cmp_data);

    /* insert label if exists */
    if (node->label != NULL){
        return validate_label(node, ic_start, cmp_data, mappings);
    }

    return TRUE;
}



boolean validate_label(ASTNode *node, int address, CmpData *cmp_data, Mappings *mappings) {
    if (reserved_word(mappings, node->label) == FALSE) {
            switch (node->lineType) {
                case LINE_OPERATION: return insert_label(&cmp_data->label_table, node->label, address, OPERATION);
                case LINE_DIRECTIVE: return insert_label(&cmp_data->label_table, node->label, address, DIRECTIVE);
                default: return FALSE;
            }
    }
    return FALSE;
}


void code_operands(ASTNode *node, CmpData *cmp_data) {
    OperandNode *current = node->operands;
    boolean reg = FALSE;
    int counter = 0;

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
                 /** todo fix this number */
                code_register_addr_mode(current, &cmp_data->code,REGISTER_POS + (3 * counter)) ;
                reg = TRUE;
                break;
        }
        current = current->next;
        counter++;
        cmp_data->code.count++;
    }
}

boolean first_word(ASTNode *node, int command_index, MemoryImage *code_img, Mappings *mappings) {
    OperandNode *current = node->operands;

    /* Code the operation name */
    set_int_code(0, 3, command_index, code_img);

    /* ARE default first word's field */
    set_bit(A, code_img);

    /* Check if there is operands to code */
    if (current == NULL) {
        code_img->count++;
        return TRUE;
    }

    /* Code first operand's address mode */
    if (mappings->instruct_table[command_index].addr_mode_op1[current->adr_mode] == 1) {

        set_bit(7 - current->adr_mode, code_img); /** Todo: define number **/
        current = current->next;

        /* if there is second operand */
        if (current != NULL) {
            if (mappings->instruct_table[command_index].addr_mode_op2[current->adr_mode] == 1) {
                set_bit(11 - current->adr_mode, code_img); /** Todo: define number **/
            } else {
                return FALSE;
            }
        }

        code_img->count++;
        return TRUE;
    }

    return FALSE;
}

boolean handle_directive(ASTNode *node, CmpData *cmp_data, Mappings *mappings) {
    int id_start = cmp_data->data.count; /* remember starting address */
    int dir_index;
    dirCommand dir_command;
    boolean had_error;

    /* Find corresponding command_index */
    dir_index = find_directive(node->operation, mappings->directive_table);
    if (dir_index == -1){ /* Invalid command_index name */
        set_error(&error, DIRECTIVE_NAME_ERROR, node->location);
        print_error(&error);
        return FALSE;
    }

    /** TODO - if global variables allowed. make function handler */
    dir_command = mappings->directive_table[dir_index].dir_command;
    switch (dir_command) {
        case DATA:
            code_data(node, &cmp_data->data);
            break;
        case STRING:
            code_string(node, &cmp_data->data);
            break;
        case ENTRY:
            if (cmp_data->phase == 1) {
                mark_word(&cmp_data->code);
            } else {
                return add_entry(&cmp_data->label_table, node->operands);
            }
            break;
        case EXTERN:
            if (handle_extern(node, cmp_data, mappings) == FALSE) {
                return FALSE;
            }
            break;
    }
    /* insert label if exists */
    if (node->label != NULL){
        if (dir_command == EXTERN || dir_command == ENTRY) {
            printf("WARNING: label before extern/entry is useless"); /* TODO: put in error*/
        } else {
            if (validate_label(node, id_start, cmp_data, mappings) == FALSE) {
                set_error(&error, INVALID_LABEL, node->location);
                print_error(&error);
            }
        }
    }
    return (error.code == NO_ERROR);
}

boolean handle_extern(ASTNode *node, CmpData *cmpData, Mappings *mappings) {
    OperandNode *current = node->operands;

    while (current) {
        if (reserved_word(mappings, current->operand) == FALSE) {
            if (insert_label(&cmpData->label_table, current->operand, 0, EXTERNAL) == FALSE) {
                set_error(&error, INVALID_LABEL, node->location);
                break;
            }
        } else {
            set_error(&error, INVALID_LABEL, node->location);
            break;
        }
        current = current->next;
    }

    if (error.code != NO_ERROR) {
        print_error(&error);
        return FALSE;
    } else {
        return TRUE;
    }
}

/**
 * Finds the corresponding function function for the inputted command_str name.
 *
 * @param command the user's inputted command_str name
 * @return a Function structure representing the corresponding function function,
 * or NULL if no matching command_str is found
 */
int find_command(const char *command, InstructMapping *instruct_mapping) {
    int i = 0; /* Index for iterating through the commandMappings array */

    if (instruct_mapping == NULL) {
        return -1;
    }
    /* Iterate through the commandMappings array until an empty string command_str is found */
    while (instruct_mapping[i].command_str[0] != '\0') {
        if (strcmp(instruct_mapping[i].command_str, command) == 0) {
            return i;
        }
        i++;
    }

    /* Return -1 if the corresponding command_str has not been found */
    return -1;
}



int find_directive(const char *command, DirectiveMapping *dir_mapping) {
    int i = 0; /* Index for iterating through the commandMappings array */

    if (dir_mapping == NULL) {
        return -1;
    }

    /* Iterate through the commandMappings array until an empty string command_str is found */
    while (dir_mapping[i].command_str[0] != '\0') {
        if (strcmp(dir_mapping[i].command_str, command) == 0) {
            return i;
        }
        i++;
    }

    /* Return -1 if the corresponding command_str has not been found */
    return -1;
}

