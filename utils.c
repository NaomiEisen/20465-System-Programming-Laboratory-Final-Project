/* ---------------------------------------------------------------------------------------
 *                                          Includes
 * --------------------------------------------------------------------------------------- */
#include <stdio.h>
#include <string.h>
#include <stdlib.h>
#include <ctype.h>
#include "boolean.h"
/* ---------------------------------------------------------------------------------------
 *                                          Functions
 * --------------------------------------------------------------------------------------- */

/* Custom implementation of the is_space function */
int is_space(char c) {
    return c == ' ' || c == '\t' || c == '\n' || c == '\r' || c == '\f' || c == '\v';
}

void trim_leading_spaces(const char** str) {
    const char *start = *str;

    /* Trim leading spaces */
    while (is_space(*start)) {
        start++;
    }

    /* Update the original string pointer */
    *str = start;
}


void trim_spaces(const char **str) {
    char *end;
    const char *start = *str;

    /* Trim leading spaces */
    trim_leading_spaces(&start);

    /* If all spaces */
    if (*start == 0) {
        *str = start;
        return;
    }

    /* Trim trailing spaces */
    end = start + strlen(start) - 1;
    while (end > start && is_space(*end)) {
        end--;
    }

    /* Null-terminate the trimmed string */
    *(end + 1) = '\0';

    /* Update the original string pointer */
    *str = start;
}

/* Function to check if the rest of the line is empty */
boolean is_empty_line(const char *line) {
    /* Trim leading spaces */
    trim_leading_spaces(&line);

    /* Check if the rest of the line is empty */
    return *line == '\0';
}

boolean is_valid_integer(const char *str) {
    if (*str == '-' || *str == '+') {
        str++;
    }
    if (!*str) {
        return FALSE;
    }
    while (*str) {
        if (!isdigit(*str)) {
            return TRUE;
        }
        str++;
    }
    return TRUE;
}

boolean create_new_file_name(const char* original_filename, char** new_filename , const char* extension) {

    /* Allocate memory for the new filename */
    size_t new_filename_length = strlen(original_filename) + strlen(extension) + 1;
    *new_filename = (char*)malloc(new_filename_length);
    if (*new_filename != NULL) {
        /* Copy the original filename and append the new extension */
        strcpy(*new_filename, original_filename);
        strcat(*new_filename, extension);
        return TRUE;
    }
    return FALSE;
}

char* my_strndup(const char* str, size_t size) {
    char* result;
    size_t len = strlen(str);

    if (size < len)
        len = size;

    result = (char*)malloc(len + 1);
    if (!result)
        return NULL;

    result[len] = '\0';
    return (char*)memcpy(result, str, len);
}

int my_atoi(const char *str) {
    int result = 0;
    int sign = 1;
    int ptr = 0;

    /* Trim leading spaces */
    trim_leading_spaces(&str);

    /* Handle optional sign */
    if (str[ptr] == '-' || str[ptr] == '+') {
        if (str[ptr] == '-') {
            sign = -1;
        }
        ptr++;
    }

    /* Convert digits to integer */
    while (isdigit(str[ptr])) {
        result = result * 10 + (str[ptr] - '0');
        ptr++;
    }

    return sign * result;
}


