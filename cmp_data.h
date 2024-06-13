#ifndef CMP_DATA_H
#define CMP_DATA_H
#include "hardware.h"
#include "label_trie.h"

/* structure containing the sets and their mappings within the program */
typedef struct {
    MemoryImage code;
    MemoryImage data;
    Trie label_table;
} CmpData;

void init_cmp_data(CmpData *data);

#endif /* CMP_DATA_H */
