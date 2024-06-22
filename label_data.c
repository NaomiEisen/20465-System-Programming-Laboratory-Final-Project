
#include "label_data.h"

boolean init_label_trie(Trie *trie) {
    return init_trie(trie);
}

/* Insert a label into the trie */
boolean insert_label(Trie *trie, const char *label, int address, LabelType label_type) {
    LabelData *label_data = (LabelData *)malloc(sizeof(LabelData));
    if (!label_data) {
        return FALSE;
    }
    label_data->address = address;
    label_data->label_type = label_type;

    return insert_to_trie(trie, label, label_data);
}

/* Get the address of a label from the trie */
int get_label_address(Trie *trie, const char *label, LabelType *label_type) {
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

    if (label_type) {
        *label_type = label_data->label_type;
    }

    return label_data->address;
}

/* Set the label type for an existing label in the trie */
boolean set_label_type(Trie *trie, const char *label, LabelType label_type) {
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

/* Free the entire label trie */
void free_label_tree(Trie *trie) {
    if (trie->root) {
        free_node(trie->root);
        trie->root = NULL;
        trie->node_count = 0;
    }
}
