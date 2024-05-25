//
// Created by naomi on 25/05/2024.
//
#include <stdlib.h>
#include <stdio.h>
#include <string.h>
#include "preprocessor.h"
#include "errors.h"
#include "general.h"
#include "macro_list.h"

const char* reserved_words[] = {
        "r0", "r1", "r2", "r3", "r4", "r5", "r6", "r7",
        "mov", "cmp", "add", "sub", "lea", "clr", "not", "inc",
        "dec", "jmp", "bne", "red", "prn", "jsr", "rts", "stop"
};

void preprocessor(char* file_origin){
    FILE* fd;                           /* the corresponding file */
    char word[MAX_LINE_LENGTH] = {0};   /* string to hold one read word from line */
    char line[MAX_LINE_LENGTH] = {0};   /* string to hold the read line */
    int line_count = 0;              /* line counter */
    bool inside_macro = false;          /* flag that indicated if read line is part of macro */
    MacroList macr_list = {NULL};       /* Initialize empty list of macros */

    /* open file in read mode */
    if (!(fd = fopen(file_origin, "r"))) {
        /* if the file fails to open, set an error and return */
        set_error(CANNOT_OPEN_FILE);
        fclose(fd); /* close the file */
        return;
    }

    /* read lines from the file */
    while (fgets(word, sizeof(line), fd) != NULL) {
        /* update counter */
        line_count++;

        /* if line is not empty */
        if (sscanf(line, "%s", word) == 1) {
            printf("%s\n", word); /* FOR ME */

            /* ignore comment line */
            if (! is_comment(word))
                continue;

            /* if this line is initialized macro */
            if (inside_macro) {
                /* copy to macro */
            }

            /* if this line is macro initialization line */
            if (!macr_start(word)) {
                inside_macro = true;

                /* verify macro */
                if (!vertify_macro(line)) {
                    printf("Error %s in line %d", global_error.message, line_count);
                    break;
                }

                /* create new macro in the macro list */
                create_macro(macr_list, word, line);
            }

            /* regular command line */
            else {
                /* copy */
            }

            // do something with the word
        }
    }

    free_macros(&macr_list);
    /* close the file */
    fclose(fd);
}

bool vertify_macro(char *line) {
    char word[MAX_LINE_LENGTH] = {0};   /* string to hold one read word from line */
    if (sscanf(line, "%s", word) != 1)
        return false;
    if (is_reserved_word(word)) {
        set_error(INVALID_MACR);
        return false;
    }
    if (is_empty_line(line) == false) {
        set_error(EXTRA_TXT);
        return false;

    }

    return true;
}

int is_comment(const char* str) {
    return str[0] == ';' ;
}

int macr_start(const char* str) {
    return strcmp(str ,MACR_START );
}

bool is_macro(const char* str) {

}

bool is_reserved_word(const char* str) {
    int i;

    for (i = 0 ; i< sizeof(reserved_words) ; i++) {
        if (strcmp(str , reserved_words[i]) == 0 ) {
            return true;
        }
    }
    return false;
}

/* Function to check whether the scanned line does not contain any words */
bool is_empty_line(const char* line) {
    char word[MAX_LINE_LENGTH];
    if (sscanf(line, "%s", word) != 1) {
        return true;/* Line does not contain any words */
    }
    return false;   /* Line contains at least one word */
}

void create_macro(MacroList* list, const char* name, const char* line) {
    insert_macro_node(list, name, line);
}