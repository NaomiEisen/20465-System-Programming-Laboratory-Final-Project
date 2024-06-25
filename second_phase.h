#ifndef SECOND_PHASE_H
#define SECOND_PHASE_H
#include "boolean.h"
#include "ast.h"
#include "cmp_data.h"

boolean second_phase_analyzer(ASTNode *node, CmpData *cmp_data);
boolean code_label_operands(ASTNode *node, CmpData *cmp_data);
boolean handle_entry(ASTNode *node, CmpData *cmp_data);

#endif /* SECOND_PHASE_H */
