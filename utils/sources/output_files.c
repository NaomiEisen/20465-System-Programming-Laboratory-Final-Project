/* ---------------------------------------------------------------------------------------
 *                                          Includes
 * --------------------------------------------------------------------------------------- */
#include "../headers/output_files.h"
#include "../../assembler/headers/code_convert.h"
/* ---------------------------------------------------------------------------------------
 *                                       Functions
 * --------------------------------------------------------------------------------------- */
/**
 * Write a label and its associated address to the specified file in fixed positions.
 * The label is left-justified and can be up to 31 characters.
 * The address is right-justified and always 4 digits.
 *
 * @param label The label to be written (maximum 30 characters).
 * @param address The address associated with the label (4 digits).
 * @param file The file to which the label and address should be written.
 */
void write_label(const char* label, int address, FILE* file) {
    fprintf(file, "%-31s %04d\n", label, address);
}

/**
 * Print the memory images to the specified output file in octal format.
 * This function prints the counts of image and data images, followed by each line
 * of the image and data images formatted in octal. The addresses are incremented from
 * a starting point, and each line is converted from its original format to octal for
 * output.
 *
 * @param output_file The file to which the memory images should be written.
 * @param data A pointer to the CmpData structure containing the image and data images.
 */
void print_memory_images(FILE *output_file, CmpData *data) {
    int i; /* Variable to iterate through the memory images */
    int data_addres = data->image.code_count+IC_START;

    /* Print the data and code images counts */
    fprintf(output_file,"%4d   %-5d\n", data->image.code_count, data->image.data_count);

    /* Print image image */
    for (i = 0; i < data->image.code_count; i++) {
        fprintf(output_file, "%04d   %05o\n", IC_START + i, convert_to_octal(data->image.lines[i]));
    }

    /* Print data image */
    for (i = MEMORY_CAPACITY-1 ; i >= MEMORY_CAPACITY-data->image.data_count; i--) {
        fprintf(output_file, "%04d   %05o\n", data_addres, convert_to_octal(data->image.lines[i]));
        data_addres++;
    }
}

