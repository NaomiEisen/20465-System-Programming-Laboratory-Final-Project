#ifndef ERRORS_H
#define ERRORS_H
#include "defines.h"
/* ---------------------------- Error Codes ---------------------------- */
typedef enum {
    /* ======= General Errors ======= */
    NO_ERROR,
    MEMORY_ALLOCATION_ERROR,
    EOF_ERROR,

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
    NOT_INTEGER,
    INVALID_STRING,
    INVALID_REGISTER,
    INVALID_PARAM_NUMBER,
    INVALID_PARAM_TYPE,

    /* ======  Hardware Errors ====== */
    CPU_MEMORY_FULL

} ErrorCode;

/* Structure that represents a location in a source file */
typedef struct {
    const char *file;                                             /* File name */
    int line;                                       /* Line number in the file */
    char *line_content;  /* command line from the source file */
} Location;

/* Structure that represents an error with associated information */
typedef struct {
    ErrorCode code;               /* Error's code */
    const char *message; /* Message to be printed */
    Location location;        /* Error's location */
} Error;

typedef enum {
    FATAL_ERROR,
    ERROR_IN_FILE,
    ERROR_FREE_FILE
} Status;

typedef struct {
    Status status;
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

void save_line_content(Location *location, char *content);

/**
 * Clears the current error, resetting it to NO_ERROR with a default location.
 */
void clear_error();

/**
 * Resets the program's status to 'error free file status.
 */
void clear_status();

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
Status  get_status();

/**
 * Prints a warning message indicating that a label before extern/entry is useless.
 */
void print_warning();

#endif /* ERRORS_H */
