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

struct LinkedList {
  struct HuffmanNode* head;
};

void initLinkedList(struct LinkedList* list);
void append(struct LinkedList* list, int data);
void printLinkedList(const struct LinkedList* list);
void freeLinkedList(struct LinkedList* list);

struct FileContent* readText(FILE* file);
int comesBefore(struct HuffmanNode* a, struct HuffmanNode* b);
struct HuffmanNode* combine(struct HuffmanNode* a, struct HuffmanNode* b);
int* countFrequencies(FILE* fp);
struct HuffmanNode* buildHuffmanTree(int* frequencies);
char** buildCodes(struct HuffmanNode* root);
char* createHeader(char* codes, char* text);

struct HuffmanNode* pop(struct LinkedList* list);
#endif
