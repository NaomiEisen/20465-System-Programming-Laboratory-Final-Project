#ifndef LABLE_TRIE_H
#define LABLE_TRIE_H

#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <stdbool.h>
#include "boolean.h"
#define ALPHABET_SIZE 62 /* 26 lowercase + 26 uppercase + 10 digits */

/* Enum for label types */
typedef enum {
    DIRECTIVE,
    OPERATION,
    EXTERNAL,
    ENTERNAL,
    DEFAULT
} LabelType;

/* Trie node structure */
typedef struct TrieNode {
    struct TrieNode *children[ALPHABET_SIZE];
    int address;
    LabelType label_type;
    boolean is_label;
} TrieNode;

/* Trie structure */
typedef struct Trie {
    TrieNode *root;
    int node_count;
} Trie;


/* Function to create a new TrieNode */
TrieNode* create_trie_node();

/* Function to insert a label and its address into the trie */
boolean insert_label(Trie *trie, const char *label, int address, LabelType label_type);
/* Function to search for a label in the trie and return its address */
int get_label_address(Trie *trie, const char *label, LabelType *label_type);
boolean set_label_type(Trie *trie, const char *label, LabelType label_type);
/* Function to print all nodes in the trie */
void print_trie(TrieNode *node, char *prefix);

/* Function to free the trie */
void free_trie(Trie *trie);


#endif /* LABLE_TRIE_H */