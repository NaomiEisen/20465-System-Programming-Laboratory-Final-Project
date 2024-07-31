#ifndef LABEL_DATA_H
#define LABEL_DATA_H
/* ----------------------------------------- Includes -----------------------------------------*/
#include "trie.h"
/* ---------------------------------------- Structures ----------------------------------------*/
/* Enum for label types */
typedef enum {
    DIRECTIVE,
    INSTRUCTION,
    EXTERNAL,
    ENTERNAL
} LabelType;

/* Trie node structure for label node */
typedef struct LabelData {
    int address;       /* label address */
    LabelType label_type; /* label type */
} LabelData;

/* ----------------------------------- Functions Prototypes ------------------------------------*/
/**
 * Initialize the label trie.
 * This function sets up the trie for storing labels, it uses the init_trie method of the
 * generic trie structure.
 *
 * @param trie Pointer to the trie structure.
 * @return TRUE if the initialization was successful, FALSE otherwise.
 */
Boolean init_label_trie(Trie *trie);

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
ErrorCode insert_label(Trie *trie, const char *label, int address, LabelType label_type);

/**
 * Retrieve the address of a label from the trie.
 * Searches the trie for the label and returns its address.
 *
 * @param trie Pointer to the trie structure.
 * @param label The label whose address is to be retrieved.
 * @return The address of the label if found, -1 otherwise.
 */
int get_label_addr(Trie *trie, const char *label);

/**
 * Retrieve the type of a label from the trie.
 * Searches the trie for the label and returns its type.
 *
 * @param trie Pointer to the trie structure.
 * @param label The label whose type is to be retrieved.
 * @return The type of the label if found, -1 otherwise.
 */
LabelType get_label_type(Trie *trie, const char *label);

/**
 * Set the label type for an existing label in the trie.
 * Searches for the label in the trie and updates its type.
 *
 * @param trie Pointer to the trie structure.
 * @param label The label whose type is to be set.
 * @param label_type The new type to be assigned to the label.
 * @return TRUE if the label type was successfully set, FALSE otherwise.
 */
Boolean set_label_type(Trie *trie, const char *label, LabelType label_type);

/**
 * Update the addresses of nodes of a specified type.
 * Increments the addresses of nodes of the given type by the specified amount.
 *
 * @param node Pointer to the current trie node.
 * @param increment The amount to increment the addresses by.
 * @param type The type of labels to update.
 */
void update_addr(TrieNode *node, int increment, LabelType type);

/**
 * Free the entire label trie.
 * Frees the memory for all nodes in the trie and resets the struct pointers to null.
 *
 * @param trie Pointer to the trie structure.
 */
void free_label_tree(Trie *trie);

#endif /* LABEL_DATA_H */
