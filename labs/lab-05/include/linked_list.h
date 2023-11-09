#include <sys/types.h>

#ifndef LINKED_LIST_H
#define LINKED_LIST_H

typedef struct Node Node;
typedef struct LinkedList LinkedList;

/* Represents a node in a linked list */
struct Node {
  /* The device ID of the file */
  dev_t dev;
  /* The inode number of the file */
  ino_t ino;
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

Node* createNode(dev_t dev, ino_t ino);
LinkedList* createLinkedList();
void insertNode(LinkedList* lls, Node* node);
Node* removeFirstNode(LinkedList* lls);

#endif /* LINKED_LIST_H */