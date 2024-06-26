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

void two_phase_assembler(const char *file_origin, const char *file_am, MacroTrie *macr_trie) {
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
    if (init_cmp_data(&cmp_data, file_origin) == FALSE) {
        return;
    }

    /* -------------------------------------- First phase -------------------------------------- */
    while (fgets(line, sizeof(line), source_file) != NULL) {
        line_count++; /* Update counter */
        node = parseLine(macr_trie, file_am, line_count, line); /* Parse line */

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

    /* Update directive address */
    updt_dir_addr(cmp_data.label_table.root, cmp_data.code.count+IC_START-1);

    /* Reset the file pointer to the beginning */
    fseek(source_file, 0, SEEK_SET);

    /* Set line counters */
    line_count = 1;
    unresolved_line = get_unresolved_line(&cmp_data);
    /* -------------------------------------- Second phase -------------------------------------- */

    while (error_in_file == FALSE && fgets(line, sizeof(line), source_file) != NULL) {
        if (line_count != unresolved_line) {
            line_count++;
            continue;
        }
        /* Parse only unresolved lines */
        node = parseLine(NULL, file_am, line_count, line);

        if (second_phase_analyzer(node, &cmp_data) == FALSE){
                error_in_file = TRUE;
            }

        line_count++;
        unresolved_line = get_unresolved_line(&cmp_data);
        free_ast_node(node);
    }


    print_trie(cmp_data.label_table.root,"");
    printf("code image:\n");
    print_memory_image_marks(&cmp_data.code);
    printf("data image:\n");
    print_memory_image_marks(&cmp_data.data);
    free_label_tree(&cmp_data.label_table);
    /* close the file */
    fclose(source_file);
    clear_data(&cmp_data);
}