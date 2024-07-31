#ifndef UTILS_H
#define UTILS_H
/* ----------------------------------------- Includes -----------------------------------------*/
#include <stdlib.h>
#include "boolean.h"
/* ----------------------------------- Functions Prototypes ---------------------------------- */
/**
 * Custom implementation of the is_space function.
 * Checks if a character is a whitespace character.
 * This includes spaces, tabs, newlines, carriage returns, form feeds, and vertical tabs.
 *
 * @param c The character to check.
 * @return Non-zero if the character is a whitespace character, zero otherwise.
 */
int is_space(char c);

/**
 * Checks if given line is empty.
 *
 * @param line The line to check.
 * @return TRUE if the line is empty or contains only whitespace characters, FALSE otherwise.
 */
Boolean is_empty_line(const char *line);

/**
 * Trims leading whitespace characters from a string.
 * The pointer to the string is updated to point to the first non-whitespace character.
 *
 * @param str Pointer to the string to be trimmed. This pointer will be updated
 *            to point to the start of the trimmed string.
 */
void trim_leading_spaces(const char** str);

/**
 * Finds the last non-space character in the given string.
 *
 *
 * @param str Pointer to the string to be processed.
 * @return Pointer to the last non-space character in the string.
 */
const char* last_nonspace_char(const char *str);

/**
 * Trims leading and trailing spaces from the given string.
 *
 * @param str Double pointer to the string to be trimmed. The pointer will be updated
 *            to point to the start of the trimmed string.
 */
void trim_spaces(const char **str);

/**
 * Duplicates a portion of a string up to a specified size.
 *
 * @param str The string to duplicate.
 * @param size The maximum number of characters to duplicate from the string.
 * @return A newly allocated string containing the first `size` characters of `str`.
 *         Returns NULL if memory allocation fails.
 */
char* my_strndup(const char* str, size_t size);

/**
 * Creates a new filename by appending the given extension to the original filename.
 * Memory for the new filename is allocated within this function.
 *
 * @param original_filename The original filename to base the new filename on.
 * @param new_filename Pointer to store the new filename. The memory is allocated here.
 * @param extension The extension to append to the original filename.
 * @return TRUE if the new filename was successfully created and memory was allocated, FALSE otherwise.
 */
Boolean create_new_file_name(const char* original_filename, char** new_filename , const char* extension);

/**
 * Validates if a string represents a valid integer.
 * The string may optionally start with a '+' or '-' sign.
 *
 * @param str The string to validate.
 * @return TRUE if the string is a valid integer, FALSE otherwise.
 */
Boolean is_valid_integer(const char *str);

/**
 * Converts a string to an integer, handling optional leading spaces and signs.
 *
 * @param str The string to convert.
 * @return The integer value of the string.
 */
int my_atoi(const char *str);

#endif /* UTILS_H */
