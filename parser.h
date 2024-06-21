#ifndef PARSER_H
#define PARSER_H
# include "ast.h"


ASTNode *parseLine(const char *line, const char *file_name, int line_num);
#endif /* PARSER_H */
