//
// Created by naomi on 25/05/2024.
//

#include "errors.h"
void controller(int argc, char* argv[]);

int main(int argc, char* argv[]) {
    /* no arguments were passed */
    if (argc < 2 ) {
        /* print error message */
        set_error(&global_error, NO_ARGUMENTS, "", 0);
        print_error(&global_error);
        return 0; /* return */
    }
    /* read files */
    controller(argc, argv);
    return 0; /* return value of main */
}