#include "lable_trie.h"

/* Function to create a new TrieNode */
TrieNode *createTrieNode() {
    TrieNode *newNode = (TrieNode *)malloc(sizeof(TrieNode));
    if (!newNode) {
        fprintf(stderr, "Memory allocation error\n");
        exit(EXIT_FAILURE);
    }
    newNode->isEndOfLabel = 0; /* Initialize as not the end of a label */
    newNode->address = -1; /* Initialize address to -1 (invalid address) */
    for (int i = 0; i < ALPHABET_SIZE; i++) {
        newNode->children[i] = NULL;
    }
    return newNode;
}

/* Function to insert a label and its address into the trie */
void insert(Trie *trie, const char *label, int address) {
    TrieNode *node = trie->root;
    while (*label) {
        if (!node->children[(unsigned char)*label - 'a']) {
            node->children[(unsigned char)*label - 'a'] = createTrieNode();
        }
        node = node->children[(unsigned char)*label - 'a'];
        label++;
    }
    node->isEndOfLabel = 1; /* Mark as end of label */
    node->address = address;
}

/* Function to search for a label in the trie and return its address */
int search(Trie *trie, const char *label) {
    TrieNode *node = trie->root;
    while (*label) {
        node = node->children[(unsigned char)*label - 'a'];
        if (!node) {
            return -1; /* Label not found */
        }
        label++;
    }
    return (node && node->isEndOfLabel) ? node->address : -1;
}

/* Function to free the trie */
void freeTrie(TrieNode *node) {
    int i;
    if (!node) return;
    for (i = 0; i < ALPHABET_SIZE; i++) {
        if (node->children[i]) {
            freeTrie(node->children[i]);
        }
    }
    free(node);
}
