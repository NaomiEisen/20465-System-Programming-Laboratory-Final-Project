#include "trie.h"

/* Function to create a new TrieNode */
TrieNode* create_trie_node() {
    int i;
    TrieNode *node = (TrieNode *)malloc(sizeof(TrieNode));
    if (node) {
        for (i = 0; i < ALPHABET_SIZE; i++) {
            node->children[i] = NULL;
        }
        node->exist = FALSE;
        node->data = NULL;
    }
    return node;
}

/* Function to get the index of a character in the trie */
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
/* Function to insert a string into the trie with generic data */
boolean insert_to_trie(Trie *trie, const char *str, void *data) {
    int index;
    TrieNode *node = trie->root;

    while (*str) {
        index = get_index(*str);
        if (index == -1) {
            /* Invalid character in str */
            return FALSE;
        }
        if (!node->children[index]) {
            /* Create the node if it does not exist */
            node->children[index] = create_trie_node();
            trie->node_count++;
        }
        node = node->children[index];
        str++;
    }

    node->exist = TRUE;
    node->data = data;
    return TRUE;
}

/* Function to search for a node in the trie */
TrieNode* search_trie(Trie *trie, const char *label) {
    TrieNode *node;
    if (trie == NULL) {
        return NULL;
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
    return NULL;
}

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


/* Free a node in the trie */
void free_node(TrieNode *node) {
    int i;
    if (node) {
        for (i = 0; i < ALPHABET_SIZE; i++) {
            if (node->children[i]) {
                free_node(node->children[i]);
            }
        }
        if (node->data) {
            free_data(node->data);
            node->data = NULL;
        }
        free(node);
    }
}


/* Function to free a LabelNode */
void free_data(void *data) {
    free(data);
}

/* Example usage */
boolean init_trie(Trie* trie) {
    trie->root = create_trie_node();

    if (trie->root == NULL) {
       return FALSE;
    }

    trie->node_count = 0;
    return TRUE;

}