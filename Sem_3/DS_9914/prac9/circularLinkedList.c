#include <stdio.h>
#include <stdlib.h>

// Node structure
typedef struct Node
{
    int data;
    struct Node *next;
} Node;

// Head structure
typedef struct
{
    Node *start;
} Head;

// Function to create a new node
Node *createNode(int data)
{
    Node *newNode = (Node *)malloc(sizeof(Node));
    newNode->data = data;
    newNode->next = NULL;
    return newNode;
}

// Function to insert a node at the end of the linked list
void insertAtEnd(Head *head, int data)
{
    Node *newNode = createNode(data);
    if (head->start == NULL)
    {
        head->start = newNode;
        newNode->next = head->start;
        return;
    }
    Node *temp = head->start;
    while (temp->next != head->start)
    {
        temp = temp->next;
    }
    temp->next = newNode;
    newNode->next = head->start;
}

// Function to insert a node at the beginning of the linked list
void insertAtBeginning(Head *head, int data)
{
    Node *newNode = createNode(data);
    if (head->start == NULL)
    {
        head->start = newNode;
        newNode->next = head->start;
        return;
    }
    Node *temp = head->start;
    while (temp->next != head->start)
    {
        temp = temp->next;
    }
    temp->next = newNode;
    newNode->next = head->start;
    head->start = newNode;
}

// Function to insert a node after the nth node of the linked list
void insertAfterNthNode(Head *head, int data, int n)
{
    Node *newNode = createNode(data);
    if (head->start == NULL)
    {
        head->start = newNode;
        newNode->next = head->start;
        return;
    }
    Node *temp = head->start;
    for (int i = 1; i < n && temp->next != head->start; i++)
    {
        temp = temp->next;
    }
    newNode->next = temp->next;
    temp->next = newNode;
}

// Function to delete a node from the linked list
void deleteNode(Head *head, int data)
{
    if (head->start == NULL)
    {
        printf("List is empty\n");
        return;
    }
    Node *temp = head->start;
    Node *prev = NULL;
    while (temp->next != head->start && temp->data != data)
    {
        prev = temp;
        temp = temp->next;
    }
    if (temp->data != data)
    {
        printf("Node not found\n");
        return;
    }
    if (temp == head->start)
    {
        head->start = temp->next;
    }
    prev->next = temp->next;
    free(temp);
}

// Function to display the linked list
void display(Head *head)
{
    if (head->start == NULL)
    {
        printf("List is empty\n");
        return;
    }
    Node *temp = head->start;
    printf("Linked list: ");
    do
    {
        printf("%d ", temp->data);
        temp = temp->next;
    } while (temp != head->start);
    printf("\n");
}

// Main function
int main()
{
    Head head;
    head.start = NULL;
    int choice, data, n;
    while (1)
    {
        printf("1. Insert at end\n");
        printf("2. Insert at beginning\n");
        printf("3. Insert after nth node\n");
        printf("4. Delete a node\n");
        printf("5. Display\n");
        printf("6. Exit\n");
        printf("Enter your choice: ");
        scanf("%d", &choice);
        switch (choice)
        {
        case 1:
            printf("Enter data: ");
            scanf("%d", &data);
            insertAtEnd(&head, data);
            break;
        case 2:
            printf("Enter data: ");
            scanf("%d", &data);
            insertAtBeginning(&head, data);
            break;
        case 3:
            printf("Enter data: ");
            scanf("%d", &data);
            printf("Enter position: ");
            scanf("%d", &n);
            insertAfterNthNode(&head, data, n);
            break;
        case 4:
            printf("Enter data: ");
            scanf("%d", &data);
            deleteNode(&head, data);
            break;
        case 5:
            display(&head);
            break;
        case 6:
            exit(0);
        default:
            printf("Invalid choice\n");
        }
    }
    return 0;
}
