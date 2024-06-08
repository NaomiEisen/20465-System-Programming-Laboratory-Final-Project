#ifndef UTILS_H
#define UTILS_H
#include "bool.h"

/* ---------------------------------------------------------------------------------------
 *                                     Function Prototypes
 * --------------------------------------------------------------------------------------- */
int is_space(char c);
bool is_empty_line(const char *line);
bool create_new_file_name(const char* original_filename, char** new_filename ,const char* extension);
void trim_spaces(const char **str);
void trim_leading_spaces(const char **str);
char* my_strndup(const char* str, size_t size);

#endif /* UTILS_H */
