#include <stdio.h>
#include <stdlib.h>
#include "errors.h"
#include "first_phase.h"
#include "mappings.h"

char *preprocessor(const char *file_origin, Mappings *mappings);

void controller(int argc, char* argv[]) {
    int i = 1; /* index for iterating through loop */
    char* file_am = NULL;
    Mappings mappings;

    /* Initialize mappings for the program */
    init_mappings(&mappings);

    /* no arguments were passed */
    if (argc < 2 ) {
        /* print error message */
        set_general_error(&error, NO_ARGUMENTS);
        print_error(&error);
        exit(0); /* return */
    }

    /* read inputted files */
    while (argc > 1) {
        file_am = preprocessor(argv[i], &mappings);
        if (error.code != NO_ERROR) {
            printf("Could not process file %s \n", argv[i]);
        }
        else {
            first_phase(file_am, &mappings);
            /* second step */
        }
        i++;
        argc--;
        free(file_am);
    }
}
