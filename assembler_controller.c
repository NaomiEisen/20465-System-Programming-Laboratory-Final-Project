#include <stdio.h>
#include <stdlib.h>
#include "errors.h"
#include "two_phase_assembler.h"
#include "macro_data.h"

char *preprocessor(const char *file_origin, MacroTrie *macro_trie);

void controller(int argc, char* argv[]) {
    int i = 1; /* index for iterating through loop */
    char* file_am = NULL;
    MacroTrie macro_trie;

    /* no arguments were passed */
    if (argc < 2 ) {
        /* print error message */
        set_general_error(NO_ARGUMENTS);
        exit(0); /* return */
    }

    /* read inputted files */
    while (argc > 1) {
        init_macr_trie(&macro_trie);
        file_am = preprocessor(argv[i], &macro_trie);
        print_trie_test(&macro_trie);
        /*free_trie_data(&macro_trie);*/

        if (error_stat() != NO_ERROR) {
            printf("Could not process file %s \n", argv[i]);
        }

        else {
            two_phase_assembler(argv[i], file_am, &macro_trie);
        }
        i++;
        argc--;
        free(file_am);
        free_macr_trie(&macro_trie);
    }
}