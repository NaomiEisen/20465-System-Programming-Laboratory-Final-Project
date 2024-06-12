#include "label_trie.h"


Trie trie_label = { NULL, 0 };

/* Function to create a new TrieNode */
TrieNode* create_trie_node() {
    TrieNode *node = (TrieNode *)malloc(sizeof(TrieNode));
    if (node) {
        node->address = -1;
        node->label_type = DIRECTIVE; /* Default value */
        node->is_label = false;
        for (int i = 0; i < ALPHABET_SIZE; i++) {
            node->children[i] = NULL;
        }
    }
    return node;
}

/* Function to get the index of a character in the trie */
int get_index(char c) {
    if (c >= 'a' && c <= 'z') {
        return c - 'a'; // Lowercase letters
    } else if (c >= 'A' && c <= 'Z') {
        return c - 'A' + 26; // Uppercase letters
    } else if (c >= '0' && c <= '9') {
        return c - '0' + 52; // Digits
    } else {
        return -1; // Invalid character
    }
}

/* Function to insert a label and its address into the trie */
boolean insert_label(Trie *trie, const char *label, int address, LabelType label_type) {
    int index;
    TrieNode *node = NULL;

    if (!trie->root) {
        trie->root = create_trie_node();
    }
    node = trie->root;

    while (*label) {
        index = get_index(*label);
        if (index == -1) {
            /* Invalid character in label */
            return false;
        }
        if (!node->children[index]) {
            /* Create the node if it does not exist */
            node->children[index] = create_trie_node();
        }
        node = node->children[index];
        label++;
    }

    /* Check if the label already exists */
    if (node->is_label) {
        return false;
    }

    /* Insert the new label */
    node->address = address;
    node->label_type = label_type;
    node->is_label = true;
    trie->node_count++;
    return true;
}

/* Function to search for a label in the trie and return its address */
int search_label(Trie *trie, const char *label, LabelType *label_type) {
    TrieNode *node = trie->root;
    while (*label) {
        int index = get_index(*label);
        if (index == -1) {
            return -1; // Invalid character
        }
        if (!node->children[index]) {
            return -1; /* Label not found */
        }
        node = node->children[index];
        label++;
    }
    if (node && node->is_label) {
        if (label_type) {
            *label_type = node->label_type;
        }
        return node->address;
    }
    return -1;
}

/* Function to print all nodes in the trie */
void print_trie(TrieNode *node, char *prefix) {
    if (!node) {
        return;
    }

    if (node->is_label) {
        printf("%-15d%s\n", node->address, prefix);
    }

    for (int i = 0; i < ALPHABET_SIZE; i++) {
        if (node->children[i]) {
            char new_prefix[2] = { (i < 26) ? 'a' + i : ((i < 52) ? 'A' + i - 26 : '0' + i - 52), '\0' };
            char new_word[strlen(prefix) + 2];
            strcpy(new_word, prefix);
            strcat(new_word, new_prefix);
            print_trie(node->children[i], new_word);
        }
    }
}

/* Function to free the nodes in the trie */
void free_trie_node(TrieNode *node) {
    if (node) {
        for (int i = 0; i < ALPHABET_SIZE; i++) {
            if (node->children[i]) {
                free_trie_node(node->children[i]);
            }
        }
        free(node);
    }
}

/* Function to free the trie */
void free_trie(Trie *trie) {
    free_trie_node(trie->root);
    trie->root = NULL;
    trie->node_count = 0;
}
