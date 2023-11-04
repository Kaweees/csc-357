#include <stdio.h>

#include "safe_file.h"

#ifndef HUFFMAN_H
#define HUFFMAN_H

#define MAX_CODE_LENGTH   256 /* total number of characters in ASCII */
#define HENCODE_ARGUEMENTS_AMOUNT 2   /* number of arguments for the program */

typedef struct FrequencyList FrequencyList;
typedef struct HuffmanNode HuffmanNode;
typedef struct LinkedList LinkedList;
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
  unsigned char char_ascii;
  /* The frequency of the character associated with the node */
  int char_freq;
  /* The left child of the node */
  HuffmanNode* left;
  /* The right child of the node */
  HuffmanNode* right;
  /* The next node in the linked list */
  HuffmanNode* next;
};

struct LinkedList {
  /* The head of the linked list */
  HuffmanNode* head;
  /* The size of the linked list */
  int size;
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

FrequencyList* createFrequencyList(size_t size);
FrequencyList* countFrequencies(FileContent* contents);
void createHeader(FrequencyList* freq_list, int outfile);
HuffmanNode* createNode(char ascii, int freq, HuffmanNode* left,
    HuffmanNode* right, HuffmanNode* next);
int comesBefore(HuffmanNode* a, HuffmanNode* b);
LinkedList* createLinkedList();
void insertNode(LinkedList* lls, HuffmanNode* node);
HuffmanNode* removeFirst(LinkedList* lls);
HuffmanNode* combine(HuffmanNode* a, HuffmanNode* b);
HuffmanNode* buildHuffmanTree(FrequencyList* frequencies);
void buildCodesHelper(HuffmanNode* node, char** huffman_codes, char* code_str);
char** buildCodes(HuffmanNode* node);
void freeFrequencyList(FrequencyList* freq_list);
void freeHuffmanTree(HuffmanNode* node);
void freeHuffmanCodes(char** huffman_codes);

#endif