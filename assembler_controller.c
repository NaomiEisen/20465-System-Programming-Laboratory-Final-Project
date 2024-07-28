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
        print_trie_test(&macro_trie); /* todo for meeee */

        /* Continue processing only if no error occurred */
        if (error_stat() == NO_ERROR) {
            phase_controller(argv[i], file_am, &macro_trie);
        } else {
            printf("Could not process file %s \n", argv[i]);
        }

        /* Check for fatal error */
        if (error_stat() == MEMORY_ALLOCATION_ERROR) {
            exit(0);
        }

        /* updated variables */
        i++;
        argc--;

        /* free structures before proceeding to the next file */
        free(file_am);
        free_macr_trie(&macro_trie);
        clear_error(); /* Clear error status for next files */
    }
}