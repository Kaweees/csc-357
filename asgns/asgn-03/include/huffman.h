#include <stdio.h>

#ifndef HUFFMAN_H
#define HUFFMAN_H

struct HuffmanNode {
  char char_asci;
  int char_freq;
  struct HuffmanNode* left;
  struct HuffmanNode* right;
};

struct LinkedList {
  struct HuffmanNode* head;
};

void initLinkedList(struct LinkedList* list);
void append(struct LinkedList* list, int data);
void printLinkedList(const struct LinkedList* list);
void freeLinkedList(struct LinkedList* list);

bool comesBefore(struct HuffmanNode* a, struct HuffmanNode* b);
HuffmanNode* combine(struct HuffmanNode* a, struct HuffmanNode* b);
int* countFrequencies(FILE* fp);
HuffmanNode* buildHuffmanTree(int* frequencies);
char** buildCodes(struct HuffmanNode* root);
char* createHeader(char* codes, char* text);

HuffmanNode* pop(struct LinkedList* list);
#endif
