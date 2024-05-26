//
// Created by naomi on 26/05/2024.
//
#include <stdio.h>
#include <string.h>
#include <stdlib.h>
#include "general.h"

/* Custom implementation of the is_space function */
int is_space(char c) {
    return c == ' ' || c == '\t' || c == '\n' || c == '\r' || c == '\f' || c == '\v';
}


/* Function to check if the rest of the line is empty */
bool is_empty_line(const char* line) {
    // Skip leading whitespace
    while (*line && is_space(*line)) {
        line++;
    }

    // Check if the rest of the line is empty
    return *line == '\0';
}

bool create_new_file_name(const char* original_filename, char** new_filename ,const char* extension) {

    /* Allocate memory for the new filename */
    size_t new_filename_length = strlen(original_filename) + strlen(extension) + 1;
    *new_filename = (char*)malloc(new_filename_length);
    if (*new_filename != NULL) {
        /* Copy the original filename and append the new extension */
        strcpy(*new_filename, original_filename);
        strcat(*new_filename, extension);
        return true;
    }
    return false;
}


void trim_spaces(char **str) {
    char *end;
    char *start = *str;

    /* Trim leading spaces */
    while (is_space( *start)) {
        start++;
    }

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

