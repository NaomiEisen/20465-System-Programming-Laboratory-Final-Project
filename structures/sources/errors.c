/* ---------------------------------------------------------------------------------------
 *                                          Includes
 * --------------------------------------------------------------------------------------- */
#include <stdio.h>
#include <string.h>
#include <stdlib.h>
#include "../headers/errors.h"
#include "../../utils/headers/utils.h"
/* --------------------------- Initialize the static variables --------------------------- */
static Error error = {NO_ERROR, "No error"};
static ProgramStatus program_status = {ERROR_FREE_FILE, 0,0, };

/* ---------------------------------------------------------------------------------------
 *                                          Functions
 * --------------------------------------------------------------------------------------- */

/**
 * Function that retrieves the error message corresponding to a given error code.
 *
 * @param code The error code for which the message is to be retrieved.
 * @return A string containing the error message.
 */
static const char* get_error_message(ErrorCode code) {
    switch (code) {
        case NO_ERROR:                   return "No error";
        case MEMORY_ALLOCATION_ERROR:    return "MEMORY ALLOCATION FAILURE";
        case EOF_ERROR:                  return "End of file reached";
        case NO_ARGUMENTS:               return "No arguments provided";
        case PROGRAM_FILE_ERROR:         return "Failed to open program's extern/entry files";
        case FAILED_OPEN_FILE:           return "Cannot open file";
        case FAILED_CREATE_FILE:         return "Cannot create file";
        case FAILED_CLOSE_FILE:          return "Failed to close file";
        case FAILED_DELETE_FILE:         return "Failed to delete file";
        case LINE_TOO_LONG:              return "Line is too long; max length is " TOSTRING(MAX_LINE_PRINTABLE) " characters";
        case MACR_RESERVED_WORD:         return "Invalid macro name - name is a reserved word";
        case EXTRA_TXT_MACR:             return "Extra text after macro definition";
        case MACR_DUPLICATE:             return "Invalid macro name - duplicate macro names found";
        case INVALID_CHAR_MACR:          return "Macro name contains invalid characters";
        case CONSECUTIVE_COMMA_ERROR:    return "Multiple consecutive commas";
        case MISSING_COMMA_ERROR:        return "Missing comma";
        case ILLEGAL_COMMA_ERROR:        return "Illegal comma";
        case INSTRUCTION_NAME_ERROR:     return "Invalid command name";
        case DIRECTIVE_NAME_ERROR:       return "Invalid directive instruction";
        case INVALID_LABEL_NAME:         return "Invalid label name - should start with an alphabetic character";
        case EXTRA_TXT:                  return "Extra text after command";
        case LABEL_RESERVED_WORD:        return "Invalid label name - cannot be a reserved word";
        case INVALID_CHAR_LABEL:         return "Label name contains invalid characters";
        case LABEL_MACR_COLLIDES:        return "Label and macro name collision";
        case EXT_ENT_COLLIDES:           return "Label extern and entry collision. Cannot be set to both.";
        case UNRECOGNIZED_LABEL:         return "Unrecognized label";
        case INVALID_LABEL_LENGTH:       return "Invalid label length - cannot exceed "TOSTRING(MAX_LABEL_PRINTABLE)" characters";
        case LABEL_DUPLICATE:            return "Invalid label name - duplicate label names found";
        case NOT_INTEGER:                return "Not an integer";
        case INTEGER_OUT_OF_RANGE:       return "Integer is out of range";
        case INVALID_START_STRING:       return "Invalid string format - does not start with double quotation mark";
        case INVALID_END_STRING:         return "Invalid string format - does not end with double quotation mark";
        case INVALID_REGISTER:           return "Invalid register name";
        case INVALID_PARAM_NUMBER:       return "Invalid number of parameters";
        case INVALID_PARAM_TYPE:         return "Invalid parameter type";
        case RAM_MEMORY_FULL:            return "RAM MEMORY FULL - file is too long\n\t ABORTING ENCODE PROCESS";
        default:                         return "An unspecified error occurred";
    }
}

 /**
  * Prints the details of the specified location.
  *
  * @param location The location to print.
  */
