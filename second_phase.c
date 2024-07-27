/* ---------------------------------------------------------------------------------------
 *                                          Includes
 * --------------------------------------------------------------------------------------- */
#include "boolean.h"
#include "ast.h"
#include "cmp_data.h"
#include "label_data.h"
#include "code_convert.h"
#include "output_files.h"
/* ---------------------------------------------------------------------------------------
 *                               Static Functions Prototypes
 * --------------------------------------------------------------------------------------- */
static Boolean code_label_operands(ASTNode *node, CmpData *cmp_data);
static Boolean handle_entry(ASTNode *node, CmpData *cmp_data);

/* ---------------------------------------------------------------------------------------
 *                                Head Function Of Second Phase
 * --------------------------------------------------------------------------------------- */
/**
 * The `second_phase_analyzer` function processes an unresolved line during the second phase of assembly.
 * It handles encoding of label operand's in instruction lines and entry directives.
 *
 * @param node The parsed line represented as an ASTNode containing an instruction or directive.
 * @param cmp_data The data structure holding various program-related data during assembly.
 * @return TRUE if the line is processed successfully, FALSE otherwise.
 */
Boolean second_phase_analyzer(ASTNode *node, CmpData *cmp_data) {
    /* Handle operation line */
    if (node->lineType == LINE_INSTRUCTION) {
        return code_label_operands(node, cmp_data);
    }
    /* Handle directive line */
    return handle_entry(node, cmp_data);
}

/* ---------------------------------------------------------------------------------------
 *                                         Functions
 * --------------------------------------------------------------------------------------- */
/**
 * The `code_label_operands` function encodes the label operands of an instruction during the second
 * phase of assembly. It resolves the addresses of labels used in direct addressing mode.
 *
 * @param node The parsed line represented as an ASTNode containing the instruction and its operands.
 * @param cmp_data The data structure holding various program-related data during assembly.
 * @return TRUE if the operands are processed and encoded successfully, FALSE otherwise.
 */
static Boolean code_label_operands(ASTNode *node, CmpData *cmp_data) {
    InstructionOperand *current_opr; /* Variable to store the current operand */
    int i;                       /* Variable to iterate through nodes operand */
    int line;            /* Variable to store the consecutive unresolved line */

    /* Iterate through node's operands */
    for (i = 1; i <= node->specific.instruction.num_operands; i++) {
        /* Get the current operand */
        current_opr = get_operand(node, i);

        /* If this is operand of type label - encode */
        if (current_opr->adr_mode == ADDR_MODE_DIRECT) {
            /* Get the unresolved line */
            line = get_marked_line(&cmp_data->code);

            if (line != -1) { /* If line exists */
                unmark_word(&cmp_data->code, line); /* Unmark the line */
                cmp_data->code.write_ptr = line; /* Set writer to the relevant address */

                /* Encode the label */
                if (code_direct_addr_mode(current_opr->value.char_val, cmp_data) == FALSE) {
                    /* If an error occurred - return FALSE */
                    set_error(UNRECOGNIZED_LABEL, node->location);
                    return FALSE;
                }
            }
        }
    }
    /* Process executed successfully */
    return TRUE;
}

/**
 * The `handle_entry` function processes an entry directive during the second phase of assembly.
 * It resolves the addresses of entry labels and writes them to the entry file.
 *
 * @param node The parsed line represented as an ASTNode containing the entry directive and its operands.
 * @param cmp_data The data structure holding various program-related data during assembly.
 * @return TRUE if the entry labels are processed and resolved successfully, FALSE otherwise.
 */
static Boolean handle_entry(ASTNode *node, CmpData *cmp_data){
    DirNode *current = node->specific.directive.operands;  /* Variable to store the current operand */
    int address;                                     /* Variable to store the current label address */

    while (current) {
        /** todo: maybe you don't need ? */
        if (set_label_type(&cmp_data->label_table, current->operand, ENTERNAL) == FALSE){
            set_error(UNRECOGNIZED_LABEL, node->location);
            break;
        }
        /* Get the entry label address */
        address = get_label_single_addr(&cmp_data->label_table, current->operand);
        /* Write the data in the entry file */
        write_label(current->operand, address, cmp_data->entry_file.file);
        /* Get the next operand */
        current = (DirNode *) current->next;
    }
    /* Return error status, indicating whether the whole process executed successfully */
    return error_stat() == NO_ERROR;
}