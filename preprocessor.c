/* ---------------------------------------------------------------------------------------
 *                                        Includes
 * --------------------------------------------------------------------------------------- */
#include <stdlib.h>
#include <stdio.h>
#include <string.h>
#include "errors.h"
#include "macro_data.h"
#include "utils.h"
#include "boolean.h"
#include "mappings.h"
#include "defines.h"
/* ---------------------------------------------------------------------------------------
 *                               Static Functions Prototypes
 * --------------------------------------------------------------------------------------- */
static void process_line(FILE *source_file, FILE* output_file, MacroTrie *macro_trie, Location location);
static Boolean validate_line_length(const char *line, Location location);
static Boolean verify_macro(const char *str, Location location, MacroTrie *macr_trie);
static int is_comment(const char* str);
static int macr_start(const char* str);
static int macr_end(const char* str);
static Boolean create_macr(MacroTrie *macr_trie, const char *str, Location location);
static void copy_macro_to_file(TrieNode *macr, FILE* file);
static void cleanup_files(FILE* source_file, FILE* output_file, char* source_filename, char* output_filename);
/* ---------------------------------------------------------------------------------------
 *                               Head Function Of Preprocessor
 * --------------------------------------------------------------------------------------- */
/**
 * The `preprocessor_controller` function handles preprocessing of the source file,
 * using `process_line` to process each line and apply macro expansions.
 * The processed output is written to a new file with the ".am" extension.
 *
 * @param file_origin The base name of the source file without extension.
 * @param macro_trie A trie structure containing macro definitions for preprocessing.
 * @return The name of the output file with the ".am" extension, or NULL if an error occurs.
 */
char *preprocessor_controller(const char *file_origin, MacroTrie *macro_trie) {
    FILE* source_file = NULL;                          /* the source file (.as) */
    FILE* output_file = NULL;                          /* the output file (.am) */
    char* source_filename = NULL;                       /* the source file name */
    char* output_filename = NULL;                       /* the output file name */
    Location location = {NULL, 0}; /* Location variable set to default */

    /* ------------- Create the source filename with the specified extension -------------*/
    if (!create_new_file_name(file_origin, &source_filename, ".as")) {
        set_general_error(MEMORY_ALLOCATION_ERROR);
        return NULL;
    }

    /* ----------------------- Open the source file in read mode ----------------------- */
    if (!(source_file = fopen(source_filename, "r"))) {
        /* If the file fails to open, set an error */
        set_general_error(FAILED_OPEN_FILE);
        /* Cleanup resources */
        cleanup_files(source_file, NULL, source_filename, NULL);
        return NULL;
    }

    /* -------------- Create the output filename with the specified extension --------------*/
    if (!create_new_file_name(file_origin, &output_filename, ".am")) {
        set_general_error(MEMORY_ALLOCATION_ERROR);
        /* Cleanup resources */
        cleanup_files(source_file, NULL, source_filename, NULL);
        return NULL;
    }

    /* ------------------------ Open the output file in write mode ------------------------ */
    if (!(output_file = fopen(output_filename, "w"))) {
        /* If the file fails to open, set an error and return */
        set_general_error(FAILED_CREATE_FILE);
        /* Cleanup resources */
        cleanup_files(source_file, NULL, source_filename, output_filename);
        return NULL;
    }

    /* ------------------------ Process each line in the source file ------------------------ */
    location.file = source_filename;
    process_line(source_file, output_file, macro_trie, location);

    /* ------------------------------------- Free memory ------------------------------------- */
    free_trie_data(macro_trie);
    /* Cleanup resources */
    cleanup_files(source_file, output_file, source_filename, NULL);
    return output_filename;
}

/* ---------------------------------------------------------------------------------------
 *                                           Functions
 * --------------------------------------------------------------------------------------- */
/**
 * Processes each line of the source file, handling macro definitions and usages,
 * and writes the processed lines to the output file.
 *
 * @param source_file The input file pointer to the source file being read.
 * @param output_file The output file pointer where processed lines are written.
 * @param macro_trie A trie structure containing macro definitions for preprocessing.
 * @param location A structure representing the current file and line number being processed.
 */
