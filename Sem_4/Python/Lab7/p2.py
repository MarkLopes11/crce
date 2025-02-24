from collections import deque

class Node:
    def __init__(self, data):
        self.data = data
        self.next = None

class LinkedList:
    def __init__(self):
        self.head = None
        self.deque = deque()

    def append(self, data):
        new_node = Node(data)
        if not self.head:
            self.head = new_node
        else:
            current = self.head
            while current.next:
                current = current.next
            current.next = new_node
        self.deque.append(new_node)

    def insert_after(self, prev_node_data, data):
        new_node = Node(data)
        current = self.head
        while current:
            if current.data == prev_node_data:
                new_node.next = current.next
                current.next = new_node
                self.deque.insert(self.deque.index(current) + 1, new_node)
                break
            current = current.next
        else:
            raise ValueError(f"Node with data '{prev_node_data}' not found")

    def remove(self, data):
        current = self.head
        prev = None
        while current:
            if current.data == data:
                if prev:
                    prev.next = current.next
                else:
                    self.head = current.next
                self.deque.remove(current)
                return
            prev = current
            current = current.next
        raise ValueError(f"Node with data '{data}' not found")

    def traverse(self):
        current = self.head
        while current:
            print(current.data, end=" ")
            current = current.next


if __name__ == "__main__":
    linked_list = LinkedList()

    linked_list.append(1)
    linked_list.append(2)
    linked_list.append(3)

    print("Linked List:")
    linked_list.traverse()

    linked_list.insert_after(3, 4)

    print("\nLinked List after insertion:")
    linked_list.traverse()

    linked_list.remove(2)

    print("\nLinked List after removal:")
    linked_list.traverse()

  