static void print_location(Location* location){
    if (location->line > 0) {
        printf("\nFile:    %s | Line: %d", location->file, location->line);
        if (location->line_content) /* Print line content if exists */
            printf("\n         %s", location->line_content);
    }
}

/**
 * Prints the current error message and location, if an error is set.
 * Activates whenever an error is set.
 */
static void print_error() {
    /* Print only if error is set */
    if (error.code != NO_ERROR) {
        printf("ERROR:   %s", error.message);
        /* If there is specified location - print it too */
        print_location(&error.location);
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
    program_status.error_counter = 0;
    program_status.warning_counter = 0;
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
    /* Set the error attributes according to the inputted code */
    error.code = code;
    error.message = get_error_message(code);
    error.location = location;

    /* If an actual error was set */
    if (code != NO_ERROR) {
        print_error(); /* Print the error message */
        program_status.error_counter++;

        /* Set the program's status accordingly */
        if (code == MEMORY_ALLOCATION_ERROR)
            set_program_status(FATAL_ERROR);
        else
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

/**
 * Makes a copy of the given string, and saves it as the line content in the specified
 * location struct.
 *
 * @param location the location struct to save the line into.
 * @param content the string to save as a content in the location struct.
 */
void save_line_content(Location *location, char *content){
    char *copy_content = NULL;
    if (!location) return; /* Null pointer */

    /* Copy the content */
    copy_content = my_strndup(content, strlen(content));

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

/**
 * Frees the memory allocated for 'Location' variable.
 *
 * @param location the Location variable to free.
 */
void free_location(Location *location){
    if (location) {
        free(location->line_content);
        /* Set pointer to NULL after freeing */
        location->line_content = NULL;
    }
}

/**
 * Returns the current error image.
 *
 * @return The current error image.
 */
ErrorCode get_error() {
    return error.code;
}

/**
 * Prints a warning message that corresponding to the specified warning code.
 *
 * @param code The warning code, indicating the cause of the warning.
 * @param location The location of the line that triggered the warning.
 */
void print_warning(WarningCode code, Location *location) {
    printf("WARNING: ");
    switch (code) { /* Print the corresponding warning */
        case LABEL_ENTRY:
            printf("Label definition before entry command is ignored");
            break;
        case LABEL_EXTERN:
            printf("Label definition before extern command is ignored");
            break;
        case ENTRY_DUPLICATE:
            printf("Label is already declared as entry; This declaration will be ignored");
            break;
        case EXTERN_DUPLICATE:
            printf("Label is already declared as extern; This declaration will be ignored");
            break;
    }
    /* Print location */
    print_location(location);
    printf("\n\n");

    /* Update counter */
    program_status.warning_counter++;
}

/**
 * Static helper function - prints line for aesthetic purposes.
 */
static void print_line(){
    printf("-------------------------------------------------\n");
}

/**
 * Prints the status of the program, including a summary of errors.
 * Indicates whether the file processing completed successfully, and the number
 * of errors and warnings that occurred during the process.
 *
 * @param file The name of the file that was processed.
 */
void print_error_summery(char *file) {
    print_line(); /* Print line separating this message from previous messages */
    printf("| ERROR SUMMARY ('%s')\n", file);
    printf("|    Errors   : %d\n", program_status.error_counter);
    printf("|    Warnings : %d\n", program_status.warning_counter);
    print_line();

    /* Print the program's status */
    if (program_status.status == ERROR_IN_FILE) {
        printf("|    Could not process file\n");
    } else if (program_status.status == FATAL_ERROR) {
        printf("|    Fatal error occurred during processing,\n");
        printf("|    terminating the program...\n");
    } else {
        printf("|    File processing completed successfully\n");
    }

    print_line();
}

