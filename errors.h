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

/* ---------------------------- Function Prototypes ---------------------------- */
void set_error(Error *error, ErrorCode code, const char *file, int line);
void print_error(Error *error);

/* Declare the global error variable */
extern Error global_error;

#endif /* ERRORS_H */
