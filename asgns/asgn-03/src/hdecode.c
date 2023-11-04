#include <arpa/inet.h>
#include <fcntl.h>
#include <stddef.h>
#include <stdint.h>
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <sys/types.h>
#include <unistd.h>

#include "huffman.h"
#include "safe_file.h"
#include "safe_mem.h"

#define HEADER_CHAR_SIZE 5 /* The size in bytes of a character and its frequency in the header */
#define BITS_PER_BYTE    8 /* The number of bits in a byte */

/**
 * @brief Reads a compressed file and decompresses it using Huffman coding
 *
 * @param infile - a pointer to the file to read from
 * @param outfile - a pointer to the file to write to
 */
void hdecode(int infile, int outfile) {
  FrequencyList* char_freq = createFrequencyList(MAX_CODE_LENGTH);
  FileContent* file_contents = safe_read(infile);
  void *curr = file_contents->file_contents;
  /* Read the amount of characters in the header */
  uint8_t size = (*(uint8_t*)curr) + 1;
  /* Move the pointer to the next byte */
  size_t bytes_read = 0;
  curr += sizeof(uint8_t);
  for (int i = 0; i < size; i++) {
    uint8_t ascii = *(uint8_t*)curr;
    /* Move the pointer to the next byte */
    curr += sizeof(uint8_t);
    bytes_read += sizeof(uint8_t);
    uint32_t frequency = htonl(*(uint32_t*)curr);
    /* Offset the pointer by the size of the frequency */
    curr += sizeof(uint32_t);
    bytes_read += sizeof(uint32_t);
    if (char_freq->frequencies[(int) ascii] == 0) {
      ++char_freq->num_non_zero_freq;
    }
    char_freq->frequencies[(int) ascii]+= frequency;
  }
  HuffmanNode* root = buildHuffmanTree(char_freq);
  char** huffman_codes = buildCodes(root);
  size_t buffer_size = 1;
  for (int i = 0; i < MAX_CODE_LENGTH; i++) {
    if (char_freq->frequencies[i] > 0) {
      buffer_size += char_freq->frequencies[i] * strlen(huffman_codes[i]);
    }
  }
  char * buffer = calloc(buffer_size, sizeof(char));
  int buffer_index = 0;
  /* Read until the end of the file */
  while ((size_t) curr < (size_t) (file_contents->file_contents + file_contents->file_size)) {
    uint32_t curr_num = htonl(*(uint32_t*)curr);
    /* Offset the pointer by the size of the frequency */
    curr += sizeof(uint32_t);
    // int num_bits = 0;
    for (int i = 0; i < sizeof(uint32_t) * BITS_PER_BYTE; i++) {
      if (buffer_index >= buffer_size) {
        break;
      } else if (curr_num & 1 << ((sizeof(uint32_t) * BITS_PER_BYTE) - 1 - i)) {
        buffer[buffer_index] = '1';
      } else {
        buffer[buffer_index] = '0';
      }
      buffer_index++;
    }
  }
  HuffmanNode* curr_node = root;
  for (int i = 0; i < buffer_size; i++) {
    if (curr_node->left == NULL && curr_node->right == NULL) {
      safe_write(outfile, &curr_node->char_ascii, sizeof(char));
      curr_node = root;
    }
    if (buffer[i] == '0') {
      curr_node = curr_node->left;
    } else {
      curr_node = curr_node->right;
    }
  }
  safe_free(buffer);
  freeFileContent(file_contents);
  freeFrequencyList(char_freq);
  freeHuffmanTree(root);           /* Free the Huffman tree */
  freeHuffmanCodes(huffman_codes); /* Free the Huffman codes */
}

int main(int argc, char* argv[]) {
  if ((argc == 3) || ((argc == 2) && (strcmp(argv[1], "-") == 0))) {
    int infile = safe_open(*(argv + 1), O_RDONLY, S_IRWXU);
    int outfile =
        safe_open(*(argv + 2), (O_WRONLY | O_CREAT | O_TRUNC), S_IRWXU);
    hdecode(infile, outfile);
    close(infile);
    close(outfile);
  }
  if (argc == 2) {
    int infile = fileno(stdin);
    int outfile = safe_open(*(argv + 2), O_RDONLY, S_IRWXU);
    hdecode(infile, outfile);
    close(infile);
  } else {
    fprintf(stderr, "Usage: %s [ ( infile | - ) [ outfile ] ]\n", argv[0]);
    return EXIT_FAILURE;
  }
  return 0;
}