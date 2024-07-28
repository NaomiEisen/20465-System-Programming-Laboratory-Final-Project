#ifndef CODE_CONVERT_H
#define CODE_CONVERT_H
/* ----------------------------------------- Includes -----------------------------------------*/
#include "cmp_data.h"
#include "ast.h"
/* ----------------------------------- Functions Prototypes ------------------------------------*/
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
void set_int_code(int start, int end, int value, MemoryImage *memory_img);

/**
 * Sets or clears a specific bit in the memory image.
 *
 * @param i The bit position to set or clear.
 * @param value The value to set (1 for setting the bit, 0 for clearing it).
 * @param memory_img Pointer to the memory image structure.
 */
void set_bit(int i, int value, MemoryImage *memory_img);

/**
 * Converts a 15-bit word to an unsigned integer in octal representation.
 *
 * @param word The 15-bit word to convert.
 * @return The octal representation of the word as an unsigned integer.
 */
unsigned int convert_to_octal(const char *word);
/**
 * Encodes an immediate address mode value into the memory image.
 *
 * @param num The immediate value to encode.
 * @param memory_img Pointer to the memory image structure.
 */
void code_immediate_addr_mode (int num, MemoryImage *memory_img);

/**
 * Encodes a direct address mode value into the memory image.
 *
 * @param label The label to encode.
 * @param cmp_data Pointer to the compilation data structure.
 * @return TRUE if the label was successfully encoded, FALSE otherwise.
 */
Boolean code_direct_addr_mode(const char *label, CmpData *cmp_data);

/**
 * Encodes a register address mode value into the memory image.
 *
 * @param reg_num The register number to encode.
 * @param memory_img Pointer to the memory image structure.
 * @param position The bit position to start encoding.
 */
void code_register_addr_mode(int reg_num, MemoryImage *memory_img, int position);
/**
 * Encodes a character into the memory image as its ASCII value.
 *
 * @param c The character to encode.
 * @param memory_img Pointer to the memory image structure.
 */
void set_char_code(char c, MemoryImage *memory_img);

/**
 * Encodes a list of data operands into the memory image.
 *
 * @param node The AST node containing the data operands.
 * @param memory_image Pointer to the memory image structure.
 */
void code_data(ASTNode *node, MemoryImage *memory_image);

/**
 * Encodes a string operand into the data memory image.
 *
 * @param node The AST node containing the string operand.
 * @param memory_img Pointer to the memory image structure.
 */
void code_string(ASTNode *node, MemoryImage *memory_img);

/**
 * Marks this word as unresolved by setting the last bit of the word to 1.
 * This mark will indicate to the second phase that this word should be encoded.
 *
 * @param code_img Pointer to the memory image structure.
 */
void mark_word(MemoryImage *code_img);

/**
 * Unmarks the last bit of the specified word in the memory image.
 * Indicates that the word is encoded.
 *
 * @param code_img Pointer to the memory image structure.
 * @param line The line number to unmark.
 */
void unmark_word(MemoryImage *code_img, int line);

/**
 * Retrieves the first marked word in the memory image.
 *
 * @param memory_img Pointer to the memory image structure.
 * @return The index of the first marked line, or -1 if no marked line is found.
 */
int get_marked_line(MemoryImage *memory_img);

#endif /* CODE_CONVERT_H */
