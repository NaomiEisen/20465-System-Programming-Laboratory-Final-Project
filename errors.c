/* ---------------------------------------------------------------------------------------
 *                                          Includes
 * --------------------------------------------------------------------------------------- */
#include <stdio.h>
#include "errors.h"

/* ------------------------ Initialize the global error variable ------------------------ */
static Error error = {NO_ERROR, "No error"};

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
        case FAILED_OPEN_FILE: return "Cannot open file";
        case FAILED_CREATE_FILE: return "Cannot create file";
        case FAILED_CLOSE_FILE: return "Failed to close file";
        case FAILED_DELETE_FILE: return "Failed to delete file";
        case LINE_TOO_LONG: return "Command line is too long. Should be 81 chars at max";
        case INVALID_MACR: return "Invalid macro name";
        case EXTRA_TXT_MACR: return "Extra text after macro define";
        case CONSECUTIVE_COMMA_ERROR: return "Multiple consecutive commas";
        case MISSING_COMMA_ERROR: return "Missing comma";
        case ILLEGAL_COMMA_ERROR: return "Illegal comma";
        case INSTRUCATION_NAME_ERROR: return "Invalid command name";
        case DIRECTIVE_NAME_ERROR: return "Invalid directive instruction";
        case INVALID_LABEL_NAME: return "Invalid label name, should start with alphabetic char";
        case EXTRA_TXT: return "Extra text after command";
        case LABEL_RESERVED_WORD: return "Invalid label name, cannot be reserved word";
        case LABEL_MACR_COLLIDES: return "label and macro's name are collides";
        case UNRECOGNIZED_LABEL: return "Unrecognized label";
        case INVALID_LABEL_LENGTH: return "Invalid label length, cannot exceed 31 chars";
        case MULTIPLE_LABEL: return "Multiple label defenition";
        case NOT_INTEGER: return "Not an integer";
        case INVALID_STRING: return "Invalid string format";
        case INVALID_REGISTER: return "Invalid register name";
        case INVALID_PARAM_NUMBER: return "Invalid param number";
        case INVALID_PARAM_TYPE: return "Invalid param type";
        case CPU_MEMORY_FULL: return "CPU memory full";
        default: return "An unspecified error occurred";
    }
}


void set_error(ErrorCode code, Location location) {
    error.code = code;
    error.message = get_error_message(code);
    error.location = location;
    print_error();
}

void set_general_error(ErrorCode code) {
    Location default_location = {NULL, 0};
    error.code = code;
    error.message = get_error_message(code);
    error.location = default_location;
    print_error();
}

void print_error() {
    if (error.code != NO_ERROR) {
        printf("Error: %s ", error.message);
        if (error.location.line > 0) {
            printf(" Location: file name - %s, line - %d", error.location.file, error.location.line);
        }
        printf("\n");
    }
}

void clear_error() {
    Location default_location = {NULL, 0};
    set_error(NO_ERROR, default_location);
}

ErrorCode error_stat() {
    return error.code;
}

void print_warning() {
    printf("WARNING: label before extern/entry is useless.");
}




/* ---------------------------------------------------------------------------------------
 *                                          Archive
 * --------------------------------------------------------------------------------------- */

/* Function to set the global error
void set_error(ErrorCode code) {
    if (code < ERROR_COUNT) {
        error = errorTable[code];
    } else {
        error = (Error){OTHER_ERROR, "Unknown error"};
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
        {FAILED_OPEN_FILE, "Cannot open file",{NULL, 0}},
        {FAILED_CREATE_FILE, "Cannot create file",{NULL, 0}},
        {INVALID_MACR, "Invalid macro name",{NULL, 0}},
        {EXTRA_TXT_MACR, "Extra text after macro define",{NULL, 0}},
        {OTHER_ERROR, "An unspecified error occurred",{NULL, 0}},
};*/