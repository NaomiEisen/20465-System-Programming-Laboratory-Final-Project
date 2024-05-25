//
// Created by naomi on 25/05/2024.
//

#ifndef INC_20465_SYSTEM_PROGRAMMING_LABORATORY_FINAL_PROJECT_ERRORS_H
#define INC_20465_SYSTEM_PROGRAMMING_LABORATORY_FINAL_PROJECT_ERRORS_H

/* Define error codes */
typedef enum {

    /* general errors */
    NO_ERROR,
    MEMORY_ALLOCATION_ERROR,
    EOF_ERROR,

    /* file open errors */
    NO_ARGUMENTS,
    CANNOT_OPEN_FILE,

    /* macro errors */
    INVALID_MACR,
    EXTRA_TXT,

    /* other */
    OTHER_ERROR,

    /* This will automatically be equal to the number of error codes */
    ERROR_COUNT
} ErrorCode;

/* Define a struct to hold error information */
typedef struct {
    ErrorCode code;
    const char *message;
} Error;

/* Declare the global error variable */
extern Error global_error;

/* Function to set the global error */
void set_error(ErrorCode code);
void print_error(Error error);

#endif //INC_20465_SYSTEM_PROGRAMMING_LABORATORY_FINAL_PROJECT_ERRORS_H
