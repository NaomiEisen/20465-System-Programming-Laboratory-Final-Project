/* ---------------------------------------------------------------------------------------
 *                                          Includes
 * --------------------------------------------------------------------------------------- */
#include <stdlib.h>
#include <stdio.h>
#include "../../utils/headers/utils.h"
#include "../../structures/headers/ast.h"
#include "../../structures/headers/cmp_data.h"
#include "../headers/first_phase.h"
#include "../headers/second_phase.h"
#include "../../utils/headers/output_files.h"
#include "../../structures/headers/macro_data.h"
#include "../headers/parser.h"
/* ---------------------------------------------------------------------------------------
 *                               Static Functions Prototypes
 * --------------------------------------------------------------------------------------- */
static void first_phase_controller(FILE* file_am, const char* file_name, MacroTrie *macr_trie, CmpData *cmp_data);
static void second_phase_controller(FILE* file_am, const char* file_name, MacroTrie *macr_trie, CmpData *cmp_data);
static Boolean create_obj_file(const char* source_file_name, CmpData* cmp_data);
static void free_program_data(CmpData *cmp_data, FILE *source_file, Boolean delete);
/* ---------------------------------------------------------------------------------------
 *                             Head Function Of Phase Controller
 * --------------------------------------------------------------------------------------- */
/**
 * The `phase_controller` function orchestrates the two phases of the assembler on the am file.
 * It conducts the first and second phases of assembly and creates the final object file if both
 * phases were successful.
 *
 * @param origin_file_name The name of the original source file.
 * @param file_name_am The name of the preprocessed source file (.am).
 * @param macr_trie The trie structure containing macro definitions.
 */
void phase_controller(const char *origin_file_name, const char *file_name_am, MacroTrie *macr_trie) {
    FILE* file_am = NULL;    /* the source file (.am) */
    CmpData cmp_data = {0};  /* program's data - initialize the memory image to 0 */
    ErrorCode cmp_init_status;

    /* Open the am file in read mode */
    if (!(file_am = fopen(file_name_am, "r"))) {
        /* if the file fails to open, set an error_in_file and return */
        set_general_error(FAILED_OPEN_FILE);
        fclose(file_am); /* close the file */
        return;
    }

    /* initialize the computer's data with the specified content */
    cmp_init_status = init_cmp_data(&cmp_data, origin_file_name);
    if (cmp_init_status != NO_ERROR) {
        set_general_error(cmp_init_status);
        free_program_data(&cmp_data, file_am, TRUE);
        return;
    }

    /* -------------------------------------- First phase -------------------------------------- */
    first_phase_controller(file_am, file_name_am, macr_trie, &cmp_data);
    if (get_status() != ERROR_FREE_FILE) {
        free_program_data(&cmp_data, file_am, TRUE);
        return;
    }

    /* Update address */
    update_addr(cmp_data.label_table.root, cmp_data.image.code_count + IC_START, DIRECTIVE);

    /* -------------------------------------- Second phase -------------------------------------- */
    second_phase_controller(file_am, file_name_am, macr_trie, &cmp_data);
    if (get_status() != ERROR_FREE_FILE) {
        free_program_data(&cmp_data, file_am, TRUE);
        return;
    }

    /* ------------------------------------ Create object file ----------------------------------- */
    if (create_obj_file(origin_file_name, &cmp_data) == FALSE) {
        free_program_data(&cmp_data, file_am, TRUE);
    } else {
        free_program_data(&cmp_data, file_am, FALSE);
    }
}

/* ---------------------------------------------------------------------------------------
 *                                       Phases Functions
 * --------------------------------------------------------------------------------------- */
/**
 * The `first_phase_controller` function performs the first phase of the assembly process.
 * It reads each line of the preprocessed file, parses it with the help of the 'parseLine' method,
 * and analyzes with the method 'first_phase_analyzer' it in the first phase context.
 * It updates the line code_count, parses lines, and handles errors if they occur.
 *
 * @param file_am The preprocessed source file (.am).
 * @param file_name The name of the preprocessed source file.
 * @param macr_trie The trie structure containing macro names.
 * @param cmp_data The data structure holding various program-related data during assembly.
 * @return TRUE if the first phase completes without errors, FALSE otherwise.
 */
