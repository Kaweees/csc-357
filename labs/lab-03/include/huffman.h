#include <stdio.h>

#ifndef HUFFMAN_H
#define HUFFMAN_H

#define MAX_CODE_LENGTH   256 /* total number of characters in ASCII */
#define ARGUEMENTS_AMOUNT 2   /* number of arguments for the program */

typedef struct HuffmanNode HuffmanNode;
typedef struct FileContent FileContent;
typedef struct HuffmanCode HuffmanCode;

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
  /* The next node in the Huffman tree (used for building the tree) */
  struct HuffmanNode* next;
};

/* Represents a priority queue (min-heap) of Huffman nodes for building a
 * Huffman tree */
typedef struct PriorityQueue {
  HuffmanNode** front;
  int size;
  int capacity;
} PriorityQueue;

/* Represents a Huffman code for a character */
struct HuffmanCode {
  /* The Huffman code */
  char* code_contents;
  /* The length of the Huffman code */
  size_t code_length;
  /* The capacity the Huffman code can hold */
  size_t code_capacity;
};

HuffmanNode* createNode(char ascii, int freq, HuffmanNode* left,
    HuffmanNode* right, HuffmanNode* next);
PriorityQueue* createPriorityQueue(unsigned int capacity);
void swapNodes(PriorityQueue* pq, int i, int j);
void minHeapify(struct PriorityQueue* pq, int idx);
void insertNode(PriorityQueue* pq, HuffmanNode* node);
HuffmanNode* buildTree(int list[MAX_CODE_LENGTH]);
int printHuffmanCodes(HuffmanNode* root, char* code, int top, unsigned char c);
void freeHuffmanTree(HuffmanNode* root);
HuffmanNode* extractMin(PriorityQueue* pq);
HuffmanNode* buildHuffmanTree(int* frequencies, int size);
void freeHuffmanTree(HuffmanNode* node);
int* countFrequencies(FILE* file);
void buildCodesHelper(HuffmanNode* node, char** huffman_codes, char* code_str);
char** buildCodes(HuffmanNode* node);
void freeHuffmanCodes(char** huffman_codes);
#endif