static void process_line(FILE *source_file, FILE* output_file, MacroTrie *macro_trie, Location location) {
    char word[MAX_LINE_LENGTH] = {0};   /* string to hold one read word from line */
    char line[MAX_LINE_LENGTH+1] = {0}; /* string to hold the read line */
    char* line_ptr = NULL;        /* pointer to go through line */
    int ch;                             /* variable to skip too long lines */
    Boolean inside_macro = FALSE;       /* flag that indicated if read line is part of macro */
    TrieNode* macr_usage = NULL;        /* node to hold macro's data in case of usage */

    while (fgets(line, sizeof(line), source_file) != NULL && get_status() != FATAL_ERROR) {
        location.line++;                                         /* Update counter */
        line_ptr = line;                         /* Set line pointer to line start */
        trim_leading_spaces((const char **) &line_ptr); /* Skip leading spaces */
        save_line_content(&location, line_ptr);        /* Save line content */

        /* Check if the line is too long */
        if (validate_line_length(line, location) == FALSE) {
            /* Skip the rest of the overly long line */
            while ((ch = fgetc(source_file)) != '\n' && ch != EOF) {}
        }

        /* If line is not empty - process the line */
        else if (sscanf(line_ptr, "%s", word) == 1) {
            /* ------------------------ 1. Ignore comment line ------------------------ */
            if (is_comment(word)) {
                free_location(&location);
                continue;
            }

            /* -------------------- 2. End of macro initialization -------------------- */
            if (!macr_end(word)) {
                inside_macro = FALSE;
                /* verify end */
                if (!is_empty_line(line_ptr+ strlen(word))) {
                    set_error(EXTRA_TXT_MACR, location);
                }
            }

            /* ------------------- 3. Inside of macro initialization ------------------- */
            else if (inside_macro) {/* copy to macro */
                add_line_to_last_macro(macro_trie, line_ptr);
            }

            /* ------------------------ 4. Macro initialization ------------------------ */
            else if (!macr_start(word)) {
                if (create_macr(macro_trie, line_ptr + strlen(word), location) == TRUE){
                    inside_macro = TRUE; /* set flag */
                }
            }

            /* --------------------------- 5. Existing macro --------------------------- */
            else if ((macr_usage = find_macro(macro_trie, word) ) != NULL) {
                /* check if there is no extra text after macro usage */
                if (!is_empty_line(line_ptr+ strlen(word))) {
                    set_error(EXTRA_TXT_MACR, location);
                }
                copy_macro_to_file(macr_usage, output_file);
            }

            /* ------------------------ 6. Regular command line ------------------------ */
            else {
                fputs(line_ptr, output_file);
            }
        }
        /* Free line content after parsing each line */
        free_location(&location);
    }
}

/* ---------------------------------------------------------------------------------------
 *                                Preprocessor Utility Functions
 * --------------------------------------------------------------------------------------- */

/**
 * The `validate_line_length` function checks if a given line exceeds the maximum allowed length.
 * If the line is too long, it sets an error and returns FALSE.
 *
 * @param line The line to check.
 * @param location The current file location being processed.
 * @return TRUE if the line length is valid, FALSE otherwise.
 */
static Boolean validate_line_length(const char *line, Location location) {
    /* Check if the length of the line exceeds the maximum allowed length */
    if (strlen(line) >= MAX_LINE_LENGTH) {
        set_error(LINE_TOO_LONG, location);
        return FALSE;
    }
    return TRUE;
}

