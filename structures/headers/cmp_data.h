#ifndef CMP_DATA_H
#define CMP_DATA_H
/* ----------------------------------------- Includes -----------------------------------------*/
#include "../../structures/headers/label_data.h"
#include "mappings.h"

/* ---------------------------------------- Structures ----------------------------------------*/
/*
 * Enum representing the different sections of a memory image.
 */
typedef enum {
    CODE_IMAGE, /* Section containing encoded instructions */
    DATA_IMAGE    /* Section containing encoded directives */
} MemoryImageType;
/*
 * MemoryImage struct
 * Represents a memory image with a fixed capacity for storing lines of bytes.
 * This structure holds encoded instructions and directives from the input assembly file.
 * Instructions are written starting from the lowest index, while directives are written
 * starting from the highest index.
 */
typedef struct MemoryImage{
    char lines[MEMORY_CAPACITY][NUM_OF_BYTES]; /* Storage for lines of bytes */
    int code_count;       /* Number of instruction lines currently in memory */
    int code_pos;                  /* Position to write the next instruction */
    int data_count;         /* Number of directive lines currently in memory */
    int data_pos;                    /* Position to write the next directive */
    Boolean full;                   /* Flag indicating if the memory is full */
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
 * @param data The program's data structure containing the unresolved line list.
 * @param line The line number to be added to the unresolved line list.
 * @return TRUE if the line was added successfully, FALSE if memory allocation failed.
 */
Boolean add_unresolved_line(CmpData *data, int line);

/**
 * Retrieves and removes the next unresolved line from the list.
 * Returns the number (integer) of the unresolved line.
 *
 * @param data The program's data structure containing the unresolved line list.
 * @return The line number of the next unresolved line, or -1 if the list is empty.
 */
int get_unresolved_line(CmpData *data);

/**
 * Updates the code memory image counter.
 * Increments the code memory image code_count and positions the writer pointer in the correct position.
 * The encoded instruction are written from the lowest index to the highest.
 *
 * @param memory_image A pointer to the MemoryImage structure containing the code counter and the write
 *                     pointer is to be updated.
 */
void updt_code_counter(MemoryImage *memory_image);

/**
 * Updates the data memory image counter.
 * Increments the memory image data_count and positions the writer pointer in the correct position.
 * The encoded directives are written from the highest index to the lowest.
 *
 * @param memory_image A pointer to the MemoryImage structure containing the code counter and the write
 *                     pointer is to be updated.
 */
void updt_data_counter(MemoryImage *memory_image);

/**
 * Sets the code image counter and writer back by one position, allowing the re-encoding of
 * the previous instruction code.
 *
 * @param memory_image A pointer to the MemoryImage structure.
 */
void seek_back(MemoryImage *memory_image);

/**
 * Frees the CmpData structure, containing all the programs data, including closing and
 * optionally deleting files.
 *
 * @param cmp_data The programs data to be freed.
 * @param delete If TRUE, delete the files associated with the CmpData structure.
 */
void free_cmp_data(CmpData *cmp_data, Boolean delete);

#endif /* CMP_DATA_H */
