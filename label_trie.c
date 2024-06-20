#include "label_trie.h"

/* Function to create a new TrieNode */
TrieNode* create_trie_node() {
    int i;
    TrieNode *node = (TrieNode *)malloc(sizeof(TrieNode));
    if (node) {
        node->address = -1;
        node->label_type = DEFAULT; /* Default value */
        node->is_label = false;
        for (i = 0; i < ALPHABET_SIZE; i++) {
            node->children[i] = NULL;
        }
    }
    return node;
}

/* Function to get the index of a character in the trie */
int get_index(char c) {
    if (c >= 'a' && c <= 'z') {
        return c - 'a'; /* Lowercase letters*/
    } else if (c >= 'A' && c <= 'Z') {
        return c - 'A' + 26; /*/ Uppercase letters*/
    } else if (c >= '0' && c <= '9') {
        return c - '0' + 52; /* Digits*/
    } else {
        return -1; /* Invalid character*/
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
            return FALSE;
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
        return FALSE;
    }

    /* Insert the new label */
    node->address = address;
    node->label_type = label_type;
    node->is_label = TRUE;
    trie->node_count++;
    return TRUE;
}

/* Function to search for a label in the trie and return its address */
int get_label_address(Trie *trie, const char *label, LabelType *label_type) {
    TrieNode *node = trie->root;
    while (*label) {
        int index = get_index(*label);
        if (index == -1) {
            return -1; /* Invalid character */
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

/* Function to set the label type for a given label in the Trie */
boolean set_label_type(Trie *trie, const char *label, LabelType label_type) {
    TrieNode *current = trie->root;
    int len = strlen(label);
    int i;

    for (i = 0; i < len; i++) {
        int index = label[i] - 'a'; /* Assuming lowercase letters */
        if (!current->children[index]) {
            current->children[index] = create_trie_node();
        }
        current = current->children[index];
    }

    /* Check if the label already exists */
    if (current->is_label) {
        /* Label already exists, set the label type */
        current->label_type = label_type;
        return TRUE; /* Process succeeded */
    } else {
        /* Label does not exist */
        return FALSE; /* Process failed */
    }
}


/* Function to print all words in the trie */
void print_trie(TrieNode *node, char *word_so_far) {
    int i;
    char *new_word;
    char new_char[2];

    if (!node) {
        return;
    }

    if (node->is_label) {
        printf("Word: %-15s | Address: %-5d | Label Type: ", word_so_far, node->address);
        switch (node->label_type) {
            case DIRECTIVE:
                printf("Directive\n");
                break;
            case OPERATION:
                printf("Operation\n");
                break;
            case EXTERNAL:
                printf("External\n");
                break;
            default:
                printf("Unknown\n");
                break;
        }
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

/* Function to free the nodes in the trie */
void free_trie_node(TrieNode *node) {
    int i;
    if (node) {
        for (i = 0; i < ALPHABET_SIZE; i++) {
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
