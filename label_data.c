
#include "label_data.h"


boolean init_label_trie(Trie *trie) {
    return init_trie(trie);
}

/* Insert a label into the trie */
boolean insert_single_addr_label(Trie *trie, const char *label, int address, LabelType label_type) {
    LabelData *label_data = (LabelData *)malloc(sizeof(LabelData));
    if (!label_data) {
        return FALSE;
    }
    label_data->address.single_address = address;
    label_data->label_type = label_type;

    return insert_to_trie(trie, label, label_data);
}

/* Get the address of a label from the trie */
int get_label_single_addr(Trie *trie, const char *label, LabelType *label_type) {
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

    if (label_data->label_type == EXTERNAL) {
        return 0;
    }

    return label_data->address.single_address;
}

/* Insert a label with an empty address list */
boolean insert_list_addr_label(Trie *trie, const char *label) {
    LabelData *labelData = (LabelData *)malloc(sizeof(LabelData));
    if (labelData == NULL) {
        return FALSE; /* Memory allocation failed */
    }

    labelData->label_type = EXTERNAL;
    labelData->address.address_list = NULL; // Initialize with empty address list

    return insert_to_trie(trie, label, labelData);
}

/* Add an address to an existing label's address list */
boolean add_addr(Trie *trie, const char *label, int address) {
    LabelData *labelData;
    TrieNode *node = search_trie(trie, label);
    AddressList *newAddress = (AddressList *)malloc(sizeof(AddressList));

    if (newAddress == NULL) {
        return FALSE; /* Memory allocation failed */
    }

    if (node == NULL || node->data == NULL) {
        return FALSE; /* Label not found in trie */
    }

    labelData = (LabelData *)node->data;

    if (labelData->label_type != EXTERNAL) {
        return FALSE; /* Label is not of list address type */
    }

    newAddress->address = address;
    newAddress->next = labelData->address.address_list;
    labelData->address.address_list = newAddress;
    return TRUE;
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

