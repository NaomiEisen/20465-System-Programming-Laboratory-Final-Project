#include "cmp_data.h"

/**
 * Function to initialize the ProgramData structure.
 * Initializes the sets to be empty sets and sets the corresponding pointers
 * of the sets to the setMappings.
 * @param data Pointer to the ProgramData to be initialized
 */
/* Function to initialize CmpData */
void init_cmp_data(CmpData *data) {
    /* Initialize code memory */
    for (int i = 0; i < MEMORY_CAPACITY; i++) {
        for (int j = 0; j < NUM_OF_BYTES; j++) {
            data->code.lines[i][j] = 0;
        }
    }
    data->code.count = 0;

    /* Initialize data memory */
    for (int i = 0; i < MEMORY_CAPACITY; i++) {
        for (int j = 0; j < NUM_OF_BYTES; j++) {
            data->data.lines[i][j] = 0;
        }
    }
    data->data.count = 0;

    /* Initialize the label table*/
    data->label_table.root = create_trie_node();
    data->label_table.node_count = 0;
}