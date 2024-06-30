#include "code_convert.h"
#include "ast.h"
#include "errors.h"
#include "cmp_data.h"
#include "utils.h"
#include "first_phase.h"
#include "output_files.h"


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
            memory_img->lines[memory_img->write_ptr][byteIndex] |= mask;  /* Set bit to 1 */
        } else {
            memory_img->lines[memory_img->write_ptr][byteIndex] &= ~mask; /* Clear bit to 0 */
        }
    } else {
        set_general_error(CPU_MEMORY_FULL);
    }
}

/* Function to convert a 15-bit word to octal */
unsigned int convert_to_octal(const char *word) {
    unsigned int value = 0;
    int i;
    for (i = 0; i < 15; i++) {
        int byteIndex = i / BYTE_SIZE;
        int bitOffset = i % BYTE_SIZE;
        value = (value << 1) | ((word[byteIndex] >> (BYTE_SIZE - 1 - bitOffset)) & 1);
    }
    return value;
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

Boolean code_direct_addr_mode(const char *label, CmpData *cmp_data) {
    LabelType label_type = get_label_type(&cmp_data->label_table, label);
    int address = get_label_single_addr(&cmp_data->label_table, label);
    int end = IMMIDIATE_DIRECTIVE_BIT_SIZE-1;

    /* Label does not found */
    if (address == -1) {
        return FALSE;
    }

    set_int_code(0 , end, address, &cmp_data->code);

    if (label_type == EXTERNAL) {
        set_bit(E,1, &cmp_data->code);
        write_label(label, cmp_data->code.write_ptr+IC_START, cmp_data->extern_file.file);
    } else {
        set_bit(R, 1, &cmp_data->code);
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
    updt_memory_image_counter(memory_img);
}


void code_data(ASTNode *node, MemoryImage *memory_image) {
    DirNode *current = node->specific.directive.operands;
    while (current) {
        /* Validate operand */
        if (is_valid_integer(current->operand)) { /** todo: define numbers **/
            set_int_code(0, 14, my_atoi(current->operand), memory_image);
            updt_memory_image_counter(memory_image);
            current = (DirNode *) current->next;
        } else {
            set_error(NOT_INTEGER, node->location);
        }
    }
}

void code_string(ASTNode *node, MemoryImage *memory_img) {
    char *str;
    size_t str_length;
    int i;

    str = node->specific.directive.operands->operand;
    str_length = strlen(str);
    for (i = 0; i <= str_length; i++) {
        set_char_code(str[i], memory_img);
    }
    /* Add Null terminator */
    /*set_char_code('\0', memory_img);*/
}

void mark_word(MemoryImage *code_img) {
    set_bit(15, 1, code_img);
}

void unmark_word(MemoryImage *code_img, int line) {
    code_img->write_ptr = line; /* Set writer to line */
    set_bit(15, 0, code_img);
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

