#include "huffman.h"

#include <stdio.h>
#include <stdlib.h>

#include "safe_mem.h"
#include <string.h>
#define MAX_TREE_HT 100

/**
 * Opens a file and counts the frequency of each character in the file
 *
 * @param file - a pointer to the file to count the frequencies of
 * @return an array of character frequencies in ascending asci order
 */
FrequencyList* countFrequencies(FILE* file) {
  int c;
  FrequencyList* char_freq = (FrequencyList*)safe_malloc(sizeof(FrequencyList));
  char_freq->num_non_zero_freq = 0;
  char_freq->size = MAX_CODE_LENGTH;
  char_freq->frequencies = (unsigned char*)safe_calloc(MAX_CODE_LENGTH, sizeof(unsigned char));
  while ((c = fgetc(file)) != EOF) {
    if (char_freq->frequencies[c] == 0) {
      ++char_freq->num_non_zero_freq;
    }
    char_freq->frequencies[c]++;
  }
  return char_freq;
}

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
    char ascii, unsigned char freq, HuffmanNode* left, HuffmanNode* right) {
  HuffmanNode* newNode = (HuffmanNode*)safe_malloc(sizeof(HuffmanNode));
  newNode->char_ascii = ascii;
  newNode->char_freq = freq;
  newNode->left = left;
  newNode->right = right;
  return newNode;
}

/**
 * Creates a Priority Queue (implemented as a Min-max-heap) of a given capacity
 *
 * @param capacity - the capacity of the queue
 * @return a pointer to the queue
 */
PriorityQueue* createPriorityQueue(unsigned int capacity) {
  PriorityQueue* minHeap = (PriorityQueue*)safe_malloc(sizeof(PriorityQueue));
  // current size is 0
  minHeap->size = 0;

  minHeap->capacity = capacity;

  minHeap->front =
      (HuffmanNode**)safe_malloc(minHeap->capacity * sizeof(HuffmanNode*));
  return minHeap;
}

/**
 * Creates a Priority Queue (implemented as a Min-max-heap) of a given capacity
 *
 * @param capacity - the capacity of the queue
 * @return a pointer to the queue
 */
void swapNodes(HuffmanNode** a, HuffmanNode** b) {
  HuffmanNode* temp = *a;
  /* Swap the pointers */
  *a = *b;
  *b = temp;
}

/**
 * Moves the node up the priority queue until its children are larger than it
 *
 * @param pq - a pointer to the PriorityQueue
 * @param idx - the index of the node to heapify
 */
void minHeapify(PriorityQueue* minHeap, int idx) {
  int smallest = idx;
  int left = 2 * idx + 1;
  int right = 2 * idx + 2;

  if (left < minHeap->size &&
      minHeap->front[left]->char_freq < minHeap->front[smallest]->char_freq)
    smallest = left;

  if (right < minHeap->size &&
      minHeap->front[right]->char_freq < minHeap->front[smallest]->char_freq)
    smallest = right;

  if (smallest != idx) {
    swapNodes(&minHeap->front[smallest], &minHeap->front[idx]);
    minHeapify(minHeap, smallest);
  }
}

/**
 * Extracts the minimum HuffmanNode from the PriorityQueue
 *
 * @param pq - a pointer to the PriorityQueue
 * @return a pointer to the minimum HuffmanNode
 */
HuffmanNode* extractMin(PriorityQueue* minHeap) {
  HuffmanNode* temp = minHeap->front[0];
  minHeap->front[0] = minHeap->front[minHeap->size - 1];

  --minHeap->size;
  minHeapify(minHeap, 0);

  return temp;
}

/**
 * Insert a new HuffmanNode into the PriorityQueue
 *
 * @param pq - a pointer to the PriorityQueue
 * @param node - a pointer to the HuffmanNode to insert
 */
void insertMinHeap(PriorityQueue* minHeap, HuffmanNode* HuffmanNode)

{
  ++minHeap->size;
  int i = minHeap->size - 1;

  while (i && HuffmanNode->char_freq < minHeap->front[(i - 1) / 2]->char_freq) {
    minHeap->front[i] = minHeap->front[(i - 1) / 2];
    i = (i - 1) / 2;
  }

  minHeap->front[i] = HuffmanNode;
}

/**
 * Maintain the min-heap property of the PriorityQueue by percolating up each
 * HuffmanNode
 *
 * @param pq - a pointer to the PriorityQueue
 */
void buildMinHeap(PriorityQueue* minHeap) {
  int n = minHeap->size - 1;
  int i;

  for (i = (n - 1) / 2; i >= 0; --i) minHeapify(minHeap, i);
}

// A utility function to print an array of size n
void printArr(int arr[], int n) {
  int i;
  for (i = 0; i < n; ++i) printf("%d", arr[i]);

  printf("\n");
}

/**
 * Check if a HuffmanNode is a leaf (it has no children)
 *
 * @param node - a pointer to the HuffmanNode
 * @return 1 if the node is a leaf, 0 otherwise
 */
int isLeaf(HuffmanNode* root) { return !(root->left) && !(root->right); }

/**
 * Create a min-heap priority queue from an array of character frequencies
  *
  @param freq_list - a pointer to a FrequencyList
  @return a pointer to the PriorityQueue
 */
PriorityQueue* createPriorityQueueFromFreqArray(FrequencyList* freq_list) {
  PriorityQueue* minHeap = createPriorityQueue(freq_list->size);
  for (int i = 0; i < freq_list->size; i++) {
    if (freq_list->frequencies[i] > 0) {
      // printf("0x%02x: %d\n", i, freq_list->frequencies[i]);
      minHeap->front[minHeap->size++] = createNode(i, freq_list->frequencies[i], NULL, NULL);
    }
  }
  buildMinHeap(minHeap);
  return minHeap;
}

/**
 * Creates a Huffman tree from an array of character frequencies
 *
 * @param freq_list - a pointer to a FrequencyList
 * @return the root of the Huffman tree
 */
HuffmanNode* buildHuffmanTree(FrequencyList* freq_list) {
  HuffmanNode *left, *right, *top, *root;
  /* Create a min heap priority queue from the frequencies array */
  PriorityQueue* minHeap = createPriorityQueueFromFreqArray(freq_list);
  /* Iterate until there is only one node left in the priority queue */
  while (!(minHeap->size == 1)) {
    /* Extract the two least frequently appearing nodes from the priority queue */
    left = extractMin(minHeap);
    right = extractMin(minHeap);
    /* Superimpose the two nodes into a single node */
    top = createNode('$', left->char_freq + right->char_freq, left, right);
    insertMinHeap(minHeap, top);
  }
  /* Return the root of the Huffman tree */
  root = extractMin(minHeap);
  /* Free the priority queue */
  freePriorityQueue(minHeap);
  return root;
}

/**
 * Frees the memory allocated for a FrequencyList
 *
 * @param node - a pointer to the root of the Huffman tree
 */
void freeFrequencyList(FrequencyList* freq_list) {
  if (freq_list == NULL) {
    return;
  }
  safe_free(freq_list->frequencies);
  safe_free(freq_list);
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
  char** huffman_codes = (char**)calloc(MAX_CODE_LENGTH, sizeof(char*));
  for (int i = 0; i < MAX_CODE_LENGTH; i++) {
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

void freePriorityQueue(PriorityQueue* pq) {
  safe_free(pq->front);
  safe_free(pq);
}