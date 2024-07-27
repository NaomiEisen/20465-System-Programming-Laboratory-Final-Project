#ifndef FIRST_PHASE_H
#define FIRST_PHASE_H
#include "macro_data.h"

/* ----------------------------------- Functions Prototypes ------------------------------------*/
/**
 * The `phase_controller` function orchestrates the two phases of the assembler on the am file.
 * It conducts the first and second phases of assembly and creates the final object file if both
 * phases were successful.
 *
 * @param origin_file_name The name of the original source file.
 * @param file_name_am The name of the preprocessed source file (.am).
 * @param macr_trie The trie structure containing macro definitions.
 */
void phase_controller(const char *origin_file_name, const char *file_name_am, MacroTrie *macr_trie);

#endif /* FIRST_PHASE_H */
