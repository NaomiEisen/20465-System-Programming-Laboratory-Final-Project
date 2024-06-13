
#ifndef SEMANTIC_ANALYZER_H
#define SEMANTIC_ANALYZER_H
#include "boolean.h"
#include "cmp_data.h"

#define A 12
#define R 13
#define E 14
#define REGISTER_POS 6

/* --------------------------- Function Prototypes -------------------------- */
boolean analyzeLine(ASTNode *node, CmpData *cmp_data);
boolean handle_opeartion(ASTNode *node, CmpData *cmp_data);
boolean handle_directive(ASTNode *node);
boolean validate_label(ASTNode *node, int address);
int find_command(const char *command);
void code_operands(ASTNode *node, CmpData *cmp_data);
boolean first_word(ASTNode *node, int command_index, MemoryImage *code_img)


#endif /*SEMANTIC_ANALYZER_H*/
