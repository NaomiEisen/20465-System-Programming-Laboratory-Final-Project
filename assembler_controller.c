
#include "assembler_controller.h"
#include "errors.h"
#include <stdio.h>
void controller(int argc, char* argv[]) {
    int i = 1; /* index for iterating through loop */

    /* read inputted files */
    while (argc > 1) {
        preprocessor(argv[i]);
        if (global_error.code != NO_ERROR) {
            printf("Could not process file %s \n", argv[i]);
        }
        else {
            /* first step */
            /* second step */
        }
        i++;
        argc--;
    }
}
