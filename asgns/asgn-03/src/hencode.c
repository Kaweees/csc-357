#include <fcntl.h>
#include <stddef.h>
#include <stdint.h>
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <unistd.h>

#include "huffman.h"
#include "safe_file.h"
#include "safe_mem.h"

/**
 * @brief Reads a file and compresses it using Huffman coding
 *
 * @param infile - a pointer to the file to read from
 * @param outfile - a pointer to the file to write to
 */
void hencode(int infile, int outfile) {
  FileContent* file = safe_read(infile);
  FrequencyList* char_freq = countFrequencies(file);
  // createHeader(char_freq, outfile);
  HuffmanNode* root = buildHuffmanTree(char_freq);
  char** huffman_codes = buildCodes(root);
  for (int i = 0; i < MAX_CODE_LENGTH; i++) {
    if (char_freq->frequencies[i] > 0) {
      printf("0x%02x: %d\n", i, char_freq->frequencies[i]);
    }
  }
  // int frequencyNetworkByte;
  // count = htonl(count);
  // writeFile(fd, &count, sizeof(int));
  freeFileContent(file);
  freeFrequencyList(char_freq);
  freeHuffmanTree(root);           /* Free the Huffman tree */
  freeHuffmanCodes(huffman_codes); /* Free the Huffman codes */
}

int main(int argc, char* argv[]) {
  if (argc == 2) {
    int infile = safe_open(*(argv + 1), O_RDONLY, S_IRWXU);
    int outfile = fileno(stdout);
    hencode(infile, outfile);
    close(infile);
  } else if (argc == 3) {
    int infile = safe_open(*(argv + 1), O_RDONLY, S_IRWXU);
    int outfile = safe_open(*(argv + 2), (O_WRONLY | O_CREAT | O_TRUNC), S_IRWXU);
    hencode(infile, outfile);
    close(infile);
    close(outfile);
  } else {
    fprintf(stderr, "Usage: %s <input_file> [ <output_file>\n", argv[0]);
    return EXIT_FAILURE;
  }
  return 0;
}