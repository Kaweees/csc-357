
#include "huffman.h"

#include <stddef.h>
#include <stdio.h>
#include <stdlib.h>
#include <string.h>

#include "safe_mem.h"

#define MAX_CODE_LENGTH     256  /* total number of characters in ASCII */
#define SPACE_CHAR          32   /* asci code for a space */
#define STRING_TERMINATOR   '\0' /* null terminator for a string */
#define INITIAL_BUFFER_SIZE 128  /* Initial size of the buffer in bytes */

/**
 * Reads a file and returns its contents as a singular string
 * @param file - a pointer to the file to read from
 * @return a pointer to the string containing the file's contents
 */
struct FileContent* readText(FILE* file) {
  struct FileContent* file_stuff =
      (struct FileContent*)safe_malloc(sizeof(struct FileContent));

  fseek(file, 0, SEEK_END);  // Move the file pointer to the end of the file
  file_stuff->file_size = ftell(file);  // Get the size of the file
  if (file_stuff->file_size == 0) {
    safe_free(file_stuff);
    return NULL;
  }
  fseek(file, 0, SEEK_SET);  // Reset the file pointer to the beginning

  /* Allocate memory for the file contents */
  file_stuff->file_contents = (char*)safe_malloc(file_stuff->file_size + 1);
  /* Read the file contents into the allocated memory */
  if (fread(file_stuff->file_contents, 1, file_stuff->file_size, file) !=
      file_stuff->file_size) {
    perror("Error reading file");
    safe_free(file_stuff);
    fclose(file);
    exit(EXIT_FAILURE);
  }
  /* Null-terminate the string */
  file_stuff->file_contents[file_stuff->file_size] = '\0';
  /* Close the file */
  fclose(file);
  return file_stuff;
}

/**
 * Returns whether a HuffmanNode should come before another HuffmanNode.
 Determined by which node appears more frequently than another, with their Asci
 values used to break ties.
 * @param a - a pointer to the first HuffmanNode
 * @param b - a pointer to the second HuffmanNode
 * @return 1 if a should come before b, 0 otherwise
 */
int comesBefore(struct HuffmanNode* a, struct HuffmanNode* b) {
  if (a->char_freq == b->char_freq) {
    return a->char_ascii < b->char_ascii;
  } else {
    return a->char_freq < b->char_freq;
  }
}

/**
 * Superimposes a Huffman tree onto a Huffman tree. The root of the new tree is
 * the root of the first tree, and the left and right children of the root are
 * the roots of the second and third trees, respectively.
 * @param a - a pointer to the first HuffmanNode
 * @param b - a pointer to the second HuffmanNode
 * @return the root of the new tree
 */
struct HuffmanNode* combine(struct HuffmanNode* a, struct HuffmanNode* b) {
  struct HuffmanNode* combined =
      (struct HuffmanNode*)safe_malloc(sizeof(struct HuffmanNode));
  char min_char_ascii =
      (a->char_ascii < b->char_ascii) ? a->char_ascii : b->char_ascii;
  if (comesBefore(a, b)) {
    combined->left = a;
    combined->right = b;
  } else {
    combined->left = b;
    combined->right = a;
  }
  combined->char_ascii = min_char_ascii;
  combined->char_freq = a->char_freq + b->char_freq;
  return combined;
}

void buildCodesHelper(
    struct HuffmanNode* node, struct HuffmanCode* codes, char* code) {
  if (node == NULL || code == NULL) {
    printf("its so over");
    return;
  }
  printf("here\n");
  if (node->left == NULL && node->right == NULL) {
    codes[(int)node->char_ascii].code_contents = code;
    codes[(int)node->char_ascii].code_length = strlen(code);
  } else {
    char* left_code = (char*)safe_calloc(sizeof(char), MAX_CODE_LENGTH);
    char* right_code = (char*)safe_calloc(sizeof(char), MAX_CODE_LENGTH);
    strcpy(left_code, code);
    strcpy(right_code, code);
    strcat(left_code, "0");
    strcat(right_code, "1");
    printf("%s\n", left_code);
    printf("%p\n", left_code);
    buildCodesHelper(node->left, codes, left_code);
    buildCodesHelper(node->right, codes, right_code);
  }
}

/**
 * Generate a lookup table for each of the codes present in the
 * @param a - a pointer to the first HuffmanNode
 * @param b - a pointer to the second HuffmanNode
 * @return the root of the new tree
 */
struct HuffmanCode* buildCodes(struct HuffmanNode* root) {
  struct HuffmanCode* codes = (struct HuffmanCode*)safe_malloc(
      sizeof(struct HuffmanCode) * MAX_CODE_LENGTH);
  for (int i = 0; i < MAX_CODE_LENGTH; i++) {
    struct HuffmanCode* code =
        (struct HuffmanCode*)safe_malloc(sizeof(struct HuffmanCode));
    code->code_contents = (char*)safe_calloc(sizeof(char), MAX_CODE_LENGTH);
    code->code_length = 0;
    code->code_capacity = MAX_CODE_LENGTH;
    codes[i] = *code;
  }
  printf("here\n");
  buildCodesHelper(root, codes, "");
  return codes;
}

