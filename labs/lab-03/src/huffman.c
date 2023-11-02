#include "huffman.h"

#include <stddef.h>
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <unistd.h>

#include "safe_mem.h"

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
  char_freq->frequencies =
      (unsigned char*)safe_calloc(MAX_CODE_LENGTH, sizeof(unsigned char));
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
    char ascii, int freq, HuffmanNode* left, HuffmanNode* right, HuffmanNode* next) {
  HuffmanNode* newNode = (HuffmanNode*)safe_malloc(sizeof(HuffmanNode));
  newNode->char_ascii = ascii;
  newNode->char_freq = freq;
  newNode->left = left;
  newNode->right = right;
  newNode->next = next;
  return newNode;
}

/**
 * Returns whether a HuffmanNode should come before another HuffmanNode.
 Determined by which node appears more frequently than another, with their Asci
 values used to break ties.
 * @param a - a pointer to the first HuffmanNode
 * @param b - a pointer to the second HuffmanNode
 * @return 1 if a should come before b, 0 otherwise
 */
int comesBefore(HuffmanNode* a, HuffmanNode* b) {
  if (a->char_freq == b->char_freq) {
    return a->char_ascii < b->char_ascii;
  } else {
    return a->char_freq < b->char_freq;
  }
}

/**
 * Creates a LinkedList
 *
 * @return a pointer to the linked list
 */
LinkedList* createLinkedList() {
  LinkedList* lls = (LinkedList*)safe_malloc(sizeof(LinkedList));
  lls->size = 0;
  lls->head = NULL;
  return lls;
}

/**
 * Inserts a HuffmanNode into a LinkedList at its correct position
 *
 * @param lls - a pointer to the LinkedList
 * @param node - a pointer to the HuffmanNode to insert
 */
void insertNode(LinkedList* lls, HuffmanNode* node) {
  if (lls->size == 0) {
    lls->head = node;
    lls->size++;
    return;
  } else {
    /* Set curr to the node before the insertion point */
    HuffmanNode* curr = lls->head;
    while (curr->next != NULL && comesBefore(curr->next, node)) {
      curr = curr->next;
    }
    node->next = curr->next;
    curr->next = node;
    lls->size++;
  }
}

/**
 * Remove and return the first node from a LinkedList
 *
 * @param lls - a pointer to the LinkedList
 * @return the first node in the LinkedList
 */
HuffmanNode* removeFirst(LinkedList* lls) {
  if (lls->size == 0) {
    return NULL;
  } else {
    HuffmanNode* temp = lls->head;
    lls->head = lls->head->next;
    lls->size--;
    return temp;
  }
}

/**
 * Superimposes a Huffman node onto another Huffman node
 * @param a - a pointer to the first HuffmanNode
 * @param b - a pointer to the second HuffmanNode
 * @return the root of the new tree
 */
HuffmanNode* combine(HuffmanNode* a, HuffmanNode* b) {
  /* Put the node with the smaller frequency on the left, and set the frequency of the new node to the sum of the children. */
  a->next = b->next = NULL;
  return createNode(0, a->char_freq + b->char_freq, comesBefore(a, b) ? a : b, comesBefore(a, b) ? b : a, NULL);
}

/**
 * Creates a Huffman tree from an array of character frequencies
 *
 * @param frequencies - an array of character frequencies in ascending asci order
 * @return the root of the Huffman tree
 */
HuffmanNode* buildHuffmanTree(FrequencyList* frequencies) {
  LinkedList* lls = createLinkedList();
  for (int i = 0; i < frequencies->size; i++) {
    if (frequencies->frequencies[i] > 0) {
      HuffmanNode* node = createNode(i, frequencies->frequencies[i], NULL, NULL, NULL);
      insertNode(lls, node);
    }
  }
  while (lls->size > 1) {
    HuffmanNode* left = removeFirst(lls);
    HuffmanNode* right = removeFirst(lls);
    HuffmanNode* combined = combine(left, right);
    insertNode(lls, combined);
  }
  HuffmanNode* root = removeFirst(lls);
  safe_free(lls);
  return root;
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
  /* Free the current node after its children are freed */
  safe_free(node);
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