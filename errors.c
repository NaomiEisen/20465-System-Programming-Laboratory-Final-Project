
/*
 Created by naomi on 25/05/2024.
*/

#include <stdio.h>
#include "errors.h"

/* Initialize the global error variable*/
Error global_error = {NO_ERROR, "No error"};

/* Array of error structs */
static const Error errorTable[ERROR_COUNT] = {
        {NO_ERROR, "No error"},
        {MEMORY_ALLOCATION_ERROR, "Memory allocation failed"},
        {EOF_ERROR, "End of file reached"},
        {NO_ARGUMENTS, "No arguments were passed"},
        {CANNOT_OPEN_FILE, "Cannot open file"},
        {CANNOT_CREATE_FILE, "Cannot create file"},
        {INVALID_MACR, "Invalid macro name."},
        {EXTRA_TXT, "Extra text after macro initialization."},
        {OTHER_ERROR, "An unspecified error occurred"},
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
        global_error = errorTable[code];
    } else {
        global_error = (Error){OTHER_ERROR, "Unknown error"};
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