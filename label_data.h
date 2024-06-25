#ifndef LABEL_DATA_H
#define LABEL_DATA_H
#include "trie.h"


/* Enum for label types */
typedef enum {
    DIRECTIVE,
    OPERATION,
    EXTERNAL,
    ENTERNAL,
    DEFAULT
} LabelType;


typedef struct AddressList {
    int address;
    struct AddressList *next;
} AddressList;

/* Trie node structure */
typedef struct LabelData {
    union {
        int single_address;
        AddressList *address_list;
    } address;
    LabelType label_type;
} LabelData;


boolean init_label_trie(Trie *trie);

/* Insert a label into the trie */
boolean insert_single_addr_label(Trie *trie, const char *label, int address, LabelType label_type);

/* Get the address of a label from the trie */
int get_label_single_addr(Trie *trie, const char *label, LabelType *label_type);
/* Set the label type for an existing label in the trie */
boolean set_label_type(Trie *trie, const char *label, LabelType label_type);
void free_label_tree(Trie *trie);
boolean add_addr(Trie *trie, const char *label, int address);
boolean insert_list_addr_label(Trie *trie, const char *label);
void collectNodesByType(Trie *label_trie, char type, char *prefix, int level);

#endif /* LABEL_DATA_H */
