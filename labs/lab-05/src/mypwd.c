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

void mypwd(int infile, int outfile) {}

int main(int argc, char* argv[]) {
  if (argc == 2) {
    int infile = safe_open(*(argv + 1), O_RDONLY, S_IRWXU);
    int outfile = fileno(stdout);
    hencode(infile, outfile);
    close(infile);
  } else if (argc == 3) {
    int infile = safe_open(*(argv + 1), O_RDONLY, S_IRWXU);
    int outfile =
        safe_open(*(argv + 2), (O_WRONLY | O_CREAT | O_TRUNC), S_IRWXU);
    hencode(infile, outfile);
    close(infile);
    close(outfile);
  } else {
    fprintf(stderr, "Usage: %s infile [ outfile ]\n", argv[0]);
    return EXIT_FAILURE;
  }
  return EXIT_SUCCESS;
}