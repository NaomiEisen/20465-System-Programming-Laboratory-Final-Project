
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include "macro_list.h"
#include "errors.h"


/* Function to create a new macro node */
MacroNode* create_macro_node(const char* macro_name) {
    MacroNode* newNode = (MacroNode*)malloc(sizeof(MacroNode));
    if (newNode == NULL) {
        fprintf(stderr, "Memory allocation error\n");
        exit(EXIT_FAILURE);
    }
    strncpy(newNode->name, macro_name, sizeof(newNode->name) - 1);
    newNode->name[sizeof(newNode->name) - 1] = '\0'; /* Ensure null-termination */
    newNode->content_lines = NULL; /* Initialize value lines to NULL (empty list) */
    newNode->next = NULL;
    return newNode;
}

/* Function to insert a new macro node with empty value list at the head of the list */
void insert_macro_node(MacroList* list, const char* name) {
    MacroNode* newNode = create_macro_node(name);
    newNode->next = list->head;
    list->head = newNode;
}

/* Function to add a line to the value lines of a macro node */
void add_content_line(MacroList* list, const char* line) {
    if (list->head == NULL) {
        fprintf(stderr, "Error: No macro node available to add value lines\n");
        return;
    }

    LineNode* newLine = (LineNode*)malloc(sizeof(LineNode));
    if (newLine == NULL) {
        fprintf(stderr, "Memory allocation error\n");
        exit(EXIT_FAILURE);
    }
    strncpy(newLine->line, line, sizeof(newLine->line) - 1);
    newLine->line[sizeof(newLine->line) - 1] = '\0'; /* Ensure null-termination */
    newLine->next = NULL;

    if (list->head->content_lines == NULL) {
        /* If the value lines are empty, set the new line as the first line */
        list->head->content_lines = newLine;
    } else {
        /* Traverse to the end of the value lines and add the new line node */
        LineNode* current = list->head->content_lines;
        while (current->next != NULL) {
            current = current->next;
        }
        current->next = newLine;
    }
}

/* Function to free a line node */
void free_line_node(LineNode* node) {
    while (node != NULL) {
        LineNode* temp = node;
        node = node->next;
        free(temp);
    }
}

/* Function to free a macro node */
void free_macro_node(MacroNode* node) {
    free_line_node(node->content_lines);
    free(node);
}

/* Function to free the entire macro list */
void free_macro_list(MacroList* list) {
    MacroNode* current = list->head;
    while (current != NULL) {
        MacroNode* temp = current;
        current = current->next;
        free_macro_node(temp);
    }
    list->head = NULL; /* Set list head to NULL after freeing */
}

/* Function to print all macros and their values */
void print_all_macros(const MacroList* list) {
    const MacroNode* current_macro = list->head;
    while (current_macro != NULL) {
        printf("Macro Name: %s\n", current_macro->name);
        const LineNode* current_line = current_macro->content_lines;
        while (current_line != NULL) {
            printf("\tValue Line: %s", current_line->line);
            current_line = current_line->next;
        }
        current_macro = current_macro->next;
    }
}