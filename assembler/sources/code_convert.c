/* ---------------------------------------------------------------------------------------
 *                                          Includes
 * --------------------------------------------------------------------------------------- */
#include "../headers/code_convert.h"
#include "../../structures/headers/ast.h"
#include "../../structures/headers/errors.h"
#include "../../structures/headers/cmp_data.h"
#include "../../utils/headers/utils.h"
#include "../../utils/headers/output_files.h"
#include "../headers/defines.h"
/* ---------------------------------------------------------------------------------------
 *                                         Functions
 * --------------------------------------------------------------------------------------- */
/**
 * Sets multiple bits in the memory image based on the given integer value.
 * The Function converts the given integer in base 10 to base 2 and writes the bits
 * in the memory image.
 *
 * @param start The starting bit position.
 * @param end The ending bit position.
 * @param value The integer value to set in the memory image.
 * @param memory_img Pointer to the memory image structure.
 */
void set_int_code(int start, int end, int value, MemoryImage *memory_img) {
    int i;                              /* Variable to iterate trough loop */
    int bit_per_line = NUM_OF_BYTES * BYTE_SIZE; /* length of word in bits */

    /* Ensure the range is within a single line's limit */
    if ((end - start + 1) > bit_per_line) {
        return;
    }

    /* Encode the bits */
    for (i = start; i <= end; i++) {
        if (value & (1 << (end - i))) {
            set_bit(i, 1,  memory_img);
        }
    }
}

/**
 * Sets or clears a specific bit in the memory image.
 *
 * @param i The bit position to set or clear.
 * @param value The value to set (1 for setting the bit, 0 for clearing it).
 * @param memory_img Pointer to the memory image structure.
 */
void set_bit(int i, int value, MemoryImage *memory_img) {
    int byteIndex = i / BYTE_SIZE;                       /* Calculate the byte index */
    int bitOffset = i % BYTE_SIZE;        /* Calculate the bit index within the byte */
    char mask = (char)(1 << (BYTE_SIZE - 1 - bitOffset)); /* Create mask accordingly */

    /* If memory image is not ran out of storage */
    if (memory_img->count < MEMORY_CAPACITY) {
        if (value) {
            memory_img->lines[memory_img->write_ptr][byteIndex] |= mask;  /* Set bit to 1 */
        } else {
            memory_img->lines[memory_img->write_ptr][byteIndex] &= ~mask; /* Clear bit to 0 */
        }
    }
}

/**
 * Converts a 15-bit word to an unsigned integer in octal representation.
 *
 * @param word The 15-bit word to convert.
 * @return The octal representation of the word as an unsigned integer.
 */
unsigned int convert_to_octal(const char *word) {
    unsigned int value = 0; /* The octal representation */
    int i;           /* Variable to iterate trough loop */
    for (i = 0; i < LAST_WORD_BIT; i++) {
        int byteIndex = i / BYTE_SIZE;  /* Calculate the byte index */
        int bitOffset = i % BYTE_SIZE; /* Calculate the bit index within the byte */
        value = (value << 1) | ((word[byteIndex] >> (BYTE_SIZE - 1 - bitOffset)) & 1);
    }
    return value;
}

/**
 * Encodes an immediate address mode value into the memory image.
 *
 * @param num The immediate value to encode.
 * @param memory_img Pointer to the memory image structure.
 */
void code_immediate_addr_mode (int num, MemoryImage *memory_img) {
    int end = IMMEDIATE_DIRECTIVE_BIT_SIZE - 1;
    set_int_code(0, end, num, memory_img);
    set_bit(A, 1, memory_img);
}

/**
 * Encodes a direct address mode value into the memory image.
 *
 * @param label The label to encode.
 * @param cmp_data Pointer to the compilation data structure.
 * @return TRUE if the label was successfully encoded, FALSE otherwise.
 */
Boolean code_direct_addr_mode(const char *label, CmpData *cmp_data) {
    /* Get the label type and address from the label table */
    LabelType label_type = get_label_type(&cmp_data->label_table, label);
    int address = get_label_addr(&cmp_data->label_table, label);
    int end = IMMEDIATE_DIRECTIVE_BIT_SIZE - 1;

    /* Label is not found */
    if (address == -1) {
        return FALSE;
    }

    /* Encode the label address into the memory image */
    set_int_code(0 , end, address, &cmp_data->code);

    if (label_type == EXTERNAL) {
        /* Set the external bit and write the label to the extern file */
        set_bit(E,1, &cmp_data->code);
        write_label(label, cmp_data->code.write_ptr+IC_START, cmp_data->extern_file.file);
        cmp_data->extern_file.delete = FALSE;  /* Set flag to false - non-empty file should not be deleted */
    } else {
        /* Set the relocatable bit */
        set_bit(R, 1, &cmp_data->code);
    }

    /* Process executed successfully */
    return TRUE;
}

