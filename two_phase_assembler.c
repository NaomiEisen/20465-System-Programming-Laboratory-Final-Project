/* ---------------------------------------------------------------------------------------
 *                                          Includes
 * --------------------------------------------------------------------------------------- */
#include <stdlib.h>
#include <stdio.h>
#include "errors.h"
#include "two_phase_assembler.h"
#include "ast.h"
#include "parser.h"
#include "boolean.h"
#include "cmp_data.h"
#include "first_phase.h"
#include "second_phase.h"
#include "label_data.h"


/* ---------------------------------------------------------------------------------------
 *                               Head Function Of First Phase
 * --------------------------------------------------------------------------------------- */

void first_phase(const char *file_am) {
    FILE* source_file;                  /* the source file (.as) */
    char* output_filename = NULL;       /* the output file name */
    int line_count = 0;                 /* line counter */
    int unresolved_line;
    char line[MAX_LINE_LENGTH] = {0};   /* string to hold the read line */
    ASTNode* node = NULL;
    boolean error_in_file = FALSE;
    CmpData cmp_data;

    /* -------------------------- Open the am file in read mode -------------------------- */
    if (!(source_file = fopen(file_am, "r"))) {
        /* if the file fails to open, set an error_in_file and return */
        set_general_error(CANNOT_OPEN_FILE);
        print_error();
        fclose(source_file); /* close the file */
        return;
    }

    /* initialize the computer's data */
    init_cmp_data(&cmp_data);
    /*print_ten(&cmp_data.code);*/

    /* ------------------------ Process each line in the source file ------------------------ */
    while (fgets(line, sizeof(line), source_file) != NULL) {
        line_count++; /* Update counter */
        node = parseLine(line, file_am, line_count); /* Parse line */
        /*print_AST_node(node);    TODO FOR ME!! PRINT NODE */

        if (error_stat() != NO_ERROR) {
            clear_error();
            error_in_file = TRUE;
            free_ast_node(node);
            continue;
        }

        if (error_in_file == FALSE) {
            if (first_phase_analyzer(node, &cmp_data) == FALSE)
                error_in_file = TRUE;
        }

        free_ast_node(node);
    }

    /* Reset the file pointer to the beginning */
    fseek(source_file, 0, SEEK_SET);
    line_count = 1;
    unresolved_line = get_unresolved_line(&cmp_data);
    /* ------------------------ Second phase ------------------------ */

    while (error_in_file == FALSE && fgets(line, sizeof(line), source_file) != NULL) {
        if (line_count != unresolved_line) {
            line_count++;
            continue;
        }
        /* Parse only unresolved lines */
        node = parseLine(line, file_am, line_count);

        if (second_phase_analyzer(node, &cmp_data) == FALSE){
                error_in_file = TRUE;
            }

        line_count++;
        free_ast_node(node);
    }


    print_error();

    /*print_trie(cmp_data.label_table.root,"");*/
    printf("code image:\n");
    print_memory_image_marks(&cmp_data.code);
    printf("data image:\n");
    print_memory_image_marks(&cmp_data.data);
    free_label_tree(&cmp_data.label_table);
    /* close the file */
    fclose(source_file);
}