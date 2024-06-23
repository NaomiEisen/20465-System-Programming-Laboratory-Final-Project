#ifndef TRIE_H
#define TRIE_H


#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <stdbool.h>
#include "boolean.h"
#define ALPHABET_SIZE 65 /* 26 lowercase + 26 uppercase + 10 digits + 3 special characters */

/* Index offsets */
#define LOWERCASE_OFFSET 0
#define UPPERCASE_OFFSET 26
#define DIGIT_OFFSET 52
#define SPECIAL_OFFSET 62


/* Trie node structure */
typedef struct TrieNode {
    struct TrieNode *children[ALPHABET_SIZE];
    boolean exist;
    void *data;  /* Generic pointer to store additional data */
} TrieNode;

/* Trie structure */
typedef struct Trie {
    TrieNode *root;
    int node_count;
} Trie;

/* Function Prototypes */
TrieNode* create_trie_node();
int get_index(char c);
boolean insert_to_trie(Trie *trie, const char *str, void *data);
TrieNode* search_trie(Trie *trie, const char *label);
void print_trie(TrieNode *node, char *word_so_far);
void free_node(TrieNode *node);
void free_data(void *data);
boolean init_trie(Trie* trie);


#endif /* TRIE_H */
