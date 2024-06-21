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
    CANNOT_OPEN_FILE,
    CANNOT_CREATE_FILE,

    /* ======= Macro Errors ======= */
    INVALID_MACR,
    EXTRA_TXT,

    /* ======= Comma Errors ======= */
    MULTIPULE_COMMA_ERROR,
    MISSING_COMMA_ERROR,
    ILLEGAL_COMMA_ERROR,

    /* ====== Semantic Errors ====== */
    COMMAND_NAME_ERROR,
    DIRECTIVE_NAME_ERROR,
    INVALID_LABEL_NAME,
    LABEL_RESERVED_WORD,
    MULTIPLE_LABEL,
    INVALID_LABEL_LENGTH,
    NOT_INTEGER,
    INVALID_STRING,
    INVALID_REGISTER,
    INVALID_PARAM_NUMBER,
    INVALID_PARAM_TYPE,

    /* ======  Hardware Errors ====== */
    CPU_MEMORY_FULL,

    /* ======= Other ======= */
    OTHER_ERROR,

    /* This will automatically be equal to the number of error codes */
    ERROR_COUNT
} ErrorCode;

/* ---------------------------- Location Information ---------------------------- */
typedef struct {
    const char *file;
    int line;
} Location;

/* ---------------------------- Error information ---------------------------- */
typedef struct {
    ErrorCode code;
    const char *message;
    Location location;
} Error;

/* ---------------------------- Functions Prototypes ---------------------------- */
void set_error(Error *error, ErrorCode code, Location location);
void set_general_error(Error *error, ErrorCode code);
void print_error(Error *error);
void clear_error (Error *error);

/* Declare the global error variable */
extern Error error;

#endif /* ERRORS_H */
