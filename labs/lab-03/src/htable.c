#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <stddef.h>
#include "huffman.h"

/**
 * Reads a file and returns its contents as a singular string
 * @param file - a pointer to the file to read from
 * @return a pointer to the string containing the file's contents
  */
void htable(FILE* file) {
  int* char_freq = countFrequencies(file);
  if (char_freq == NULL) {
    return;
  }
  HuffmanNode* root = buildTree(char_freq); 
  int i;
  char* code;
  for (i = 0; i < MAX_CODE_LENGTH; i++) {
    if (char_freq[i] > 0) {
      if ((code = calloc(sizeof(char), MAX_CODE_LENGTH))) {
        printHuffmanCodes(root, code, 0, i);
        printf("0x%02x: %s\n", i, code);
      }
    free(code);
    }
  }
}

int main(int argc, char* argv[]) {
  if (argc == ARGUEMENTS_AMOUNT) {
    FILE* infile;
    infile = fopen(*(argv + 1), "r");
    if (infile == NULL) {
      perror("Error opening file\n");
      exit(EXIT_FAILURE);
    } else {
      htable(infile);
      fclose(infile);
    }
  } else {
    fprintf(stderr, "Usage: %s <input_file>\n", argv[0]);
    return EXIT_FAILURE;
  }
  return 0;
}