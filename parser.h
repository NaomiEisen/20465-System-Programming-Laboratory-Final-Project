#ifndef PARSER_H
#define PARSER_H
/* ----------------------------------------- Includes -----------------------------------------*/
# include "ast.h"
#include "macro_data.h"
/* ----------------------------------- Functions Prototypes ------------------------------------*/
/**
 * Parses a line of assembly code and creates an ASTNode representing the line.
 * This function processes a single line of assembly code, determining its type
 * (empty/comment, label, directive, or instruction) and extracting the relevant
 * information into an ASTNode structure. It uses several helper functions to
 * handle different parts of the line.
 *
 * @param macr_trie A trie containing macro definitions to check for collisions with labels.
 * @param file_name The name of the file being parsed, used for error reporting.
 * @param line_num The line number in the file, used for error reporting.
 * @param line The line of assembly code to parse.
 *
 * @return A pointer to the newly created ASTNode representing the parsed line.
 */
ASTNode *parseLine(const MacroTrie *macr_trie, const char *file_name, int line_num, const char *line);

#endif /* PARSER_H */
