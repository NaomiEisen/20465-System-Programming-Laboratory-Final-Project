#ifndef CODE_CONVERT_H
#define CODE_CONVERT_H
#include "cmp_data.h"
#include "ast.h"

#define BYTE_SIZE 8
#define REGISTER_BIT_SIZE 3
#define IMMIDIATE_DIRECTIVE_BIT_SIZE 12

void set_bit(int i, MemoryImage *memory_img);
void set_int_code(int start, int end, int value, MemoryImage *memory_img);
void code_immediate_addr_mode (OperandNode *operand, MemoryImage *memory_img);

void code_immediate_addr_mode (OperandNode *operand, MemoryImage *memory_img);

void code_direct_addr_mode (OperandNode *operand, CmpData *cmp_data);

void code_register_addr_mode(OperandNode *operand, MemoryImage *memory_img, int position);
void code_data(ASTNode *node, MemoryImage *memory_image);
void code_string(ASTNode *node, MemoryImage *memory_img);
void code_register_indirect_addr_mode (OperandNode *operand, MemoryImage *memory_img);
#endif /* CODE_CONVERT_H */
