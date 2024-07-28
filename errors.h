#ifndef ERRORS_H
#define ERRORS_H
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
    INVALID_MACR,
    EXTRA_TXT_MACR,

    /* ======= Comma Errors ======= */
    CONSECUTIVE_COMMA_ERROR,
    MISSING_COMMA_ERROR,
    ILLEGAL_COMMA_ERROR,

    /* ====== Semantic Errors ====== */
    INSTRUCTION_NAME_ERROR,
    DIRECTIVE_NAME_ERROR,
    EXTRA_TXT,
    INVALID_LABEL_NAME,
    LABEL_RESERVED_WORD,
    LABEL_MACR_COLLIDES,
    UNRECOGNIZED_LABEL,
    MULTIPLE_LABEL,
    INVALID_LABEL_LENGTH,
    NOT_INTEGER,
    INVALID_STRING,
    INVALID_REGISTER,
    INVALID_PARAM_NUMBER,
    INVALID_PARAM_TYPE,

    /* ======  Hardware Errors ====== */
    CPU_MEMORY_FULL,

} ErrorCode;

/* Structure that represents a location in a source file */
typedef struct {
    const char *file;       /* File name */
    int line; /* Line number in the file */
} Location;

/* Structure that represents an error with associated information */
typedef struct {
    ErrorCode code;               /* Error's code */
    const char *message; /* Message to be printed */
    Location location;        /* Error's location */
} Error;


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
 * Clears the current error, resetting it to NO_ERROR with a default location.
 */
void clear_error();

/**
 * Returns the current error code.
 *
 * @return The current error code.
 */
ErrorCode error_stat();

/**
 * Prints a warning message indicating that a label before extern/entry is useless.
 */
void print_warning();

#endif /* ERRORS_H */
