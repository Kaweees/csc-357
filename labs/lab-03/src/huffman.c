#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <stddef.h>

#include "huffman.h"
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
 * Opens a file and counts the frequency of each character in the file
 *
 * @param file - a pointer to the file to count the frequencies of
 * @return an array of character frequencies in ascending asci order
 */
int* countFrequencies(FILE* file) {
  int changes = 0;
  int c;
  int* char_freq = (int*)safe_calloc(MAX_CODE_LENGTH, sizeof(int));
  while ((c = fgetc(file)) != EOF) {
    char_freq[c]++;
    changes++;
  }
  if (changes == 0) {
    return NULL;
  }
  return char_freq;
}

void insert(HuffmanNode* head, HuffmanNode* node){
   HuffmanNode *firstNode;
   firstNode = head;
  while((firstNode->next != NULL) && (firstNode->next->char_freq < node->char_freq ||
 (firstNode->next->char_freq == node->char_freq && firstNode->next->char_ascii < node->char_ascii))) {
     firstNode = firstNode->next;
   }
   node->next = firstNode->next;
   firstNode->next = node;
}
  
HuffmanNode *buildTree(int list[MAX_CODE_LENGTH]) {
  HuffmanNode *head;
  int i;
  
  head = createNode(0, 0, NULL, NULL, NULL);
  /* compares if value in "list" is > 0 */
  for (i = 0; i < MAX_CODE_LENGTH; i++) {
    HuffmanNode *newNode;
    if (list[i] > 0) {
      /* create and insert new node */
      newNode = createNode(i, list[i], NULL, NULL, NULL);
      insert(head, newNode);
    }
  }
  
  HuffmanNode *leftNode;
  HuffmanNode *rightNode;
    
  /* linked list */
  while (head->next->next != NULL) {
    leftNode = head->next;
    rightNode = head->next->next;
    /* creates new node with updated freq */
    HuffmanNode *newNode = createNode(0, leftNode->char_freq + rightNode->char_freq, NULL, NULL, NULL);
    head->next = head->next->next->next;
    leftNode->next = NULL;
    rightNode->next = NULL;
    newNode->left = leftNode;
    newNode->right = rightNode;
    insert(head, newNode);
  }
  /* move head to next node and free old head*/
  HuffmanNode *oldHead;
  oldHead = head;
  head = head->next;   
  free(oldHead);
  return head;
}
  
int printHuffmanCodes(HuffmanNode *root, char *code, int top, unsigned char c) {
  /* Traverse tree */
  if (root == NULL) {
    return 0;
  }
  /* left node */
  if (root->left) {
    code[top] = 0; /* left node */
    if (printHuffmanCodes(root->left, code, top + 1, c)) {
      code[top] = '0';
      return 1;
    }
  }
  /* right node */
  if (root->right) {
    if (printHuffmanCodes(root->right, code, top + 1, c)) {
      /* right node */
      code[top] = '1';
      return 1;
    }
  }
  /* leaf node */
  if (!(root->left) && !(root->right)) {
    return (root->char_ascii == c);
  }
  return 0;
}

/* free memory */
void cleanUp(HuffmanNode* root) {
  if (root == NULL) {
    return;
  }
  /* left tree */
  if (root->left) {
    cleanUp(root->left);
  }
  /* right tree */
  if (root->right) {
    cleanUp(root->right);
  }
  free(root);
}
