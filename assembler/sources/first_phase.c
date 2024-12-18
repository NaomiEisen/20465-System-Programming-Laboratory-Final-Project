/* ---------------------------------------------------------------------------------------
 *                                          Includes
 * --------------------------------------------------------------------------------------- */
#include "../../structures/headers/ast.h"
#include "../../structures/headers/cmp_data.h"
#include "../headers/code_convert.h"
/* ---------------------------------------------------------------------------------------
 *                               Static Functions Prototypes
 * --------------------------------------------------------------------------------------- */
static void handle_instruction(ASTNode *node, CmpData *cmp_data);
static Boolean first_word(ASTNode *node, int command_index, MemoryImage *image);
static Boolean code_first_word_addr(int command_index, MemoryImage *code_img, int addr_mode, int offset, int param);
static void code_operands(ASTNode *node, CmpData *cmp_data);
static void handle_directive(ASTNode *node, CmpData *cmp_data);
static void handle_extern(ASTNode* node, CmpData* cmp_data);
static void add_label(ASTNode *node, int address, CmpData *cmp_data);

/* ---------------------------------------------------------------------------------------
 *                              Head Function of First Phase
 * --------------------------------------------------------------------------------------- */
/**
 * The `first_phase_analyzer` function analyzes a parsed line (ASTNode) during the first phase of assembly.
 * It determines the type of line and processes it accordingly with the helper functions.
 *
 * @param node The parsed line represented as an Abstract Syntax Tree (AST) node.
 * @param cmp_data The data structure holding various program-related data during assembly.
 * @return TRUE if the line is processed successfully, FALSE otherwise.
 */
void first_phase_analyzer(ASTNode *node, CmpData *cmp_data) {
    /* Check if line is empty or comment */
    if (node->lineType == LINE_EMPTY || node->lineType == LINE_COMMENT) {
        return;
    }
    /* Handle operation line */
    if (node->lineType == LINE_INSTRUCTION) {
        handle_instruction(node, cmp_data);
    } else { /* Handle directive line */
        handle_directive(node, cmp_data);
    }
}

/* ---------------------------------------------------------------------------------------
 *                                       Functions
 * --------------------------------------------------------------------------------------- */
/**
 * Processes an instruction line during the first phase of assembly.
 * It validates the number of parameters, encodes the instruction, and updates the `CmpData` structure.
 * The instructions is coded in the image image, stored in the `CmpData` structure.
 *
 * @param node The parsed line represented as an ASTNode.
 * @param cmp_data The data structure holding various program-related data during assembly.
 * @return TRUE if the instruction is processed successfully, FALSE otherwise.
 */
static void handle_instruction(ASTNode *node, CmpData *cmp_data) {
    int ic_start = cmp_data->image.code_count; /* remember starting address */
    short command_index = node->specific.instruction.operation;

    /* Validate number of parameters */
    if (get_num_param(command_index) != node->specific.instruction.num_operands) {
        set_error(INVALID_PARAM_NUMBER, node->location);
    }

    /* Code first word if the number of parameters is valid */
    else if (first_word(node, command_index, &cmp_data->image) == FALSE) {
        set_error(INVALID_PARAM_TYPE, node->location);
    }

    /* Code the second/third word */
    code_operands(node, cmp_data);

    /* Insert label if exists */
    if (node->label[0] != '\0' ){
        /* Insert the address with the 100 offset (IC_START) */
        add_label(node, ic_start+IC_START, cmp_data);
    }
}

/**
 * Encodes the first word of an instruction into the image memory image.
 * It sets the operation image, addressing modes, and updates the memory image counter.
 *
 * @param node The parsed line represented as an ASTNode.
 * @param command_index The index of the command in the operation table.
 * @param image The memory image where the encoded instruction will be stored.
 * @return TRUE if the first word is encoded successfully, FALSE otherwise.
 */
