#ifndef DATA_H
#define DATA_H
#include "trie.h"


/* Enum for label types */
typedef enum {
    DIRECTIVE,
    OPERATION,
    EXTERNAL,
    ENTERNAL,
    DEFAULT
} LabelType;

/* Trie node structure */
typedef struct LabelData {
    int address;
    LabelType label_type;
} LabelData;


boolean init_label_trie(Trie *trie);

/* Insert a label into the trie */
boolean insert_label(Trie *trie, const char *label, int address, LabelType label_type);

/* Get the address of a label from the trie */
int get_label_address(Trie *trie, const char *label, LabelType *label_type);
/* Set the label type for an existing label in the trie */
boolean set_label_type(Trie *trie, const char *label, LabelType label_type);
void free_label_tree(Trie *trie);
#endif /* DATA_H */
