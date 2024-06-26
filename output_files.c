#include "output_files.h"


void write_label(const char* label, int address, FILE* file) {
    fprintf(file, "%s  |  %d\n", label, address);
}