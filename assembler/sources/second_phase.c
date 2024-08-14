/* ---------------------------------------------------------------------------------------
 *                                          Includes
 * --------------------------------------------------------------------------------------- */
#include "../../structures/headers/ast.h"
#include "../../structures/headers/cmp_data.h"
#include "../headers/code_convert.h"
#include "../../utils/headers/output_files.h"
/* ---------------------------------------------------------------------------------------
 *                               Static Functions Prototypes
 * --------------------------------------------------------------------------------------- */
static void code_label_operands(ASTNode *node, CmpData *cmp_data);
static void handle_entry(ASTNode *node, CmpData *cmp_data);

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
void second_phase_analyzer(ASTNode *node, CmpData *cmp_data) {
    /* Handle operation line */
    if (node->lineType == LINE_INSTRUCTION) {
        code_label_operands(node, cmp_data);
    } else { /* Handle directive line */
        handle_entry(node, cmp_data);
    }
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
static void code_label_operands(ASTNode *node, CmpData *cmp_data) {
    InstructionOperand *current_opr; /* Variable to store the current operand */
    short i;                     /* Variable to iterate through nodes operand */
    int line;            /* Variable to store the consecutive unresolved line */

    /* Iterate through node's operands */
    for (i = 1; i <= node->specific.instruction.num_operands; i++) {
        /* Get the current operand */
        current_opr = get_operand(node, i);

        /* If this is operand of type label - encode */
        if (current_opr->adr_mode == ADDR_MODE_DIRECT) {
            /* Get the unresolved line */
            line = get_marked_line(&cmp_data->image);

            if (line != -1) { /* If line exists */
                unmark_word(&cmp_data->image, line); /* Unmark the line */
                cmp_data->image.code_pos = line; /* Set writer to the relevant address */

                /* Encode the label */
                if (code_direct_addr_mode(current_opr->value.char_val, cmp_data) == FALSE) {
                    /* If an error occurred - return FALSE */
                    set_error(UNRECOGNIZED_LABEL, node->location);
                    return;
                }
            }
        }
    }
}

/**
 * The `handle_entry` function processes an entry directive during the second phase of assembly.
 * It resolves the addresses of entry labels and writes them to the entry file.
 *
 * @param node The parsed line represented as an ASTNode containing the entry directive and its operands.
 * @param cmp_data The data structure holding various program-related data during assembly.
 * @return TRUE if the entry labels are processed and resolved successfully, FALSE otherwise.
 */
static void handle_entry(ASTNode *node, CmpData *cmp_data){
    /* Variable to store the current operand */
    DirNode *current = node->specific.directive.operands;
    /* Variable to store the current label type */
    LabelType type = get_label_type(&cmp_data->label_table, current->operand);
    int address; /* Variable to store the current label address */

    while (current) {
        /* Label was already set as entry */
        if (type == ENTERNAL) {
            /* Print warning that this line will be ignored */
            print_warning(ENTRY_DUPLICATE, &node->location);
            continue;
        }

        if (type == EXTERNAL) {
            set_error(UNRECOGNIZED_LABEL, node->location);
            break;
        }

        /* Set label type to ENTERNAL */
        if (set_label_type(&cmp_data->label_table, current->operand, ENTERNAL) == FALSE){
            set_error(UNRECOGNIZED_LABEL, node->location);
            break;
        }

        /* Get the entry label address */
        address = get_label_addr(&cmp_data->label_table, current->operand);
        /* Write the data in the entry file */
        write_label(current->operand, address, cmp_data->entry_file.file);
        /* Get the next operand */
        current = (DirNode *) current->next;
    }

    cmp_data->entry_file.delete = FALSE; /* Set flag to false - non-empty file should not be deleted */
}