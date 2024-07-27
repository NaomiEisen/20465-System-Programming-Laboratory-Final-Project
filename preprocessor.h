#ifndef PREPROCESSOR_H
#define PREPROCESSOR_H
#include "macro_data.h"

/* ----------------------------------- Prototypes ------------------------------------*/
/**
 * The `preprocessor_controller` function handles preprocessing of the source file,
 * applying macro expansions, and writing the processed output to a new file with ".am" extension.
 *
 * @param file_origin The base name of the source file without extension.
 * @param macro_trie A trie structure containing macro definitions for preprocessing.
 * @return The name of the output file with the ".am" extension, or NULL if an error occurs.
 */
char *preprocessor_controller(const char *file_origin, MacroTrie *macro_trie);

#endif /* PREPROCESSOR_H */
