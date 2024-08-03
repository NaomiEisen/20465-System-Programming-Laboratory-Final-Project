#ifndef SECOND_PHASE_H
#define SECOND_PHASE_H
/* ----------------------------------------- Includes -----------------------------------------*/
#include "../../structures/headers/ast.h"
#include "../../structures/headers/cmp_data.h"
/* ----------------------------------- Functions Prototypes ------------------------------------*/
/**
 * The `second_phase_analyzer` function processes an unresolved line during the second phase of assembly.
 * It handles encoding of label operand's in instruction lines and entry directives.
 *
 * @param node The parsed line represented as an ASTNode containing an instruction or directive.
 * @param cmp_data The data structure holding various program-related data during assembly.
 * @return TRUE if the line is processed successfully, FALSE otherwise.
 */
void second_phase_analyzer(ASTNode *node, CmpData *cmp_data);

#endif /* SECOND_PHASE_H */
