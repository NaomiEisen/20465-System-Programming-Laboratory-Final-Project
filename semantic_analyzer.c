
#include "semantic_analyzer.h"
#include "ast.h"
#include "lable_trie.h"

bool analyzeLine(ASTNode* node) {
    if (node->lineType == LINE_EMPTY || node->lineType == LINE_COMMENT ) {
        return true;
    }

    if (node->label != NULL) {

    }

}
