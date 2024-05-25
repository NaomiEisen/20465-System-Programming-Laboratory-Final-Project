//
// Created by naomi on 25/05/2024.
//

#include "errors.h"
void controller(int argc, char* argv[]);

int main(int argc, char* argv[]) {
    /* no arguments were passed */
    if (argc < 2 ) {
        /* print error message */
        set_error(NO_ARGUMENTS);
        return 0; /* return */
    }
    /* read files */
    controller(argc, argv);
    return 0; /* return value of main */
}