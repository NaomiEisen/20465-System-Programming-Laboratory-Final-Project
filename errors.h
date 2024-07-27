#ifndef ERRORS_H
#define ERRORS_H
#include "boolean.h"

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

/* ----------------------------- Location struct ------------------------------- */
typedef struct {
    const char *file;
    int line;
} Location;

/* -------------------------------- Error struct -------------------------------- */
typedef struct {
    ErrorCode code;
    const char *message;
    Location location;
} Error;

typedef struct {
    Boolean error;
} FatalError;

/* ---------------------------- Functions Prototypes ---------------------------- */
void set_error(ErrorCode code, Location location);
void set_general_error(ErrorCode code);
void print_error();
void clear_error();
ErrorCode error_stat();
void print_warning();

#endif /* ERRORS_H */
