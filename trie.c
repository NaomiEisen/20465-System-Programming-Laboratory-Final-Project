/* ---------------------------------------------------------------------------------------
 *                                          Includes
 * --------------------------------------------------------------------------------------- */
#include "trie.h"
#include "errors.h"
/* ---------------------------------------------------------------------------------------
 *                                          Functions
 * --------------------------------------------------------------------------------------- */
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
TrieNode* create_trie_node() {
    int i; /* Variable to iterate through loop */
    TrieNode *node = (TrieNode *)malloc(sizeof(TrieNode));

    if (node) { /* If memory allocation succeeded */
        for (i = 0; i < ALPHABET_SIZE; i++) {
            /* Set all child pointer in the children array to NULL */
            node->children[i] = NULL;
        }
        /* Empty node */
        node->exist = FALSE;
        node->data = NULL;
    }
    return node;
}

/**
 * Function to initialize an empty Trie.
 *
 * @param trie A pointer to the Trie to be initialized.
 * @return Boolean TRUE if the Trie is successfully initialized; FALSE otherwise.
 */
Boolean init_trie(Trie *trie) {
    /* Create empty trie node for thr root */
    trie->root = create_trie_node();

    /* Memory allocation failure */
    if (!trie->root) {
        return FALSE;
    }
    /* Trie is successfully initialized */
    return TRUE;

}

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
Boolean insert_to_trie(Trie *trie, const char *str, void *data) {
    int index; /* Current char index */
    TrieNode *node = trie->root;

    while (*str) {
        index = get_index(*str); /* Get the relevant index */
        if (index == -1) {
            /* Invalid character in str */
            return FALSE;
        }
        if (!node->children[index]) {
            /* Create the node if it does not exist */
            node->children[index] = create_trie_node();
        }
        node = node->children[index];
        str++;
    }
    node->exist = TRUE; /* set existing node to true */
    node->data = data; /* attach the data */
    return TRUE;
}

/**
 * Function to get the index of a character in the trie.
 *
 * This function determines the index of a given character based on predefined offsets
 * for different character types: lowercase letters, uppercase letters, digits, and
 * specific special characters ('-', '_', and '.').
 *
 * @param c The character whose index is to be determined.
 * @return int The index of the character if it is valid; otherwise, returns -1.
 */
int get_index(char c) {
    if (c >= 'a' && c <= 'z') {
        return c - 'a' + LOWERCASE_OFFSET; /* Lowercase letters */
    } else if (c >= 'A' && c <= 'Z') {
        return c - 'A' + UPPERCASE_OFFSET; /* Uppercase letters */
    } else if (c >= '0' && c <= '9') {
        return c - '0' + DIGIT_OFFSET; /* Digits */
    } else if (c == '-') {
        return 62; /* Special character: - */
    } else if (c == '_') {
        return 63; /* Special character: _ */
    } else if (c == '.') {
        return 64; /* Special character: . */
    } else {
        return -1; /* Invalid character */
    }
}

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
TrieNode* search_trie(const Trie *trie, const char *label) {
    TrieNode *node;
    if (trie == NULL) {
        return NULL; /* if the trie pointer is NULL, return */
    }

    node = trie->root;
    while (*label) {
        int index = get_index(*label);
        if (index == -1 || !node->children[index]) {
            return NULL; /* Label not found or invalid character */
        }
        node = node->children[index];
        label++;
    }

    if (node && node->exist) {
        return node; /* Return the node if it exists */
    }
    return NULL; /* If node was not found */
}

/**
 * Function to free a node in the trie.
 *
 * This function recursively frees all child nodes of the given trie node.
 * It also frees any data associated with the node using the `free_data` function.
 * Finally, it frees the node itself.
 *
 * @param node A pointer to the TrieNode to be freed.
 */
void free_node(TrieNode *node) {
    int i;  /* Variable to iterate through loop */
    if (node) {
        for (i = 0; i < ALPHABET_SIZE; i++) {
            if (node->children[i]) {
                /* Free nodes recursively */
                free_node(node->children[i]);
            }
        }
        if (node->data) {
            /* Free node's data */
            free_data(node->data);
            /* Set pointer to NULL */
            node->data = NULL;
        }
        /* Free node itself */
        free(node);
    }
}

/**
 * Function to free the node's data.
 *
 * @param data A pointer to the data to be freed.
 */
void free_data(void *data) {
    free(data);
}

/* --------------------------------------------------------------------------------------- */

/* Function to print all words in the trie */
void print_trie(TrieNode *node, char *word_so_far) {
    int i;
    char *new_word;
    char new_char[2];

    if (!node) {
        return;
    }

    if (node->exist) {
        printf("%s\n", word_so_far); /* Print the current word */
    }

    for (i = 0; i < ALPHABET_SIZE; i++) {
        if (node->children[i]) {
            new_char[0] = (i < 26) ? 'a' + i : ((i < 52) ? 'A' + i - 26 : '0' + i - 52);
            new_char[1] = '\0';

            /* Allocate memory for the new word */
            new_word = (char *)malloc(strlen(word_so_far) + 2); /* +2 for new char and null terminator */
            if (new_word) {
                strcpy(new_word, word_so_far);
                strcat(new_word, new_char);
                print_trie(node->children[i], new_word);
                free(new_word);
            }
        }
    }
}



