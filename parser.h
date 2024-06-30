#ifndef PARSER_H
#define PARSER_H
# include "ast.h"
#include "macro_data.h"


Boolean check_empty_line (const char** line, ASTNode* node);
Boolean validate_label(const char *label, ASTNode *node, MacroTrie *macr_trie);
Boolean is_label(const char **line, ASTNode *node, MacroTrie *macr_trie);
Boolean parse_operation(const char **line, ASTNode *node);
Boolean validate_operation(const char *operation, ASTNode* node);
Boolean parse_string(const char **line, ASTNode *node);
Boolean parse_operands(const char **line, ASTNode *node);
void parse_instruct_operand(ASTNode *node, const char *operand);
ASTNode *parseLine(MacroTrie *macr_trie, const char *file_name, int line_num, const char *line);
void parse_int(ASTNode* node, const char *operand);
void parse_reg(ASTNode* node, const char *operand, int addr_mode);
void parse_label (ASTNode* node, const char *operand);
#endif /* PARSER_H */
