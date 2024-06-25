#include "code_convert.h"
#include "ast.h"
#include "errors.h"
#include "cmp_data.h"
#include "utils.h"
#include "first_phase.h"


/* Function to set multiple bits from an integer */
void set_int_code(int start, int end, int value, MemoryImage *memory_img) {
    int i;
    int bit_per_line = NUM_OF_BYTES * BYTE_SIZE;


    /* Ensure the range is within a single line's limit */
    if ((end - start + 1) > bit_per_line) {
        return;
    }

    for (i = start; i <= end; i++) {
        if (value & (1 << (end - i))) {
            set_bit(i, 1,  memory_img);
        }
    }
}

/* Function to set or clear a specific bit in the memory image */
void set_bit(int i, int value, MemoryImage *memory_img) {
    int byteIndex = i / BYTE_SIZE;
    int bitOffset = i % BYTE_SIZE;
    char mask = (char)(1 << (BYTE_SIZE - 1 - bitOffset));

    if (memory_img->count < MEMORY_CAPACITY) {
        if (value) {
            memory_img->lines[memory_img->count][byteIndex] |= mask;  /* Set bit to 1 */
        } else {
            memory_img->lines[memory_img->count][byteIndex] &= ~mask; /* Clear bit to 0 */
        }
    } else {
        set_general_error(CPU_MEMORY_FULL);
    }
}



/**
 * Function that sets the bit at the specified index to 1 in the relevant memory image.
 * @param i
 * @param memory_img

void set_bit(int i, MemoryImage *memory_img) {

    char mask;
    int byteIndex = i / BYTE_SIZE;
    int bitOffset = i % BYTE_SIZE;

    if (byteIndex >= NUM_OF_BYTES) {
        return;
    }

    mask = (char)(1 << (BYTE_SIZE -1 - bitOffset));

    if (memory_img->count < MEMORY_CAPACITY) {
        memory_img->lines[memory_img->count][byteIndex] |= mask;
    } else {
        set_general_error(CPU_MEMORY_FULL);
    }

}*/

void code_immediate_addr_mode (int num, MemoryImage *memory_img) {
    int end = IMMIDIATE_DIRECTIVE_BIT_SIZE-1;
    set_int_code(0, end, num, memory_img);
    set_bit(A, 1, memory_img);
}

boolean code_direct_addr_mode(const char *label, CmpData *cmp_data, int line) {
    LabelType label_type;
    int address = get_label_single_addr(&cmp_data->label_table, label, &label_type);
    int end = IMMIDIATE_DIRECTIVE_BIT_SIZE-1;
    int temp_count = cmp_data->code.count;

    /* Label does not found */
    if (address == -1) {
        return FALSE;
    }

    cmp_data->code.count = line;
    set_int_code(0 , end, address, &cmp_data->code);
    cmp_data->code.count = temp_count;

    if (label_type == EXTERNAL) {
        set_bit(E,1, &cmp_data->code);
        cmp_data->code.count++;
    } else {
        set_bit(R, 1, &cmp_data->code);
        cmp_data->code.count++;
    }

    return TRUE;
}

void code_register_addr_mode(int reg_num, MemoryImage *memory_img, int position) {
    int end = position+REGISTER_BIT_SIZE-1;

    set_int_code(position, end, reg_num, memory_img);
    set_bit(A, 1, memory_img);
}

void set_char_code(char c, MemoryImage *memory_img) {
    int ascii_value;
    if (memory_img == NULL) {
        return;
    }

    /** TODO: fix the numbers */
    /* ASCII value of the character */
    ascii_value = (int)c;

    /* Ensure the ASCII value fits within 15 bits */
    if (ascii_value > 32767) {
        return;
    }

    /* Set the binary code of the ASCII value using set_int_code */
    set_int_code(0, 14, ascii_value, memory_img);
    memory_img->count++;
}


void code_data(ASTNode *node, MemoryImage *memory_image) {
    DirNode *current = node->specifics.directive.operands;
    while (current) {
        /* Validate operand */
        if (is_valid_integer(current->operand)) { /** todo: define numbers **/
            set_int_code(0, 14, my_atoi(current->operand), memory_image);
            memory_image->count++;
            current = current->next;
        } else {
            set_error(NOT_INTEGER, node->location);
            print_error();
        }
    }
}

void code_string(ASTNode *node, MemoryImage *memory_img) {
    char *str;
    size_t str_length;
    int i;

    str = node->specifics.directive.operands->operand;
    str_length = strlen(str);
    for (i = 0; i < str_length; i++) {
        set_char_code(str[i], memory_img);
    }
    /* Add Null terminator */
    set_char_code('\0', memory_img);
}

void mark_word(MemoryImage *code_img) {
    set_bit(15, 1, code_img);
}

void unmark_word(MemoryImage *code_img, int line) {
    int temp_count = code_img->count;
    code_img->count = line;
    set_bit(15, 0, code_img);
    code_img->count = temp_count;
}

/* Get the first marked line */
int get_marked_line(MemoryImage *memory_img) {
    int i, byteIndex = 15 / BYTE_SIZE, bitOffset = 15 % BYTE_SIZE;
    char mask = (char)(1 << (BYTE_SIZE - 1 - bitOffset));

    for (i = 0; i < memory_img->count; i++) {
        if ((memory_img->lines[i][byteIndex] & mask) != 0) {
            return i;
        }
    }
    return -1; /* No marked line found */
}

