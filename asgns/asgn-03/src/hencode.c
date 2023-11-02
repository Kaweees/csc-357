#include <stddef.h>
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <unistd.h>

#include "huffman.h"
#include "safe_mem.h"

/**
 * @brief Reads a file and compresses it using Huffman coding
 * 
 * @param infile - a pointer to the file to read from
 * @param outfile - a pointer to the file to write to
 */
void hencode(FILE* infile, FILE* outfile) {
  FrequencyList* char_freq = countFrequencies(infile);
  createHeader(char_freq, outfile);
  // HuffmanNode* root = buildHuffmanTree(char_freq);
  // char** huffman_codes = buildCodes(root);
  // int i;
  // for (i = 0; i < MAX_CODE_LENGTH; i++) {
  //   if (char_freq->frequencies[i] > 0) {
  //     write(fileno(outfile), huffman_codes[i], strlen(huffman_codes[i]));
  //   }
  // }
  freeFrequencyList(char_freq);
  // freeHuffmanTree(root);           /* Free the Huffman tree */
  // freeHuffmanCodes(huffman_codes); /* Free the Huffman codes */
}

int main(int argc, char* argv[]) {
  if (argc == 2) {
    FILE* infile; FILE* outfile;
    infile = fopen(*(argv + 1), "r");
    outfile = stdout;
    if (infile == NULL || outfile == NULL) {
      perror("Error opening file\n");
      exit(EXIT_FAILURE);
    } else {
      hencode(infile, outfile);
      fclose(infile);
      fclose(outfile);
    }
  } else if (argc == 3) {
    FILE* infile; FILE* outfile;
    infile = fopen(*(argv + 1), "r");
    outfile = fopen(*(argv + 1), "r");
    if (infile == NULL || outfile == NULL) {
      perror("Error opening file\n");
      exit(EXIT_FAILURE);
    } else {
      hencode(infile, outfile);
      fclose(infile);
    }
  } else {
    fprintf(stderr, "Usage: %s <input_file> [ <output_file>\n", argv[0]);
    return EXIT_FAILURE;
  }
  return 0;
}