static Boolean first_word(ASTNode *node, int command_index, MemoryImage *image) {
    int opr_addr_dest;              /* Address mode of the destination operand */
    int opr_addr_src;                    /* Address mode of the source operand */
    Boolean valid_addr = TRUE; /* Flag indicating if the address mode is valid */
                                            /* number of operands in this node */
    int num_operands = node->specific.instruction.num_operands;

    /* Code the operation name */
    set_int_code(0, 3, command_index, image, CODE_IMAGE);

    /* ARE default first word's field */
    set_bit(A, 1, image, CODE_IMAGE);

    /* Check if there is operands to code */
    if (num_operands == 1) {
        opr_addr_dest = node->specific.instruction.operand1.adr_mode;
        valid_addr = code_first_word_addr(command_index, image, opr_addr_dest, DEST_OFFSET, 1);
    }
    else if (num_operands == 2) {
        opr_addr_dest = node->specific.instruction.operand2.adr_mode;
        opr_addr_src = node->specific.instruction.operand1.adr_mode;
        valid_addr = code_first_word_addr(command_index, image, opr_addr_dest, DEST_OFFSET, 2) &&
                     code_first_word_addr(command_index, image, opr_addr_src, SRC_OFFSET, 1);
    }

    /* Update the code image counter */
    updt_code_counter(image);
    return valid_addr; /* Return if the first word is valid */
}

/**
 * Encodes the addressing mode of an operand into the first word
 * of the instruction.
 *
 * @param command_index The index of the command in the operation table.
 * @param code_img The memory image where the encoded instruction will be stored.
 * @param addr_mode The addressing mode of the operand.
 * @param offset The bit offset for the operand in the encoded word.
 * @param param The param order from the read line.
 * @return TRUE if the addressing mode is valid and encoded successfully, FALSE otherwise.
 */
static Boolean code_first_word_addr(int command_index, MemoryImage *code_img, int addr_mode, int offset, int param) {
    /* Encode only if the address mode of the operand is valid for the operation */
    if (valid_addr_mode(command_index, addr_mode, param) == TRUE) {
        set_bit(offset - addr_mode, 1, code_img, CODE_IMAGE);
        return TRUE;
    } else return FALSE;
}

/**
 * Adds a label to the label table in the `CmpData` structure.
 * The label's address and type (INSTRUCTION or DIRECTIVE) are also specified.
 *
 * @param node The parsed line represented as an ASTNode.
 * @param address The address of the label.
 * @param cmp_data The data structure holding various program-related data during assembly.
 * @return TRUE if the label is added successfully, FALSE otherwise.
 */
static void add_label(ASTNode *node, int address, CmpData *cmp_data) {
    ErrorCode insert_status;

    /* Check if label already defined */
    if ((get_label_addr(&cmp_data->label_table,node->label) >= 0)) {
        set_error(LABEL_DUPLICATE, node->location);
        return;
    }

    /* Add label to the label table according to it's type */
    switch (node->lineType) {
        case LINE_INSTRUCTION:
            insert_status = insert_label(&cmp_data->label_table, node->label, address, INSTRUCTION);
            break;
        case LINE_DIRECTIVE:
            insert_status = insert_label(&cmp_data->label_table, node->label, address, DIRECTIVE);
            break;
        default: /* Invalid line type */
            return;
    }

    /* Check for errors */
    switch (insert_status) {
        case MEMORY_ALLOCATION_ERROR:
            set_general_error(MEMORY_ALLOCATION_ERROR);
            break;
        case INVALID_CHAR:
            set_error(INVALID_CHAR_LABEL, node->location);
            break;
        case DUPLICATE:
            set_error(LABEL_DUPLICATE, node->location);
        default:
            break;
    }
}

/**
 * Encodes the operands of an instruction into the memory image.
 *
 * @param node The parsed line represented as an ASTNode containing an instruction.
 * @param cmp_data The data structure holding various program-related data during assembly.
 */
