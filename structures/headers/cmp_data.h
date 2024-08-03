#ifndef CMP_DATA_H
#define CMP_DATA_H
/* ----------------------------------------- Includes -----------------------------------------*/
#include "../../structures/headers/label_data.h"
#include "mappings.h"

/* ---------------------------------------- Structures ----------------------------------------*/
typedef enum {
    CODE_IMAGE,
    DATA_IMAGE
} MemoryImageType;

/* MemoryImage struct
 * Represents a memory image with a fixed capacity for storing lines of bytes.
 * Contains a code_count of lines and a pointer for writing the next line.
 */
typedef struct MemoryImage{
    char lines[MEMORY_CAPACITY][NUM_OF_BYTES]; /* Storage for lines of bytes */
    int code_count;                        /* Number of lines currently in memory */
    int writer_code;             /* Write pointer for writing on the next line */
    int data_count;
    int writer_data;
    Boolean full;
} MemoryImage;

/*
 * UnresolvedLineList struct
 * Represents a linked list of unresolved lines in the image.
 * Each node contains a line number and a pointer to the next node.
 */
typedef struct UnresolvedLineList{
    int line;                          /* Line number of the unresolved line */
    struct UnresolvedLineList *next; /* Pointer to the next node in the list */
} UnresolvedLineList;

/* File struct - Represents a file with a pointer to the file stream and the file name */
typedef struct File{
    FILE* file;                                /* Pointer to the file stream */
    char* file_name;                                      /* The file's name */
    Boolean delete;
} File;

/*
 * CmpData struct
 * Contains all the necessary data for the compilation process.
 * Includes memory images for image and data, a label table,
 * a list of unresolved lines, and file information for external and entry files.
 */
typedef struct CmpData{
    MemoryImage image;                /* Memory image for the image section */
    Trie label_table;                            /* Trie for storing labels */
    UnresolvedLineList *line_list;       /* Linked list of unresolved lines */
    File extern_file;             /* File information for the external file */
    File entry_file;                 /* File information for the entry file */
} CmpData;

/* ----------------------------------- Functions Prototypes ------------------------------------*/
/**
 * Function to initialize the ProgramData structure.
 * Initializes the sets to be empty sets and sets the corresponding pointers
 * of the sets to the setMappings.
 * @param data Pointer to the ProgramData to be initialized
 */
ErrorCode init_cmp_data(CmpData *data, const char *file_name);


/**
 * Adds a line to the unresolved line list for the second phase.
 * Saves the line number in thr unresolved line list.
 *
 * @param data: The program's data structure containing the unresolved line list.
 * @param line: The line number to be added to the unresolved line list.
 * @return TRUE if the line was added successfully, FALSE if memory allocation failed.
 */
Boolean add_unresolved_line(CmpData *data, int line);

/**
 * Retrieves and removes the next unresolved line from the list.
 * Returns the number (integer) of the unresolved line.
 *
 * @param data: The program's data structure containing the unresolved line list.
 * @return The line number of the next unresolved line, or -1 if the list is empty.
 */
int get_unresolved_line(CmpData *data);

/**
 * Updates the memory image counter.
 * Increments the memory image code_count and positions the writer pointer in the correct position.
 *
 * @param memory_image The memory image whose counter and write pointer is to be updated.
 */
void updt_code_image_counter(MemoryImage *memory_image);

void updt_data_image_counter(MemoryImage *memory_image);

void seek_back(MemoryImage *memory_image);

/**
 * Frees the CmpData structure, containing all the programs data, including closing and
 * optionally deleting files.
 *
 * @param cmp_data: The programs data to be freed.
 * @param delete: If TRUE, delete the files associated with the CmpData structure.
 */
void free_cmp_data(CmpData *cmp_data, Boolean delete);


void print_memory_image_marks(const MemoryImage *memory_image);

#endif /* CMP_DATA_H */
