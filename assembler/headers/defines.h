#ifndef DEFINES_H
#define DEFINES_H
/* -------------------- General macro's --------------------*/
/* Maximum length of a command line in the source file */
/* 80 chars + '/n' if exists + '/0' */
#define MAX_LINE_LENGTH 82
/* For printing purposes */
#define MAX_LINE_PRINTABLE (MAX_LINE_LENGTH - 2)

/* Maximum length of a label */
/* 31 chars + '/0' (null-terminator) */
#define MAX_LABEL_LENGTH 32
/* For printing purposes */
#define MAX_LABEL_PRINTABLE (MAX_LABEL_LENGTH - 1)

/* The range of integer values that can be represented with 15 bits */
/*#define MAX_VALUE 16383*/
/*#define MIN_VALUE (-16384)*/

/* --------------- Macro's for assembler phases --------------*/
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

/* Last bit for a word */
#define WORD_END_POS 14
/* Last bit of the struct */
#define LAST_WORD_BIT 15

/* ----------------- Macro's for address modes  ----------------*/
#define ADDR_MODE_IMMEDIATE 0
#define ADDR_MODE_DIRECT 1
#define ADDR_INDIRECT_REG 2
#define ADDR_DIRECT_REG 3

/* ----------------- Macro's for programs data  ----------------*/
#define MEMORY_CAPACITY 3996 /* Starting address is 100, therefore 4096-100 */
#define NUM_OF_BYTES 2
#define BYTE_SIZE 8
#define IC_START 100
#define REGISTER_BIT_SIZE 3
#define IMMEDIATE_DIRECTIVE_BIT_SIZE 12

#endif /*DEFINES_H*/
