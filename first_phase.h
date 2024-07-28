#ifndef FIRST_PHASE_H
#define FIRST_PHASE_H
/* ----------------------------------------- Includes -----------------------------------------*/
#include "cmp_data.h"
/* ----------------------------------- Functions Prototypes ------------------------------------*/
/**
 * The `first_phase_analyzer` function analyzes a parsed line (ASTNode) during the first phase of assembly.
 * It determines the type of line and processes it accordingly, updating the `CmpData` structure.
 *
 * @param node The parsed line represented as an Abstract Syntax Tree (AST) node.
 * @param cmp_data The data structure holding various program-related data during assembly.
 * @return TRUE if the line is processed successfully, FALSE otherwise.
 */
void first_phase_analyzer(ASTNode *node, CmpData *cmp_data);

#endif /* FIRST_PHASE_H */
