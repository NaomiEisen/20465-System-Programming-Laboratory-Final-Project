
#ifndef FIRST_PHASE_H
#define FIRST_PHASE_H

#include "mappings.h"
#include "macro_data.h"

#define MAX_LINE_LENGTH 81
void two_phase_assembler(const char *file_origin, const char *file_am, MacroTrie *macr_trie);
#endif /* FIRST_PHASE_H */
