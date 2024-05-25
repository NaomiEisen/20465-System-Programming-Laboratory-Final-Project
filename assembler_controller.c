//
// Created by naomi on 25/05/2024.
//
#include "assembler_controller.h"
#include "errors.h"
void controller(int argc, char* argv[]) {
    int i = 1; /* index for iterating through loop */

    /* read inputted files */
    while (argc > 1) {
        preprocessor(argv[i]);
        if (global_error.code != NO_ERROR) {
            printf("%s %s", global_error.message, argv[i]);
        }
        i++;
        argc--;
    }
}
