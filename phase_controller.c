/* ---------------------------------------------------------------------------------------
 *                                          Includes
 * --------------------------------------------------------------------------------------- */
#include <stdlib.h>
#include <stdio.h>
#include "errors.h"
#include "utils.h"
#include "ast.h"
#include "parser.h"
#include "boolean.h"
#include "cmp_data.h"
#include "first_phase.h"
#include "second_phase.h"
#include "label_data.h"
#include "output_files.h"
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
    CmpData cmp_data; /* program's data */

    /* Open the am file in read mode */
    if (!(file_am = fopen(file_name_am, "r"))) {
        /* if the file fails to open, set an error_in_file and return */
        set_general_error(FAILED_OPEN_FILE);
        fclose(file_am); /* close the file */
        return;
    }

    /* initialize the computer's data */
    if (init_cmp_data(&cmp_data, origin_file_name) == FALSE) {
        return;
    }

    /* -------------------------------------- First phase -------------------------------------- */
    /*if (first_phase_controller(file_am, file_name_am, macr_trie, &cmp_data) == FALSE) {
        free_program_data(&cmp_data, file_am, TRUE);
        return;
    }*/

    first_phase_controller(file_am, file_name_am, macr_trie, &cmp_data);
    if (get_status() != ERROR_FREE_FILE) {
        free_program_data(&cmp_data, file_am, TRUE);
        return;
    }

    /* Update address */
    update_addr(cmp_data.label_table.root, cmp_data.code.count + IC_START, DIRECTIVE);

    /* -------------------------------------- Second phase -------------------------------------- */
   /* if (second_phase_controller(file_am, file_name_am, macr_trie, &cmp_data) == FALSE) {
        free_program_data(&cmp_data, file_am, TRUE);
        return;
    }*/

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

    /* todo FOR ME !! ------------------------------------------*/
    print_trie(cmp_data.label_table.root,"");
    printf("code image:\n");
    print_memory_image_marks(&cmp_data.code);
    printf("data image:\n");
    print_memory_image_marks(&cmp_data.data);
    /*--------------------------------------------------------------*/
}

/* ---------------------------------------------------------------------------------------
 *                                       Phases Functions
 * --------------------------------------------------------------------------------------- */
/**
 * The `first_phase_controller` function performs the first phase of the assembly process.
 * It reads each line of the preprocessed file, parses it with the help of the 'parseLine' method,
 * and analyzes with the method 'first_phase_analyzer' it in the first phase context.
 * It updates the line count, parses lines, and handles errors if they occur.
 *
 * @param file_am The preprocessed source file (.am).
 * @param file_name The name of the preprocessed source file.
 * @param macr_trie The trie structure containing macro names.
 * @param cmp_data The data structure holding various program-related data during assembly.
 * @return TRUE if the first phase completes without errors, FALSE otherwise.
 */
static void first_phase_controller(FILE* file_am, const char* file_name, MacroTrie *macr_trie, CmpData *cmp_data) {
    int line_count = 0;                 /* line counter */
    char line[MAX_LINE_LENGTH] = {0};   /* string to hold the read line */
    ASTNode* node = NULL;

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
 * It updates the line count and handles unresolved lines specifically.
 *
 * @param file_am The preprocessed source file (.am).
 * @param file_name The name of the preprocessed source file.
 * @param macr_trie The trie structure containing macro names.
 * @param cmp_data The data structure holding various program-related data during assembly.
 * @return TRUE if the second phase completes without errors, FALSE otherwise.
 */
static void second_phase_controller(FILE* file_am, const char* file_name, MacroTrie *macr_trie, CmpData *cmp_data) {
    char line[MAX_LINE_LENGTH] = {0};   /* string to hold the read line */
    ASTNode* node = NULL;
    int line_count = 1;                 /* line counter */
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
 * The `create_obj_file` function creates an object file with the `.ob` extension from the source file name.
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
        /* if the file fails to open, set an error and return */
        set_general_error(FAILED_OPEN_FILE);
        /*fclose(object_file);  close the file */
        free(file_ob);
        return FALSE;
    }
    else {
        print_memory_images(object_file, cmp_data);
        fclose(object_file); /* close the file */
        free(file_ob);
        return TRUE;
    }
}

/**
 * The `free_program_data` function frees the resources allocated for the program data,
 * closes the source file, and optionally deletes the files associated with the compiled data.
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