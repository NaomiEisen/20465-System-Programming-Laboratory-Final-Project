/* ---------------------------------------------------------------------------------------
 *                                          Includes
 * --------------------------------------------------------------------------------------- */
#include <stdio.h>
#include <string.h>
#include <stdlib.h>
#include <ctype.h>
#include "boolean.h"
#include "hardware.h"
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

const char* trim_trailing_spaces(const char *str) {
    const char *end = str + strlen(str) - 1;

    /* Move the end pointer back while it points to a space*/
    while (end >= str && is_space(*end)) {
        end--;
    }

    /* The end pointer is now at the last non-space character*/
    return end;
}

boolean reserved_word(const char *str) {
    int i = 0; /* Index for iterating through the commandMappings array */

    /* check directive */
    while (directive_table[i].command_str[0] != '\0') {
        if (strcmp(directive_table[i].command_str, str) == 0) {
            return TRUE;
        }
        i++;
    }

    /* check instruction */
    i = 0;
    while (command_table[i].command_str[0] != '\0') {
        if (strcmp(command_table[i].command_str, str) == 0) {
            return TRUE;
        }
        i++;
    }

    i = 0;
    /* check register */
    while (registers[i] != NULL) {
        if( (registers[i], str) == 0) {
            return TRUE;
        }
        i++;
    }

    return FALSE;
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
            return FALSE;
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

/**
 * Removes the specified number of characters from the operand string and allocates new memory for it.
 * @param operand - pointer to the original operand string pointer
 * @param num_chars - number of characters to remove from the beginning
 * @return - true if successful, false if allocation fails
 */
boolean strip_first_chars(char **operand, size_t num_chars) {
    char *new_operand;
    size_t len = strlen(*operand);
    if (num_chars >= len) {
        return FALSE; /* If num_chars is greater than or equal to the length of the string, fail*/
    }
    new_operand = (char *)malloc(len - num_chars + 1); /* Allocate memory for new string */
    if (new_operand) {
        strcpy(new_operand, *operand + num_chars); /* Copy the rest of the string */
        free(*operand);
        *operand = new_operand;
        return TRUE;
    }
    return FALSE;
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


