#ifndef LABLE_TRIE_H
#define LABLE_TRIE_H

#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <stdbool.h>

/* Define the number of possible characters (assuming ASCII) */
#define ALPHABET_SIZE 256

/* Trie node structure */
typedef struct TrieNode {
    struct TrieNode *children[ALPHABET_SIZE];
    int isEndOfLabel;
    int address;
} TrieNode;

/* Struct to hold the root of the trie */
typedef struct Trie {
    TrieNode *root;
} Trie;

/* Global lable trie */
/*Trie lable_trie = { NULL };*/


/* Function to create a new TrieNode */
TrieNode *createTrieNode();
/* Function to insert a label and its address into the trie */
void insert(Trie *trie, const char *label, int address);
/* Function to search for a label in the trie and return its address */
int search(Trie *trie, const char *label);

/* Function to free the trie */
void freeTrie(TrieNode *node);

#endif /* LABLE_TRIE_H */
