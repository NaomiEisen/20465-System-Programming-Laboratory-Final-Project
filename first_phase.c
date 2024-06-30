#include "first_phase.h"
#include "ast.h"
#include "cmp_data.h"
#include "mappings.h"
#include "boolean.h"
#include "code_convert.h"
#include "label_data.h"


Boolean first_phase_analyzer(ASTNode *node, CmpData *cmp_data) {
    /* Check if line is empty or comment */
    if (node->lineType == LINE_EMPTY || node->lineType == LINE_COMMENT) {
        return TRUE;
    }
    /* Handle operation line */
    if (node->lineType == LINE_INSTRUCTION) {
        return handle_instruction(node, cmp_data);
    }
    /* Handle directive line */
    return handle_directive(node, cmp_data);
}

Boolean handle_instruction(ASTNode *node, CmpData *cmp_data) {
    int ic_start = cmp_data->code.count; /* remember starting address */
    int command_index = node->specific.instruction.operation;

    /* Validate number of parameters */
    if (get_num_param(command_index) != node->specific.instruction.num_operands) {
        set_error(INVALID_PARAM_NUMBER, node->location);
        return FALSE;
    }

    /* Code first word */
    if (first_word(node, command_index, &cmp_data->code) == FALSE) {
        set_error(INVALID_PARAM_TYPE, node->location);
        return FALSE;
    }

    /* Code the second/third word */
    code_operands(node, cmp_data);

    /* insert label if exists */
    if (node->label[0] != '\0' ){
        if( add_label(node, ic_start+IC_START, cmp_data) == FALSE) {
            set_error(MULTIPLE_LABEL, node->location);
            return FALSE;
        }
    }

    return TRUE;
}

Boolean first_word(ASTNode *node, int command_index, MemoryImage *code_img) {
    int opr_addr_dest;
    int opr_addr_src;
    int num_operands = node->specific.instruction.num_operands;
    Boolean valid_addr;

    /* Code the operation name */
    set_int_code(0, 3, command_index, code_img);

    /* ARE default first word's field */
    set_bit(A, 1, code_img);

    /* Check if there is operands to code */
    if (num_operands == 1) {
        opr_addr_dest = node->specific.instruction.operand1.adr_mode;
        valid_addr = code_first_word_addr(command_index, code_img, opr_addr_dest, DEST_OFFSET);
        updt_memory_image_counter(code_img);
        return valid_addr;
    }
    else if (num_operands == 2) {
        opr_addr_dest = node->specific.instruction.operand2.adr_mode;
        opr_addr_src = node->specific.instruction.operand1.adr_mode;
        valid_addr = code_first_word_addr(command_index, code_img, opr_addr_dest, DEST_OFFSET) &&
                     code_first_word_addr(command_index, code_img, opr_addr_src, SRC_OFFSET);
        updt_memory_image_counter(code_img);
        return valid_addr;
    }
    return TRUE;
}

Boolean code_first_word_addr(int command_index, MemoryImage *code_img, int addr_mode, int offset) {
    if (valid_addr_mode(command_index, addr_mode, 1) == TRUE) {
        set_bit(offset - addr_mode, 1, code_img);
        return TRUE;
    } else {
        return FALSE;
    }
}

Boolean add_label(ASTNode *node, int address, CmpData *cmp_data) {
    switch (node->lineType) {
        case LINE_INSTRUCTION:
            return insert_single_addr_label(&cmp_data->label_table, node->label, address, INSTRUCTION);
        case LINE_DIRECTIVE:
            return insert_single_addr_label(&cmp_data->label_table, node->label, address, DIRECTIVE);
        default:
            return FALSE;
    }
}


void code_operands(ASTNode *node, CmpData *cmp_data) {
    Boolean reg = FALSE;
    InstructionOperand *current_opr;
    int current_addr;
    int i;

    for (i = 1; i <= node->specific.instruction.num_operands; i++) {
        current_opr = get_operand(node, i);
        current_addr = current_opr->adr_mode;

        switch (current_addr) {
            case 0:
                code_immediate_addr_mode(current_opr->value.int_val, &cmp_data->code);
                break;
            case 1:
                /* Don't code label in the first phase */
                mark_word(&cmp_data->code);
                if (add_unresolved_line(cmp_data, node->location.line) == FALSE) {
                    set_general_error(MEMORY_ALLOCATION_ERROR);
                    return;
                }
                break;
            case 2:
            case 3:
                if (reg == TRUE) {
                    /* write on the previous word */
                    cmp_data->code.count--;
                    cmp_data->code.write_ptr = cmp_data->code.count;
                }
                /** todo fix this number */
                code_register_addr_mode(current_opr->value.int_val, &cmp_data->code, REGISTER_POS + (3 * (i - 1)));
                reg = TRUE;
                break;
        }
        updt_memory_image_counter(&cmp_data->code);
    }
}


Boolean handle_directive(ASTNode *node, CmpData *cmp_data) {
    int id_start = cmp_data->data.count; /* remember starting address */

    switch (node->specific.directive.operation) {
        case DATA:
            code_data(node, &cmp_data->data);
            /*updt_memory_image_counter(&cmp_data->data);*/
            break;
        case STRING:
            code_string(node, &cmp_data->data);
            /*updt_memory_image_counter(&cmp_data->data);*/
            break;
        case ENTRY:
            if (add_unresolved_line(cmp_data, node->location.line) == FALSE) {
                set_general_error(MEMORY_ALLOCATION_ERROR);
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
        if (node->specific.directive.operation == EXTERN ||
        node->specific.directive.operation == ENTRY) {
            print_warning();
        } else {
            if (add_label(node, id_start, cmp_data) == FALSE) {
                set_error(MULTIPLE_LABEL, node->location);
            }
        }
    }
    return (error_stat() == NO_ERROR);
}

Boolean handle_extern(ASTNode* node, CmpData* cmp_data) {
    DirNode *current = node->specific.directive.operands;

    while (current) {
        if (insert_single_addr_label(&cmp_data->label_table,current->operand, 0, EXTERNAL) == FALSE) {
            set_error(MULTIPLE_LABEL, node->location);
            return FALSE;
        }
        current = (DirNode *) current->next;
    }

    return error_stat() == NO_ERROR;
}


