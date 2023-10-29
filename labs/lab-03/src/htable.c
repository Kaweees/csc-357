#include <stddef.h>
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <unistd.h>

#include "huffman.h"
#include "safe_mem.h"

int main(int argc, char* argv[]) {
  if (argc == 2) {
    FILE* infile;
    infile = fopen(*(argv + 1), "r");
    if (infile == NULL) {
      perror("Error opening file\n");
      exit(EXIT_FAILURE);
    } else {
      htable(infile);
    }
  } else {
    fprintf(stderr, "Usage: %s <input_file>\n", argv[0]);
    return 1;
  }
  return 0;
}