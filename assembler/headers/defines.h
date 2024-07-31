#ifndef DEFINES_H
#define DEFINES_H

/* ---------- General macro's ----------*/
/* Maximum length of a command line in the source file */
#define MAX_LINE_LENGTH 81
#define MAX_INTEGER 32767


/* ------- Macro's for assembler phases ------*/
/* Second register's position in a word */
#define SECOND_REG_POSITION 3
#define REGISTER_POS 6

#define A 12 /* Absolute */
#define R 13 /* Relocatable */
#define E 14 /* External */

/* offset of the source operand in the first word */
#define SRC_OFFSET 7
/* offset of the destination operand in the first word */
#define DEST_OFFSET 11

#define WORD_END_POS 14
#define LAST_WORD_BIT 15


/* ------- Macro's for address modes  ------*/

#define ADDR_MODE_IMMEDIATE 0
#define ADDR_MODE_DIRECT 1
#define ADDR_INDIRECT_REG 2
#define ADDR_DIRECT_REG 3

/* ------- Macro's for programs data  ------*/
#define MAX_LABEL_LENGTH 32
#define MEMORY_CAPACITY 4096
#define NUM_OF_BYTES 2
#define IC_START 100
#define BYTE_SIZE 8
#define REGISTER_BIT_SIZE 3
#define IMMIDIATE_DIRECTIVE_BIT_SIZE 12


#endif /*DEFINES_H*/
