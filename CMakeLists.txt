cmake_minimum_required(VERSION 3.27)
project(20465_System_Programming_Laboratory_Final_Project C)

set(CMAKE_C_STANDARD 11)


# Source files
set(SOURCE_FILES
        assembler/sources/assembler.c
        assembler/sources/assembler_controller.c
        assembler/sources/code_convert.c
        assembler/sources/first_phase.c
        assembler/sources/parser.c
        assembler/sources/phase_controller.c
        assembler/sources/preprocessor.c
        assembler/sources/second_phase.c
        # Structures sources
        structures/sources/ast.c
        structures/sources/cmp_data.c
        structures/sources/errors.c
        structures/sources/label_data.c
        structures/sources/macro_data.c
        structures/sources/mappings.c
        structures/sources/trie.c
        # Utils sources
        utils/sources/output_files.c
        utils/sources/utils.c
)

# Header files
set(HEADER_FILES
        # Assembler headers
        assembler/headers/assembler_controller.h
        assembler/headers/code_convert.h
        assembler/headers/defines.h
        assembler/headers/first_phase.h
        assembler/headers/parser.h
        assembler/headers/phase_controller.h
        assembler/headers/preprocessor.h
        assembler/headers/second_phase.h

        # Structures headers
        structures/headers/ast.h
        structures/headers/cmp_data.h
        structures/headers/errors.h
        structures/headers/label_data.h
        structures/headers/macro_data.h
        structures/headers/mappings.h
        structures/headers/trie.h

        # Utils headers
        utils/headers/boolean.h
        utils/headers/output_files.h
        utils/headers/utils.h
        )

# Add executable target
add_executable(20465_System_Programming_Laboratory_Final_Project ${SOURCE_FILES} ${HEADER_FILES})
