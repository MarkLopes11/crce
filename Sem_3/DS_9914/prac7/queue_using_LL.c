#include <stdio.h>
#include <stdlib.h>

// Node of the linked list
typedef struct node
{
  int data;
  struct node *next;
} Node;

// Queue using LinkedList
typedef struct
{
  Node *front, *rear;
} QueueLL;

// enqueue a node
void enqueue(QueueLL *q, int x)
{
  Node *p;
  p = (Node *)malloc(sizeof(Node)); // memory allocation
  p->data = x;
  p->next = NULL;
  // if it's the first node
  if (q->rear == NULL)
  {
    q->rear = q->front = p; // both front and rear points towards p
  }
  else
  {
    // rear gets incremented
    q->rear->next = p;
    q->rear = p;
  }
}

// dequeue a node from start
void dequeue(QueueLL *q)
{
  Node *p;
  int x;
  if (q->front == NULL)
  {
    printf("Queue empty\n");
    return;
  }
  else
  {
    p = q->front;
    x = p->data;
    q->front = q->front->next;
    if (q->front == NULL) // if only one node exists
    {
      q->rear = NULL;
    }
    free(p); // free the space consumed
    printf("Dequeued element is:%d\n", x);
  }
}

// display the elements of the queue
void display(QueueLL l)
{
  Node *p;
  printf("The linked list is:\n");
  for (p = l.front; p != NULL; p = p->next)
  {
    printf("%d\n", p->data);
  }
}

// main function
int main()
{
  QueueLL l;
  l.front = NULL;
  l.rear = NULL;
  int option, x;

  while (1)
  {
    printf("Enter: 1.Enqueue  2.Dequeue  3.Display  4.Exit :\t");
    scanf("%d", &option);
    switch (option)
    {
    case 1:
      printf("Enter element to be enqueued :\t");
      scanf("%d", &x);
      enqueue(&l, x);
      break;
    case 2:
      dequeue(&l);
      break;
    case 3:
      display(l);
      break;
    case 4:
      exit(0);
    default:
      printf("Enter valid option\n");
    }
  }
  return 0;
}
