#include <stdio.h>
#include <stdlib.h>
#include "assembler_controller.h"
#include "errors.h"
#include "first_phase.h"

char* preprocessor(const char* file_origin);

void controller(int argc, char* argv[]) {
    int i = 1; /* index for iterating through loop */
    char* file_am = NULL;

    /* no arguments were passed */
    if (argc < 2 ) {
        /* print error message */
        set_general_error(&global_error, NO_ARGUMENTS);
        print_error(&global_error);
        exit(0); /* return */
    }

    /* read inputted files */
    while (argc > 1) {
        file_am = preprocessor(argv[i]);
        if (global_error.code != NO_ERROR) {
            printf("Could not process file %s \n", argv[i]);
        }
        else {
            first_phase(file_am);
            /* second step */
        }
        i++;
        argc--;
        free(file_am);
    }
}
