
#ifndef INC_20465_SYSTEM_PROGRAMMING_LABORATORY_FINAL_PROJECT_FIRST_PHASE_H
#define INC_20465_SYSTEM_PROGRAMMING_LABORATORY_FINAL_PROJECT_FIRST_PHASE_H
#include "cmp_data.h"

#define A 12
#define R 13
#define E 14
#define REGISTER_POS 6
#define SRC_OFFSET 7
#define DEST_OFFSET 11


Boolean add_label(ASTNode *node, int address, CmpData *cmp_data);
void code_operands(ASTNode *node, CmpData *cmp_data);
Boolean handle_directive(ASTNode *node, CmpData *cmp_data);
Boolean first_word(ASTNode *node, int command_index, MemoryImage *code_img);
Boolean code_first_word_addr(int command_index, MemoryImage *code_img, int addr_mode, int offset);
Boolean handle_extern(ASTNode* node, CmpData* cmp_data);
Boolean handle_instruction(ASTNode *node, CmpData *cmp_data);
Boolean first_phase_analyzer(ASTNode *node, CmpData *cmp_data);

#endif /*INC_20465_SYSTEM_PROGRAMMING_LABORATORY_FINAL_PROJECT_FIRST_PHASE_H*/
