/* ---------------------------------------------------------------------------------------
 *                                          Includes
 * --------------------------------------------------------------------------------------- */
#include "../headers/output_files.h"
#include "../../assembler/headers/code_convert.h"
/* ---------------------------------------------------------------------------------------
 *                                       Functions
 * --------------------------------------------------------------------------------------- */
/**
 * Write a label and its associated address to the specified file.
 *
 * @param label The label to be written.
 * @param address The address associated with the label.
 * @param file The file to which the label and address should be written.
 */
void write_label(const char* label, int address, FILE* file) {
    fprintf(file, "%s   %04d\n", label, address);
}

/**
 * Print the memory images to the specified output file in octal format.
 * This function prints the counts of code and data images, followed by each line
 * of the code and data images formatted in octal. The addresses are incremented from
 * a starting point, and each line is converted from its original format to octal for
 * output.
 *
 * @param output_file The file to which the memory images should be written.
 * @param data A pointer to the CmpData structure containing the code and data images.
 */
void print_memory_images(FILE *output_file, CmpData *data) {
    int i; /* Variable to iterate through the memory images */

    /* Print  the code and data images counts */
    fprintf(output_file, "  %d   %d\n", data->code.count, data->data.count);

    /* Print code image */
    for (i = 0; i < data->code.count; i++) {
        fprintf(output_file, "%04d   %05o\n", IC_START + i, convert_to_octal(data->code.lines[i]));
    }

    /* Print data image */
    for (i = 0; i < data->data.count; i++) {
        fprintf(output_file, "%04d   %05o\n", data->code.count +IC_START+ i, convert_to_octal(data->data.lines[i]));
    }
}

