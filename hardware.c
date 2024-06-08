#include "hardware.h"
#include <stdlib.h>
#include <stdio.h>

/* Function to create a new node with a specified address */
Word *createNode(int address) {
    Word *newNode = (Word *)malloc(sizeof(Word));
    if (!newNode) {
        fprintf(stderr, "Memory allocation error\n");
        exit(EXIT_FAILURE);
    }
    newNode->address = address;
    newNode->binaryCode = 0; /* Initialize binaryCode to 0 */
    newNode->next = NULL;
    return newNode;
}

/* Function to insert a new node at the end of the MemoryImage */
void insertNode(MemoryImage *image, int address) {
    Word* newNode = createNode(address);
    Word* current = NULL;
    if (image->head == NULL) {
        image->head = newNode;
    } else {
        current = image->head;
        while (current->next != NULL) {
            current = current->next;
        }
        current->next = newNode;
    }
    image->nodeCount++;
}

/* Function to set a specific bit in the binaryCode of a Word node */
void setBit(Word *node, int index, int bit) {
    if (index >= 0 && index < 16) {
        if (bit) {
            node->binaryCode |= (1 << index);
        } else {
            node->binaryCode &= ~(1 << index);
        }
    } else {
        fprintf(stderr, "Error: Index out of range\n");
    }
}

/* Function to print the MemoryImage */
void printMemoryImage(const MemoryImage *image) {
    const Word *current = image->head;
    while (current != NULL) {
        printf("Address: %d, Binary Code: %04X\n", current->address, current->binaryCode);
        current = current->next;
    }
}

/* Function to free the MemoryImage */
void freeMemoryImage(MemoryImage *image) {
    Word *current = image->head;
    Word *next;
    while (current != NULL) {
        next = current->next;
        free(current);
        current = next;
    }
    image->head = NULL; /* Set head to NULL after freeing nodes */
    image->nodeCount = 0; /* Reset node count to 0 */
}