/**
 * Encodes a register address mode value into the memory image.
 *
 * @param reg_num The register number to encode.
 * @param memory_img Pointer to the memory image structure.
 * @param position The bit position to start encoding.
 */
void code_register_addr_mode(int reg_num, MemoryImage *memory_img, int position) {
    int end = position+REGISTER_BIT_SIZE-1;
    set_int_code(position, end, reg_num, memory_img);
    set_bit(A, 1, memory_img);
}

/**
 * Encodes a character into the memory image as its ASCII value.
 *
 * @param c The character to encode.
 * @param memory_img Pointer to the memory image structure.
 */
void set_char_code(char c, MemoryImage *memory_img) {
    int ascii_value; /* Variable to store the ascii value */

    /* ASCII value of the character */
    ascii_value = (int)c;

    /* Ensure the ASCII value fits within 15 bits */
    if (ascii_value > MAX_INTEGER) {
        return;
    }

    /* Set the binary code of the ASCII value using set_int_code */
    set_int_code(0, WORD_END_POS, ascii_value, memory_img);
    updt_memory_image_counter(memory_img);
}

/**
 * Encodes a list of data operands into the memory image.
 *
 * @param node The AST node containing the data operands.
 * @param memory_image Pointer to the memory image structure.
 */
void code_data(ASTNode *node, MemoryImage *memory_image) {
    DirNode *current = node->specific.directive.operands;
    while (current) {
        /* Validate operand */
        if (is_valid_integer(current->operand)) {
            /* Convert the text to integer and encode */
            set_int_code(0, WORD_END_POS, my_atoi(current->operand), memory_image);
            updt_memory_image_counter(memory_image); /* Update counter */
            current = (DirNode *) current->next;
        } else {
            /* Not an integer */
            set_error(NOT_INTEGER, node->location);
            current = (DirNode *) current->next;
        }
    }
}

/**
 * Encodes a string operand into the data memory image.
 *
 * @param node The AST node containing the string operand.
 * @param memory_img Pointer to the memory image structure.
 */
void code_string(ASTNode *node, MemoryImage *memory_img) {
    char *str;  /* Variable to hold the string */
    size_t str_length;      /* String's length */
    int i; /* Variable to iterate through loop */

    /* Get the string */
    str = node->specific.directive.operands->operand;

    /* Get the string's length */
    str_length = strlen(str);

    /* Code each char of the string */
    for (i = 0; i <= str_length; i++) {
        set_char_code(str[i], memory_img);
    }
    /* Add Null terminator */
    /*set_char_code('\0', memory_img);*/
}

/**
 * Marks this word as unresolved by setting the last bit of the word to 1.
 * This mark will indicate to the second phase that this word should be encoded.
 *
 * @param code_img Pointer to the memory image structure.
 */
void mark_word(MemoryImage *code_img) {
    set_bit(LAST_WORD_BIT, 1, code_img);
}

/**
 * Unmarks the last bit of the specified word in the memory image.
 * Indicates that the word is encoded.
 *
 * @param code_img Pointer to the memory image structure.
 * @param line The line number to unmark.
 */
void unmark_word(MemoryImage *code_img, int line) {
    code_img->write_ptr = line; /* Set writer to line */
    set_bit(LAST_WORD_BIT, 0, code_img);
}

/**
 * Retrieves the first marked word in the memory image.
 *
 * @param memory_img Pointer to the memory image structure.
 * @return The index of the first marked line, or -1 if no marked line is found.
 */
int get_marked_line(MemoryImage *memory_img) {
    int i; /* Variable to iterate through loop */
    int byteIndex = LAST_WORD_BIT / BYTE_SIZE; /* Calculate the byte index */
    int bitOffset = LAST_WORD_BIT % BYTE_SIZE; /* Calculate the bit index */

    /* Create the relevant mask */
    char mask = (char)(1 << (BYTE_SIZE - 1 - bitOffset));

    /* Go through the memory image and fine the first unresolved word */
    for (i = 0; i < memory_img->count; i++) {
        if ((memory_img->lines[i][byteIndex] & mask) != 0) {
            return i;
        }
    }
    return -1; /* No marked word found */
}