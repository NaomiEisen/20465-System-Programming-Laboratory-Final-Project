#include "first_phase.h"
#include "ast.h"
#include "cmp_data.h"
#include "mappings.h"
#include "boolean.h"
#include "code_convert.h"
#include "label_data.h"


boolean first_phase_analyzer(ASTNode *node, CmpData *cmp_data) {
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

boolean first_word(ASTNode *node, int command_index, MemoryImage *code_img) {
    int opr_addr;
    int num_operands = node->specifics.instruction.num_operands;

    /* Code the operation name */
    set_int_code(0, 3, command_index, code_img);

    /* ARE default first word's field */
    set_bit(A, 1, code_img);


    /* Check if there is operands to code */
    if (num_operands > 0) {
        opr_addr = node->specifics.instruction.operand1.adr_mode;
        if (valid_addr_mode(command_index, opr_addr, 1) == TRUE) {
            set_bit(OPERAND1_OFFSET - opr_addr, 1, code_img);
            num_operands--;
            if (num_operands > 0) {
                opr_addr = node->specifics.instruction.operand2.adr_mode;
                if (valid_addr_mode(command_index, opr_addr, 2) == TRUE) {
                    set_bit(OPERAND2_OFFSET - opr_addr, 1, code_img);
                } else {
                    return FALSE;
                }
            }
        } else {
            return FALSE;
        }
    }
    code_img->count++;
    return TRUE;
}

boolean add_label(ASTNode *node, int address, CmpData *cmp_data) {
    switch (node->lineType) {
        case LINE_INSTRUCTION:
            return insert_single_addr_label(&cmp_data->label_table, node->label, address, OPERATION);
        case LINE_DIRECTIVE:
            return insert_single_addr_label(&cmp_data->label_table, node->label, address, DIRECTIVE);
        default:
            return FALSE;
    }
}


void code_operands(ASTNode *node, CmpData *cmp_data) {
    boolean reg = FALSE;
    InstructionOperand *current_opr;
    int current_addr;
    int i;

    for (i = 1; i <= node->specifics.instruction.num_operands; i++) {
        current_opr = get_operand(node, i);
        current_addr = current_opr->adr_mode;

        switch (current_addr) {
            case 0:
                code_immediate_addr_mode(current_opr->value.int_val, &cmp_data->code);
                break;
            case 1:
                /* Don't code label in the first phase */
                mark_word(&cmp_data->code);
                cmp_data->code.count++;
                if (add_unresolved_line(cmp_data, node->location.line) == FALSE) {
                    set_general_error(MEMORY_ALLOCATION_ERROR);
                    print_error();
                    return;
                }
                break;
            case 2:
            case 3:
                if (reg == TRUE) {
                    cmp_data->code.count--; /* write on the previous word */
                }
                /** todo fix this number */
                code_register_addr_mode(current_opr->value.int_val, &cmp_data->code, REGISTER_POS + (3 * (i - 1)));
                reg = TRUE;
                break;
        }
        cmp_data->code.count++;

    }
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
            if (add_unresolved_line(cmp_data, node->location.line) == FALSE) {
                set_general_error(MEMORY_ALLOCATION_ERROR);
                print_error();
                return FALSE;
            }
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
            print_warning();
        } else {
            if (add_label(node, id_start, cmp_data) == FALSE) {
                set_error(MULTIPLE_LABEL, node->location);
                print_error();
            }
        }
    }
    return (error_stat() == NO_ERROR);
}

boolean handle_extern(ASTNode* node, CmpData* cmp_data) {
    DirNode *current = node->specifics.directive.operands;

    while (current) {
        if (insert_list_addr_label(&cmp_data->label_table, current->operand) == FALSE) {
            set_error(MULTIPLE_LABEL, node->location);
            print_error();
            break;
        }
        current = (DirNode *) current->next;
    }

    return error_stat() == NO_ERROR;
}


