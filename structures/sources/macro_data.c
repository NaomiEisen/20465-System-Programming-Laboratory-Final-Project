/* ---------------------------------------------------------------------------------------
 *                                          Includes
 * --------------------------------------------------------------------------------------- */
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include "../headers/macro_data.h"
/* ---------------------------------------------------------------------------------------
 *                                         Functions
 * --------------------------------------------------------------------------------------- */
/**
 * Initializes the Macro Trie.
 *
 * @param macr_trie Pointer to the Macro Trie structure.
 * @return TRUE if initialization is successful, FALSE otherwise.
 */
Boolean init_macr_trie(MacroTrie *macr_trie) {
    if (init_trie(&macr_trie->trie) == TRUE) {
        macr_trie->last_added_node = NULL;
        return TRUE;
    }
    return FALSE;
}

/**
 * Inserts a macro name into the Macro Trie.
 *
 * @param macr_trie Pointer to the Macro Trie structure.
 * @param macr_name The name of the macro to be added.
 * @return ErrorCode - Error image of the error that occurred or NO_ERROR if the process
 *          executed successfully.
 */
ErrorCode add_macr(MacroTrie *macr_trie, const char *macr_name) {
    ErrorCode status;
    MacroData *macr_data = (MacroData *)malloc(sizeof(MacroData));

    /* Memory allocation failure */
    if (!macr_data) return MEMORY_ALLOCATION_ERROR;

    macr_data->head = NULL;
    macr_data->tail = NULL;

    /* Try inserting the macro to the trie node */
    status = insert_to_trie(&macr_trie->trie, macr_name, macr_data);
    if (status == NO_ERROR) {
        macr_trie->last_added_node = search_trie(&macr_trie->trie, macr_name);
    } else {
        /* Process failed */
        free(macr_data);
    }
    return status;
}

/**
 * Adds a line to the last added macro.
 *
 * @param macr_trie Pointer to the Macro Trie structure.
 * @param line The line to be added.
 * @return TRUE if the line is added successfully, FALSE otherwise.
 */
void add_line_to_last_macro(MacroTrie *macr_trie, const char *line) {
    MacroData *macr_data = NULL;
    LineNode *new_line_node = NULL;

    /* Null pointer */
    if (!macr_trie->last_added_node) return;

    macr_data = (MacroData *)macr_trie->last_added_node->data;
    new_line_node = (LineNode *)malloc(sizeof(LineNode));

    /* Memory allocation failure */
    if (!new_line_node) {
        set_general_error(MEMORY_ALLOCATION_ERROR);
        return;
    }

    /* Copy line to macro data structure */
    strncpy(new_line_node->line, line, MAX_LINE_LENGTH);
    new_line_node->next = NULL;

    /* Update the linked list */
    if (macr_data->tail) {
        macr_data->tail->next = new_line_node;
        macr_data->tail = new_line_node;
    } else {
        macr_data->head = new_line_node;
        macr_data->tail = new_line_node;
    }
}

/**
 * Finds a macro in the Macro Trie.
 *
 * @param macr_trie Pointer to the Macro Trie structure.
 * @param macr_name The name of the macro.
 * @return Pointer to the TrieNode representing the macro, or NULL if not found.
 */
TrieNode* find_macro(const MacroTrie *macr_trie, const char *macr_name) {
    return search_trie(&macr_trie->trie, macr_name);
}

/**
 * Static function - frees a LineNode linked list associated with a Macro
 * Trie node.
 *
 * @param head Pointer to the head of the LineNode linked list.
 */
static void free_line_nodes(LineNode *head) {
    LineNode *current = head;
    LineNode *next;

    /* Iterate through list*/
    while (current) {
        next = current->next;
        /* Free nodes */
        free(current);
        current = next;
    }
}

/**
 * Static function - frees the data associated with a Macro Trie node.
 *
 * @param data Pointer to the data to be freed.
 */
static void free_macr_data(void *data) {
    MacroData *macro_data = NULL;

    if (data) { /* If this is not a null pointer */
        macro_data = (MacroData *)data;
        /* Free macros data */
        free_line_nodes(macro_data->head);
        free(macro_data);
    }
}

/**
 * Static function - frees all data in the trie without freeing the nodes.
 * Leaving the only necessary data for the following steps.
 *
 * @param node Pointer to the root of the trie.
 */
void free_data_recursive(TrieNode* node) {
    int i;
    if (node) {
        /* Iterate through all nodes children */
        for (i = 0; i < ALPHABET_SIZE; i++) {
            if (node->children[i]) {
                free_data_recursive(node->children[i]);
            }
        }
        /* Free all associated data with this node, if exists */
        if (node->data) {
            free_macr_data(node->data);
            node->data = NULL;
        }
    }
}

/**
 * Frees all data in the trie without freeing the nodes.
 *
 * @param macr_trie Pointer to the Macro Trie structure.
 */
void free_trie_data(MacroTrie *macr_trie) {
    free_data_recursive(macr_trie->trie.root);
}

/**
 * Frees the entire Macro Trie.
 *
 * @param macr_trie Pointer to the Macro Trie structure.
 */
void free_macr_trie(MacroTrie *macr_trie) {
    free_node(macr_trie->trie.root);
}