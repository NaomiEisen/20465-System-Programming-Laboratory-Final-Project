
#include "label_data.h"


Boolean init_label_trie(Trie *trie) {
    return init_trie(trie);
}

/* Insert a label into the trie */
Boolean insert_single_addr_label(Trie *trie, const char *label, int address, LabelType label_type) {
    LabelData *label_data = (LabelData *)malloc(sizeof(LabelData));
    if (!label_data) {
        return FALSE;
    }
    label_data->address = address;
    label_data->label_type = label_type;

    return insert_to_trie(trie, label, label_data);
}

/* Get the address of a label from the trie */
int get_label_single_addr(Trie *trie, const char *label) {
    TrieNode *node;
    LabelData *label_data;

    node = search_trie(trie, label);
    if (node == NULL) {
        return -1;
    }

    label_data = (LabelData *)node->data;
    if (label_data == NULL ) {
        return -1;
    }

    return label_data->address;
}

/* Get the address of a label from the trie */
LabelType get_label_type(Trie *trie, const char *label) {
    TrieNode *node;
    LabelData *label_data;

    node = search_trie(trie, label);
    if (node == NULL) {
        return -1;
    }

    label_data = (LabelData *)node->data;
    if (label_data == NULL ) {
        return -1;
    }

    return label_data->label_type;
}


/* Set the label type for an existing label in the trie */
Boolean set_label_type(Trie *trie, const char *label, LabelType label_type) {
    TrieNode *node;
    LabelData *label_data;

    node = search_trie(trie, label);
    if (node == NULL) {
        return FALSE; /* Label not found */
    }

    label_data = (LabelData *)node->data;
    if (label_data == NULL) {
        return FALSE; /* No data associated with this label */
    }

    label_data->label_type = label_type;
    return TRUE;
}

/* Function to update addresses of nodes of type DIRECTIVE */
void updt_addr(TrieNode *node, int increment, LabelType type) {
    int i;
    LabelData *data;
    if (node == NULL) return;

    data = (LabelData *)node->data;
    if (data && data->label_type == type) {
        data->address += increment;
    }

    /* Recursively update child nodes */
    for (i = 0; i < ALPHABET_SIZE; ++i) {
        if (node->children[i] != NULL) {
            updt_addr(node->children[i], increment, type);
        }
    }
}


/* Free the entire label trie */
void free_label_tree(Trie *trie) {
    if (trie->root) {
        free_node(trie->root);
        trie->root = NULL;
        trie->node_count = 0;
    }
}

