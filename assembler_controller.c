/* ---------------------------------------------------------------------------------------
 *                                          Includes
 * --------------------------------------------------------------------------------------- */
#include <stdio.h>
#include <stdlib.h>
#include "errors.h"
#include "phase_controller.h"
#include "macro_data.h"
#include "preprocessor.h"
/* ---------------------------------------------------------------------------------------
 *                                         Functions
 * --------------------------------------------------------------------------------------- */
void controller(int argc, char* argv[]) {
    int i = 1;            /* Index for iterating through loop */
    char* file_am = NULL; /* File name with .am extension */
    MacroTrie macro_trie; /* Macro trie to save macro's data */

    /* no arguments were passed */
    if (argc < 2 ) {
        /* print error message */
        set_general_error(NO_ARGUMENTS);
        exit(0); /* return */
    }

    /* read inputted files */
    while (argc > 1) {
        /* Initialize the macro data structure */
        init_macr_trie(&macro_trie);
        /* Preprocess file */
        file_am = preprocessor_controller(argv[i], &macro_trie);
        print_trie_test(&macro_trie);

        /* If an error occur, continue to the next file */
        if (error_stat() != NO_ERROR) {
            printf("Could not process file %s \n", argv[i]);
        }

        /* Process file */
        else {
            phase_controller(argv[i], file_am, &macro_trie);
        }

        /* updated variables */
        i++;
        argc--;

        /* free structures before proceeding to the next file */
        free(file_am);
        free_macr_trie(&macro_trie);
    }
}