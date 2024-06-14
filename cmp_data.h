#ifndef CMP_DATA_H
#define CMP_DATA_H
#include "hardware.h"
#include "label_trie.h"

#define MEMORY_CAPACITY 4096
#define NUM_OF_BYTES 2


typedef struct {
    char lines[MEMORY_CAPACITY][NUM_OF_BYTES];
    int count;
} MemoryImage;


/* structure containing the sets and their mappings within the program */
typedef struct {
    MemoryImage code;
    MemoryImage data;
    Trie label_table;
} CmpData;

/**
 * Initializes the CmpData structure by setting code and data memory to zeros
 * and initializing the label table with an empty root node.
 * @param data Pointer to the CmpData structure to be initialized
 */
void init_cmp_data(CmpData *data);

#endif /* CMP_DATA_H */
