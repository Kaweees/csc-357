#include "huffman.h"

#include <stddef.h>
#include <stdio.h>
#include <stdlib.h>
#include <string.h>

#define MAX_CODE_LENGTH 256 /* total number of characters in ASCII */
#define SPACE_CHAR      32  /* asci code for a space */

struct HuffmanNode {
  char char_asci;
  int char_freq;
  struct HuffmanNode* left;
  struct HuffmanNode* right;
};

void initLinkedList(struct LinkedList* list);
void append(struct LinkedList* list, int data);
void printLinkedList(const struct LinkedList* list);
void freeLinkedList(struct LinkedList* list);

int comesBefore(struct HuffmanNode* a, struct HuffmanNode* b);
struct HuffmanNode* combine(struct HuffmanNode* a, struct HuffmanNode* b);

char** buildCodes(struct HuffmanNode* root);

char* createHeader(char* codes, char* text) {
  // for (int i = 0; i < strlen()) HuffmanNode* pop(struct LinkedList * list);
}

void vaildate_malloc(void* ptr) {
  if (ptr == NULL) {
    perror("Memory allocation error");
    exit(1);
  }
}
/**
 * Takes a string of the form "x y z" and returns an array of frequencies
 * ascending asci order
 *
 * @param file - a pointer to the file to read the line from
 * @return a pointer to the line read from the file
 */
int* parseHeader(char* header, char* text) {
  int* freq_list = malloc(sizeof(int) * MAX_CODE_LENGTH);
  int ch; /* character read from file */
  vaildate_malloc(freq_list);
  int header_length = strlen(header);
  int i = 0;
  while (i < header_length) {
    while ((ch = header[i]) != SPACE_CHAR) {
      i++
    }
  }

  // for (int i = 0; i < ; i++) {
  //   while

  while ((ch = fgetc(file)) != SPACE_CHAR) {
  }
}

//   // for (int i = 0; i < strlen()) HuffmanNode* pop(struct LinkedList *
//   // list);
// }

/**
 * Opens a file and counts the frequency of each character in the file
 *
 * @param file - a pointer to the file to count the frequencies of
 * @return an array of character frequencies in ascending asci order
 */
int* countFrequencies(FILE* fp) {
  int* freq_list;
  int ch; /* character read from file */
  freq_list = (int*)calloc(MAX_CODE_LENGTH, sizeof(int));
  vaildate_malloc(freq_list);
  while ((ch = fgetc(fp)) != EOF) {
    freq_list[ch]++;
  }
}

/**
 * Creates a Huffman tree from an array of character frequencies
 *
 * @param frequencies - an array of character frequencies in ascending asci
 * @return the root of the Huffman tree
 */
struct HuffmanNode* buildHuffmanTree(int* frequencies) {
  struct HuffmanNode** non_zero_nodes =
      malloc(sizeof(struct HuffmanNode*) *
             MAX_CODE_LENGTH); /* node list containing HuffmanNodes of
                                  characters with non-zero frequencies */
  vaildate_malloc(non_zero_nodes);
  size_t non_zero_nodes_size = 0;
  for (int i = 0; i < MAX_CODE_LENGTH; i++) {
    if (frequencies[i] > 0) {
      struct HuffmanNode* node = malloc(sizeof(struct HuffmanNode));
      vaildate_malloc(node);
      node->left = NULL;
      node->right = NULL;
      node->char_freq = frequencies[i];
      node->char_asci = i;
      non_zero_nodes[non_zero_nodes_size] = node;
      non_zero_nodes_size++;
    }
  }
  non_zero_nodes_size = realloc(
      non_zero_nodes, sizeof(struct HuffmanNode*) * non_zero_nodes_size);
  vaildate_malloc(non_zero_nodes);

  struct LinkedList* list = malloc(sizeof(struct LinkedList));
  vaildate_malloc(list);

  initLinkedList(list);

  for (int i = 0; i < non_zero_nodes_size; i++) {
    append(list, non_zero_nodes[i]);
  }

  while (list->size > 1) {
    struct HuffmanNode* a = pop(list);
    struct HuffmanNode* b = pop(list);
    struct HuffmanNode* combined = combine(a, b);
    append(list, combined);
  }
}

size_t huffmanEncode(FILE* infile, FILE* outfile) {
  int* char_freq = countFrequencies(infile);
  free(char_freq);
  struct HuffmanNode* root = buildHuffmanTree(char_freq);
}