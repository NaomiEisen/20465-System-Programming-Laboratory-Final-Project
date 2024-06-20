#include "hardware.h"
#include <stdio.h>

/* Directive table*/
DirectiveMapping directive_table[] = {
        {"string", STRING},
        {"data", DATA},
        {"entry", ENTRY},
        {"extern", EXTERN},
        {""} /* Empty string terminator */
};

/* Register table */
const char* registers[] = {
        "r0", "r1", "r2", "r3", "r4", "r5",
        "r6", "r7", NULL /* Null-terminate for safety */
};

/* Representing a mapping from a function name to its corresponding function  */
InstructMapping command_table[] = {
        {"mov", 2, {1,1,1,1} , {0,1,1,1} },
        {"cmp", 2, {1,1,1,1} , {1,1,1,1}},
        {"add", 2, {1,1,1,1} , {0,1,1,1}},
        {"sub", 2, {1,1,1,1} , {0,1,1,1}},
        {"lea", 2, {0,1,0,0} , {0,1,1,1}},
        {"clr", 1,  {0,1,1,1}},
        {"not", 1,  {0,1,1,1}},
        {"inc", 1, {0,1,1,1}},
        {"dec", 1, {0,1,1,1}},
        {"jmp", 1, {0,1,1,0}},
        {"bne", 1, {0,1,1,0}},
        {"red", 1, {0,1,1,1}},
        {"prn", 1, {1,1,1,1}},
        {"jsr", 1, {0,1,1,0}},
        {"rts", 0},
        {"stop",  0},
        {"", 0, }  /* Empty string terminator */
};
