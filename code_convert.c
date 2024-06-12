#include "code_convert.h"
#include "boolean.h"
#include "hardware.h"
#include "ast.h"
#include "errors.h"

void code_command_name (int command_code) {


}

/* Function to set multiple bits from an integer */
void set_int_code(int position, int value, LineType type) {
    int i;
    int bit_per_line = NUM_OF_BYTES * BYTE_SIZE;

    for (i = 0; i < bit_per_line ; i++) {
        if (value & (1 << i)) {
            set_bit(position + i, type);
        }
    }
}


/**
 * Function that sets the bit at the specified index to 1 in the relevant memory image.
 * @param i
 * @param type
 */
void set_bit(int i, LineType type) {
    char mask;
    int byteIndex = i / BYTE_SIZE;
    int bitOffset = i % BYTE_SIZE;

    if (byteIndex >= NUM_OF_BYTES) {
        return;
    }

    mask = (char)(1 << bitOffset);

    switch (type) {
        case LINE_OPERATION:
            if (code.count < MEMORY_CAPACITY) {
                code.lines[code.count][byteIndex] |= mask;
                return;
            }
            break;

        case LINE_DIRECTIVE:
            if (data.count < MEMORY_CAPACITY) {
                data.lines[data.count][byteIndex] |= mask;
                return;
            }
            break;
    }
    set_general_error(&global_error, CPU_MEMORY_FULL);
}