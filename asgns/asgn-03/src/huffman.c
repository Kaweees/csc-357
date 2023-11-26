#include <arpa/inet.h>
#include <fcntl.h>
#include <stddef.h>
#include <stdint.h>
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <sys/types.h>
#include <unistd.h>

#include "linked_list.h"
#include "safe_file.h"
#include "safe_mem.h"

/**
 * Creates a FrequencyList
 *
 * @return a pointer to the FrequencyList
 */
FrequencyList* createFrequencyList(size_t size) {
  FrequencyList* freq = (FrequencyList*)safe_calloc(sizeof(FrequencyList), 1);
  freq->num_non_zero_freq = 0;
  freq->size = size;
  freq->frequencies =
      (unsigned int*)safe_calloc(freq->size, sizeof(unsigned int));
  return freq;
}

/**
 * Opens a file and counts the frequency of each character in the file
 *
 * @param file - a pointer to the file to count the frequencies of
 * @return an array of character frequencies in ascending asci order
 */
FrequencyList* countFrequencies(FileContent* file_contents) {
  FrequencyList* char_freq = createFrequencyList(MAX_CODE_LENGTH);
  int i;
  for (i = 0; i < file_contents->file_size; i++) {
    if (char_freq->frequencies[(int)file_contents->file_contents[i]] == 0) {
      char_freq->num_non_zero_freq++;
    }
    char_freq->frequencies[(int)file_contents->file_contents[i]]++;
  }
  return char_freq;
}
/**
 * Read a frequency list and write it to a file as a header
 *
 * @param ascii - the ASCII character
 * @param freq - the frequency of the character
 * @param left - the left child of the node
 * @param right - the right child of the node
 * @return a pointer to the node
 */
void createHeader(FrequencyList* freq_list, int outfile) {
  uint8_t size = freq_list->num_non_zero_freq - 1;
  safe_write(outfile, &size, sizeof(uint8_t));
  int i;
  for (i = 0; i < freq_list->size; i++) {
    if (freq_list->frequencies[i] > 0) {
      uint8_t ascii = i;
      uint32_t frequency = freq_list->frequencies[i];
      frequency = htonl(frequency);
      safe_write(outfile, &ascii, sizeof(uint8_t));
      safe_write(outfile, &frequency, sizeof(uint32_t));
    }
  }
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
HuffmanNode* createNode(char ascii, int freq, HuffmanNode* left,
    HuffmanNode* right, HuffmanNode* next) {
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
  return a->char_freq < b->char_freq ||
         (a->char_freq == b->char_freq && a->char_ascii < b->char_ascii);
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

void insert(HuffmanNode* head, HuffmanNode* node) {
  HuffmanNode* firstNode;
  firstNode = head;
  while ((firstNode->next != NULL) && comesBefore(firstNode->next, node)) {
    firstNode = firstNode->next;
  }
  node->next = firstNode->next;
  firstNode->next = node;
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
  } else {
    /* Set curr to the node before the insertion point */
    HuffmanNode* curr = lls->head;
    while (curr->next != NULL && comesBefore(curr->next, node)) {
      curr = curr->next;
    }
    node->next = curr->next;
    curr->next = node;
  }
  lls->size++;
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
    temp->next = NULL;
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
  /* Put the node with the smaller frequency on the left, and set the frequency
   * of the new node to the sum of the children. */
  a->next = b->next = NULL;
  return createNode(0, a->char_freq + b->char_freq, comesBefore(a, b) ? a : b,
      comesBefore(a, b) ? b : a, NULL);
}

/**
 * Creates a Huffman tree from an array of character frequencies
 *
 * @param frequencies - an array of character frequencies in ascending asci
 * order
 * @return the root of the Huffman tree
 */
HuffmanNode* buildHuffmanTree(FrequencyList* frequencies) {
  HuffmanNode* head;
  int i;
  head = createNode(0, 0, NULL, NULL, NULL);
  for (i = 0; i < MAX_CODE_LENGTH; i++) {
    HuffmanNode* newNode;
    if (frequencies->frequencies[i] > 0) {
      newNode = createNode(i, frequencies->frequencies[i], NULL, NULL, NULL);
      insert(head, newNode);
    }
  }

  /* creates new node with updated freq */
  while (head->next->next != NULL) {
    HuffmanNode* leftNode = head->next;
    HuffmanNode* rightNode = head->next->next;
    /* create the new node */
    HuffmanNode* newNode = createNode(
        0, leftNode->char_freq + rightNode->char_freq, NULL, NULL, NULL);
    head->next = head->next->next->next;
    leftNode->next = NULL;
    rightNode->next = NULL;
    newNode->left = leftNode;
    newNode->right = rightNode;
    insert(head, newNode);
  }
  /* move the new node with the updated freq to the front */
  HuffmanNode* oldHead;
  oldHead = head;
  head = head->next;
  free(oldHead);
  return head;
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
    strcpy(left_code, code_str);
    strcpy(right_code, code_str);
    strcat(left_code, "0");
    strcat(right_code, "1");
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