static void first_phase_controller(FILE* file_am, const char* file_name, MacroTrie *macr_trie, CmpData *cmp_data) {
    int line_count = 0;                                 /* line counter */
    char line[MAX_LINE_LENGTH] = {0};   /* string to hold the read line */
    ASTNode* node = NULL; /* Ast node that will contain the parsed line */

    /* Read line from the am file */
    while (fgets(line, sizeof(line), file_am) != NULL) {
        line_count++; /* Update counter */

        /* Parse line */
        node = parseLine(macr_trie, file_name, line_count, line);

        /* If an error occurred - ASTNode is not completed, therefore cannot be encoded */
        if (get_error() != NO_ERROR) {
            /* Clear for enabling the processing of the next lines */
            clear_error();
            free_ast_node(node); /* Free nodes */
            continue;
        }

        first_phase_analyzer(node, cmp_data);

        free_ast_node(node); /* Free the astNode */
        clear_error(); /* Clear error for the next line */
    }

    /* Reset the file pointer to the beginning */
    fseek(file_am, 0, SEEK_SET);
}

/**
 * The `second_phase_controller` function performs the second phase of the assembly process.
 * It reads each unresolved line from the preprocessed file, parses it with the help of the 'parseLine'
 * method, and analyzes it with the method 'second_phase_analyzer' in the second phase context.
 * It updates the line code_count and handles unresolved lines specifically.
 *
 * @param file_am The preprocessed source file (.am).
 * @param file_name The name of the preprocessed source file.
 * @param macr_trie The trie structure containing macro names.
 * @param cmp_data The data structure holding various program-related data during assembly.
 * @return TRUE if the second phase completes without errors, FALSE otherwise.
 */
static void second_phase_controller(FILE* file_am, const char* file_name, MacroTrie *macr_trie, CmpData *cmp_data) {
    char line[MAX_LINE_LENGTH] = {0};   /* string to hold the read line */
    ASTNode* node = NULL; /* Ast node that will contain the parsed line */
    int line_count = 1;                                 /* line counter */
    int unresolved_line = get_unresolved_line(cmp_data);

    /* Read line from the am file */
    while (fgets(line, sizeof(line), file_am) != NULL) {
        if (line_count != unresolved_line) {
            line_count++;
            continue;
        }

        /* Parse only unresolved lines */
        node = parseLine(macr_trie, file_name, line_count, line);

        /* Encode Unresolved line - the second phase */
        second_phase_analyzer(node, cmp_data);

        /* Update counters and get the next line */
        line_count++;
        unresolved_line = get_unresolved_line(cmp_data);
        free_ast_node(node); /* Free the ASTNode memory */
    }
}

/* ---------------------------------------------------------------------------------------
 *                                Utility Functions Functions
 * --------------------------------------------------------------------------------------- */
/**
 * Creates an object file with the `.ob` extension from the source file name.
 * It writes the program's memory images to the object file with the method 'print_memory_images'.
 *
 * @param source_file_name The name of the source file.
 * @param cmp_data The data structure holding various program-related data during assembly.
 * @return TRUE if the object file is created and written successfully, FALSE otherwise.
 */
static Boolean create_obj_file(const char* source_file_name, CmpData* cmp_data) {
    char* file_ob = NULL;              /* the object file name */
    FILE* object_file;                 /* the object file (.ob) */


    /* ------------- Create the source filename with the specified extension -------------*/
    if (!create_new_file_name(source_file_name, &file_ob, ".ob")) {
        set_general_error(MEMORY_ALLOCATION_ERROR);
        return FALSE;
    }

    /* ----------------------- Open the source file in read mode ----------------------- */
    if (!(object_file = fopen(file_ob, "w"))) {
        /* If the file fails to open, set an error and return */
        set_general_error(FAILED_OPEN_FILE);
        free(file_ob);
        return FALSE;
    }
    else { /* Print the memory image */
        print_memory_images(object_file, cmp_data);
        fclose(object_file); /* close the file */
        free(file_ob);
        return TRUE;
    }
}

/**
 * Frees the resources allocated for the program data, closes the source file,
 * and optionally deletes the files associated with the compiled data.
 * The files will be deleted if an error occurred during the processing stages.
 *
 * @param cmp_data The data structure holding various program-related data during assembly.
 * @param source_file The source file to be closed.
 * @param delete Flag indicating whether to delete the files associated with the compiled data.
 */
static void free_program_data(CmpData *cmp_data, FILE *source_file, Boolean delete) {
    free_label_tree(&cmp_data->label_table);
    fclose(source_file);
    free_cmp_data(cmp_data, delete);
}