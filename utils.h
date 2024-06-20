#ifndef UTILS_H
#define UTILS_H
#include "boolean.h"

/* ---------------------------------------------------------------------------------------
 *                                     Function Prototypes
 * --------------------------------------------------------------------------------------- */
int is_space(char c);
boolean is_empty_line(const char *line);
boolean create_new_file_name(const char* original_filename, char** new_filename , const char* extension);
void trim_spaces(const char **str);
void trim_leading_spaces(const char **str);
boolean is_valid_integer(const char *str);
char* my_strndup(const char* str, size_t size);
int my_atoi(const char *str);
boolean strip_first_chars(char **operand, size_t num_chars);
const char* trim_trailing_spaces(const char *str);

#endif /* UTILS_H */
