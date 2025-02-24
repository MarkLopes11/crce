#include <stdio.h>
#include <stdlib.h>
#define SIZE 10


// creating a structure for queue to store the float.
typedef struct
{
  int a[SIZE];
  int front, rear;
} Queue;


// adding elements to the queue
void enqueue(Queue *p, int ele)
{
  // if the queue is full, then
  if (p->rear == SIZE - 1)
  {
    printf("\nQueue Full\n");
    printf("----------------------------------------------------------------");
    return;
  }
  p->rear++;
  p->a[p->rear] = ele;
  // front should only be updated when size is -1 and when elements are dequeued
  if (p->front == -1)
    p->front = 0;
  printf("Element enqueued is %d\n", ele);
  printf("----------------------------------------------------------------");
}


// displays the elements of the queue
void display(Queue q)
{


  printf("\nElements are:\n");
  for (int i = q.front; i <= q.rear; i++)
  {
    printf("\t%d\n", q.a[i]);
  }
  printf("----------------------------------------------------------------");
}


// remove the first element in the queue and returns it
int dequeue(Queue *p)
{
  if (p->front == -1)
  {
    printf("\nQueue Empty");


    return -1;
  }
  int ele = p->a[p->front];
  if (p->front == p->rear)
  {
    p->front = p->rear = -1;
    printf("The queue has been reset as there are no more elements in the queue.\n");
    // to prevent reseting we can put a flag which flip for this condition and then check if the flag is flipped or not for the enqueue and dequeue method.Or we can just ignore this condition as this condition is used to reset the queue
  }
  else
  {
    p->front += 1;
  }
  return ele;
}


// returns the size of the queue
int getSize(Queue *q)
{
  int size = q->rear - q->front + 1;
  printf("\nSize is %d\n", size);
  printf("----------------------------------------------------------------");


  return size;
}


// checks whether the size is empty or not
void isEmpty(Queue *q)
{
  if (q->front == -1)
  {
    printf("\nQueue is empty\n");
  }
  else
  {
    printf("\nNot Empty\n");
  }
  printf("----------------------------------------------------------------");
}


// check whether the queue is full or not
void isFull(Queue *q)
{
  if (q->rear == SIZE - 1)
  {
    printf("\nFull\n");
  }
  else
  {
    printf("\nNot Full\n");
  }
  printf("----------------------------------------------------------------");
}


// start of the main function
int main()
{
  Queue q1;
  q1.front = q1.rear = -1; // initializing an empty queue
  int choice, ele;


  do
  {
    printf("\nMenu:\n");
    printf("1. Enqueue\n");
    printf("2. Dequeue\n");
    printf("3. Display\n");
    printf("4. Check Empty\n");
    printf("5. Check Full\n");
    printf("6. Get Size\n");
    printf("7. Exit\n");
    printf("Enter your choice: ");
    scanf("%d", &choice);
    printf("----------------------------------------------------------------");
    switch (choice)
    {
    case 1:
      printf("\nEnter element to enqueue: ");
      scanf("%d", &ele);
      enqueue(&q1, ele);
      break;
    case 2:
      ele = dequeue(&q1);
      if (ele != -1)
        printf("\nRemoved element is: %d\n", ele);
      printf("----------------------------------------------------------------");
      break;
    case 3:
      display(q1);
      break;
    case 4:
      isEmpty(&q1);
      break;
    case 5:
      isFull(&q1);
      break;
    case 6:
      getSize(&q1);
      break;
    case 7:
      printf("\nExiting program...\n");
      exit(0);
    default:
      printf("\nInvalid choice.\n");
      printf("----------------------------------------------------------------");
    }
  } while (choice != 7);
  return 0;
}
