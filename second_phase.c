#include "second_phase.h"
#include "boolean.h"
#include "ast.h"
#include "cmp_data.h"
#include "label_data.h"
#include "code_convert.h"

boolean second_phase_analyzer(ASTNode *node, CmpData *cmp_data) {
    /* Handle operation line */
    if (node->lineType == LINE_INSTRUCTION) {
        return code_label_operands(node, cmp_data);
    }
    /* Handle directive line */
    return handle_entry(node, cmp_data);
}

boolean code_label_operands(ASTNode *node, CmpData *cmp_data) {
    InstructionOperand *current_opr;
    int i;
    int line;

    for (i = 1; i <= node->specifics.instruction.num_operands; i++) {
        current_opr = get_operand(node, i);
        if (current_opr->adr_mode == ADDR_MODE_DIRECT) {
            line = get_marked_line(&cmp_data->code);
            if (line != -1) {
                unmark_word(&cmp_data->code, line);
                if (code_direct_addr_mode(current_opr->value.char_val, cmp_data, line) == FALSE) {
                    set_error(UNRECOGNIZED_LABEL, node->location);
                    print_error();
                    return FALSE;
                }
            }
        }
    }

    return TRUE;
}

boolean handle_entry(ASTNode *node, CmpData *cmp_data){
    DirNode *current = node->specifics.directive.operands;

    while (current) {
        if (set_label_type(&cmp_data->label_table, current->operand, ENTERNAL) == FALSE){
            set_error(UNRECOGNIZED_LABEL, node->location);
            print_error();
            break;
        }
        current = (DirNode *) current->next;
    }
    return error_stat() == NO_ERROR;
}