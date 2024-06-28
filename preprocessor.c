/* ---------------------------------------------------------------------------------------
 *                                          Includes
 * --------------------------------------------------------------------------------------- */
#include <stdlib.h>
#include <stdio.h>
#include <string.h>
#include "preprocessor.h"
#include "errors.h"
#include "macro_data.h"
#include "utils.h"
#include "mappings.h"

/* ---------------------------------------------------------------------------------------
 *                               Head Function Of Preprocessor
 * --------------------------------------------------------------------------------------- */

char *preprocessor_controller(const char *file_origin, MacroTrie *macro_trie) {
    FILE* source_file;                  /* the source file (.as) */
    FILE* output_file;                  /* the output file (.am) */
    char* source_filename = NULL;       /* the source file name */
    char* output_filename = NULL;       /* the output file name */
    Location location = {NULL, 0};

    /* ------------- Create the source filename with the specified extension -------------*/
    if (!create_new_file_name(file_origin, &source_filename, ".as")) {
        set_general_error(MEMORY_ALLOCATION_ERROR);
        return NULL;
    }

    /* ----------------------- Open the source file in read mode ----------------------- */
    if (!(source_file = fopen(source_filename, "r"))) {
        /* if the file fails to open, set an error and return */
        set_general_error(FAILED_OPEN_FILE);
        fclose(source_file); /* close the file */
        free(source_filename);
        return NULL;
    }

    /* -------------- Create the output filename with the specified extension --------------*/
    if (!create_new_file_name(file_origin, &output_filename, ".am")) {
        set_general_error(MEMORY_ALLOCATION_ERROR);
        return NULL;
    }

    /* ------------------------ Open the output file in write mode ------------------------ */
    if (!(output_file = fopen(output_filename, "w"))) {
        /* if the file fails to open, set an error and return */
        set_general_error(FAILED_CREATE_FILE);
        fclose(source_file); /* close the source file */
        fclose(output_file); /* close the output file */
        free(output_filename);
        free(source_filename);
        return NULL;
    }

    /* ------------------------ Process each line in the source file ------------------------ */
    location.file = source_filename;
    process_line(source_file, output_file, macro_trie, location);

    /* ------------------------------------- Free memory ------------------------------------- */
    free_trie_data(macro_trie);
    /* close the file */
    fclose(source_file);
    fclose(output_file);
    free(source_filename);
    return output_filename;
}

void process_line(FILE *source_file, FILE* output_file, MacroTrie *macro_trie, Location location) {
    char word[MAX_LINE_LENGTH] = {0};   /* string to hold one read word from line */
    char line[MAX_LINE_LENGTH+1] = {0}; /* string to hold the read line */
    const char* line_ptr = NULL;        /* pointer to go through line */
    int ch;                             /* variable to skip too long lines */
    boolean inside_macro = FALSE;       /* flag that indicated if read line is part of macro */
    TrieNode* macr_usage = NULL;        /* node to hold macro's data in case of usage */

    while (fgets(line, sizeof(line), source_file) != NULL) {
        location.line++;                    /* Update counter */
        line_ptr = line;                    /* Set line pointer to line start */
        trim_leading_spaces(&line_ptr); /* Skip leading spaces */

        /* Check if the line is too long */
        if (validate_line_length(line, location) == FALSE) {
            /* Skip the rest of the overly long line */
            while ((ch = fgetc(source_file)) != '\n' && ch != EOF) {}
            continue; /* Skip processing this line*/
        }

        /* If line is not empty - process the line */
        if (sscanf(line_ptr, "%s", word) == 1) {

            /* ______ 1. Ignore comment line ______ */
            if (is_comment(word))
                continue;

            /* ______ 2. End of macro initialization ______ */
            if (!macr_end(word)) {
                inside_macro = FALSE;
                /* verify end */
                if (!is_empty_line(line_ptr+ strlen(word))) {
                    set_error(EXTRA_TXT_MACR, location);
                    clear_error();
                }
                continue;
            }

            /* ______ 3. Inside of macro initialization ______ */
            if (inside_macro) {/* copy to macro */
                add_line_to_last_macro(macro_trie, line_ptr);
            }

            /* ______ 4. Macro initialization ______ */
            else if (!macr_start(word)) {
                if (create_macr(macro_trie, line_ptr + strlen(word), location) == TRUE){
                    inside_macro = TRUE; /* set flag */
                }
                    /* todo : decide if continue check or not */
                else {
                    break;
                }
            }

            /* ______ 5. Existing macro ______ */
            else if ((macr_usage = find_macro(macro_trie, word) ) != NULL) {
                copy_macro_to_file(macr_usage, output_file);
            }

            /* ______ 6. Regular command line ______ */
            else {
                fputs(line_ptr, output_file);
            }
        }
    }
}

/* ---------------------------------------------------------------------------------------
 *                                   Utility Functions
 * --------------------------------------------------------------------------------------- */


boolean validate_line_length(const char *line, Location location) {
    if (strlen(line) >= MAX_LINE_LENGTH) {
        set_error(LINE_TOO_LONG, location);
        return FALSE;
    }
    return TRUE;
}

boolean verify_macro(const char *str, Location location) {
    char word[MAX_LINE_LENGTH] = {0};   /* string to hold one read word from str */

    /* if macr don't have name */
    if (sscanf(str, "%s", word) != 1)
        return FALSE;

    /* macr name is reserved name */
    if (reserved_word(word)) {
        set_error(INVALID_MACR, location);
        return FALSE;
    }

    /* macr initialization line contain extra text */
    if (is_empty_line(str + strlen(word) + 1) == FALSE) {
        set_error(EXTRA_TXT_MACR, location);
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


boolean create_macr(MacroTrie *macr_trie, const char *str, Location location) {
    trim_spaces(&str);
    if (verify_macro(str, location) == TRUE) {
        return add_macr(macr_trie, str);
    }
    return FALSE;
}


void copy_macro_to_file(TrieNode *macr, FILE* file) {
    MacroData* data = (MacroData*) macr->data;
    LineNode* current = data->head;
    while (current != NULL) {
        fputs(current->line, file);
        current = current->next;
    }
}