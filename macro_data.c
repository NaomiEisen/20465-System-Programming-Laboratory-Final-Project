/* ---------------------------------------------------------------------------------------
 *                                          Includes
 * --------------------------------------------------------------------------------------- */
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include "macro_data.h"
#include "boolean.h"
#include "trie.h"


/* Initialize the Macro Trie */
Boolean init_macr_trie(MacroTrie *macr_trie) {
    if (init_trie(&macr_trie->trie) == TRUE) {
        macr_trie->last_added_node = NULL;
        return TRUE;
    }
    return FALSE;
}

/* Insert a macro name into the Macro Trie */
Boolean add_macr(MacroTrie *macr_trie, const char *macr_name) {
    MacroData *macr_data = (MacroData *)malloc(sizeof(MacroData));
    if (!macr_data) {
        return FALSE;
    }
    macr_data->head = NULL;
    macr_data->tail = NULL;

    if (insert_to_trie(&macr_trie->trie, macr_name, macr_data)) {
        macr_trie->last_added_node = search_trie(&macr_trie->trie, macr_name);
        return TRUE;
    }
    free(macr_data);
    return FALSE;
}

/* Get the content of a macro  */
LineNode* get_macr_content(MacroTrie *macr_trie, const char *macr_name) {
    TrieNode *node;
    MacroData *macr_data;

    node = search_trie(&macr_trie->trie, macr_name);
    if (node == NULL) {
        return NULL;
    }
    macr_data = (MacroData *)node->data;
    return macr_data->head;
}


/* Add a line to the last added macro */
Boolean add_line_to_last_macro(MacroTrie *macr_trie, const char *line) {
    MacroData *macr_data = NULL;
    LineNode *new_line_node = NULL;
    if (macr_trie->last_added_node == NULL) {
        return FALSE;
    }

    macr_data = (MacroData *)macr_trie->last_added_node->data;
    new_line_node = (LineNode *)malloc(sizeof(LineNode));
    if (!new_line_node) {
        return FALSE;
    }
    strncpy(new_line_node->line, line, MAX_LINE_LENGTH);
    new_line_node->next = NULL;

    if (macr_data->tail) {
        macr_data->tail->next = new_line_node;
        macr_data->tail = new_line_node;
    } else {
        macr_data->head = new_line_node;
        macr_data->tail = new_line_node;
    }
    return TRUE;
}

/* Function to find a macro in the Macro Trie */
TrieNode* find_macro(const MacroTrie *macr_trie, const char *macr_name) {
    return search_trie(&macr_trie->trie, macr_name);
}

/* Free a LineNode linked list */
void free_line_nodes(LineNode *head) {
    LineNode *current = head;
    LineNode *next;

    while (current) {
        next = current->next;
        free(current);
        current = next;
    }
}

/* Free the data associated with a Macro Trie node */
void free_macr_data(void *data) {
    MacroData *macro_data = NULL;
    if (data) {
        macro_data = (MacroData *)data;
        free_line_nodes(macro_data->head);
        free(macro_data);
    }
}

/* Function to free all data in the trie without freeing the nodes */
void free_data_recursive(TrieNode* node) {
    int i;
    if (node) {
        for (i = 0; i < ALPHABET_SIZE; i++) {
            if (node->children[i]) {
                free_data_recursive(node->children[i]);
            }
        }
        if (node->data) {
            free_macr_data(node->data);
            node->data = NULL;
        }
    }
}

/* Function to get the character from an index in the trie */
char get_char(int index) {
    if (index >= 0 && index < 26) {
        return 'a' + index;
    } else if (index >= 26 && index < 52) {
        return 'A' + (index - 26);
    } else if (index >= 52 && index < 62) {
        return '0' + (index - 52);
    } else if (index == 62) {
        return '-';
    } else if (index == 63) {
        return '_';
    } else if (index == 64) {
        return '.';
    } else {
        return '\0'; /* Invalid index */
    }
}

/* Function to print lines of a macro */
void print_lines(LineNode* head) {
    LineNode* current = head;
    while (current != NULL) {
        printf("%s\n", current->line);
        current = current->next;
    }
}

/* Recursive function to print the trie */
void print_trie_recursive(TrieNode* node, char* buffer, int depth) {
    MacroData* macro_data = NULL;
    int i;
    if (node == NULL) {
        return;
    }

    if (node->exist) {
        buffer[depth] = '\0'; /* Null-terminate the current prefix */
        printf("Macro: %s\n", buffer);
        macro_data = (MacroData*)node->data;
        if (macro_data != NULL) {
            print_lines(macro_data->head);
        }
        printf("\n");
    }

    for (i = 0; i < ALPHABET_SIZE; i++) {
        if (node->children[i] != NULL) {
            buffer[depth] = get_char(i);
            print_trie_recursive(node->children[i], buffer, depth + 1);
        }
    }
}

/* Function to print the entire trie */
void print_trie_test(MacroTrie* macr_trie) {
    char buffer[256]; /* Buffer to store the current prefix */
    print_trie_recursive(macr_trie->trie.root, buffer, 0);
}


/* Function to free all data in the trie without freeing the nodes */
void free_trie_data(MacroTrie *macr_trie) {
    free_data_recursive(macr_trie->trie.root);
}

/* Free the entire Macro Trie */
void free_macr_trie(MacroTrie *macr_trie) {
    free_node(macr_trie->trie.root);
}

