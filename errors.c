/* ---------------------------------------------------------------------------------------
 *                                          Includes
 * --------------------------------------------------------------------------------------- */
#include <stdio.h>
#include "errors.h"

/* ------------------------ Initialize the global error variable ------------------------ */
Error global_error = {NO_ERROR, "No error"};

/* ---------------------------------------------------------------------------------------
 *                                          Functions
 * --------------------------------------------------------------------------------------- */

/* Function to get the error message for a given error code */
const char* get_error_message(ErrorCode code) {
    switch (code) {
        case NO_ERROR: return "No error";
        case MEMORY_ALLOCATION_ERROR: return "Memory allocation failed";
        case EOF_ERROR: return "End of file reached";
        case NO_ARGUMENTS: return "No arguments were passed";
        case CANNOT_OPEN_FILE: return "Cannot open file";
        case CANNOT_CREATE_FILE: return "Cannot create file";
        case INVALID_MACR: return "Invalid macro name";
        case EXTRA_TXT: return "Extra text after macro define";
        case MULTIPULE_COMMA_ERROR: return "Multiple consecutive commas";
        case MISSING_COMMA_ERROR: return "Missing comma";
        case ILLEGAL_COMMA_ERROR: return "Illegal comma";
        default: return "An unspecified error occurred";
    }
}

void set_error(Error *error, ErrorCode code, const char *file, int line) {
    error->code = code;
    error->message = get_error_message(code);
    error->location.file = file;
    error->location.line = line;
}

void print_error(Error *error) {
    if (error->code != NO_ERROR) {
        printf("Error: %s | ", error->message);
        printf("Location: file name - %s", error->location.file);
        if (error->location.line > 0)
            printf(", line - %d", error->location.line);
        printf("\n");
    }
}

void clear_error (Error *error) {
    set_error(error, NO_ERROR, NULL, 0);
}




/* ---------------------------------------------------------------------------------------
 *                                          Archive
 * --------------------------------------------------------------------------------------- */

/* Function to set the global error
void set_error(ErrorCode code) {
    if (code < ERROR_COUNT) {
        global_error = errorTable[code];
    } else {
        global_error = (Error){OTHER_ERROR, "Unknown error"};
    }
}*/

/* Function to set the global error
void set_error(Error *error, ErrorCode code, const char *file, int line) {
    if (code < ERROR_COUNT) {
        *error = errorTable[code];
    } else {
        *error = errorTable[OTHER_ERROR];
    }
    error->location.file = file;
    error->location.line = line;
}*/

/* Array of error structs
static const Error errorTable[ERROR_COUNT] = {
        {NO_ERROR, "No error",{NULL, 0}},
        {MEMORY_ALLOCATION_ERROR, "Memory allocation failed",{NULL, 0}},
        {EOF_ERROR, "End of file reached",{NULL, 0}},
        {NO_ARGUMENTS, "No arguments were passed",{NULL, 0}},
        {CANNOT_OPEN_FILE, "Cannot open file",{NULL, 0}},
        {CANNOT_CREATE_FILE, "Cannot create file",{NULL, 0}},
        {INVALID_MACR, "Invalid macro name",{NULL, 0}},
        {EXTRA_TXT, "Extra text after macro define",{NULL, 0}},
        {OTHER_ERROR, "An unspecified error occurred",{NULL, 0}},
};*/