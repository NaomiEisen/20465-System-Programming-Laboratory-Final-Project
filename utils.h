//
// Created by naomi on 26/05/2024.
//

#ifndef INC_20465_SYSTEM_PROGRAMMING_LABORATORY_FINAL_PROJECT_UTILS_H
#define INC_20465_SYSTEM_PROGRAMMING_LABORATORY_FINAL_PROJECT_UTILS_H

/* Function prototypes */
int isspace(int c); /* Declaration of isspace function*/
bool is_empty_line(const char* line);
void set_error(Error *error, ErrorCode code, const char* file, int line);
bool create_new_file_name(const char* original_filename, char** new_filename ,const char* extension);

#endif //INC_20465_SYSTEM_PROGRAMMING_LABORATORY_FINAL_PROJECT_UTILS_H