/**
* The `verify_macro` function verifies the validity of a macro initialization line.
* It checks if the macro has a valid name: if the name is not a reserved word,
* and if there is no extra text after the macro name.
*
* @param str The string to verify as a macro initialization line.
* @param location The current file location being processed.
* @return TRUE if the macro initialization line is valid, FALSE otherwise.
*/
static Boolean verify_macro(const char *str, Location location, MacroTrie *macr_trie) {
    char word[MAX_LINE_LENGTH] = {0};   /* string to hold one read word from str */

    /* If macro don't have name */
    if (sscanf(str, "%s", word) != 1)
        return FALSE;

    /* Macro name is reserved name */
    if (reserved_word(word)) {
        set_error(INVALID_MACR, location);
        return FALSE;
    }

    /* Macro name collides with previous macro declarations */
    if (find_macro(macr_trie, str)) {
        set_error(MACR_DUPLICATE, location);
        return FALSE;
    }

    /* Macro initialization line contain extra text */
    if (is_empty_line(str + strlen(word) + 1) == FALSE) {
        set_error(EXTRA_TXT_MACR, location);
        return FALSE;

    }
    return TRUE; /* Macro is valid */
}

/**
 * The `is_comment` function checks if a given string is a comment line.
 * A comment line defined as a line that starts with the char ';'.
 *
 * @param str The string to check.
 * @return Non-zero if the string is a comment line, 0 otherwise.
 */
static int is_comment(const char* str) {
    return str != NULL && str[0] == ';';
}

/**
 * The `macr_start` function checks if a given string is the start of a macro definition.
 *
 * @param str The string to check.
 * @return Non-zero if the string is the start of a macro, 0 otherwise.
 */
static int macr_start(const char* str) {
    return str != NULL && strcmp(str ,MACR_START );
}

/**
 * The `macr_end` function checks if a given string is the end of a macro definition.
 *
 * @param str The string to check.
 * @return Non-zero if the string is the end of a macro, 0 otherwise.
 */
static int macr_end(const char* str) {
    return str != NULL && strcmp(str ,MACR_END );
}

/**
 * The `create_macr` function creates a new macro in the macro trie if the given string
 * is a valid macro initialization line.
 *
 * @param macr_trie The trie structure containing macro definitions.
 * @param str The string containing the macro initialization line.
 * @param location The current file location being processed.
 * @return TRUE if the macro was successfully created, FALSE otherwise.
 */
static Boolean create_macr(MacroTrie *macr_trie, const char *str, Location location) {
    /* Remove leading and trailing spaces from the string */
    trim_spaces(&str);

    /* Verify the macro initialization line */
    if (verify_macro(str, location, macr_trie) == TRUE) {
        /* Add the macro to the trie and return the status of this process */
        return add_macr(macr_trie, str);
    }
    /* If the process failed - return FALSE */
    return FALSE;
}

/**
 * The `copy_macro_to_file` function copies the contents of a macro to the specified output file.
 *
 * @param macr The macro node containing the macro data to copy.
 * @param file The output file to write the macro contents to.
 */
static void copy_macro_to_file(TrieNode *macr, FILE* file) {
    /* Retrieve the macro data from the trie node */
    MacroData* data = (MacroData*) macr->data;
    /* Get the head of the line list in the macro data */
    LineNode* current = data->head;

    /* Iterate over each line and write it to the output file */
    while (current != NULL) {
        fputs(current->line, file);
        current = current->next;
    }
}

/**
 * Private function - cleans up resources by closing files and freeing allocated memory for filenames.
 * This helper function is used to centralize the cleanup process . It closes any open files and frees
 * any allocated memory for filenames if they are not NULL.
 *
 * @param source_file Pointer to the source file to be closed. If NULL, no action is taken.
 * @param output_file Pointer to the output file to be closed. If NULL, no action is taken.
 * @param source_filename Pointer to the source filename string to be freed. If NULL, no action is taken.
 * @param output_filename Pointer to the output filename string to be freed. If NULL, no action is taken.
 */
static void cleanup_files(FILE* source_file, FILE* output_file, char* source_filename, char* output_filename) {
    /* Close files */
    if (source_file) fclose(source_file);
    if (output_file) fclose(output_file);

    /* Free files names */
    if (source_filename) free(source_filename);
    if (output_filename) free(output_filename);

}
