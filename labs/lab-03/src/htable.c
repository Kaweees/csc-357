#include <stddef.h>
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <unistd.h>

#include "huffman.h"
#include "safe_mem.h"

#define INITIAL_BUFFER_SIZE    128  /* Initial size of the buffer in bytes */
#define NEWLINE_CHAR           10   /* asci code for a newline */
#define CARRIAGE_CHAR          13   /* asci code for a carriage return */
#define STRING_TERMINATOR      '\0' /* null terminator for a string */
#define STDOUT_FILE_DESCRIPTOR 1
#define MAX_CODE_LENGTH        256 /* total number of characters in ASCII */

/**
 * Reads a file and returns its contents as a singular string
 * @param file - a pointer to the file to read from
 * @return a pointer to the string containing the file's contents
 */
void htable(FILE* file) {
  struct FileContent* file_text = readText(file);
  /* char* header = createHeader(codes, text); */
  printf("%s\n", file_text->file_contents);
  int* char_freq = countFrequencies(file_text);
  printf("%d\n", char_freq[(int)'d']);
  /*

  struct HuffmanNode* root = buildHuffmanTree(char_freq);
  char** codes = buildCodes(root);

  safe_free(root);
  safe_free(codes);
  */
  safe_free(file_text->file_contents);
  safe_free(file_text);
  safe_free(char_freq);
}

int main(int argc, char* argv[]) {
  if (argc == 2) {
    FILE* infile;
    infile = fopen(*(argv + 1), "r");
    if (infile == NULL) {
      perror("Memory allocation error");
      exit(EXIT_FAILURE);
    } else {
      htable(infile);
    }
  }
  return 0;
}