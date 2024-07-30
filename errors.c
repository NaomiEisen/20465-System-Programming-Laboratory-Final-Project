/* ---------------------------------------------------------------------------------------
 *                                          Includes
 * --------------------------------------------------------------------------------------- */
#include <stdio.h>
#include <string.h>
#include <stdlib.h>
#include "errors.h"
#include "utils.h"

/* ------------------------ Initialize the global error variable ------------------------ */
static Error error = {NO_ERROR, "No error"};
static ProgramStatus program_status = {ERROR_FREE_FILE};

/* ---------------------------------------------------------------------------------------
 *                                          Functions
 * --------------------------------------------------------------------------------------- */

/**
 * Function that retrieves the error message corresponding to a given error code.
 *
 * @param code The error code for which the message is to be retrieved.
 * @return A string containing the error message.
 */
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
        case LINE_TOO_LONG: return "Command line is too long. Should be 81 chars at max"; /* todo fix number */
        case MACR_RESERVED_WORD: return "Invalid macro name - macro's name is reserved word";
        case EXTRA_TXT_MACR: return "Extra text after macro define";
        case MACR_DUPLICATE: return "Macro with the same name already exists";
        case INVALID_CHAR_MACR: return "Macro's name contains invalid character";
        case CONSECUTIVE_COMMA_ERROR: return "Multiple consecutive commas";
        case MISSING_COMMA_ERROR: return "Missing comma";
        case ILLEGAL_COMMA_ERROR: return "Illegal comma";
        case INSTRUCTION_NAME_ERROR: return "Invalid command name";
        case DIRECTIVE_NAME_ERROR: return "Invalid directive instruction";
        case INVALID_LABEL_NAME: return "Invalid label name, should start with alphabetic char";
        case EXTRA_TXT: return "Extra text after command";
        case LABEL_RESERVED_WORD: return "Invalid label name, cannot be reserved word";
        case INVALID_CHAR_LABEL: return "Label's name contains invalid char";
        case LABEL_MACR_COLLIDES: return "Label and macro's name are collides";
        case UNRECOGNIZED_LABEL: return "Unrecognized label";
        case INVALID_LABEL_LENGTH: return "Invalid label length, cannot exceed 31 chars"; /* todo fix number */
        case LABEL_DUPLICATE: return "Multiple label definition";
        case NOT_INTEGER: return "Not an integer";
        case INVALID_STRING: return "Invalid string format";
        case INVALID_REGISTER: return "Invalid register name";
        case INVALID_PARAM_NUMBER: return "Invalid param number";
        case INVALID_PARAM_TYPE: return "Invalid param type";
        case CPU_MEMORY_FULL: return "CPU memory full";
        default: return "An unspecified error occurred";
    }
}
/**
 * Private methods - prints the current error message and location, if an error is set.
 * Activates whenever an error is set.
 */
static void print_error() {
    /* Print only if error is set */
    if (error.code != NO_ERROR) {
        printf("Error: %s", error.message);
        /* If there is specified location - print it too */
        if (error.location.line > 0) {
            printf(" | File: %s | Line: %d", error.location.file, error.location.line);
            if (error.location.line_content) /* Print line content if exists */
                printf("\n\t %s", error.location.line_content);
        }
        printf("\n\n");
    }
}

/**
 * Private function - sets the program's status to the specified status.
 *
 * @param status The program's status.
 */
static void set_program_status(Status status) {
    program_status.status = status;
}

/**
 * Resets the program's status to 'error free file' status.
 */
void clear_status() {
    program_status.status = ERROR_FREE_FILE;
}

/**
 * Returns the current program's status.
 *
 * @return The current program's status.
 */
Status get_status() {
    return program_status.status;
}

/**
 * Sets the current error with the specified error code and location.
 *
 * @param code The error code to set.
 * @param location The location in the source file where the error occurred.
 */
void set_error(ErrorCode code, Location location) {
    error.code = code;
    error.message = get_error_message(code);
    error.location = location;
    print_error();

    if (code == MEMORY_ALLOCATION_ERROR) {
        set_program_status(FATAL_ERROR);
    } else if (code != NO_ERROR){
        set_program_status(ERROR_IN_FILE);
    }
}

/**
 * Sets a general error with the specified error code but without a specific location.
 *
 * @param code The error code to set.
 */
void set_general_error(ErrorCode code) {
    Location default_location = {NULL, 0};
    set_error(code, default_location);
}

void save_line_content(Location *location, char *content){
    if (!location) return;

    char *copy_content = my_strndup(content, strlen(content));
    if (!copy_content) {
        set_general_error(MEMORY_ALLOCATION_ERROR);
    } else {
        location->line_content = copy_content;
    }
}
/**
 * Clears the current error, resetting it to NO_ERROR with a default location.
 */
void clear_error() {
    set_general_error(NO_ERROR);
}

void free_location(Location *location){
    if (location) {
        free(location->line_content);
        /* Set pointer to NULL after freeing */
        location->line_content = NULL;
    }
}

/**
 * Returns the current error code.
 *
 * @return The current error code.
 */
ErrorCode get_error() {
    return error.code;
}

/**
 * Prints a warning message indicating that a label before extern/entry is useless.
 */
void print_warning() {
    printf("WARNING: label before extern/entry is useless.");
}
