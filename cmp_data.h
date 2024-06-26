#ifndef CMP_DATA_H
#define CMP_DATA_H
#include "label_data.h"
#include "mappings.h"

#define MEMORY_CAPACITY 4096
#define NUM_OF_BYTES 2


typedef struct MemoryImage{
    char lines[MEMORY_CAPACITY][NUM_OF_BYTES];
    int count;
    int write_ptr;
} MemoryImage;

typedef struct UnresolvedLineList{
    int line;
    struct UnresolvedLineList *next;
} UnresolvedLineList;

typedef struct CmpData{
    MemoryImage code;
    MemoryImage data;
    Trie label_table;
    UnresolvedLineList *line_list;
    FILE* extern_file;
    FILE* entry_file;
} CmpData;

/**
 * Initializes the CmpData structure by setting code and data memory to zeros
 * and initializing the label table with an empty trieNode node.
 * @param data Pointer to the CmpData structure to be initialized
 */
boolean init_cmp_data(CmpData *data, const char *file_name);
void print_memory_image(const MemoryImage *memory_image);
void print_memory_image_marks(const MemoryImage *memory_image);
void print_ten(const MemoryImage *memory_image);
boolean add_unresolved_line(CmpData *data, int line);
int get_unresolved_line(CmpData *data);
void clear_data(CmpData* cmp_data);
void updt_memory_image_counter(MemoryImage *memory_image);

#endif /* CMP_DATA_H */
