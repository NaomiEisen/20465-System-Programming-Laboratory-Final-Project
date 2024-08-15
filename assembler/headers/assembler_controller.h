#ifndef ASSEMBLER_CONTROLLER_H
#define ASSEMBLER_CONTROLLER_H
/* ----------------------------------- Functions Prototypes ------------------------------------*/
/**
 * Controller function to handle the execution of the assembler program.
 *
 * This function manages the workflow for processing each input file passed as command line arguments.
 * It first preprocesses the files to handle macros, and then proceeds with the main processing phases.
 * If any errors occur during processing, appropriate messages are displayed and processing may halt
 * depending on the severity of the error.
 *
 * @param argc The number of command line arguments.
 * @param argv The array of command line arguments.
 */
void controller(int argc, char* argv[]);
#endif /* ASSEMBLER_CONTROLLER_H */
