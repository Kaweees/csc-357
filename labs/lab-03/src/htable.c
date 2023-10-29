#include <stddef.h>
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <unistd.h>

#include "huffman.h"
#include "safe_mem.h"

/**
 * Reads a file and returns its contents as a singular string
 * @param file - a pointer to the file to read from
 * @return a pointer to the string containing the file's contents
//  */
void htable(FILE* file) {
  int* char_freq = countFrequencies(file);
  HuffmanNode* root = buildHuffmanTree(char_freq, MAX_CODE_LENGTH);
  char** huffman_codes = buildCodes(root);
  for (int i = 0; i < MAX_CODE_LENGTH; i++) {
    if (char_freq[i] > 0) {
      printf("0x%02x: %s\n", i, huffman_codes[i]);
    }
  }
  safe_free(char_freq);
  freeHuffmanTree(root); /* Free the Huffman tree */
  freeHuffmanCodes(huffman_codes); /* Free the Huffman codes */
}

int main(int argc, char* argv[]) {
  if (argc == 2) {
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
    return 1;
  }
  return 0;
}