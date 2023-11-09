#include "linked_list.h"

#include <sys/types.h>

#include "safe_mem.h"

/**
 * Creates a Node
 *
 * @return a pointer to the node
 */
Node* createNode(dev_t dev, ino_t ino) {
  Node* node = (Node*)safeMalloc(sizeof(Node));
  node->dev = dev;
  node->ino = ino;
  node->next = NULL;
  return node;
}

/**
 * Creates a LinkedList
 *
 * @return a pointer to the linked list
 */
LinkedList* createLinkedList() {
  LinkedList* lls = (LinkedList*)safeMalloc(sizeof(LinkedList));
  lls->size = 0;
  lls->head = NULL;
  return lls;
}

/**
 * Inserts a Node into a LinkedList at the beginning
 *
 * @param lls - a pointer to the LinkedList
 * @param node - a pointer to the HuffmanNode to insert
 */
void insertNode(LinkedList* lls, Node* node) {
  if (lls->size == 0) {
    lls->head = node;
  } else {
    node->next = lls->head;
    lls->head = node;
  }
  lls->size++;
}

/**
 * Removes a Node from the beginning of a LinkedList
 *
 * @param lls - a pointer to the LinkedList
 * @return  a pointer of the removed Node
 */
Node* removeFirstNode(LinkedList* lls) {
  if (lls->size == 0) {
    return NULL;
  } else {
    Node* temp = lls->head;
    lls->head = lls->head->next;
    lls->size--;
    temp->next = NULL;
    return temp;
  }
}