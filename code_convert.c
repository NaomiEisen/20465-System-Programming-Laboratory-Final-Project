#include "code_convert.h"
#include "ast.h"
#include "errors.h"
#include "cmp_data.h"
#include "utils.h"
#include "semantic_analyzer.h"


/* Function to set multiple bits from an integer */
void set_int_code(int start, int end, int value, MemoryImage *memory_img) {
    int i;
    int bit_per_line = NUM_OF_BYTES * BYTE_SIZE;

    if (end - start + 1 > bit_per_line) {
        return;
    }

    for (i = 0; i <= end - start; i++) {
        if (value & (1 << i)) {
            set_bit(start + i, memory_img);
        }
    }
}


/**
 * Function that sets the bit at the specified index to 1 in the relevant memory image.
 * @param i
 * @param memory_img
 */
void set_bit(int i, MemoryImage *memory_img) {
    char mask;
    int byteIndex = i / BYTE_SIZE;
    int bitOffset = i % BYTE_SIZE;

    if (byteIndex >= NUM_OF_BYTES) {
        return;
    }

    mask = (char)(1 << bitOffset);

    if (memory_img->count < MEMORY_CAPACITY) {
        memory_img->lines[memory_img->count][byteIndex] |= mask;
    } else {
        set_general_error(&global_error, CPU_MEMORY_FULL);
    }

}

void code_immediate_addr_mode (OperandNode *operand, MemoryImage *memory_img) {
    int end = IMMIDIATE_DIRECTIVE_BIT_SIZE-1;
    set_int_code(0, end, my_atoi(operand->operand), memory_img);
    set_bit(A, memory_img);
}

void code_direct_addr_mode (OperandNode *operand, CmpData *cmp_data) {
    LabelType label_type;
    int address = search_label(&cmp_data->label_table, operand->operand, &label_type);
    int end = IMMIDIATE_DIRECTIVE_BIT_SIZE-1;

    if (address != -1) {
        set_int_code(0 , end, address, &cmp_data->code);
    }

    if (label_type == EXTERNAL) {
        set_bit(E, &cmp_data->code);
    } else {
        set_bit(R, &cmp_data->code);
    }
}

void code_register_addr_mode(OperandNode *operand, MemoryImage *memory_img, int position) {
    int end = position+REGISTER_BIT_SIZE-1;

    set_int_code(position, end, my_atoi(operand->operand), memory_img);
    set_bit(A, memory_img);
}

