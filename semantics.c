#include "ast.h"
#include "cmp_data.h"
#include "mappings.h"
#include "boolean.h"
#include "code_convert.h"
#include "semantic.h"
#include "label_data.h"


boolean analyzeLine(ASTNode *node, CmpData *cmp_data) {

    /* Check if line is empty or comment */
    if (node->lineType == LINE_EMPTY || node->lineType == LINE_COMMENT) {
        return TRUE;
    }

    /* Handle operation line */
    if (node->lineType == LINE_INSTRUCTION) {
        return handle_operation(node, cmp_data);
    }

    /* Handle directive line */
    return handle_directive(node, cmp_data);
}

boolean handle_operation(ASTNode *node, CmpData *cmp_data) {
    int ic_start = cmp_data->code.count; /* remember starting address */
    int command_index = node->specifics.instruction.operation;

    /* Validate number of parameters */
    if (get_num_param(command_index) != node->specifics.instruction.num_operands) {
        set_error(INVALID_PARAM_NUMBER, node->location);
        print_error();
        return FALSE;
    }

    /* Code first word */
    if (first_word(node, command_index, &cmp_data->code) == FALSE) {
        set_error(INVALID_PARAM_TYPE, node->location);
        print_error();
        return FALSE;
    }

    /* Code the second/third word */
    code_operands(node, cmp_data);

    /* insert label if exists */
    if (node->label[0] != '\0' ){
        if( add_label(node, ic_start, cmp_data) == FALSE) {
            set_error(MULTIPLE_LABEL, node->location);
            print_error();
            return FALSE;
        }
    }

    return TRUE;
}


boolean add_label(ASTNode *node, int address, CmpData *cmp_data) {
    switch (node->lineType) {
        case LINE_INSTRUCTION:
            return insert_label(&cmp_data->label_table, node->label, address, OPERATION);
        case LINE_DIRECTIVE:
            return insert_label(&cmp_data->label_table, node->label, address, DIRECTIVE);
        default:
            return FALSE;
    }
}



void code_operands(ASTNode *node, CmpData *cmp_data) {
    boolean reg = FALSE;
    int counter = 0;
    int current_opr_num = 1;
    InstructionOperand* current_opr;
    int current_addr;

    if (node->specifics.instruction.num_operands < 1 ) {
        return;
    }

    while (current_opr_num > node->specifics.instruction.num_operands) {
        current_opr = get_operand(node, current_opr_num);
        current_addr = current_opr->adr_mode;

        switch (current_addr) {
            case 0: code_immediate_addr_mode(current_opr->value.int_val, &cmp_data->code);
                break;
            case 1: code_direct_addr_mode(current_opr->value.char_val,cmp_data);
                break;
            case 2:
            case 3:
                if (reg == TRUE) {
                    cmp_data->code.count--; /* write on the previous word */
                }
                /** todo fix this number */
                code_register_addr_mode(current_opr->value.int_val, &cmp_data->code,REGISTER_POS + (3 * counter));
                reg = TRUE;
                break;
        }
        counter++;
        cmp_data->code.count++;
    }
}


boolean first_word(ASTNode *node, int command_index, MemoryImage *code_img) {
    int opr_addr;

    /* Code the operation name */
    set_int_code(0, 3, command_index, code_img);

    /* ARE default first word's field */
    set_bit(A, code_img);

    /* Check if there is operands to code */
    if ( node->specifics.instruction.num_operands == 0) {
        code_img->count++;
        return TRUE;
    }

    /* Code first operand's address mode */
    opr_addr = node->specifics.instruction.operand1.adr_mode;
    if (valid_addr_mode(command_index, opr_addr, 1) == TRUE) {

        set_bit(7 - opr_addr, code_img); /** Todo: define number **/

        /* if there is second operand */
        if (node->specifics.instruction.num_operands == 2) {
            opr_addr = node->specifics.instruction.operand2.adr_mode;
            if (valid_addr_mode(command_index, opr_addr, 2) == TRUE) {
                set_bit(11 - opr_addr, code_img); /** Todo: define number **/
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

    switch (node->specifics.directive.operation) {
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
    if (node->label[0] != '\0'){
        if (node->specifics.directive.operation == EXTERN ||
        node->specifics.directive.operation == ENTRY) {
            printf("WARNING: label before extern/entry is useless"); /* TODO: put in error*/
        } else {
            if (add_label(node, id_start, cmp_data) == FALSE) {
                set_error(MULTIPLE_LABEL, node->location);
                print_error();
            }
        }
    }
    return (error_stat() == NO_ERROR);
}

boolean handle_extern(ASTNode* node, CmpData* cmpData) {
    DirNode *current = node->specifics.directive.operands;

    while (current) {
        if (insert_label(&cmpData->label_table, current->operand, 0, EXTERNAL) == FALSE) {
            set_error(MULTIPLE_LABEL, node->location);
            break;
        }
        current = (DirNode *) current->next;
    }
    if (error_stat() != NO_ERROR) {
        print_error();
        return FALSE;

    } else {
        return TRUE;
    }
}


