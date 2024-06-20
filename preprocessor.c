/* ---------------------------------------------------------------------------------------
 *                                          Includes
 * --------------------------------------------------------------------------------------- */
#include <stdlib.h>
#include <stdio.h>
#include <string.h>
#include "preprocessor.h"
#include "errors.h"
#include "macro_list.h"
#include "utils.h"

const char* reserved_words[] = {
        "r0", "r1", "r2", "r3", "r4", "r5", "r6", "r7",
        "mov", "cmp", "add", "sub", "lea", "clr", "not", "inc",
        "dec", "jmp", "bne", "red", "prn", "jsr", "rts", "stop"
};

/* ---------------------------------------------------------------------------------------
 *                               Head Function Of Preprocessor
 * --------------------------------------------------------------------------------------- */

char* preprocessor(const char* file_origin){
    FILE* source_file;                  /* the source file (.as) */
    FILE* output_file;                  /* the output file (.am) */
    char* source_filename = NULL;       /* the source file name */
    char* output_filename = NULL;       /* the output file name */
    char word[MAX_LINE_LENGTH] = {0};   /* string to hold one read word from line */
    char line[MAX_LINE_LENGTH] = {0};   /* string to hold the read line */
    const char* line_ptr = NULL;        /* pointer to go through line */
    int line_count = 0;                 /* line counter */
    boolean inside_macro = FALSE;       /* flag that indicated if read line is part of macro */
    MacroList macr_list = {NULL};       /* Initialize empty list of macros */
    MacroNode* current_macr = NULL;
    Location location = {NULL, 0};

    /* ------------- Create the source filename with the specified extension -------------*/
    if (!create_new_file_name(file_origin, &source_filename, ".as")) {
        set_general_error(&error, MEMORY_ALLOCATION_ERROR);
        print_error(&error);
        return NULL;
    }

    /* ----------------------- Open the source file in read mode ----------------------- */
    if (!(source_file = fopen(source_filename, "r"))) {
        /* if the file fails to open, set an error and return */
        set_general_error(&error, CANNOT_OPEN_FILE);
        print_error(&error);
        fclose(source_file); /* close the file */
        free(source_filename);
        return NULL;
    }

    /* -------------- Create the output filename with the specified extension --------------*/
    if (!create_new_file_name(file_origin, &output_filename, ".am")) {
        set_general_error(&error, MEMORY_ALLOCATION_ERROR);
        print_error(&error);
        return NULL;
    }

    /* ------------------------ Open the output file in write mode ------------------------ */
    if (!(output_file = fopen(output_filename, "w"))) {
        /* if the file fails to open, set an error and return */
        set_general_error(&error, CANNOT_CREATE_FILE);
        print_error(&error);
        fclose(source_file); /* close the source file */
        fclose(output_file); /* close the output file */
        free(output_filename);
        free(source_filename);
        return NULL;
    }

    /* ------------------------ Process each line in the source file ------------------------ */
    location.file = source_filename;
    while (fgets(line, sizeof(line), source_file) != NULL) {
        location.line++; /* Update counter */
        /*line_count++;  Update counter */
        line_ptr = line;  /* Set line pointer to line start */
        trim_leading_spaces(&line_ptr); /* Skip leading spaces */

        /* If line is not empty - process the line */
        if (sscanf(line_ptr, "%s", word) == 1) {

            /* ============ 1. Ignore comment line ============ */
            if (is_comment(word))
                continue;

            /* ======== 2. End of macro initialization ======== */
            if (!macr_end(word)) {
                inside_macro = FALSE;
                /* verify end */
                if (!is_empty_line(line_ptr+ strlen(word))) {
                    set_error(&error, error.code, location);
                    print_error(&error);
                    clear_error(&error);
                }
                continue;
            }

            /* ======== 3. Inside of macro initialization ======== */
            if (inside_macro) {
                /* copy to macro */
                add_content_line(&macr_list, line_ptr);
            }

            /* ============ 4. Macro initialization ============ */
            else if (!macr_start(word)) {
                inside_macro = TRUE; /* set flag */

                if (!create_macr(&macr_list, line_ptr + strlen(word), location)){ /* if macro creation fails */
                    clear_error(&error);
                    inside_macro = FALSE; /* no macro was initialized */
                }
            }

            /* =============== 5. Existing macro =============== */
            else if ((current_macr = is_macro(&macr_list, word) )!= NULL) {
                copy_macro_to_file(current_macr, output_file);
            }

            /* ============ 6. Regular command_str line ============ */
            else {
                fputs(line_ptr, output_file);
            }
        }
    }
    print_all_macros(&macr_list);
    free_macro_list(&macr_list);
    /* close the file */
    fclose(source_file);
    fclose(output_file);
    /*free(output_filename);*/
    free(source_filename);

    return output_filename;
}

/* ---------------------------------------------------------------------------------------
 *                                   Utility Functions
 * --------------------------------------------------------------------------------------- */

boolean verify_macro(const char *str, Location location) {
    char word[MAX_LINE_LENGTH] = {0};   /* string to hold one read word from str */

    /* if macr don't have name */
    if (sscanf(str, "%s", word) != 1)
        return FALSE;

    /* macr name is reserved name */
    if (is_reserved_word(word)) {
        set_error(&error, INVALID_MACR, location);
        print_error(&error);
        return FALSE;
    }

    /* macr initialization line contain extra text */
    if (is_empty_line(str + strlen(word) + 1) == FALSE) {
        set_error(&error, EXTRA_TXT, location);
        print_error(&error);
        return FALSE;

    }
    return TRUE; /* macr is valid */
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


boolean create_macr(MacroList *list, const char *str, Location location) {
    trim_spaces(&str);
    if (verify_macro(str, location)) {
        insert_macro_node(list, str);
        if (error.code == NO_ERROR) {
            return TRUE;
        }
    }
    return FALSE;
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
/** TODO : delete! */
boolean is_reserved_word(const char* str) {
    int i;
    int reserved_words_count = sizeof(reserved_words) / sizeof(reserved_words[0]);

    for (i = 0 ; i < reserved_words_count ; i++) {
        if (strcmp(str , reserved_words[i]) == 0) {
            return TRUE;
        }
    }
    return FALSE;
}

void copy_macro_to_file(MacroNode* macr, FILE* file) {
    LineNode* current = macr->content_lines;
    while (current != NULL) {
        fputs(current->line, file);
        current = current->next;
    }
}
