
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
boolean init_macr_trie(MacroTrie *macr_trie);
/* Insert a macro name into the Macro Trie */
boolean add_macr(MacroTrie *macr_trie, const char *macr_name);
/* Get the content of a macro  */
LineNode* get_macr_content(MacroTrie *macr_trie, const char *macr_name);
/* Add a line to the last added macro */
boolean add_line_to_last_macro(MacroTrie *macr_trie, const char *line);
TrieNode* find_macro(MacroTrie *macr_trie, const char *macr_name);
/* Free a LineNode linked list */
void free_line_nodes(LineNode *head);
/* Free the data associated with a Macro Trie node */
void free_macr_data(void *data);
/* Function to free all data in the trie without freeing the nodes */
void free_data_recursive(TrieNode* node);
/* Function to free all data in the trie without freeing the nodes */
void free_trie_data(MacroTrie *macr_trie);
/* Free the entire Macro Trie */
void free_macr_trie(MacroTrie *macr_trie);


#endif /* MACRO_LIST_H */