char* createHeader(char* codes, char* text) {
  // for (int i = 0; i < strlen()) HuffmanNode* pop(struct LinkedList * list);
  char* header = (char*)safe_malloc(sizeof(char) * MAX_CODE_LENGTH);
  return header;
}

/**
 * Takes a string of the form "x y z" and returns an array of frequencies in
 * ascending asci order
 *
 * @param file - a pointer to the file to read the line from
 * @return a pointer to the line read from the file
 */
int* parseHeader(char* header, char* text) {
  int* freq_list = (int*)safe_malloc(sizeof(int) * MAX_CODE_LENGTH);
  int ch; /* character read from file */
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
  return freq_list;
}

/**
 * Opens a file and counts the frequency of each character in the file
 *
 * @param file - a pointer to the file to count the frequencies of
 * @return an array of character frequencies in ascending asci order
 */
int* countFrequencies(struct FileContent* text) {
  if (text == NULL) {
    return NULL;
  }
  int* freq_list = (int*)safe_calloc(MAX_CODE_LENGTH, sizeof(int));
  int i = 0;
  for (i = 0; i < text->file_size; i++) {
    /* increment the frequency of the corresponding character */
    freq_list[(int)text->file_contents[i]]++;
  }
  return freq_list;
}

void merge(struct HuffmanNode arr[], int l, int m, int r) {
  int i, j, k;
  int n1 = m - l + 1;
  int n2 = r - m;

  // Create temporary arrays
  struct HuffmanNode L[n1], R[n2];

  // Copy data to temporary arrays L[] and R[]
  for (i = 0; i < n1; i++) L[i] = arr[l + i];
  for (j = 0; j < n2; j++) R[j] = arr[m + 1 + j];

  // Merge the temporary arrays back into arr[l..r]
  i = 0;  // Initial index of first subarray
  j = 0;  // Initial index of second subarray
  k = l;  // Initial index of merged subarray
  while (i < n1 && j < n2) {
    if (comesBefore(&L[i], &R[j]) == 0) {
      arr[k] = L[i];
      i++;
    } else {
      arr[k] = R[j];
      j++;
    }
    k++;
  }

  // Copy the remaining elements of L[], if there are any
  while (i < n1) {
    arr[k] = L[i];
    i++;
    k++;
  }

  // Copy the remaining elements of R[], if there are any
  while (j < n2) {
    arr[k] = R[j];
    j++;
    k++;
  }
}

// l is for left index and r is right index of the
// sub-array of arr to be sorted
void mergeSort(struct HuffmanNode arr[], int l, int r) {
  if (l < r) {
    // Same as (l+r)/2, but avoids overflow for large l and r
    int m = l + (r - l) / 2;

    // Sort first and second halves
    mergeSort(arr, l, m);
    mergeSort(arr, m + 1, r);

    // Merge the sorted halves
    merge(arr, l, m, r);
  }
}

/**
 * Creates a Huffman tree from an array of character frequencies
 *
 * @param frequencies - an array of character frequencies in ascending asci
 * @return the root of the Huffman tree
 */
struct HuffmanNode* buildHuffmanTree(int* frequencies) {
  if (frequencies == NULL) {
    return NULL;
  }
  struct HuffmanNode* root = NULL;
  struct HuffmanNode* non_zero_nodes = (struct HuffmanNode*)safe_malloc(
      sizeof(struct HuffmanNode) * MAX_CODE_LENGTH); /* node list containing
HuffmanNodes of characters with non-zero frequencies */
  int non_zero_nodes_size = 0;
  for (int i = 0; i < MAX_CODE_LENGTH; i++) {
    if (frequencies[i] > 0) {
      struct HuffmanNode node;
      node.left = NULL;
      node.right = NULL;
      node.char_freq = frequencies[i];
      node.char_ascii = i;
      non_zero_nodes[non_zero_nodes_size++] = node;
    }
  }
  non_zero_nodes = (struct HuffmanNode*)safe_realloc(
      non_zero_nodes, non_zero_nodes_size * sizeof(struct HuffmanNode));
  if (non_zero_nodes_size == 0) {
    safe_free(non_zero_nodes);
    return root;
  } else {
    while (non_zero_nodes_size > 1) {
      mergeSort(non_zero_nodes, 0, non_zero_nodes_size - 1);
      struct HuffmanNode a = non_zero_nodes[non_zero_nodes_size - 1];
      struct HuffmanNode b = non_zero_nodes[non_zero_nodes_size - 2];
      struct HuffmanNode* combined = combine(&a, &b);
      non_zero_nodes[0] = *combined;
      free(combined);
      non_zero_nodes_size--;
    }
    root = (struct HuffmanNode*)realloc(
        non_zero_nodes, non_zero_nodes_size * sizeof(struct HuffmanNode));
    return root;
  }
}