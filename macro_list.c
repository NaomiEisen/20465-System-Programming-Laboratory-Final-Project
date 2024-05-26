//
// Created by naomi on 25/05/2024.
//
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include "macro_list.h"
#include "errors.h"

/* Function to create a new macro node */
MacroNode* create_macro_node(const char* macro_name, const char* macro_value) {
    MacroNode* newNode = (MacroNode*)malloc(sizeof(MacroNode));

    if (newNode == NULL) {
        fprintf(stderr, "Memory allocation error\n");
        exit(EXIT_FAILURE);
    }

    strncpy(newNode->name, macro_name, sizeof(newNode->name) - 1);
    newNode->name[sizeof(newNode->name) - 1] = '\0'; /* Ensure null-termination */
    strncpy(newNode->value, macro_value, sizeof(newNode->value) - 1);
    newNode->value[sizeof(newNode->value) - 1] = '\0'; /* Ensure null-termination */
    newNode->next = NULL;
    return newNode;
}

/* Function to insert a new macro node at the beginning of the list */
void insert_macro_node(MacroList* list, const char* name, const char* value) {
    MacroNode* newNode = create_macro_node(name, value);
    newNode->next = list->head; /* Point the new node's next to the current head */
    list->head = newNode;       /* Update the head to be the new node */
}

/* Function to print all macros in the list */
void print_macros(const MacroList* list) {
    const MacroNode* current = list->head;

    while (current != NULL) {
        printf("Macro Name: %s\tMacro Value: %s\n", current->name, current->value);
        current = current->next;
    }
}

bool add_value_to_head(MacroList* list, const char* value) {
    if (list->head == NULL){
        set_error(&global_error, OTHER_ERROR, "", 0);
        return false;
    }
    size_t oldLen = strlen(list->head->value);
    char* combinedValue = (char*)malloc(oldLen + MAX_LINE_LENGTH + 1); /* Allocate memory for the combined value */
    if (combinedValue == NULL) {
        set_error(&global_error, MEMORY_ALLOCATION_ERROR, "", 0);
        return false;
    }

    strcpy(combinedValue, value); /* Copy the new text to the combined value */
    strcat(combinedValue, list->head->value); /* Concatenate the old value to the combined value */
}

/* Function to free the memory allocated for the list */
void free_macros(MacroList* list) {
    MacroNode* current = list->head;
    while (current != NULL) {
        MacroNode* temp = current;
        current = current->next;
        free(temp->value); /* Free the duplicated macro value */
        free(temp);
    }
    list->head = NULL; /* Ensure the list is now empty */
}
