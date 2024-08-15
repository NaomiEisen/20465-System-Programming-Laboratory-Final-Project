/* ---------------------------------------------------------------------------------------
 *                                          Includes
 * --------------------------------------------------------------------------------------- */
#include "../../structures/headers/cmp_data.h"
#include "../../utils/headers/utils.h"
/* ---------------------------------------------------------------------------------------
 *                                 Static Functions Prototypes
 * --------------------------------------------------------------------------------------- */
static void close_files(CmpData* cmp_data);
static void free_file_names(CmpData* cmp_data);
static void delete_files(CmpData* cmp_data);
static void free_unresolved_list(UnresolvedLineList *head);
/* ---------------------------------------------------------------------------------------
 *                                         Functions
 * --------------------------------------------------------------------------------------- */
/**
 * Function to initialize the ProgramData structure.
 * Initializes the sets to be empty sets and sets the corresponding pointers
 * of the sets to the setMappings.
 * @param data Pointer to the ProgramData to be initialized
 */
ErrorCode init_cmp_data(CmpData *data, const char *file_name) {
    char *extern_file = NULL;
    char *entry_file = NULL;

    /* Create and open a file for writing */
    create_new_file_name(file_name, &entry_file, ".ent");
    create_new_file_name(file_name, &extern_file, ".ext");

    /* Memory allocation failure */
    if (entry_file == NULL || extern_file == NULL) return MEMORY_ALLOCATION_ERROR;

    /* Open in write mode */
    data->entry_file.file = fopen(entry_file, "w");
    data->extern_file.file = fopen(extern_file, "w");

    /* Failed to open */
    if (data->extern_file.file == NULL || data->entry_file.file == NULL) {
        free(extern_file);
        free(entry_file);
        return PROGRAM_FILE_ERROR;
    }

    /* Save the file name */
    data->entry_file.file_name = entry_file;
    data->extern_file.file_name = extern_file;

    /* Default state - delete the files. Will be changed only if text will be written on them */
    data->entry_file.delete = TRUE;
    data->extern_file.delete = TRUE;

    /* Reset the memory image counters and writing pointers */
    data->image.code_count = 0;
    data->image.code_pos = 0;
    data->image.data_count = 0;
    data->image.data_pos = MEMORY_CAPACITY - 1;

    data->image.full = FALSE; /* Set capacity error to false */

    data->line_list = NULL; /* Initialize unresolved line list line_list to NULL*/

    /* return the status of trie initialization */
    return (init_label_trie(&data->label_table) == TRUE) ? NO_ERROR : MEMORY_ALLOCATION_ERROR;
}

/**
 * Adds a line to the unresolved line list for the second phase.
 * Saves the line number in thr unresolved line list.
 *
 * @param data: The program's data structure containing the unresolved line list.
 * @param line: The line number to be added to the unresolved line list.
 * @return TRUE if the line was added successfully, FALSE if memory allocation failed.
 */
