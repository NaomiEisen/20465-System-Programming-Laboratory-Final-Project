#ifndef OUTPUT_FILES_H
#define OUTPUT_FILES_H
/* ----------------------------------------- Includes -----------------------------------------*/
#include <stdio.h>
#include "../../structures/headers/cmp_data.h"
/* ----------------------------------- Functions Prototypes ---------------------------------- */
/**
 * Write a label and its associated address to the specified file.
 *
 * @param label The label to be written.
 * @param address The address associated with the label.
 * @param file The file to which the label and address should be written.
 */
void write_label(const char* label, int address, FILE* file);

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
void print_memory_images(FILE *output_file, CmpData *data);

#endif /* OUTPUT_FILES_H */
