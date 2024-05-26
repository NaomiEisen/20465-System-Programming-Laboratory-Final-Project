
#ifndef UTILS_H
#define UTILS_H

/* Function prototypes */
int is_space(char c);
bool is_empty_line(char* line);
bool create_new_file_name(const char* original_filename, char** new_filename ,const char* extension);
void trim_spaces(char **str);
void trim_leading_spaces(char **str);

#endif /* UTILS_H */
