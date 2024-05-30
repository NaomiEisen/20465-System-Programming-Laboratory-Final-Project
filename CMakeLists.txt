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
        general.h
        macro_list.h
        macro_list.c
        utils.c
        utils.h
        parcer.c
        ast.h
        hardware.h
        ast.c)
