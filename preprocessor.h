
#ifndef PREPROCESSOR_H
#define PREPROCESSOR_H

#include "general.h"
#include "macro_list.h"
#define MACR_START "macr"
#define MACR_END "endmacr"

int is_comment(const char* str);
bool verify_macro(const char *str);
int macr_start(const char* str);
int macr_end(const char* str);
MacroNode* is_macro(MacroList* list, const char* str);
void copy_macro_to_file(MacroNode* macr, FILE* file);
bool is_reserved_word(const char* str);
bool create_macr(MacroList* list, const char *str);
#endif /* PREPROCESSOR_H */
