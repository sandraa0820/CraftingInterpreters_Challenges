#include <stdio.h>
#include <stdlib.h>

//Define the Doubly Linked List Node
typedef struct Node {
    int data;
    struct Node* prev; // a pointer: holds the address of the PREVIOUS node
    struct Node* next; // a pointer: holds the addresss of the NEXT node
} Node;

//Helper to create a new node
Node* createNode(int data) {
    Node* newNode = (Node*)malloc(sizeof(Node));

    newNode->data = data;
    newNode->prev = NULL;
    newNode->next = NULL;

    return newNode; //hands back the memory address of the new node
}

void insertFront(Node** head, int data) {
    //Create a new Node
    Node* newNode = createNode(data);

    //Attached the new node's 'next' pointer to the current 1st node
    newNode->next = *head;

    //create an if statement 
    if (*head != NULL) {
        (*head)->prev = newNode;
    }

    *head = newNode;
}

Node* findNode(Node* head, int key) {
    Node* current = head;

    while (current != NULL) {
        if (current->data == key) {
            return current;
        }
        current = current->next;
    }
    return NULL;
}

void deleteNode(Node** head, Node* delNode) {
    if (*head == NULL || delNode == NULL) return;

    if (*head == delNode) {
        *head = delNode->next;
    }

    if (delNode->next != NULL) {
        delNode->next->prev = delNode->prev;
    }

    if (delNode->prev != NULL) {
        delNode->prev->next = delNode->next;
    }

    free(delNode);
}

void printList(Node* head) {
    Node* current = head;
    printf("List: ");
    while (current != NULL) {
        printf("%d <-> ", current->data);
        current = current->next;
    }
    printf("NULL\n");
}

int main() {
    printf("Hello, World! Starting Doubly Linked List Tests:\n\n");
    Node* head = NULL;

    // Test Insertions
    insertFront(&head, 10);
    insertFront(&head, 20);
    insertFront(&head, 30);
    printList(head); // Expected: 30 <-> 20 <-> 10 <-> NULL

    // Test Find
    printf("\nSearching for 20...\n");
    Node* target = findNode(head, 20);
    if (target != NULL) {
        printf("Found node with value: %d\n", target->data);
    } else {
        printf("Node not found.\n");
    }

    // Test Deletion
    printf("\nDeleting node 20...\n");
    deleteNode(&head, target);
    printList(head); // Expected: 30 <-> 10 <-> NULL

    // Clean up remaining memory
    while (head != NULL) {
        deleteNode(&head, head);
    }

    return 0;
}