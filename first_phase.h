
#ifndef INC_20465_SYSTEM_PROGRAMMING_LABORATORY_FINAL_PROJECT_FIRST_PHASE_H
#define INC_20465_SYSTEM_PROGRAMMING_LABORATORY_FINAL_PROJECT_FIRST_PHASE_H
#include "cmp_data.h"

#define A 12
#define R 13
#define E 14
#define REGISTER_POS 6
#define OPERAND1_OFFSET 7
#define OPERAND2_OFFSET 11


boolean add_label(ASTNode *node, int address, CmpData *cmp_data);
void code_operands(ASTNode *node, CmpData *cmp_data);
boolean handle_directive(ASTNode *node, CmpData *cmp_data);
boolean first_word(ASTNode *node, int command_index, MemoryImage *code_img);
boolean handle_extern(ASTNode* node, CmpData* cmp_data);
boolean handle_instruction(ASTNode *node, CmpData *cmp_data);
boolean first_phase_analyzer(ASTNode *node, CmpData *cmp_data);

#endif /*INC_20465_SYSTEM_PROGRAMMING_LABORATORY_FINAL_PROJECT_FIRST_PHASE_H*/
