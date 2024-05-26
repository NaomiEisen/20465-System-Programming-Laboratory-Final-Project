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
bool verify_macro(char *line);
int macr_start(const char* str);
int macr_end(const char* str);
void create_macro(MacroList* list, const char* name, const char* line);
bool is_macro(const char* str);
bool is_reserved_word(const char* str);
bool is_empty_line(const char* line);
#endif //INC_20465_SYSTEM_PROGRAMMING_LABORATORY_FINAL_PROJECT_PREPROCESSOR_H
