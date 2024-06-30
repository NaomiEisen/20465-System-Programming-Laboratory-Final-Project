/* ---------------------------------------------------------------------------------------
 *                                          Includes
 * --------------------------------------------------------------------------------------- */
#include <stdlib.h>
#include <stdio.h>
#include "errors.h"
#include "utils.h"
#include "phase_controller.h"
#include "ast.h"
#include "parser.h"
#include "boolean.h"
#include "cmp_data.h"
#include "first_phase.h"
#include "second_phase.h"
#include "label_data.h"
#include "output_files.h"


/* ---------------------------------------------------------------------------------------
 *                               Head Function Of Two Phase Assembler
 * --------------------------------------------------------------------------------------- */

void phase_controller(const char *origin_file_name, const char *file_name_am, MacroTrie *macr_trie) {
    FILE* file_am;    /* the source file (.am) */
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
    if (first_phase_controller(file_am, file_name_am, macr_trie, &cmp_data) == FALSE) {
        free_program_data(&cmp_data, file_am, TRUE);
        return;
    }

    /* Update address */
    updt_addr(cmp_data.label_table.root, cmp_data.code.count + IC_START, DIRECTIVE);

    /* -------------------------------------- Second phase -------------------------------------- */

    if (second_phase_controller(file_am, file_name_am, macr_trie, &cmp_data) == FALSE) {
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

Boolean first_phase_controller(FILE* file_am, const char* file_name, MacroTrie *macr_trie, CmpData *cmp_data) {
    int line_count = 0;                 /* line counter */
    char line[MAX_LINE_LENGTH] = {0};   /* string to hold the read line */
    ASTNode* node = NULL;
    Boolean no_error = TRUE;

    while (fgets(line, sizeof(line), file_am) != NULL) {
        line_count++; /* Update counter */
        node = parseLine(macr_trie, file_name, line_count, line); /* Parse line */

        if (error_stat() != NO_ERROR) {
            clear_error(); /* Enable processing the rest of the lines */
            no_error = FALSE;
            free_ast_node(node);
            continue;
        }

        if (no_error == TRUE) {
            if (first_phase_analyzer(node, cmp_data) == FALSE)
                no_error = FALSE;
        }
        free_ast_node(node);
    }

    /* Reset the file pointer to the beginning */
    fseek(file_am, 0, SEEK_SET);
    return no_error;
}

Boolean second_phase_controller(FILE* file_am, const char* file_name, MacroTrie *macr_trie, CmpData *cmp_data) {
    char line[MAX_LINE_LENGTH] = {0};   /* string to hold the read line */
    ASTNode* node = NULL;
    int line_count = 1;                 /* line counter */
    int unresolved_line = get_unresolved_line(cmp_data);
    Boolean no_error = TRUE;

    while (fgets(line, sizeof(line), file_am) != NULL) {
        if (line_count != unresolved_line) {
            line_count++;
            continue;
        }
        /* Parse only unresolved lines */
        node = parseLine(macr_trie, file_name, line_count, line);

        if (second_phase_analyzer(node, cmp_data) == FALSE){
            no_error = FALSE;
        }

        line_count++;
        unresolved_line = get_unresolved_line(cmp_data);
        free_ast_node(node);
    }
    return no_error;
}

Boolean create_obj_file(const char* source_file_name, CmpData* cmp_data) {
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

void free_program_data(CmpData *cmp_data, FILE *source_file, Boolean delete) {
    free_label_tree(&cmp_data->label_table);
    fclose(source_file);
    free_cmp_data(cmp_data, delete);
}