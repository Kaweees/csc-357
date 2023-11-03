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

/**
 * @brief Reads a compressed file and decompresses it using Huffman coding
 *
 * @param infile - a pointer to the file to read from
 * @param outfile - a pointer to the file to write to
 */
void hdecode(int infile, int outfile) {
  FileContent* file_contents = safe_read(infile);
  // uint8_t size = file_contents->file_contents[0];
  // printf("size: %d\n", size);
  FrequencyList* char_freq = createFrequencyList(MAX_CODE_LENGTH);

  // FrequencyList* char_freq = countFrequencies(file_contents);
  // createHeader(char_freq, outfile);
  // HuffmanNode* root = buildHuffmanTree(char_freq);
  // char** huffman_codes = buildCodes(root);
  // for (int i = 0; i < char_freq->size; i++) {
  //   if (char_freq->frequencies[i] > 0) {
  //     printf("%c: %s\n", i, huffman_codes[i]);
  //   }
  // }
  // printf("size: %d\n", (int) file_contents->file_size);
  // int count = 0;
  // uint32_t frequencyNetworkByte = 0;
  // char* correspondingCode = NULL;
  // for (int i = 0; i < file_contents->file_size; i++) {
  //   correspondingCode = huffman_codes[(int)file_contents->file_contents[i]];
  //   for (int j = 0; j < strlen(correspondingCode); j++) {
  //     printf("%c: %d\n", correspondingCode[j], count);
  //     if (count == SPECIAL_INT_SIZE) {
  //       /* Change the byte order to network byte order (big endian) */
  //       frequencyNetworkByte = htonl(frequencyNetworkByte);
  //       /* Write the byte to the file if the byte is full */
  //       safe_write(outfile, &frequencyNetworkByte, sizeof(uint32_t));
  //       frequencyNetworkByte = 0;
  //       count = 0;
  //     }
  //     frequencyNetworkByte <<= 1;
  //     if (correspondingCode[j] == '1') {
  //       frequencyNetworkByte++;
  //     }
  //     count++;
  //   }
  //   /* Change the byte order to network byte order (big endian) */
  //   frequencyNetworkByte = htonl(frequencyNetworkByte);
  //   /* Write the byte to the file if the byte is full */
  //   safe_write(outfile, &frequencyNetworkByte, sizeof(uint32_t));
  // }
  freeFileContent(file_contents);
  freeFrequencyList(char_freq);
  // freeHuffmanTree(root);           /* Free the Huffman tree */
  // freeHuffmanCodes(huffman_codes); /* Free the Huffman codes */
}

int main(int argc, char* argv[]) {
  if (argc == 2) {
    int infile = fileno(stdin);
    int outfile = safe_open(*(argv + 1), O_RDONLY, S_IRWXU);
    hdecode(infile, outfile);
    close(infile);
  } else if (argc == 3) {
    int infile = safe_open(*(argv + 1), O_RDONLY, S_IRWXU);
    int outfile =
        safe_open(*(argv + 2), (O_WRONLY | O_CREAT | O_TRUNC), S_IRWXU);
        hdecode(infile, outfile);
    close(infile);
    close(outfile);
  } else {
    fprintf(stderr, "Usage: %s <input_file> [ <output_file>\n", argv[0]);
    return EXIT_FAILURE;
  }
  return 0;
}