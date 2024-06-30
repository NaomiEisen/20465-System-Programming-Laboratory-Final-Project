#ifndef LABEL_DATA_H
#define LABEL_DATA_H
#include "trie.h"


/* Enum for label types */
typedef enum {
    DIRECTIVE,
    INSTRUCTION,
    EXTERNAL,
    ENTERNAL
} LabelType;


/*typedef struct AddressList {
    int address;
    struct AddressList *next;
} AddressList;*/

/* Trie node structure */
typedef struct LabelData {
    int address;
    LabelType label_type;
} LabelData;


Boolean init_label_trie(Trie *trie);

/* Insert a label into the trie */
Boolean insert_single_addr_label(Trie *trie, const char *label, int address, LabelType label_type);

/* Get the address of a label from the trie */
int get_label_single_addr(Trie *trie, const char *label);
/* Set the label type for an existing label in the trie */
Boolean set_label_type(Trie *trie, const char *label, LabelType label_type);
void free_label_tree(Trie *trie);
LabelType get_label_type(Trie *trie, const char *label);
void updt_addr(TrieNode *node, int increment, LabelType type);

#endif /* LABEL_DATA_H */
