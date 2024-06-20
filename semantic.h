
#ifndef INC_20465_SYSTEM_PROGRAMMING_LABORATORY_FINAL_PROJECT_SEMANTIC_H
#define INC_20465_SYSTEM_PROGRAMMING_LABORATORY_FINAL_PROJECT_SEMANTIC_H
#include "cmp_data.h"

#define A 12
#define R 13
#define E 14
#define REGISTER_POS 6

boolean validate_label(ASTNode *node, int address, CmpData *cmp_data, Mappings *mappings);
void code_operands(ASTNode *node, CmpData *cmp_data);
boolean handle_directive(ASTNode *node, CmpData *cmp_data, Mappings *mappings);
boolean first_word(ASTNode *node, int command_index, MemoryImage *code_img, Mappings *mappings);
boolean handle_extern(ASTNode *node, CmpData *cmpData, Mappings *mappings);
int find_command(const char *command, InstructMapping *instruct_mapping);
int find_directive(const char *command, DirectiveMapping *dir_mapping);
boolean handle_operation(ASTNode *node, CmpData *cmp_data, Mappings *mappings);
boolean analyzeLine(ASTNode *node, CmpData *cmp_data, Mappings *mappings);

#endif /*INC_20465_SYSTEM_PROGRAMMING_LABORATORY_FINAL_PROJECT_SEMANTIC_H*/
