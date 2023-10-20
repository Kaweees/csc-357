#include "huffman.h"

#include <stddef.h>
#include <stdio.h>
#include <stdlib.h>
#include <string.h>

#define MAX_CODE_LENGTH 256 /* total number of characters in ASCII */
#define SPACE_CHAR      32  /* asci code for a space */

struct HuffmanNode {
  char char_asci;
  int char_freq;
  struct HuffmanNode* left;
  struct HuffmanNode* right;
};

void initLinkedList(struct LinkedList* list);
void append(struct LinkedList* list, int data);
void printLinkedList(const struct LinkedList* list);
void freeLinkedList(struct LinkedList* list);

int comesBefore(struct HuffmanNode* a, struct HuffmanNode* b);
struct HuffmanNode* combine(struct HuffmanNode* a, struct HuffmanNode* b);

char** buildCodes(struct HuffmanNode* root);

char* createHeader(char* codes, char* text) {
  // for (int i = 0; i < strlen()) HuffmanNode* pop(struct LinkedList * list);
}

void vaildate_malloc(void* ptr) {
  if (ptr == NULL) {
    perror("Memory allocation error");
    exit(1);
  }
}

/**
 * Takes a string of the form "x y z" and returns an array of frequencies in
 * ascending asci order
 *
 * @param file - a pointer to the file to read the line from
 * @return a pointer to the line read from the file
 */
int* parseHeader(char* header, char* text) {
  int* freq_list = malloc(sizeof(int) * MAX_CODE_LENGTH);
  int ch; /* character read from file */
  vaildate_malloc(freq_list);
  int header_length = strlen(header);
  int i = 0;
  while (i < header_length) {
    while ((ch = header[i]) != SPACE_CHAR) {
      i = (i * 10) + (ch - '0');
      i++;
    }
    freq_list[i]++;
    i = 0;
  }
}

/**
 * Opens a file and counts the frequency of each character in the file
 *
 * @param file - a pointer to the file to count the frequencies of
 * @return an array of character frequencies in ascending asci order
 */
int* countFrequencies(FILE* fp) {
  int* freq_list;
  int ch; /* character read from file */
  freq_list = (int*)calloc(MAX_CODE_LENGTH, sizeof(int));
  vaildate_malloc(freq_list);
  while ((ch = fgetc(fp)) != EOF) {
    freq_list[ch]++;
  }
}

void merge(int arr[], int left, int mid, int right) {
  int n1 = mid - left + 1;
  int n2 = right - mid;

  int L[n1], R[n2];

  for (int i = 0; i < n1; i++) {
    L[i] = arr[left + i];
  }
  for (int i = 0; i < n2; i++) {
    R[i] = arr[mid + 1 + i];
  }

  int i = 0, j = 0, k = left;
  while (i < n1 && j < n2) {
    if (L[i] >= R[j]) {
      arr[k] = L[i];
      i++;
    } else {
      arr[k] = R[j];
      j++;
    }
    k++;
  }

  while (i < n1) {
    arr[k] = L[i];
    i++;
    k++;
  }

  while (j < n2) {
    arr[k] = R[j];
    j++;
    k++;
  }
}

void mergeSort(int arr[], int left, int right) {
  if (left < right) {
    int mid = left + (right - left) / 2;

    mergeSort(arr, left, mid);
    mergeSort(arr, mid + 1, right);

    merge(arr, left, mid, right);
  }
}

/**
 * Creates a Huffman tree from an array of character frequencies
 *
 * @param frequencies - an array of character frequencies in ascending asci
 * @return the root of the Huffman tree
 */
struct HuffmanNode* buildHuffmanTree(int* frequencies) {
  struct HuffmanNode** non_zero_nodes = (struct HuffmanNode**)malloc(
      MAX_CODE_LENGTH *
      sizeof(struct HuffmanNode*)); /* node list containing HuffmanNodes of
                                                       characters with non-zero
                                        frequencies */
  vaildate_malloc(non_zero_nodes);
  int non_zero_nodes_size = 0;
  for (int i = 0; i < MAX_CODE_LENGTH; i++) {
    if (frequencies[i] > 0) {
      struct HuffmanNode* node = malloc(sizeof(struct HuffmanNode));
      vaildate_malloc(node);
      node->left = NULL;
      node->right = NULL;
      node->char_freq = frequencies[i];
      node->char_asci = i;
      non_zero_nodes[non_zero_nodes_size] = node;
      non_zero_nodes_size++;
    }
  }
  non_zero_nodes = (struct HuffmanNode**)realloc(
      non_zero_nodes, non_zero_nodes_size * sizeof(struct HuffmanNode));
  vaildate_malloc(non_zero_nodes);
  if (non_zero_nodes_size == 0) {
    return NULL;
  } else if (non_zero_nodes_size == 1) {
    return non_zero_nodes[0];
  } else {
    while (non_zero_nodes_size > 1) {
      mergeSort(non_zero_nodes, 0, non_zero_nodes_size - 1);
      struct HuffmanNode* a = non_zero_nodes[0];
      struct HuffmanNode* b = non_zero_nodes[1];
      struct HuffmanNode* combined = combine(a, b);
      non_zero_nodes[0] = combined;
      for (int i = 1; i < non_zero_nodes_size - 1; i++) {
        non_zero_nodes[i] = non_zero_nodes[i + 1];
      }
      non_zero_nodes_size--;
    }
  }

  struct LinkedList* list = malloc(sizeof(struct LinkedList));
  vaildate_malloc(list);

  initLinkedList(list);

  for (int i = 0; i < non_zero_nodes_size; i++) {
    append(list, non_zero_nodes[i]);
  }

  while (list->size > 1) {
    struct HuffmanNode* a = pop(list);
    struct HuffmanNode* b = pop(list);
    struct HuffmanNode* combined = combine(a, b);
    append(list, combined);
  }
}

size_t huffmanEncode(FILE* infile, FILE* outfile) {
  int* char_freq = countFrequencies(infile);
  struct HuffmanNode* root = buildHuffmanTree(char_freq);
  free(char_freq);
  free(root);
}