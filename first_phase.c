/* ---------------------------------------------------------------------------------------
 *                                          Includes
 * --------------------------------------------------------------------------------------- */
#include <stdlib.h>
#include <stdio.h>
#include "errors.h"
#include "first_phase.h"
#include "ast.h"
#include "parser.h"
#include "boolean.h"
#include "cmp_data.h"

/* ---------------------------------------------------------------------------------------
 *                               Head Function Of First Phase
 * --------------------------------------------------------------------------------------- */

void first_phase(const char *file_am) {
    FILE* source_file;                  /* the source file (.as) */
    char* output_filename = NULL;       /* the output file name */
    int line_count = 0;                 /* line counter */
    char line[MAX_LINE_LENGTH] = {0};   /* string to hold the read line */
    ASTNode* node = NULL;
    boolean error = FALSE;
    CmpData cmp_data;

    /* -------------------------- Open the am file in read mode -------------------------- */
    if (!(source_file = fopen(file_am, "r"))) {
        /* if the file fails to open, set an error and return */
        set_general_error(&global_error, CANNOT_OPEN_FILE);
        print_error(&global_error);
        fclose(source_file); /* close the file */
        return;
    }

    /* initialize the computer's data */
    init_cmp_data(&cmp_data);

    /* ------------------------ Process each line in the source file ------------------------ */
    while (fgets(line, sizeof(line), source_file) != NULL) {
        line_count++; /* Update counter */
        node = parseLine(line, file_am, line_count); /* Parse line */
        printASTNode(node); /* TODO FOR ME!! PRINT NODE */


        if (global_error.code != NO_ERROR) {
            clear_error(&global_error);
            error = TRUE;
            free_ASTNode(node);
            continue;
        }

        if (error == FALSE) {
            /* proceed analyzing line */

        }

        free_ASTNode(node); /* Free AST nodes */

    }

    /* close the file */
    fclose(source_file);
}