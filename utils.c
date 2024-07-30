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
/* ------------------------------------ String Utils ------------------------------------- */
/**
 * Custom implementation of the is_space function.
 * Checks if a character is a whitespace character.
 * This includes spaces, tabs, newlines, carriage returns, form feeds, and vertical tabs.
 *
 * @param c The character to check.
 * @return Non-zero if the character is a whitespace character, zero otherwise.
 */
int is_space(char c) {
    return c == ' ' || c == '\t' || c == '\n' || c == '\r' || c == '\f' || c == '\v';
}

/**
 * Trims leading whitespace characters from a string.
 * The pointer to the string is updated to point to the first non-whitespace character.
 *
 * @param str Pointer to the string to be trimmed. This pointer will be updated
 *            to point to the start of the trimmed string.
 */
void trim_leading_spaces(const char** str) {
    const char *start = *str;

    /* Trim leading spaces */
    while (is_space(*start)) {
        start++;
    }
    /* Update the original string pointer */
    *str = start;
}

/**
 * Finds the last non-space character in the given string.
 *
 *
 * @param str Pointer to the string to be processed.
 * @return Pointer to the last non-space character in the string.
 */
const char* last_nonspace_char(const char *str) {
    const char *end = str + strlen(str) - 1;

    /* Move the end pointer back while it points to a space*/
    while (end >= str && is_space(*end)) {
        end--;
    }

    /* Return a pointer to the last non-space character */
    return end;
}

/**
 * Trims leading and trailing spaces from the given string.
 *
 * @param str Double pointer to the string to be trimmed. The pointer will be updated
 *            to point to the start of the trimmed string.
 */
void trim_spaces(const char **str) {
    const char *start = *str;
    const char *end;

    /* Trim leading spaces */
    trim_leading_spaces((const char **) &start);

    /* If all spaces */
    if (*start == '\0') {
        *str = start;
        return;
    }

    /* Trim trailing spaces */
    end = last_nonspace_char(start);

    /* Null-terminate the trimmed string */
    *(char *)(end + 1) = '\0';

    /* Update the original string pointer */
    *str = start;
}

/**
 * Checks if given line is empty.
 *
 * @param line The line to check.
 * @return TRUE if the line is empty or contains only whitespace characters, FALSE otherwise.
 */
Boolean is_empty_line(const char *line) {
    /* Trim leading spaces */
    trim_leading_spaces(&line);

    /* Check if the rest of the line is empty */
    return *line == '\0';
}

/**
 * Duplicates a portion of a string up to a specified size.
 *
 * @param str The string to duplicate.
 * @param size The maximum number of characters to duplicate from the string.
 * @return A newly allocated string containing the first `size` characters of `str`.
 *         Returns NULL if memory allocation fails.
 */
char* my_strndup(const char* str, size_t size) {
    char* result; /* Variable to store the duplicated string */
    size_t len = strlen(str);              /* Strings length */

    /* Invalid strings length - will duplicate only a portion of it */
    if (size < len) len = size;

    result = (char*)malloc(len + 1);
    if (!result) return NULL; /* Memory allocation failure */

    result[len] = '\0'; /* Null-terminate the string */
    return (char*)memcpy(result, str, len);
}

/* ------------------------------------ File Handling Utils ------------------------------------- */
/**
 * Creates a new filename by appending the given extension to the original filename.
 * Memory for the new filename is allocated within this function.
 *
 * @param original_filename The original filename to base the new filename on.
 * @param new_filename Pointer to store the new filename. The memory is allocated here.
 * @param extension The extension to append to the original filename.
 * @return TRUE if the new filename was successfully created and memory was allocated, FALSE otherwise.
 */
Boolean create_new_file_name(const char* original_filename, char** new_filename , const char* extension) {
    /* Allocate memory for the new filename */
    size_t new_filename_length = strlen(original_filename) + strlen(extension) + 1;
    *new_filename = (char*)malloc(new_filename_length);

    if (*new_filename) {
        /* Copy the original filename and append the new extension */
        strcpy(*new_filename, original_filename);
        strcat(*new_filename, extension);
        return TRUE;
    }

    /* Memory allocation failure */
    return FALSE;
}

/* ---------------------------------------- Integer Utils ----------------------------------------- */
/**
 * Validates if a string represents a valid integer.
 * The string may optionally start with a '+' or '-' sign.
 *
 * @param str The string to validate.
 * @return TRUE if the string is a valid integer, FALSE otherwise.
 */
Boolean is_valid_integer(const char *str) {
    /* Null pointer */
    if (!*str)  return FALSE;

    /* Skip sign indicator */
    if (*str == '-' || *str == '+') str++;

    while (*str) { /* Check string's chars */
        if (!isdigit(*str)) return FALSE;
        str++;
    }

    /* The specified string is a valid integer */
    return TRUE;
}

/**
 * Converts a string to an integer, handling optional leading spaces and signs.
 *
 * @param str The string to convert.
 * @return The integer value of the string.
 */
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

    /* Return the integer according to the specified sign */
    return sign * result;
}





/*
void trim_spaces(char **str) {
    char *end;
    char *start = *str;


    trim_leading_spaces((const char **) &start);

    if (*start == 0) {
        *str = start;
        return;
    }



    end = start + strlen(start) - 1;
    while (end > start && is_space(*end)) {
        end--;
    }


    *(end + 1) = '\0';


    *str = start;
}
*/


