#include "cmp_data.h"

/**
 * Function to initialize the ProgramData structure.
 * Initializes the sets to be empty sets and sets the corresponding pointers
 * of the sets to the setMappings.
 * @param data Pointer to the ProgramData to be initialized
 */
/* Function to initialize CmpData */
void init_cmp_data(CmpData *data) {
    int i;
    int j;

    /* Initialize code memory */
    for (i = 0; i < MEMORY_CAPACITY; i++) {
        for (j = 0; j < NUM_OF_BYTES; j++) {
            data->code.lines[i][j] = 0;
        }
    }
    data->code.count = 0;

    /* Initialize data memory */
    for (i = 0; i < MEMORY_CAPACITY; i++) {
        for (j = 0; j < NUM_OF_BYTES; j++) {
            data->data.lines[i][j] = 0;
        }
    }
    data->data.count = 0;

    /* Initialize the label table*/
    data->label_table.root = create_trie_node();
    data->label_table.node_count = 0;
}

void print_memory_image(const MemoryImage *memory_image) {
    int i,j;
    if (memory_image == NULL) {
        printf("Empty memory image.\n");
        return;
    }

    printf("Count: %d\n", memory_image->count);

    for (i = 0; i < memory_image->count; i++) {
        printf("%04d    ", i); /* Print the line number*/

        for (j = 0; j < NUM_OF_BYTES && j < 2; j++) { /* Print only the first 15 bits (2 bytes)*/
            char current_char = memory_image->lines[i][j];
            for (int k = 7; k >= 1; k--) { /* Iterate from 7 to 1 (bits 0-14)*/
                char bit = (current_char & (1 << k)) ? '1' : '0';
                printf("%c", bit);
            }
            printf(" "); /* Add a space between bytes for readability*/
        }
        printf("\n");
    }
}