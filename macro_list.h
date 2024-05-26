//
// Created by naomi on 25/05/2024.
//

#ifndef INC_20465_SYSTEM_PROGRAMMING_LABORATORY_FINAL_PROJECT_MACRO_LIST_H
#define INC_20465_SYSTEM_PROGRAMMING_LABORATORY_FINAL_PROJECT_MACRO_LIST_H
#include "general.h"

/* Define the structure for a macro node in the linked list */
typedef struct MacroNode {
    char name[MAX_LINE_LENGTH]; /* Maximum length for macro name */
    char *value; /* Maximum length for macro value */
    struct macro_node* next;
} MacroNode;

/* Define the structure for the macro list */
typedef struct {
    MacroNode* head; /* Pointer to the head of the list */
} MacroList;


MacroNode* create_macro_node(const char* macro_name, const char* macro_value);
void printMacros(const MacroNode* head);
void free_macros(MacroList* list);
void insert_macro_node(MacroList* list, const char* name, const char* value);
bool add_value_to_head(MacroList* list, const char* value);

#endif //INC_20465_SYSTEM_PROGRAMMING_LABORATORY_FINAL_PROJECT_MACRO_LIST_H
