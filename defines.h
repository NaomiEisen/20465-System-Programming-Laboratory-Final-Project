#ifndef DEFINES_H
#define DEFINES_H

/* ---------- General macro's ----------*/
#define MAX_LINE_LENGTH 81

/* ------- Macro's for preprocessor ------*/
#define MACR_START "macr"
#define MACR_END "endmacr"

/* ------- Macro's for assembler phases ------*/
#define SECOND_REG_POSITION 3
#define A 12
#define R 13
#define E 14
#define REGISTER_POS 6
#define SRC_OFFSET 7
#define DEST_OFFSET 11

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


#endif /*DEFINES_H*/
