
#ifndef PREPROCESSOR_H
#define PREPROCESSOR_H

#include "boolean.h"
#include "macro_data.h"
#include "errors.h"
#include "mappings.h"

#define MACR_START "macr"
#define MACR_END "endmacr"

Boolean validate_line_length(const char *line, Location location);
int is_comment(const char* str);
Boolean verify_macro(const char *str, Location location);
int macr_start(const char* str);
int macr_end(const char* str);
TrieNode * is_macro(MacroTrie *macro_trie, const char* str);
void copy_macro_to_file(TrieNode *macr, FILE* file);
Boolean create_macr(MacroTrie *macr_trie, const char *str, Location location);
void process_line(FILE *source_file, FILE* output_file, MacroTrie *macro_trie, Location location);
#endif /* PREPROCESSOR_H */
