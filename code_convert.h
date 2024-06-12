#ifndef CODE_CONVERT_H
#define CODE_CONVERT_H

#include "hardware.h"

#define BYTE_SIZE 8;

void set_bit(int i, LineType type);
void set_int_code(int position, int value, LineType type);

#endif /* CODE_CONVERT_H */
