cmake_minimum_required(VERSION 3.27)
project(20465_System_Programming_Laboratory_Final_Project C)

set(CMAKE_C_STANDARD 11)

add_executable(20465_System_Programming_Laboratory_Final_Project
        assembler.c
        errors.h
        errors.c
        assembler_controller.c
        preprocessor.h
        preprocessor.c
        assembler_controller.h
        macro_data.h
        macro_data.c
        utils.c
        utils.h
        parser.c
        ast.h
        mappings.h
        ast.c
        two_phase_assembler.h
        two_phase_assembler.c
        parser.h
        mappings.c
        first_phase.c
        code_convert.h
        code_convert.c
        cmp_data.h
        cmp_data.c
        first_phase.h
        trie.h
        trie.c
        label_data.h
        label_data.c
        second_phase.h
        second_phase.c
        output_files.h
        output_files.c)
