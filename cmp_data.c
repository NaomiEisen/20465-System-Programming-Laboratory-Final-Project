#include "cmp_data.h"
#include "label_data.h"
#include "utils.h"

/**
 * Function to initialize the ProgramData structure.
 * Initializes the sets to be empty sets and sets the corresponding pointers
 * of the sets to the setMappings.
 * @param data Pointer to the ProgramData to be initialized
 */
/* Function to initialize CmpData */
boolean init_cmp_data(CmpData *data, const char *file_name) {
    int i, j;
    char* extern_file = NULL;
    char* entry_file= NULL;

    /* Create and open a file for writing */
    create_new_file_name(file_name, &entry_file, ".ent");
    create_new_file_name(file_name, &extern_file, ".ext");

    /* todo: do you really need to check this? */
    if (entry_file == NULL || extern_file == NULL) {
        return FALSE;
    }

    data->entry_file.file = fopen(entry_file, "w");
    data->extern_file.file = fopen(extern_file, "w");

   if (data->extern_file.file == NULL || data->entry_file.file == NULL) {
        free(extern_file);
        free(entry_file);
        return FALSE;
    }

    data->entry_file.file_name = entry_file;
   data->extern_file.file_name = extern_file;

    /* Initialize code memory */
    for (i = 0; i < MEMORY_CAPACITY; i++) {
        for (j = 0; j < NUM_OF_BYTES; j++) {
            data->code.lines[i][j] = 0;
        }
    }
    data->code.count = 0;
    data->code.write_ptr = 0;

    /* Initialize data memory */
    for (i = 0; i < MEMORY_CAPACITY; i++) {
        for (j = 0; j < NUM_OF_BYTES; j++) {
            data->data.lines[i][j] = 0;
        }
    }

    data->data.count = 0;
    data->data.write_ptr = 0;
    data->line_list = NULL; /* Initialize unresolved line list line_list to NULL*/

    return init_label_trie(&data->label_table);
}


/* Add a line to the unresolved line list */
boolean add_unresolved_line(CmpData *data, int line) {
    UnresolvedLineList *newNode = (UnresolvedLineList *)malloc(sizeof(UnresolvedLineList));
    if (newNode == NULL) {
        return FALSE; /* Memory allocation failed */
    }
    newNode->line = line;
    newNode->next = NULL;

    if (data->line_list == NULL) {
        data->line_list = newNode; /* List is empty, new node is the head */
    } else {
        UnresolvedLineList *temp = data->line_list;
        while (temp->next != NULL) {
            if (temp->line == line) {
                free(newNode);
                return TRUE; /* Line already exists */
            }
            temp = temp->next;
        }
        if (temp->line == line) {
            free(newNode);
            return TRUE; /* Check the last node */
        }
        temp->next = newNode; /* Add new node at the end */
    }
    return TRUE;
}

/* Get the next unresolved line from the list */
int get_unresolved_line(CmpData *data) {
    UnresolvedLineList *temp = NULL;
    int line;

    if (data->line_list == NULL) {
        return -1; /* List is empty */
    }

    temp = data->line_list;
    line = temp->line;
    data->line_list = data->line_list->next;
    free(temp);
    return line;
}

/* Function to free the entire linked list */
void free_unresolved_list(UnresolvedLineList *head) {
    UnresolvedLineList *current = head;
    UnresolvedLineList *next;

    while (current != NULL) {
        next = current->next;
        free(current);
        current = next;
    }
}

void updt_memory_image_counter(MemoryImage *memory_image) {
    memory_image->count++;
    memory_image->write_ptr = memory_image->count;
}

void free_cmp_data(CmpData* cmp_data) {
    if (cmp_data->entry_file.file_name != NULL) {
        fclose(cmp_data->entry_file.file);
        free(cmp_data->entry_file.file_name);
    }

    if (cmp_data->extern_file.file_name != NULL) {
        fclose(cmp_data->extern_file.file);
        free(cmp_data->extern_file.file_name);
    }

    free_unresolved_list(cmp_data->line_list);
}

void delete_files(CmpData* cmp_data) {
    /* Close files if they are open */
    if (cmp_data->entry_file.file != NULL) {
        if (fclose(cmp_data->entry_file.file) != 0) {
            set_general_error(FAILED_CLOSE_FILE);
        }
        cmp_data->entry_file.file = NULL; /* Reset the file pointer to NULL */
    }
    if (cmp_data->extern_file.file != NULL) {
        if (fclose(cmp_data->extern_file.file) != 0) {
            set_general_error(FAILED_CLOSE_FILE);
        }
        cmp_data->extern_file.file = NULL; /* Reset the file pointer to NULL */
    }

    /* Delete files */
    if (remove(cmp_data->entry_file.file_name) != 0) {
        set_general_error(FAILED_DELETE_FILE);
        printf("The file name: %s\n", cmp_data->entry_file.file_name);
    }
    if (remove(cmp_data->extern_file.file_name) != 0) {
        set_general_error(FAILED_DELETE_FILE);
        printf("The file name: %s\n", cmp_data->extern_file.file_name);
    }

    /* Free allocated file names */
    free(cmp_data->entry_file.file_name);
    cmp_data->entry_file.file_name = NULL;
    free(cmp_data->extern_file.file_name);
    cmp_data->extern_file.file_name = NULL;
}

void print_memory_image(const MemoryImage *memory_image) {
    int i, j, k;
    char bit;

    if (memory_image == NULL) {
        printf("Empty memory image.\n");
        return;
    }

    printf("Count: %d\n", memory_image->count);

    for (i = 0; i < memory_image->count; i++) {
        printf("%04d    ", i); /* Print the line number */

        for (j = 0; j < NUM_OF_BYTES; j++) {
            char current_char = memory_image->lines[i][j];
            for (k = 7; k >= 0; k--) {
                if (j == 1 && k == 0) {
                    continue; /* Skip the last bit */
                }
                bit = (current_char & (1 << k)) ? '1' : '0';
                printf("%c", bit);
                if (j == 0 && (k == 4 || k == 0)) { /* Add a space after every 4 bits in the first byte */
                    printf(" ");
                }
                if (j == 1 && k == 4) { /* Add a space after 4 bits in the second byte */
                    printf(" ");
                }
            }
        }
        printf("\n");
    }
}

void print_memory_image_marks(const MemoryImage *memory_image) {
    int i, j, k;
    char bit;

    if (memory_image == NULL) {
        printf("Empty memory image.\n");
        return;
    }

    printf("Count: %d\n", memory_image->count);

    for (i = 0; i < memory_image->count; i++) {
        printf("%04d    ", i); /* Print the line number */

        for (j = 0; j < NUM_OF_BYTES; j++) {
            char current_char = memory_image->lines[i][j];
            for (k = 7; k >= 0; k--) {
                if (j == 1 && k == 0) {
                    continue; /* Skip the last bit */
                }
                bit = (current_char & (1 << k)) ? '1' : '0';
                printf("%c", bit);
                if (j == 0 && (k == 4 || k == 0)) { /* Add a space after every 4 bits in the first byte */
                    printf(" ");
                }
                if (j == 1 && k == 4) { /* Add a space after 4 bits in the second byte */
                    printf(" ");
                }
            }
        }
        /* Print the skipped 15th bit (bit 0 of the second byte) separately */
        printf(" %c", (memory_image->lines[i][1] & 1) ? '1' : '0');
        printf("\n");
    }
}
