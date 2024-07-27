/* ---------------------------------------------------------------------------------------
 *                                          Includes
 * --------------------------------------------------------------------------------------- */
#include "first_phase.h"
#include "ast.h"
#include "cmp_data.h"
#include "mappings.h"
#include "boolean.h"
#include "code_convert.h"
#include "label_data.h"
#include "defines.h"

/* ---------------------------------------------------------------------------------------
 *                               Static Functions Prototypes
 * --------------------------------------------------------------------------------------- */
static Boolean handle_instruction(ASTNode *node, CmpData *cmp_data);
static Boolean first_word(ASTNode *node, int command_index, MemoryImage *code_img);
static Boolean code_first_word_addr(int command_index, MemoryImage *code_img, int addr_mode, int offset);
static void code_operands(ASTNode *node, CmpData *cmp_data);
static Boolean handle_directive(ASTNode *node, CmpData *cmp_data);
static Boolean handle_extern(ASTNode* node, CmpData* cmp_data);
static Boolean add_label(ASTNode *node, int address, CmpData *cmp_data);

/* ---------------------------------------------------------------------------------------
 *                              Head Function of First Phase
 * --------------------------------------------------------------------------------------- */
/**
 * The `first_phase_analyzer` function analyzes a parsed line (ASTNode) during the first phase of assembly.
 * It determines the type of line and processes it accordingly, updating the `CmpData` structure.
 *
 * @param node The parsed line represented as an Abstract Syntax Tree (AST) node.
 * @param cmp_data The data structure holding various program-related data during assembly.
 * @return TRUE if the line is processed successfully, FALSE otherwise.
 */
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
/* ---------------------------------------------------------------------------------------
 *                                       Functions
 * --------------------------------------------------------------------------------------- */
/**
 * The `handle_instruction` function processes an instruction line during the first phase of assembly.
 * It validates the number of parameters, encodes the instruction, and updates the `CmpData` structure.
 * The instructions is coded in the code image, stored in the `CmpData` structure.
 *
 * @param node The parsed line represented as an ASTNode.
 * @param cmp_data The data structure holding various program-related data during assembly.
 * @return TRUE if the instruction is processed successfully, FALSE otherwise.
 */
static Boolean handle_instruction(ASTNode *node, CmpData *cmp_data) {
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
    /* The process executed successfully */
    return TRUE;
}

/**
 * The `first_word` function encodes the first word of an instruction into the code memory image.
 * It sets the operation code, addressing modes, and updates the memory image counter.
 *
 * @param node The parsed line represented as an ASTNode.
 * @param command_index The index of the command in the operation table.
 * @param code_img The memory image where the encoded instruction will be stored.
 * @return TRUE if the first word is encoded successfully, FALSE otherwise.
 */
static Boolean first_word(ASTNode *node, int command_index, MemoryImage *code_img) {
    int opr_addr_dest;              /* Address mode of the destination operand */
    int opr_addr_src;                    /* Address mode of the source operand */
    Boolean valid_addr = TRUE; /* Flag indicating if the address mode is valid */
                                            /* number of operands in this node */
    int num_operands = node->specific.instruction.num_operands;

    /* Code the operation name */
    set_int_code(0, 3, command_index, code_img);

    /* ARE default first word's field */
    set_bit(A, 1, code_img);

    /* Check if there is operands to code */
    if (num_operands == 1) {
        opr_addr_dest = node->specific.instruction.operand1.adr_mode;
        valid_addr = code_first_word_addr(command_index, code_img, opr_addr_dest, DEST_OFFSET);
    }
    else if (num_operands == 2) {
        opr_addr_dest = node->specific.instruction.operand2.adr_mode;
        opr_addr_src = node->specific.instruction.operand1.adr_mode;
        valid_addr = code_first_word_addr(command_index, code_img, opr_addr_dest, DEST_OFFSET) &&
                     code_first_word_addr(command_index, code_img, opr_addr_src, SRC_OFFSET);
    }

    /* Update the code image counter */
    updt_memory_image_counter(code_img);
    return valid_addr; /* Return if the first word is valid */
}

