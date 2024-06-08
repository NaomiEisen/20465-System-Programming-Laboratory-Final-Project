/* ---------------------------------------------------------------------------------------
 *                                          Includes
 * --------------------------------------------------------------------------------------- */
#include <stdlib.h>
#include <stdio.h>
#include <string.h>
#include "preprocessor.h"
#include "errors.h"
#include "general.h"
#include "macro_list.h"
#include "utils.h"
#include "first_phase.h"
#include "ast.h"
#include "parser.h"

/* ---------------------------------------------------------------------------------------
 *                               Head Function Of First Phase
 * --------------------------------------------------------------------------------------- */

void first_phase(const char* file_am, const char* origin_filename) {
    FILE* source_file;                  /* the source file (.as) */
    FILE* output_file;                  /* the output file (.am) */
    char* output_filename = NULL;       /* the output file name */
    int line_count = 0;                 /* line counter */
    char line[MAX_LINE_LENGTH] = {0};   /* string to hold the read line */
    ASTNode* node = NULL;
    bool error = false;

    /* -------------------------- Open the am file in read mode -------------------------- */
    if (!(source_file = fopen(file_am, "r"))) {
        /* if the file fails to open, set an error and return */
        set_error(&global_error, CANNOT_OPEN_FILE, origin_filename, 0);
        print_error(&global_error);
        fclose(source_file); /* close the file */
        return;
    }


    /* ------------------------ Process each line in the source file ------------------------ */
    while (fgets(line, sizeof(line), source_file) != NULL) {
        line_count++; /* Update counter */
        node = parseLine(line); /* Parse line */
        printASTNode(node); /* TODO FOR ME!! PRINT NODE */


        if (global_error.code != NO_ERROR) {
            set_error(&global_error, global_error.code, origin_filename, line_count);
            print_error(&global_error);
            clear_error(&global_error);
            error = true;
            freeASTNode(node);
            continue;
        }

        if (error == false) {
            /* proceed analyzing line */

        }

        freeASTNode(node); /* Free AST nodes */

    }

    /* close the file */
    fclose(source_file);
}