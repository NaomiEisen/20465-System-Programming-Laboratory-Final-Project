
#ifndef MACRO_LIST_H
#define MACRO_LIST_H

#define MAX_LINE_LENGTH 81

/* Define a structure for a single line in the macro value */
typedef struct LineNode {
    char line[MAX_LINE_LENGTH];
    struct LineNode* next;
} LineNode;

/* Define the structure for a macro node in the linked list */
typedef struct MacroNode {
    char name[MAX_LINE_LENGTH]; /* Maximum length for macro name */
    LineNode* content_lines; /* Linked list to store lines of macro value */
    struct MacroNode* next;
} MacroNode;

/* Define the structure for the macro list */
typedef struct {
    MacroNode* head; /* Pointer to the head of the list */
} MacroList;


void insert_macro_node(MacroList* list, const char* name);
void add_content_line(MacroList* list, const char* line);
void free_macro_list(MacroList* list);
void print_all_macros(const MacroList* list);

#endif /* MACRO_LIST_H */
