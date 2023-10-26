#include <stdio.h>

#ifndef HUFFMAN_H
#define HUFFMAN_H

/* Represents the contents of a file */
struct FileContent {
  /* The length of the file contents in bytes */
  size_t file_size;
  /* The pointer to the file contents */
  char* file_contents;
};

/* Represents a node in a Huffman tree */
struct HuffmanNode {
  /* The ASCII character code value */
  char char_ascii;
  /* The frequency of the character associated with the node */
  int char_freq;
  /* The left child of the node */
  struct HuffmanNode* left;
  /* The right child of the node */
  struct HuffmanNode* right;
};

/* Represents a Huffman code for a character */
struct HuffmanCode {
  /* The Huffman code */
  char* code_contents;
  /* The length of the Huffman code */
  size_t code_length;
  /* The capacity the Huffman code can hold */
  size_t code_capacity;
};

struct FileContent* readText(FILE* file);
int comesBefore(struct HuffmanNode* a, struct HuffmanNode* b);
struct HuffmanNode* combine(struct HuffmanNode* a, struct HuffmanNode* b);
int* countFrequencies(struct FileContent*);
struct HuffmanNode* buildHuffmanTree(int* frequencies);
struct HuffmanCode* buildCodes(struct HuffmanNode* root);
void buildCodesHelper(
    struct HuffmanNode* node, struct HuffmanCode* codes, char* code);
char* createHeader(char* codes, char* text);
#endif
