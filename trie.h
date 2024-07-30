#ifndef TRIE_H
#define TRIE_H
/* ----------------------------------------- Includes -----------------------------------------*/
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <stdbool.h>
#include "boolean.h"
#include "errors.h"
/* ------------------------------------------ Defines ------------------------------------------*/
/* 26 lowercase + 26 uppercase + 10 digits + 3 special characters */
#define ALPHABET_SIZE 65

/* Index offsets */
#define LOWERCASE_OFFSET 0
#define UPPERCASE_OFFSET 26
#define DIGIT_OFFSET 52
/* ---------------------------------------- Structures ----------------------------------------*/

/* Trie node structure */
typedef struct TrieNode {
    struct TrieNode *children[ALPHABET_SIZE];       /* Array of pointers to child nodes */
    Boolean exist; /* Flag indicating if this node represents the end of a valid string */
    void *data;                             /* Generic pointer to store additional data */
} TrieNode;

/* Trie structure */
typedef struct Trie {
    TrieNode *root;  /* Pointer to the root node of the trie */
} Trie;

/* ----------------------------------- Functions Prototypes ---------------------------------- */
/**
 * Function to create a new TrieNode.
 *
 * This function allocates memory for a new TrieNode and initializes its members.
 * The TrieNode is initialized to an empty state:
 * Each child pointer in the children array is set to NULL, the `exist` flag is set to FALSE,
 * and the `data` pointer is set to NULL.
 *
 * @return TrieNode* A pointer to the newly created TrieNode. If the allocation
 *         fails, the function returns NULL.
 */
TrieNode* create_trie_node();

/**
 * Function to initialize an empty Trie.
 *
 * @param trie A pointer to the Trie to be initialized.
 * @return Boolean TRUE if the Trie is successfully initialized; FALSE otherwise.
 */
Boolean init_trie(Trie *trie);

/**
 * Function to insert a string into the trie with generic data.
 *
 * The function uses the characters of the input string to navigate through the trie.
 * When the position for the string is found, the function marks the final node as existing
 * and attaches the provided data to it.
 *
 * @param trie A pointer to the Trie in which the string will be inserted.
 * @param str The string to be inserted into the trie.
 * @param data A pointer to the generic data to be stored at the final node of the string.
 * @return Boolean TRUE if the string is successfully inserted; otherwise, FALSE.
 */
ErrorCode insert_to_trie(Trie *trie, const char *str, void *data);


/**
 * Function to search for a node in the trie.
 *
 * This function searches the trie for a node corresponding to the given label.
 * It navigates through the trie using the characters of the label. If a node
 * corresponding to the entire label is found and it exists, the function returns
 * a pointer to that node. Otherwise, it returns NULL.
 *
 * @param trie A pointer to the Trie in which to search for the label.
 * @param label The label to be searched in the trie.
 * @return TrieNode* A pointer to the found node if the label exists; otherwise, NULL.
 */
TrieNode* search_trie(const Trie *trie, const char *label);
/**
 * Function to free a node in the trie.
 *
 * This function recursively frees all child nodes of the given trie node.
 * It also frees any data associated with the node using the `free_data` function.
 * Finally, it frees the node itself.
 *
 * @param node A pointer to the TrieNode to be freed.
 */
void free_node(TrieNode *node);
/**
 * Function to free the node's data.
 *
 * @param data A pointer to the data to be freed.
 */
void free_data(void *data);


void print_trie(TrieNode *node, char *word_so_far);

#endif /* TRIE_H */
