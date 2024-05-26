//
// Created by naomi on 25/05/2024.
//

#ifndef INC_20465_SYSTEM_PROGRAMMING_LABORATORY_FINAL_PROJECT_PREPROCESSOR_H
#define INC_20465_SYSTEM_PROGRAMMING_LABORATORY_FINAL_PROJECT_PREPROCESSOR_H

#include "general.h"
#include "macro_list.h"
#define MACR_START "macr"
#define MACR_END "endmacr"

int is_comment(const char* str);
bool verify_macro(char *str);
int macr_start(const char* str);
int macr_end(const char* str);
MacroNode* is_macro(MacroList* list, const char* str);
void copy_macro_to_file(MacroNode* macr, FILE* file);
bool is_reserved_word(const char* str);
bool is_empty_line(const char* line);
bool create_macr(MacroList* list,char* str);
bool create_macr(MacroList* list,char* str);
#endif //INC_20465_SYSTEM_PROGRAMMING_LABORATORY_FINAL_PROJECT_PREPROCESSOR_H
