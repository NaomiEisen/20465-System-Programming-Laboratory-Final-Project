
#ifndef PREPROCESSOR_H
#define PREPROCESSOR_H

#include "boolean.h"
#include "macro_list.h"
#include "errors.h"
#define MACR_START "macr"
#define MACR_END "endmacr"

int is_comment(const char* str);
boolean verify_macro(const char *str,Location location);
int macr_start(const char* str);
int macr_end(const char* str);
MacroNode* is_macro(MacroList* list, const char* str);
void copy_macro_to_file(MacroNode* macr, FILE* file);
boolean is_reserved_word(const char* str);
boolean create_macr(MacroList *list, const char *str, Location location);
#endif /* PREPROCESSOR_H */