Boolean add_unresolved_line(CmpData *data, int line) {
    /* Allocate memory for list's node */
    UnresolvedLineList *newNode = (UnresolvedLineList *)malloc(sizeof(UnresolvedLineList));
    if (!newNode) {
        return FALSE; /* Memory allocation failed */
    }

    newNode->line = line;
    newNode->next = NULL;

    /* If list is empty, new node is the head */
    if (!data->line_list) {
        data->line_list = newNode;
    } else {
        /* Put the line in the accurate position - the end of the list */
        UnresolvedLineList *temp = data->line_list;
        while (temp->next) {
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
    /* Process completed successfully */
    return TRUE;
}

/**
 * Retrieves and removes the next unresolved line from the list.
 * Returns the number (integer) of the unresolved line.
 *
 * @param data: The program's data structure containing the unresolved line list.
 * @return The line number of the next unresolved line, or -1 if the list is empty.
 */
int get_unresolved_line(CmpData *data) {
    UnresolvedLineList *temp = NULL; /* Variable to hole the head of the line's list */
    int line;        /* Variable to hold the number of the unresolved line to return */

    if (!data->line_list) {
        return -1; /* List is empty */
    }

    temp = data->line_list;
    line = temp->line;

    /* Remove current line node - move the head to the next line node */
    data->line_list = data->line_list->next;
    free(temp); /* Free the line's node */
    return line; /* return the freed line number */
}

/**
 * Private function - checks if the memory image is full.
 * If so - sets appropriate error.
 * @param memory_image
 */
static void check_capacity(MemoryImage *memory_image){
    /* Check if the program is run out of memory */
    if (memory_image->code_count + memory_image->data_count > MEMORY_CAPACITY) {
        set_general_error(RAM_MEMORY_FULL);
        memory_image->full = TRUE;
    }
}

/**
 * Updates the code memory image counter.
 * Increments the code memory image code_count and positions the writer pointer in the correct position.
 * The encoded instruction are written from the lowest index to the highest.
 *
 * @param memory_image A pointer to the MemoryImage structure containing the code counter and the write
 *                     pointer is to be updated.
 */
void updt_code_counter(MemoryImage *memory_image) {
    /* Update only if there is remaining memory */
    if (memory_image->full == FALSE){
        memory_image->code_count++;
        memory_image->code_pos = memory_image->code_count;
        check_capacity(memory_image);
    }
}

/**
 * Updates the data memory image counter.
 * Increments the memory image data_count and positions the writer pointer in the correct position.
 * The encoded directives are written from the highest index to the lowest.
 *
 * @param memory_image A pointer to the MemoryImage structure containing the code counter and the write
 *                     pointer is to be updated.
 */
void updt_data_counter(MemoryImage *memory_image) {
    /* Update only if there is remaining memory */
    if (memory_image->full == FALSE) {
        memory_image->data_count++;
        memory_image->data_pos--;
        check_capacity(memory_image);
    }
}

/**
 * Sets the code image counter and writer back by one position, allowing the re-encoding of
 * the previous instruction code.
 *
 * @param memory_image A pointer to the MemoryImage structure.
 */
void seek_back(MemoryImage *memory_image){
    /* Validate that the pointer will not be out of array's bound */
    if (memory_image->code_count > 0) {
        memory_image->code_count--;
        memory_image->code_pos = memory_image->code_count;
    }
}

/**
 * Frees the CmpData structure, containing all the programs data, including closing and
 * optionally deleting files.
 *
 * @param cmp_data The programs data to be freed.
 * @param delete If TRUE, delete the files associated with the CmpData structure.
 */
void free_cmp_data(CmpData *cmp_data, Boolean delete) {
    close_files(cmp_data); /* Close the files */
    if (delete == TRUE) {
        /* Delete files if specified so */
        cmp_data->extern_file.delete = TRUE;
        cmp_data->entry_file.delete = TRUE;
    }
    /* Delete the necessary files */
    delete_files(cmp_data);

    /* Reset file pointers to null */
    cmp_data->entry_file.file = NULL;
    cmp_data->extern_file.file = NULL;

    /* Free the memory allocated to the file names strings */
    free_file_names(cmp_data);

    /* Free the unresolved lines list */
    free_unresolved_list(cmp_data->line_list);
}

/**
 * Static function - frees the entire linked list of unresolved lines.
 *
 * @param head The head of the unresolved line list to be freed.
 */
static void free_unresolved_list(UnresolvedLineList *head) {
    /* Variables to iterate through list */
    UnresolvedLineList *current = head;
    UnresolvedLineList *next;

    /* Iterate through list and free all it's nodes */
    while (current != NULL) {
        next = current->next;
        free(current);
        current = next;
    }
}

/**
 * Static function - closes the files associated with the CmpData structure.
 *
 * @param cmp_data The program's data structure containing the files to be closed.
 */
static void close_files(CmpData* cmp_data) {
    /* If file exists - close file */
    if (cmp_data->entry_file.file) {
        if (fclose(cmp_data->entry_file.file) != 0) {
            set_general_error(FAILED_CLOSE_FILE);
        }
    }

    /* If file exists - close file */
    if (cmp_data->extern_file.file) {
        if (fclose(cmp_data->extern_file.file) != 0) {
            set_general_error(FAILED_CLOSE_FILE);
        }
    }
}

/**
 * Static function - frees the memory allocated for the file names in the CmpData structure.
 *
 * @param cmp_data The program's data structure containing the file names to be freed.
 */
static void free_file_names(CmpData* cmp_data){
    /* Free the files names */
    free(cmp_data->entry_file.file_name);
    free(cmp_data->extern_file.file_name);

    /* Reset the variable to null */
    cmp_data->entry_file.file_name = NULL;
    cmp_data->extern_file.file_name = NULL;
}

/**
 * Static function - deletes the files associated with the CmpData structure.
 *
 * @param cmp_data The program's data structure containing the files to be deleted.
 */
static void delete_files(CmpData* cmp_data) {
    /* Delete file */
    if (cmp_data->entry_file.delete == TRUE){
    if (cmp_data->entry_file.file_name != NULL && remove(cmp_data->entry_file.file_name) != 0) {
        /* Failed to delete */
        set_general_error(FAILED_DELETE_FILE);
        printf("The file name: %s\n", cmp_data->entry_file.file_name);
    }
}
    /* Delete file */
    if (cmp_data->extern_file.delete == TRUE) {
    if (cmp_data->extern_file.file_name != NULL && remove(cmp_data->extern_file.file_name) != 0) {
        /* Failed to delete */
        set_general_error(FAILED_DELETE_FILE);
        printf("The file name: %s\n", cmp_data->extern_file.file_name);
    }
}
}
