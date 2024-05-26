
/*
 Created by naomi on 25/05/2024.
*/

#include <stdio.h>
#include "errors.h"

/* Initialize the global error variable*/
Error global_error = {NO_ERROR, "No error"};

/* Array of error structs */
static const Error errorTable[ERROR_COUNT] = {
        {NO_ERROR, "No error",{NULL, 0}},
        {MEMORY_ALLOCATION_ERROR, "Memory allocation failed",{NULL, 0}},
        {EOF_ERROR, "End of file reached",{NULL, 0}},
        {NO_ARGUMENTS, "No arguments were passed",{NULL, 0}},
        {CANNOT_OPEN_FILE, "Cannot open file",{NULL, 0}},
        {CANNOT_CREATE_FILE, "Cannot create file",{NULL, 0}},
        {INVALID_MACR, "Invalid macro name.",{NULL, 0}},
        {EXTRA_TXT, "Extra text after macro initialization.",{NULL, 0}},
        {OTHER_ERROR, "An unspecified error occurred",{NULL, 0}},
};

/* Function to set the global error
void set_error(ErrorCode code) {
    if (code < ERROR_COUNT) {
        global_error = errorTable[code];
    } else {
        global_error = (Error){OTHER_ERROR, "Unknown error"};
    }
}*/

/* Function to set the global error */
void set_error(Error *error, ErrorCode code, const char *file, int line) {
    if (code < ERROR_COUNT) {
        *error = errorTable[code];
    } else {
        *error = errorTable[OTHER_ERROR];
    }
    error->location.file = file;
    error->location.line = line;
}

void print_error(Error *error) {
    if (error->code != NO_ERROR) {
        printf("Error: %s\n", error->message);
        printf("Location: file %s", error->location.file);
        if (error->location.line > 0)
            printf(" in line %d", error->location.line);
    }
}