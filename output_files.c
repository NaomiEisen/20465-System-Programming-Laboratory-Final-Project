#include "output_files.h"
#include "code_convert.h"

void write_label(const char* label, int address, FILE* file) {
    fprintf(file, "%s  |  %d\n", label, address);
}

/* Function to print the memory images in octal format */
void print_memory_images(FILE *output_file, CmpData *data) {
    int i;
    fprintf(output_file, "  %d   %d\n", data->code.count, data->data.count);

    /* Print code image */
    for (i = 0; i < data->code.count; i++) {
        fprintf(output_file, "%04o   %05o\n", 0100 + i, convert_to_octal(data->code.lines[i]));
    }

    /* Print data image */
    for (i = 0; i < data->data.count; i++) {
        fprintf(output_file, "%04o   %05o\n", data->code.count + i, convert_to_octal(data->data.lines[i]));
    }
}