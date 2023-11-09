#include <sys/types.h>

#ifndef LINKED_LIST_H
#define LINKED_LIST_H

typedef struct Node Node;
typedef struct LinkedList LinkedList;

/* Represents a node in a linked list */
struct Node {
  /* The data stored by the node */
  struct stat* data;
  /* The next node in the linked list */
  Node* next;
};

/* Represents a linked list */
struct LinkedList {
  /* The head of the linked list */
  Node* head;
  /* The size of the linked list */
  ssize_t size;
};

Node* createNode(struct stat* data);
LinkedList* createLinkedList();
void insertNode(LinkedList* lls, Node* node);
Node* removeFirstNode(LinkedList* lls);

#endif