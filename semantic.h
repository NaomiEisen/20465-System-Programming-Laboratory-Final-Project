
#ifndef INC_20465_SYSTEM_PROGRAMMING_LABORATORY_FINAL_PROJECT_SEMANTIC_H
#define INC_20465_SYSTEM_PROGRAMMING_LABORATORY_FINAL_PROJECT_SEMANTIC_H
#include "cmp_data.h"

#define A 12
#define R 13
#define E 14
#define REGISTER_POS 6

boolean add_label(ASTNode *node, int address, CmpData *cmp_data);
void code_operands(ASTNode *node, CmpData *cmp_data);
boolean handle_directive(ASTNode *node, CmpData *cmp_data);
boolean first_word(ASTNode *node, int command_index, MemoryImage *code_img);
boolean handle_extern(ASTNode* node, CmpData* cmpData);
boolean handle_operation(ASTNode *node, CmpData *cmp_data);
boolean analyzeLine(ASTNode *node, CmpData *cmp_data);

#endif /*INC_20465_SYSTEM_PROGRAMMING_LABORATORY_FINAL_PROJECT_SEMANTIC_H*/
