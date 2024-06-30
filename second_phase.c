#include "second_phase.h"
#include "boolean.h"
#include "ast.h"
#include "cmp_data.h"
#include "label_data.h"
#include "code_convert.h"
#include "output_files.h"

Boolean second_phase_analyzer(ASTNode *node, CmpData *cmp_data) {
    /* Handle operation line */
    if (node->lineType == LINE_INSTRUCTION) {
        return code_label_operands(node, cmp_data);
    }
    /* Handle directive line */
    return handle_entry(node, cmp_data);
}

Boolean code_label_operands(ASTNode *node, CmpData *cmp_data) {
    InstructionOperand *current_opr;
    int i;
    int line;

    for (i = 1; i <= node->specific.instruction.num_operands; i++) {
        current_opr = get_operand(node, i);
        if (current_opr->adr_mode == ADDR_MODE_DIRECT) {
            line = get_marked_line(&cmp_data->code);
            if (line != -1) {
                unmark_word(&cmp_data->code, line);
                cmp_data->code.write_ptr = line; /* Set writer to the relevant address */
                if (code_direct_addr_mode(current_opr->value.char_val, cmp_data) == FALSE) {
                    set_error(UNRECOGNIZED_LABEL, node->location);
                    return FALSE;
                }
            }
        }
    }

    return TRUE;
}

Boolean handle_entry(ASTNode *node, CmpData *cmp_data){
    DirNode *current = node->specific.directive.operands;
    int address;

    while (current) {
        /** todo: maybe you don't need ? */
        if (set_label_type(&cmp_data->label_table, current->operand, ENTERNAL) == FALSE){
            set_error(UNRECOGNIZED_LABEL, node->location);
            break;
        }
        address = get_label_single_addr(&cmp_data->label_table, current->operand);
        write_label(current->operand, address, cmp_data->entry_file.file);
        current = (DirNode *) current->next;
    }
    return error_stat() == NO_ERROR;
}