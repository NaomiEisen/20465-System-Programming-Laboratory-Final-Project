#include "ast.h"
#include "cmp_data.h"
#include "mappings.h"
#include "boolean.h"
#include "code_convert.h"
#include "semantic.h"


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
    return handle_directive(node, cmp_data);
}

boolean handle_operation(ASTNode *node, CmpData *cmp_data) {
    int ic_start = cmp_data->code.count; /* remember starting address */
    int command_index;

    /* Find corresponding command_index */
    command_index = get_instruct_index(node->operation);
    if (command_index == -1){ /* Invalid command_index name */
        set_error(&error, COMMAND_NAME_ERROR, node->location);
        print_error(&error);
        return FALSE;
    }

    /* Validate number of parameters */
    if (get_num_param(command_index) != node->numOperands) {
        set_error(&error, INVALID_PARAM_NUMBER, node->location);
        print_error(&error);
        return FALSE;
    }

    /* Code first word */
    if (first_word(node, command_index, &cmp_data->code) == FALSE) {
        set_error(&error, INVALID_PARAM_TYPE, node->location);
        print_error(&error);
        return FALSE;
    }

    /* Code the second/third word */
    code_operands(node, cmp_data);

    /* insert label if exists */
    if (node->label != NULL){
        if( add_label(node, ic_start, cmp_data) == FALSE) {
            set_error(&error, MULTIPLE_LABEL, node->location);
            print_error(&error);
            return FALSE;
        }
    }

    return TRUE;
}


boolean add_label(ASTNode *node, int address, CmpData *cmp_data) {
    switch (node->lineType) {
        case LINE_OPERATION:
            return insert_label(&cmp_data->label_table, node->label, address, OPERATION);
        case LINE_DIRECTIVE:
            return insert_label(&cmp_data->label_table, node->label, address, DIRECTIVE);
        default:
            return FALSE;
    }
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

boolean first_word(ASTNode *node, int command_index, MemoryImage *code_img) {
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

    if (valid_addr_mode(command_index, current->adr_mode, 1) == TRUE) {

        set_bit(7 - current->adr_mode, code_img); /** Todo: define number **/
        current = current->next;

        /* if there is second operand */
        if (current != NULL) {
            if (valid_addr_mode(command_index, current->adr_mode, 2) == TRUE) {
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

boolean handle_directive(ASTNode *node, CmpData *cmp_data) {
    int id_start = cmp_data->data.count; /* remember starting address */
    int dir_index;
    dirCommand dir_command;
    boolean had_error;

    /* Find corresponding command_index */
    dir_index = get_dir_index(node->operation);
    if (dir_index == -1){ /* Invalid command_index name */
        set_error(&error, DIRECTIVE_NAME_ERROR, node->location);
        print_error(&error);
        return FALSE;
    }

    /** TODO - if global variables allowed. make function handler */
    dir_command = get_dir_command(dir_index);
    switch (dir_command) {
        case DATA:
            code_data(node, &cmp_data->data);
            break;
        case STRING:
            code_string(node, &cmp_data->data);
            break;
        case ENTRY:

            break;
        case EXTERN:
            if (handle_extern(node, cmp_data) == FALSE) {
                return FALSE;
            }
            break;
    }
    /* insert label if exists */
    if (node->label != NULL){
        if (dir_command == EXTERN || dir_command == ENTRY) {
            printf("WARNING: label before extern/entry is useless"); /* TODO: put in error*/
        } else {
            if (add_label(node, id_start, cmp_data) == FALSE) {
                set_error(&error, MULTIPLE_LABEL, node->location);
                print_error(&error);
            }
        }
    }
    return (error.code == NO_ERROR);
}

boolean handle_extern(ASTNode* node, CmpData* cmpData) {
    OperandNode *current = node->operands;

    while (current) {
        if (insert_label(&cmpData->label_table, current->operand, 0, EXTERNAL) == FALSE) {
            set_error(&error, MULTIPLE_LABEL, node->location);
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


