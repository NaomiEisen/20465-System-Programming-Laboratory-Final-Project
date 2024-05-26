//
// Created by naomi on 25/05/2024.
//
#include <stdlib.h>
#include <stdio.h>
#include <string.h>
#include "preprocessor.h"
#include "errors.h"
#include "general.h"
#include "macro_list.h"
#include "utils.h"

const char* reserved_words[] = {
        "r0", "r1", "r2", "r3", "r4", "r5", "r6", "r7",
        "mov", "cmp", "add", "sub", "lea", "clr", "not", "inc",
        "dec", "jmp", "bne", "red", "prn", "jsr", "rts", "stop"
};

void preprocessor(const char* file_origin){
    FILE* source_file;                  /* the source file */
    FILE* output_file;
    char *output_filename = NULL;
    char word[MAX_LINE_LENGTH] = {0};   /* string to hold one read word from line */
    char line[MAX_LINE_LENGTH] = {0};   /* string to hold the read line */
    char *line_ptr = NULL;
    int line_count = 0;                 /* line counter */
    bool inside_macro = false;          /* flag that indicated if read line is part of macro */
    MacroList macr_list = {NULL};       /* Initialize empty list of macros */
    MacroNode* current_macr = NULL;

    /* ---------------------------- Open the source file in read mode ---------------------------- */
    if (!(source_file = fopen(file_origin, "r"))) {
        /* if the file fails to open, set an error and return */
        set_error(&global_error, CANNOT_OPEN_FILE, file_origin, 0);
        fclose(source_file); /* close the file */
        return;
    }

    /* ------------------ Create the output filename with the specified extension ------------------*/
    if (!create_new_file_name(file_origin, &output_filename, ".am")) {
        set_error(&global_error, MEMORY_ALLOCATION_ERROR, file_origin, 0);
        return;
    }

    /* ---------------------------- Open the output file in write mode ---------------------------- */
    if (!(output_file = fopen(output_filename, "w"))) {
        /* if the file fails to open, set an error and return */
        set_error(&global_error, CANNOT_CREATE_FILE, output_filename, 0);
        fclose(source_file); /* close the file */
        fclose(output_file); /* close the file */
        free(output_filename);
        return;
    }

    /* ---------------------------- Process each line in the source file ---------------------------- */
    while (fgets(line, sizeof(line), source_file) != NULL) {
        /* update counter */
        line_count++;
        line_ptr = line;
        trim_leading_spaces(&line_ptr);

        /* if line is not empty */
        if (sscanf(line_ptr, "%s", word) == 1) {
            printf("%s\n", word); /* FOR ME */

            /* ignore comment line */
            if (is_comment(word))
                continue;

            /* end of macro */
            if (!macr_end(word)) {
                inside_macro = false;
                /* verify end */
                if (!is_empty_line(line_ptr+ strlen(word))) {
                    set_error(&global_error, global_error.code, file_origin, line_count);
                    break;
                }
                continue;
            }

            /* if this line is inside initialized macro */
            if (inside_macro) {
                /* copy to macro */
                add_content_line(&macr_list, line_ptr);
            }

            /* if this line is macro initialization line */
            else if (!macr_start(word)) {
                inside_macro = true;

                if (sscanf(line_ptr+ strlen(word), "%s", word) == 1)
                {
                    if (!create_macr(&macr_list, word)){
                        set_error(&global_error, global_error.code, file_origin, line_count);
                        break;
                    }
                }
                else {
                    set_error(&global_error, INVALID_MACR, file_origin, line_count);
                }
            }

            /* if this is a macro */
            else if ((current_macr = is_macro(&macr_list, word) )!= NULL) {
                copy_macro_to_file(current_macr, output_file);
            }

            /* regular command line */
            else {
                fputs(line_ptr, output_file);
            }
        }
    }
    print_all_macros(&macr_list);
    print_error(&global_error);
    free_macro_list(&macr_list);
    free(output_filename);
    /* close the file */
    fclose(source_file);
}

bool verify_macro(char *str) {
    char word[MAX_LINE_LENGTH] = {0};   /* string to hold one read word from str */

    /* if macr don't have name */
    if (sscanf(str, "%s", word) != 1)
        return false;

    /* macr name is reserved name */
    if (is_reserved_word(word)) {
        set_error(&global_error, INVALID_MACR, "", 0);
        return false;
    }

    /* macr initialization line contain extra text */
    if (is_empty_line(str + strlen(word) + 1) == false) {
        set_error(&global_error, EXTRA_TXT, "", 0);
        return false;

    }
    return true; /* macr is valid */
}

int is_comment(const char* str) {
    return str != NULL && str[0] == ';';
}

int macr_start(const char* str) {
    return str != NULL && strcmp(str ,MACR_START );
}

int macr_end(const char* str) {
    return str != NULL && strcmp(str ,MACR_END );
}


bool create_macr(MacroList* list,char* str) {
    trim_spaces(&str);
    if (verify_macro(str)) {
        insert_macro_node(list, str);
        return true;
    }
    return false;
}
MacroNode* is_macro(MacroList* list, const char* str) {
    MacroNode* current = list->head;

    while (current != NULL) {
        if (strcmp(current->name, str) == 0) {
            return current; /* Found a macro with the same name */
        }
        current = current->next;
    }

    return NULL; /* No macro with the same name found */
}

bool is_reserved_word(const char* str) {
    int i;
    int reserved_words_count = sizeof(reserved_words) / sizeof(reserved_words[0]);

    for (i = 0 ; i < reserved_words_count ; i++) {
        if (strcmp(str , reserved_words[i]) == 0) {
            return true;
        }
    }
    return false;
}

void copy_macro_to_file(MacroNode* macr, FILE* file) {
    LineNode* current = macr->content_lines;
    while (current != NULL) {
        fputs(current->line, file);
        current = current->next;
    }
}
