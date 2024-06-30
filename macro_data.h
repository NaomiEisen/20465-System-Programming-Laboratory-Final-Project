
#ifndef MACRO_LIST_H
#define MACRO_LIST_H
#include "trie.h"

#define MAX_LINE_LENGTH 81

/* Define a structure for a single line in the macro value */
typedef struct LineNode {
    char line[MAX_LINE_LENGTH];
    struct LineNode* next;
} LineNode;

/* Define the structure for a macro node in the linked list */
typedef struct MacroData {
    LineNode* head; /* Head of linked list to store lines of macro value */
    LineNode* tail; /* Tail of linked list to store lines of macro value */
} MacroData;

/* Trie node structure */
typedef struct MacroTrie {
    Trie trie;
    TrieNode* last_added_node;
} MacroTrie;


/* Function prototypes */
/* Initialize the Macro Trie */
Boolean init_macr_trie(MacroTrie *macr_trie);
/* Insert a macro name into the Macro Trie */
Boolean add_macr(MacroTrie *macr_trie, const char *macr_name);
/* Get the content of a macro  */
LineNode* get_macr_content(MacroTrie *macr_trie, const char *macr_name);
/* Add a line to the last added macro */
Boolean add_line_to_last_macro(MacroTrie *macr_trie, const char *line);
TrieNode* find_macro(MacroTrie *macr_trie, const char *macr_name);
/* Function to free all data in the trie without freeing the nodes */
void free_trie_data(MacroTrie *macr_trie);
/* Free the entire Macro Trie */
void free_macr_trie(MacroTrie *macr_trie);
void print_trie_test(MacroTrie* macr_trie);

#endif /* MACRO_LIST_H */