/**
 * The `code_first_word_addr` function encodes the addressing mode of an operand into the first word of the instruction.
 *
 * @param command_index The index of the command in the operation table.
 * @param code_img The memory image where the encoded instruction will be stored.
 * @param addr_mode The addressing mode of the operand.
 * @param offset The bit offset for the addressing mode in the instruction word.
 * @return TRUE if the addressing mode is valid and encoded successfully, FALSE otherwise.
 */
static Boolean code_first_word_addr(int command_index, MemoryImage *code_img, int addr_mode, int offset) {
    /* Encode only if the address mode of the operand is valid for the operation */
    if (valid_addr_mode(command_index, addr_mode, 1) == TRUE) {
        set_bit(offset - addr_mode, 1, code_img);
        return TRUE;
    } else {
        return FALSE;
    }
}

/**
 * The `add_label` function adds a label to the label table in the `CmpData` structure.
 * The label's address and type (INSTRUCTION or DIRECTIVE) are also specified.
 *
 * @param node The parsed line represented as an ASTNode.
 * @param address The address of the label.
 * @param cmp_data The data structure holding various program-related data during assembly.
 * @return TRUE if the label is added successfully, FALSE otherwise.
 */
static Boolean add_label(ASTNode *node, int address, CmpData *cmp_data) {
    /* Add label to the label table according to it's type */
    switch (node->lineType) {
        case LINE_INSTRUCTION:
            return insert_single_addr_label(&cmp_data->label_table, node->label, address, INSTRUCTION);
        case LINE_DIRECTIVE:
            return insert_single_addr_label(&cmp_data->label_table, node->label, address, DIRECTIVE);
        default: /* None existing label type - Error */
            return FALSE;
    }
}

/**
 * The `code_operands` function encodes the operands of an instruction into the memory image.
 *
 * @param node The parsed line represented as an ASTNode containing an instruction.
 * @param cmp_data The data structure holding various program-related data during assembly.
 */
static void code_operands(ASTNode *node, CmpData *cmp_data) {
    Boolean reg = FALSE; /* Flag indicating that operand of register type have been encoded */
    InstructionOperand *current_opr;         /* Variable to store current ASTNode's operand */
    int current_addr;                      /* Value to store current operand's address mode */
    int i;                         /* Variable for iterating through the ASTNode's operands */

    /* Iterate through node's operands */
    for (i = 1; i <= node->specific.instruction.num_operands; i++) {
        current_opr = get_operand(node, i); /* Get current operand */
        current_addr = current_opr->adr_mode; /* Get current operand's address mode */

        /* Encode accordingly to the operand's address mode */
        switch (current_addr) {
            case 0: /* Immediate address mode */
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
                code_register_addr_mode(current_opr->value.int_val, &cmp_data->code, REGISTER_POS + (SECOND_REG_POSITION * (i - 1)));
                reg = TRUE;
                break;
        }
        updt_memory_image_counter(&cmp_data->code);
    }
}

/**
 * The `handle_directive` function processes a directive line during the first phase of assembly.
 * It encodes the directive data into the data memory image and updates the `CmpData` structure.
 *
 * @param node The parsed line represented as an ASTNode containing a directive.
 * @param cmp_data The data structure holding various program-related data during assembly.
 * @return TRUE if the directive is processed successfully, FALSE otherwise.
 */
static Boolean handle_directive(ASTNode *node, CmpData *cmp_data) {
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
                set_general_error(MEMORY_ALLOCATION_ERROR); /* TODO memory allocation! */
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

/**
 * The `handle_extern` function processes an EXTERN directive during the first phase of assembly.
 * It adds the external label to the label table in the `CmpData` structure.
 *
 * @param node The parsed line represented as an ASTNode containing an EXTERN directive.
 * @param cmp_data The data structure holding various program-related data during assembly.
 * @return TRUE if the EXTERN directive is processed successfully, FALSE otherwise.
 */
static Boolean handle_extern(ASTNode* node, CmpData* cmp_data) {
    DirNode *current = node->specific.directive.operands; /* DirNode variable to iterate through node's operand */

    while (current) {
        /* Try adding the label to the label table */
        if (insert_single_addr_label(&cmp_data->label_table,current->operand, 0, EXTERNAL) == FALSE) {
            set_error(MULTIPLE_LABEL, node->location);
            return FALSE;
        }
        current = (DirNode *) current->next;
    }

    /* Return the error status - to insure no error occurred during the labels adding process */
    return error_stat() == NO_ERROR;
}


