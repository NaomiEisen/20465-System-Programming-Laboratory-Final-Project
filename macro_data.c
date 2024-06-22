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
boolean init_macr_trie(MacroTrie *macr_trie) {
    if (init_trie(&macr_trie->trie) == TRUE) {
        macr_trie->last_added_node = NULL;
        return TRUE;
    }
    return FALSE;
}

/* Insert a macro name into the Macro Trie */
boolean add_macr(MacroTrie *macr_trie, const char *macr_name) {
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
boolean add_line_to_last_macro(MacroTrie *macr_trie, const char *line) {
    if (macr_trie->last_added_node == NULL) {
        return FALSE;
    }

    MacroData *macr_data = (MacroData *)macr_trie->last_added_node->data;
    LineNode *new_line_node = (LineNode *)malloc(sizeof(LineNode));
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
TrieNode* find_macro(MacroTrie *macr_trie, const char *macr_name) {
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
    if (data) {
        MacroData *macro_data = (MacroData *)data;
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
            free_data(node->data);
            node->data = NULL;
        }
    }
}

/* Function to free all data in the trie without freeing the nodes */
void free_trie_data(MacroTrie *macr_trie) {
    free_data_recursive(macr_trie->trie.root);
}

/* Free the entire Macro Trie */
void free_macr_trie(MacroTrie *macr_trie) {
    free_node(macr_trie->trie.root);
}

