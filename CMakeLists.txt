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
        macro_list.h
        macro_list.c
        utils.c
        utils.h
        parser.c
        ast.h
        mappings.h
        ast.c
        first_phase.h
        first_phase.c
        parser.h
        label_trie.h
        mappings.c
        semantics.c
        label_trie.c
        code_convert.h
        code_convert.c
        cmp_data.h
        cmp_data.c
        semantic.h
        trie.h
        trie.c
        label_data.h
        label_data.c)
