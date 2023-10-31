#include <stdio.h>

#ifndef HUFFMAN_H
#define HUFFMAN_H

#define MAX_CODE_LENGTH   256 /* total number of characters in ASCII */
#define ARGUEMENTS_AMOUNT 2   /* number of arguments for the program */

/* Typedefs for the types of structs used in the program */
typedef struct FrequencyList FrequencyList;
typedef struct HuffmanNode HuffmanNode;
typedef struct PriorityQueue PriorityQueue;
typedef struct FileContent FileContent;
typedef struct HuffmanCode HuffmanCode;

/* Represents a list of character frequencies */
struct FrequencyList {
  /* The array of frequencies for each ASCII character */
  unsigned char* frequencies;
  /* The number of non-zero frequencies in the list */
  unsigned int num_non_zero_freq;
  /* The size of the list */
  unsigned int size;
};

/* Represents a node in a Huffman tree */
struct HuffmanNode {
  /* The ASCII character code value */
  char char_ascii;
  /* The frequency of the character associated with the node */
  unsigned char char_freq;
  /* The left child of the node */
  HuffmanNode* left;
  /* The right child of the node */
  HuffmanNode* right;
};

/* Represents a priority queue (via a Min-max-heap) of Huffman nodes used for building a
 * Huffman tree */
struct PriorityQueue {
  /* The number of nodes currently in the priority queue */
  unsigned int size;
  /* The maximum capacity of the priority queue */
  unsigned int capacity;
  /* The array of Huffman nodes the priority queue is composed of */
  HuffmanNode** front;
};

/* Represents the contents of a file */
struct FileContent {
  /* The length of the file contents in bytes */
  size_t file_size;
  /* The pointer to the file contents */
  char* file_contents;
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

FrequencyList* countFrequencies(FILE* file);
HuffmanNode* createNode(
    char ascii, unsigned char freq, HuffmanNode* left, HuffmanNode* right);
PriorityQueue* createPriorityQueue(unsigned int capacity);
void swapNodes(HuffmanNode** a, HuffmanNode** b);
void minHeapify(PriorityQueue* pq, int idx);
HuffmanNode* extractMin(PriorityQueue* pq);
void insertNode(PriorityQueue* pq, HuffmanNode* node);
void buildMinHeap(PriorityQueue* pq);
int isLeaf(HuffmanNode* node);
PriorityQueue* createPriorityQueueFromFreqArray(FrequencyList* freq_list);
HuffmanNode* buildHuffmanTree(FrequencyList* freq_list);


void freeFrequencyList(FrequencyList* freq_list);
void freeHuffmanTree(HuffmanNode* node);
void buildCodesHelper(HuffmanNode* node, char** huffman_codes, char* code_str);
char** buildCodes(HuffmanNode* node);
void freeHuffmanCodes(char** huffman_codes);
#endif