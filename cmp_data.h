#ifndef CMP_DATA_H
#define CMP_DATA_H
#include "label_trie.h"
#include "mappings.h"

#define MEMORY_CAPACITY 4096
#define NUM_OF_BYTES 2


typedef struct MemoryImage{
    char lines[MEMORY_CAPACITY][NUM_OF_BYTES];
    int count;
} MemoryImage;


typedef struct CmpData{
    MemoryImage code;
    MemoryImage data;
    Trie label_table;
    int phase;
} CmpData;

/**
 * Initializes the CmpData structure by setting code and data memory to zeros
 * and initializing the label table with an empty root node.
 * @param data Pointer to the CmpData structure to be initialized
 */
void init_cmp_data(CmpData *data);
void print_memory_image(const MemoryImage *memory_image);
void print_memory_image_marks(const MemoryImage *memory_image);
void print_ten(const MemoryImage *memory_image);

#endif /* CMP_DATA_H */
