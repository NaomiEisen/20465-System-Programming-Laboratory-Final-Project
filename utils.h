//
// Created by naomi on 26/05/2024.
//

#ifndef INC_20465_SYSTEM_PROGRAMMING_LABORATORY_FINAL_PROJECT_UTILS_H
#define INC_20465_SYSTEM_PROGRAMMING_LABORATORY_FINAL_PROJECT_UTILS_H

/* Function prototypes */
int is_space(char c);
bool is_empty_line(const char* line);
void set_error(Error *error, ErrorCode code, const char* file, int line);
bool create_new_file_name(const char* original_filename, char** new_filename ,const char* extension);
void trim_spaces(char **str);

#endif //INC_20465_SYSTEM_PROGRAMMING_LABORATORY_FINAL_PROJECT_UTILS_H
