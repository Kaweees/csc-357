#include "huffman.h"

#include <stddef.h>
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <unistd.h>

#include "safe_mem.h"

/**
 * Creates a Huffman node
 *
 * @param ascii - the ASCII character
 * @param freq - the frequency of the character
 * @param left - the left child of the node
 * @param right - the right child of the node
 * @return a pointer to the node
 */
HuffmanNode* createNode(
    char ascii, int freq, HuffmanNode* left, HuffmanNode* right) {
  HuffmanNode* newNode = (HuffmanNode*)safe_malloc(sizeof(HuffmanNode));
  newNode->char_ascii = ascii;
  newNode->char_freq = freq;
  newNode->left = left;
  newNode->right = right;
  return newNode;
}

/**
 * Creates a Priority Queue
 *
 * @param capacity - the capacity of the queue
 * @return a pointer to the queue
 */
PriorityQueue* createPriorityQueue(unsigned int capacity) {
  PriorityQueue* pq = (PriorityQueue*)safe_malloc(sizeof(PriorityQueue));
  pq->size = 0;
  pq->capacity = capacity;
  pq->front = (HuffmanNode**)safe_malloc(sizeof(HuffmanNode*) * capacity);
  return pq;
}

/**
 * Swaps the position of two HuffmanNodes in a PriorityQueue
 *
 * @param pq - a pointer to the PriorityQueue
 * @param a - a pointer to the first HuffmanNode
 * @param b - a pointer to the second HuffmanNode
 */
void swapNodes(PriorityQueue* pq, int i, int j) {
  HuffmanNode* temp = pq->front[i];
  pq->front[i] = pq->front[j];
  pq->front[j] = temp;
}

/**
 * Maintains the min-heap property of the PriorityQueue
 *
 * @param pq - a pointer to the PriorityQueue
 * @param idx - the index of the node to heapify
 */
void minHeapify(struct PriorityQueue* pq, int idx) {
  int smallest = idx;
  int left = 2 * idx + 1;
  int right = 2 * idx + 2;

  if (left < pq->size &&
      pq->front[left]->char_freq < pq->front[smallest]->char_freq)
    smallest = left;

  if (right < pq->size &&
      pq->front[right]->char_freq < pq->front[smallest]->char_freq)
    smallest = right;

  if (smallest != idx) {
    swapNodes(pq, idx, smallest);
    minHeapify(pq, smallest);
  }
}

/**
 * Insert a HuffmanNode into the PriorityQueue
 *
 * @param pq - a pointer to the PriorityQueue
 * @param node - a pointer to the HuffmanNode to insert
 */
void insertNode(PriorityQueue* pq, HuffmanNode* node) {
  int i = pq->size;
  pq->size++;
  pq->front[i] = node;

  while (i > 0 && pq->front[i]->char_freq < pq->front[(i - 1) / 2]->char_freq) {
    swapNodes(pq, i, (i - 1) / 2);
    i = (i - 1) / 2;
  }
}

/**
 * Extracts the minimum HuffmanNode from the PriorityQueue
 *
 * @param pq - a pointer to the PriorityQueue
 * @return a pointer to the minimum HuffmanNode
 */
HuffmanNode* extractMin(PriorityQueue* pq) {
  HuffmanNode* temp;
  if (pq->size <= 0) {
    return NULL;
  }
  temp = pq->front[0];
  pq->size--;

  if (pq->size > 0) {
    pq->front[0] = pq->front[pq->size];
    minHeapify(pq, 0);
  }

  return temp;
}

/**
 * Creates a Huffman tree from an array of character frequencies
 *
 * @param frequencies - an array of character frequencies in ascending asci
 * @return the root of the Huffman tree
 */
HuffmanNode* buildHuffmanTree(int* frequencies, int size) {
  PriorityQueue* pq = createPriorityQueue(size);
  int i;
  HuffmanNode* root;
  for (i = 0; i < size; i++) {
    if (frequencies[i] > 0) {
      HuffmanNode* node = createNode(i, frequencies[i], NULL, NULL);
      insertNode(pq, node);
    }
  }
  while (pq->size > 1) {
    HuffmanNode* left = extractMin(pq);
    HuffmanNode* right = extractMin(pq);
    HuffmanNode* combined =
        createNode(0, left->char_freq + right->char_freq, NULL, NULL);
    combined->left = left;
    combined->right = right;
    insertNode(pq, combined);
  }

  root = extractMin(pq);
  free(pq->front);
  free(pq);
  return root;
}

/**
 * Frees the Huffman tree nodes and associated memory
 *
 * @param node - a pointer to the root of the Huffman tree
 */
void freeHuffmanTree(HuffmanNode* node) {
  if (node == NULL) {
    return;
  }
  freeHuffmanTree(node->left);
  freeHuffmanTree(node->right);
  safe_free(node);  // Free the current node after its children are freed
}

/**
 * Opens a file and counts the frequency of each character in the file
 *
 * @param file - a pointer to the file to count the frequencies of
 * @return an array of character frequencies in ascending asci order
 */
int* countFrequencies(FILE* file) {
  int c;
  int* char_freq = (int*)safe_calloc(MAX_CODE_LENGTH, sizeof(int));
  while ((c = fgetc(file)) != EOF) {
    char_freq[c]++;
  }
  return char_freq;
}

void buildCodesHelper(HuffmanNode* node, char** huffman_codes, char* code_str) {
  if (node == NULL) {
    return;
  }
  if (node->left == NULL && node->right == NULL) {
    huffman_codes[(int)node->char_ascii] = strdup(code_str);
  } else {
    char left_code[strlen(code_str) + 2];
    char right_code[strlen(code_str) + 2];
    sprintf(left_code, "%s0", code_str);
    sprintf(right_code, "%s1", code_str);
    buildCodesHelper(node->left, huffman_codes, left_code);
    buildCodesHelper(node->right, huffman_codes, right_code);
  }
}

char** buildCodes(HuffmanNode* node) {
  int i;
  char** huffman_codes = (char**)calloc(MAX_CODE_LENGTH, sizeof(char*));
  for (i = 0; i < MAX_CODE_LENGTH; i++) {
    huffman_codes[i] = NULL;
  }
  buildCodesHelper(node, huffman_codes, "");
  return huffman_codes;
}

/**
 * Frees the memory allocated for Huffman codes
 *
 * @param huffman_codes - an array of Huffman codes
 */
void freeHuffmanCodes(char** huffman_codes) {
  int i;
  for (i = 0; i < MAX_CODE_LENGTH; i++) {
    if (huffman_codes[i] != NULL) {
      safe_free(huffman_codes[i]);
    }
  }
  safe_free(huffman_codes);
}