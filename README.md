# Assembler Project in C90 🤟

This project is the final assignment for the course **'Laboratory C'** in the Open University of israel :)

## Overview
The assembler converts an assembly file into an object file, as well as additional extern and entry files.
The assembler follows a two-phase process:
1. **Preprocessing**: Expands macros and prepares the input file for further processing.
2. **First Phase**: Checks syntax, generates the symbol table, and translates all instructions that do not depend on labels.
3. **Second Phase**: Handles the translation of the remaining instructions and generates the output files.
---
## Assembly Syntax
The assembly language instructions used in this project are described below:
### Two Argument Instructions
| Instruction       | Description                                                      |
| ----------------- | ------------------------------------------------------------------ |
| **mov** | Moves data from the source operand to the destination operand. |
| **cmp** | Compares two operands and updates the status flags based on the result. |
| **add** | Adds the value of the source operand to the destination operand. |
| **sub** | Subtracts the value of the source operand from the destination operand. |
| **lea** | Loads the effective address of the source operand into the destination operand. |

### One Argument Instructions
| Instruction       | Description                                                      |
| ----------------- | ------------------------------------------------------------------ |
| **clr** | Clears the value of the operand, setting it to zero. |
| **not** | Inverts all bits of the operand. |
| **inc** | Increments the operand by one. |
| **dec** | Decrements the operand by one. |
| **jmp** | Unconditionally jumps to the specified address. |
| **bne** | Jumps to the specified address if the previous comparison result was not equal. |
| **red** | Reads a value from the input and stores it in the operand. |
| **prn** | Prints the value of the operand. |
| **jsr** | Jumps to a subroutine at the specified address and stores the return address for later use. |

### No Argument Instructions
| Instruction       | Description                                                      |
| ----------------- | ------------------------------------------------------------------ |
| **rts** | Returns from a subroutine, resuming execution at the return address stored on the stack. |
| **stop** | Halts the program execution. |

### Registers
There are 8 registers used in this project, named `r0` to `r7`. These registers are used for storing temporary data and performing calculations. Each register can hold a value, and they are referenced in assembly instructions to perform operations like loading, storing, and arithmetic operations. 

** The actual execution of the assembly instructions is not relevant to the project, so a detailed understanding of each instruction is not required for completing the translation. However, it is useful to have a basic understanding of the types of operations the assembler supports. :)



---

## Project Structure

The project is organized into folders, each containing 'headers' and 'sources' files for different components:

### assembler
This folder contains files responsible for the entire assembling process, including:

- **preprocessor**: Prepares the input file by handling macros and file includes.
- **parser**: Analyzes each line of the assembly file, validates syntax, and prepares instructions for processing.
- **first_pass**: Handles the first phase of the assembler, during which the lable table is generated and the eligible assembly instructions are being translated to machine code.
- **second_pass**:  Completes the second phase by converting the remaining assembly instructions into machine code and generating the extern and entry files.
- **assembler_controller**: Coordinates all stages of the assembly process
- **code_convert**: Manages the binary conversion of assembly instructions.
- **assembler**: The main file that launches the program.

### Data Structures
This folder contains files defining various data structures and their associated methods used throughout the project:

- **ast**: Abstract Syntax Tree. Represents an assembly instruction in all its possible formats.
- **trie**: A generic Trie data structure used to create efficient data structures, allowing fast insertion and searching based on string values.
- **macro_data**: A data structure for storing macros, implemented using the generic Trie structure.
- **label_data**: A data structure for storing labels, including their name, address, and type, also built on the generic Trie structure.
- **mappings**: Structures that store data about assembly instructions and reserved keywords.
- **cmp_data**: Represents the "imaginary" computer's memory and data needed for file processing. It includes two memory sections—data and code, that grow towards each other, allowing flexible management of the shared maximum capacity. Additionally, it stores label structures, external/entry files, and a list of unresolved lines for the second assembly phase.
- **error**: A structure for handling errors during execution. It contains error codes, their corresponding messages, and a location structure indicating where the error occurred.

### Utils
Contains utility functions and structures used throughout the project.

- **Boolean**: Defines boolean enums for clarity and simplicity. 
- **output_files**: Handles the writing of processed data to output files.
- **utils**: Provides various utility functions for handling strings, integers, and file name extensions.


---

## How to Run
1. Clone the repository.
2. Navigate to the project directory.
3. Compile the project using the provided makefile:
   ```bash
   make
