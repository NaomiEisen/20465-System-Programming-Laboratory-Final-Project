
#ifndef SEMANTIC_ANALYZER_H
#define SEMANTIC_ANALYZER_H
#include "hardware.h"
#include "boolean.h"

/* --------------------------- Function Prototypes -------------------------- */
boolean analyzeLine(ASTNode* node);
boolean handle_opeartion(ASTNode* node);
boolean handle_directive(ASTNode *node);
boolean validate_label(ASTNode *node, int address);
/*Function find_command_handler(const char *command);*/
int find_command(const char *command);
boolean code_operands(ASTNode *node);

#endif /*SEMANTIC_ANALYZER_H*/
