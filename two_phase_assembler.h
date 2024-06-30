
#ifndef FIRST_PHASE_H
#define FIRST_PHASE_H

#include "mappings.h"
#include "macro_data.h"
#include "cmp_data.h"
#define MAX_LINE_LENGTH 81

void two_phase_assembler(const char *origin_file_name, const char *file_name_am, MacroTrie *macr_trie);
Boolean first_phase_controller(FILE* file_am, const char* file_name, MacroTrie *macr_trie, CmpData *cmp_data);
Boolean second_phase_controller(FILE* file_am, const char* file_name, MacroTrie *macr_trie, CmpData *cmp_data);
void free_program_data(CmpData *cmp_data, FILE *source_file, Boolean delete);
Boolean create_obj_file(const char* source_file_name, CmpData* cmp_data);
#endif /* FIRST_PHASE_H */
