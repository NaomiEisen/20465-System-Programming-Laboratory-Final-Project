#ifndef MACRO_LIST_H
#define MACRO_LIST_H
/* ----------------------------------------- Includes -----------------------------------------*/
#include "trie.h"
#include "defines.h"

/* ---------------------------------------- Structures ----------------------------------------*/
/* Define a structure for a single line in the macro value */
typedef struct LineNode {
    char line[MAX_LINE_LENGTH]; /* line in macro value */
    struct LineNode* next; /* pointer to the next line */
} LineNode;

/* Define the structure for a macro data node */
typedef struct MacroData {
    LineNode* head; /* Head of linked list to store lines of macro value */
    LineNode* tail; /* Tail of linked list to store lines of macro value */
} MacroData;

/*
 * Trie node structure. Contains pointer to the last added node to consecutively
 * read macros content into the structure
 */
typedef struct MacroTrie {
    Trie trie; /* Trie to hold the macro names and associated data */
    TrieNode* last_added_node;   /* Pointer to the last added node */
} MacroTrie;

/* ----------------------------------- Functions Prototypes ------------------------------------*/
/**
 * Initialize the Macro Trie.
 *
 * @param macr_trie Pointer to the Macro Trie structure.
 * @return TRUE if initialization is successful, FALSE otherwise.
 */
Boolean init_macr_trie(MacroTrie *macr_trie);

/**
 * Insert a macro name into the Macro Trie.
 *
 * @param macr_trie Pointer to the Macro Trie structure.
 * @param macr_name The name of the macro to be added.
 * @return TRUE if the macro is added successfully, FALSE otherwise.
 */
Boolean add_macr(MacroTrie *macr_trie, const char *macr_name);

/**
 * Add a line to the last added macro.
 *
 * @param macr_trie Pointer to the Macro Trie structure.
 * @param line The line to be added.
 * @return TRUE if the line is added successfully, FALSE otherwise.
 */
void add_line_to_last_macro(MacroTrie *macr_trie, const char *line);

/**
 * Find a macro in the Macro Trie.
 *
 * @param macr_trie Pointer to the Macro Trie structure.
 * @param macr_name The name of the macro.
 * @return Pointer to the TrieNode representing the macro, or NULL if not found.
 */
TrieNode* find_macro(const MacroTrie *macr_trie, const char *macr_name);

/**
 * Free all data in the trie without freeing the nodes.
 *
 * @param macr_trie Pointer to the Macro Trie structure.
 */
void free_trie_data(MacroTrie *macr_trie);

/**
 * Free the entire Macro Trie.
 *
 * @param macr_trie Pointer to the Macro Trie structure.
 */
void free_macr_trie(MacroTrie *macr_trie);

/**
 * Print lines of a macro.
 *
 * @param head Pointer to the head of the LineNode linked list.
 */
void print_lines(LineNode* head);





void print_trie_test(MacroTrie* macr_trie);

#endif /* MACRO_LIST_H */
