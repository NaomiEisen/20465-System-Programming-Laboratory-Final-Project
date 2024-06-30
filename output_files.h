#ifndef OUTPUT_FILES_H
#define OUTPUT_FILES_H
#include <stdio.h>
#include "cmp_data.h"

void write_label(const char* label, int address, FILE* file);
void print_memory_images(FILE *output_file, CmpData *data);
#endif /* OUTPUT_FILES_H */
