#ifndef ERRORS_H
#define ERRORS_H
#include "../../assembler/headers/defines.h"
/* ----------------------------- Defines ----------------------------- */
#define STRINGIFY(x) #x
#define TOSTRING(x) STRINGIFY(x)
/* ---------------------------- Error Codes ---------------------------- */
typedef enum {
    /* ======= General Errors ======= */
    NO_ERROR,
    MEMORY_ALLOCATION_ERROR,
    EOF_ERROR,
    PROGRAM_FILE_ERROR,

    /* ======= File Errors ======= */
    NO_ARGUMENTS,
    FAILED_OPEN_FILE,
    FAILED_CREATE_FILE,
    FAILED_CLOSE_FILE,
    FAILED_DELETE_FILE,

    /* ======== Line Errors ======= */
    LINE_TOO_LONG,

    /* ======= Macro Errors ======= */
    MACR_RESERVED_WORD,
    EXTRA_TXT_MACR,
    MACR_DUPLICATE,
    INVALID_CHAR_MACR,

    /* ======= Label Errors ======= */
    INVALID_LABEL_NAME,
    LABEL_RESERVED_WORD,
    LABEL_MACR_COLLIDES,
    UNRECOGNIZED_LABEL,
    LABEL_DUPLICATE,
    INVALID_LABEL_LENGTH,
    INVALID_CHAR_LABEL,
    EXT_ENT_COLLIDES,

    /* ======= Comma Errors ======= */
    CONSECUTIVE_COMMA_ERROR,
    MISSING_COMMA_ERROR,
    ILLEGAL_COMMA_ERROR,

    /* ====== Semantic Errors ====== */
    INSTRUCTION_NAME_ERROR,
    DIRECTIVE_NAME_ERROR,
    EXTRA_TXT,
    INVALID_CHAR,
    DUPLICATE,
    INVALID_START_STRING,
    INVALID_END_STRING,

    /* ====== Logic Errors ====== */
    NOT_INTEGER,
    INTEGER_OUT_OF_RANGE,
    INVALID_REGISTER,
    INVALID_PARAM_NUMBER,
    INVALID_PARAM_TYPE,

    /* ======  Hardware Errors ====== */
    RAM_MEMORY_FULL

} ErrorCode;

/* Warning codes */
typedef enum {
    LABEL_ENTRY,                         /* Label before entry */
    LABEL_EXTERN,                       /* Label before extern */
    ENTRY_DUPLICATE,  /* Defining the same label 'entry' twice */
    EXTERN_DUPLICATE /* Defining the same label 'extern' twice */
} WarningCode;

/* Structure that represents a location in a source file */
typedef struct {
    const char *file;                            /* File name */
    int line;                      /* Line number in the file */
    char *line_content;  /* command line from the source file */
} Location;

/* Structure that represents an error with associated information */
typedef struct {
    ErrorCode code;              /* Error's image */
    const char *message; /* Message to be printed */
    Location location;        /* Error's location */
} Error;

typedef enum {
    FATAL_ERROR,          /* Error indicating for terminating the program */
    ERROR_IN_FILE, /* Indicating the abortion of creating the object file */
    ERROR_FREE_FILE                    /* No errors in the processed file */
} Status;

typedef struct {
    Status status;                      /* Status code */
    int error_counter;     /* Number of errors in file */
    int warning_counter; /* Number of warnings in file */
} ProgramStatus;

/* ---------------------------- Functions Prototypes ---------------------------- */
/**
 * Sets the current error with the specified error code and location.
 *
 * @param code The error code to set.
 * @param location The location in the source file where the error occurred.
 */
void set_error(ErrorCode code, Location location);

/**
 * Sets a general error with the specified error code but without a specific location.
 *
 * @param code The error code to set.
 */
void set_general_error(ErrorCode code);

/**
 * Makes a copy of the given string, and saves it as the line content in the specified
 * location struct.
 * @param location the location struct to save the line into.
 * @param content the string to save as a content in the location struct.
 */
void save_line_content(Location *location, char *content);

/**
 * Clears the current error, resetting it to NO_ERROR with a default location.
 */
void clear_error();

/**
 * Resets the program's status to 'error free file status'.
 */
void clear_status();

/**
 * Frees the memory allocated for the Location struct.
 * @param location the specified Location struct to free.
 */
void free_location(Location *location);

/**
 * Returns the current error code.
 *
 * @return The current error code.
 */
ErrorCode get_error();

/**
 * Returns the current program's status.
 *
 * @return The current program's status.
 */
Status get_status();

 /**
  * Prints a warning message that corresponding to the specified warning code.
  *
  * @param code The warning code, indicating the cause of the warning.
  * @param location The location of the line that triggered the warning.
  */
void print_warning(WarningCode code, Location *location);

/**
 * Prints the status of the program, including a summary of errors.
 * Indicates whether the file processing completed successfully, and the number
 * of errors and warnings that occurred during the process.
 *
 * @param file The name of the file that was processed.
 */
void print_error_summery(char *file);

#endif /* ERRORS_H */
