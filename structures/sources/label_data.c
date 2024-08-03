/* ---------------------------------------------------------------------------------------
 *                                        Includes
 * --------------------------------------------------------------------------------------- */
#include "../../structures/headers/label_data.h"
/* ---------------------------------------------------------------------------------------
 *                                        Functions
 * --------------------------------------------------------------------------------------- */
/**
 * Initialize the label trie.
 * This function sets up the trie for storing labels, it uses the init_trie method of the
 * generic trie structure.
 *
 * @param trie Pointer to the trie structure.
 * @return TRUE if the initialization was successful, FALSE otherwise.
 */
Boolean init_label_trie(Trie *trie) {
    return init_trie(trie);
}

/**
 * Insert a label into the trie with its address and type.
 * Allocates memory for label data and inserts it into the trie.
 * Uses the generic trie structure methods.
 *
 * @param trie Pointer to the trie structure.
 * @param label The label to be inserted.
 * @param address The address associated with the label.
 * @param label_type The type of the label.
 * @return ErrorCode - Error code of the error that occurred or NO_ERROR if the process
 *          executed successfully.
 */
ErrorCode insert_label(Trie *trie, const char *label, int address, LabelType label_type) {
    /* Allocate memory for the labelData struct */
    LabelData *label_data = (LabelData *)malloc(sizeof(LabelData));
    if (!label_data) return MEMORY_ALLOCATION_ERROR;

    /* Set the specified values */
    label_data->address = address;
    label_data->label_type = label_type;

    /* Insert the label in the trie and return the process status */
    return insert_to_trie(trie, label, label_data);
}

/**
 * Retrieve the address of a label from the trie.
 * Searches the trie for the label and returns its address.
 *
 * @param trie Pointer to the trie structure.
 * @param label The label whose address is to be retrieved.
 * @return The address of the label if found, -1 otherwise.
 */
int get_label_addr(Trie *trie, const char *label) {
    TrieNode *node;
    LabelData *label_data;

    /* Search for the label in the trie */
    node = search_trie(trie, label);
    if (!node) return -1;/* Label was not found */

    label_data = (LabelData *)node->data;

    /* label does not contain any associated data */
    if (!label_data) return -1;

    /* Return labels address */
    return label_data->address;
}

/**
 * Retrieve the type of a label from the trie.
 * Searches the trie for the label and returns its type.
 *
 * @param trie Pointer to the trie structure.
 * @param label The label whose type is to be retrieved.
 * @return The type of the label if found, -1 otherwise.
 */
LabelType get_label_type(Trie *trie, const char *label) {
    TrieNode *node;
    LabelData *label_data;

    /* Search for the label in the trie */
    node = search_trie(trie, label);
    if (!node) return -1;/* Label was not found */

    label_data = (LabelData *)node->data;
    /* label does not contain any associated data */
    if (!label_data) return -1;

    /* return labels type */
    return label_data->label_type;
}

/**
 * Set the label type for an existing label in the trie.
 * Searches for the label in the trie and updates its type.
 *
 * @param trie Pointer to the trie structure.
 * @param label The label whose type is to be set.
 * @param label_type The new type to be assigned to the label.
 * @return TRUE if the label type was successfully set, FALSE otherwise.
 */
Boolean set_label_type(Trie *trie, const char *label, LabelType label_type) {
    TrieNode *node;
    LabelData *label_data;

    /* Search for the label */
    node = search_trie(trie, label);

    if (!node) return FALSE; /* Label was not found */

    label_data = (LabelData *)node->data;

    /* No data associated with this label */
    if (!label_data) return FALSE;

    /* Set the specified type */
    label_data->label_type = label_type;
    return TRUE; /* Process executed successfully */
}

/**
 * Update the addresses of nodes of a specified type.
 * Increments the addresses of nodes of the given type by the specified amount.
 *
 * @param node Pointer to the current trie node.
 * @param increment The amount to increment the addresses by.
 * @param type The type of labels to update.
 */
void update_addr(TrieNode *node, int increment, LabelType type) {
    int i;
    LabelData *data;

    if (!node) return; /* Null pointer */

    data = (LabelData *)node->data;

    /* If data exists and it is the accurate label type */
    if (data && data->label_type == type) {
        data->address += increment;
    }

    /* Recursively update child nodes */
    for (i = 0; i < ALPHABET_SIZE; ++i) {
        if (node->children[i] != NULL) {
            update_addr(node->children[i], increment, type);
        }
    }
}

/**
 * Free the entire label trie.
 * Frees the memory for all nodes in the trie and resets the struct pointers to null.
 *
 * @param trie Pointer to the trie structure.
 */
void free_label_tree(Trie *trie) {
    if (trie->root) {
        free_node(trie->root);
        trie->root = NULL;
    }
}

