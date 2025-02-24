class Node:
    def __init__(self, data):
        self.data = data
        self.prev = None
        self.next = None

class DoublyLinkedList:
    def __init__(self):
        self.head = None
        self.tail = None

    def append(self, data):
        new_node = Node(data)
        if not self.head: # if there is no node in the linked list
            self.head = new_node # the new node is the first node
        else:
            self.tail.next = new_node  
            new_node.prev = self.tail
        self.tail = new_node

    def insert_after(self, existing_node_data, data):
        new_node = Node(data)
        current = self.head

        while current:
            if current.data == existing_node_data:
                new_node.next = current.next
                new_node.prev = current
                if current.next:
                    current.next.prev = new_node
                current.next = new_node
                if current == self.tail:
                    self.tail = new_node
                break
            current = current.next

    def traverse(self):
        current = self.head
        while current:
            print(current.data, end=" ")
            current = current.next
        print()


if __name__ == "__main__":
    dll = DoublyLinkedList()

    dll.append(1)
    dll.append(2)
    dll.append(3)
    dll.append(4)
    dll.append(5)

    print("Doubly linked list: ")
    dll.traverse()

    dll.insert_after(3, 6)

    print("Doubly linked list after insertion: ")
    dll.traverse()

