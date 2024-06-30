#ifndef CODE_CONVERT_H
#define CODE_CONVERT_H
#include "cmp_data.h"
#include "ast.h"

#define BYTE_SIZE 8
#define REGISTER_BIT_SIZE 3
#define IMMIDIATE_DIRECTIVE_BIT_SIZE 12

void set_bit(int i, int value, MemoryImage *memory_img);
unsigned int convert_to_octal(const char *word);
void set_int_code(int start, int end, int value, MemoryImage *memory_img);
void code_immediate_addr_mode (int num, MemoryImage *memory_img);
Boolean code_direct_addr_mode(const char *label, CmpData *cmp_data);
void code_register_addr_mode(int reg_num, MemoryImage *memory_img, int position);
void code_data(ASTNode *node, MemoryImage *memory_image);
void code_string(ASTNode *node, MemoryImage *memory_img);
void mark_word(MemoryImage *code_img);
void unmark_word(MemoryImage *code_img, int line);
int get_marked_line(MemoryImage *memory_img);

#endif /* CODE_CONVERT_H */