static void code_operands(ASTNode *node, CmpData *cmp_data) {
    Boolean reg = FALSE; /* Flag indicating that operand of register type have been encoded */
    InstructionOperand *current_opr;         /* Variable to store current ASTNode's operand */
    int current_addr;                      /* Value to store current operand's address mode */
    short i;                       /* Variable for iterating through the ASTNode's operands */

    /* Iterate through node's operands */
    for (i = 1; i <= node->specific.instruction.num_operands; i++) {
        current_opr = get_operand(node, i); /* Get current operand */
        current_addr = current_opr->adr_mode;    /* Get current operand's address mode */

        /* Encode accordingly to the operand's address mode */
        switch (current_addr) {
            /* ------------------------ Immediate address mode ------------------------ */
            case ADDR_MODE_IMMEDIATE:
                code_immediate_addr_mode(current_opr->value.int_val, &cmp_data->image, node);
                break;
            /* -------------------------- Direct address mode -------------------------- */
            case ADDR_MODE_DIRECT:
                /* Don't code labels in the first pass - mark them for the second phase */
                mark_word(&cmp_data->image);
                if (add_unresolved_line(cmp_data, node->location.line) == FALSE) {
                    set_general_error(MEMORY_ALLOCATION_ERROR);
                    return;
                }
                break;
            /* ----------------- Direct/Indirect register address mode ---------------- */
            case ADDR_INDIRECT_REG:
            case ADDR_DIRECT_REG:
                /* If there is only one register operand - it is the target register */
                if (node->specific.instruction.num_operands == 1) i++;

                /* Two register operands can be written in a single word */
                else if (reg == TRUE) {
                    /* write on the previous word */
                    seek_back(&cmp_data->image);
                }

                /* Code register operand */
                code_register_addr_mode(current_opr->value.int_val, &cmp_data->image,
                                        REGISTER_POS + (SECOND_REG_POSITION * (i - 1)));
                reg = TRUE; /* Set flag to indicate that the encoded operand was a register */
                break;

            default: break;
        }
        /* Update the memory image counter after each written operand  */
        updt_code_counter(&cmp_data->image);
    }
}

/**
 * Processes a directive line during the first phase of assembly.
 * It encodes the directive data into the data memory image and updates the `CmpData` structure.
 *
 * @param node The parsed line represented as an ASTNode containing a directive.
 * @param cmp_data The data structure holding various program-related data during assembly.
 * @return TRUE if the directive is processed successfully, FALSE otherwise.
 */
static void handle_directive(ASTNode *node, CmpData *cmp_data) {
    int id_start = cmp_data->image.data_count; /* remember starting address */

    switch (node->specific.directive.operation) {
        case DATA: /* Data directive */
            code_data(node, &cmp_data->image);
            break;
        case STRING: /* String directive */
            code_string(node, &cmp_data->image);
            break;
        case ENTRY: /* Entry directive will be handled in the second phase */
            if (add_unresolved_line(cmp_data, node->location.line) == FALSE) {
                set_general_error(MEMORY_ALLOCATION_ERROR);
                return;
            } break;
        case EXTERN: /* Extern directive */
            handle_extern(node, cmp_data);
            break;
    }

    /* insert label if exists */
    if (node->label[0] != '\0'){
        /* Label before extern/entry directive is useless */
        if (node->specific.directive.operation == EXTERN){
            /* Print warning - label ignored */
            print_warning(LABEL_EXTERN, &node->location);
        }
        else if (node->specific.directive.operation == ENTRY) {
            /* Print warning - label ignored */
            print_warning(LABEL_ENTRY, &node->location);
        } else {add_label(node, id_start, cmp_data);}
    }
}

/**
 * Processes an EXTERN directive during the first phase of assembly.
 * It adds the external label to the label table in the `CmpData` structure.
 *
 * @param node The parsed line represented as an ASTNode containing an EXTERN directive.
 * @param cmp_data The data structure holding various program-related data during assembly.
 * @return TRUE if the EXTERN directive is processed successfully, FALSE otherwise.
 */
static void handle_extern(ASTNode* node, CmpData* cmp_data) {
    /* DirNode variable to iterate through the parameters list */
    DirNode *current = node->specific.directive.operands;

    while (current) {
        /* Try adding the label to the label table */
        switch (insert_label(&cmp_data->label_table, current->operand, 0, EXTERNAL)) {
            /* Check for errors */
            case MEMORY_ALLOCATION_ERROR:
                set_general_error(MEMORY_ALLOCATION_ERROR);
                break;

            case INVALID_CHAR:
                set_error(INVALID_CHAR_LABEL, node->location);
                break;

            case DUPLICATE:
                /* The same label has been declared 'extern' twice */
                if (get_label_type(&cmp_data->label_table,current->operand) == EXTERNAL) {
                    /* Print warning and ignore this label */
                    print_warning(EXTERN_DUPLICATE, &node->location);
                } else {
                    /* Duplicate label definition */
                    set_error(LABEL_DUPLICATE, node->location);
                }
                break;

            default: /* Process executed successfully */
                break;
        }
        current = (DirNode *) current->next;
    }
}