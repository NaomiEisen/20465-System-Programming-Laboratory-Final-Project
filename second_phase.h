#ifndef SECOND_PHASE_H
#define SECOND_PHASE_H
#include "boolean.h"
#include "ast.h"
#include "cmp_data.h"

Boolean second_phase_analyzer(ASTNode *node, CmpData *cmp_data);
Boolean code_label_operands(ASTNode *node, CmpData *cmp_data);
Boolean handle_entry(ASTNode *node, CmpData *cmp_data);

#endif /* SECOND_PHASE_H */
