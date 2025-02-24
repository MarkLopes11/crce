class Node:
    def __init__(self, item_name, quantity, price):
        self.item_name = item_name
        self.quantity = quantity
        self.price = price
        self.next = None

class ShoppingCart:
    def __init__(self):
        self.head = None  #Initialize the head of the linked list

    def add_item(self, item_name, quantity, price):
        new_item = Node(item_name, quantity, price)
        if not self.head:
            self.head = new_item
        else:
            current = self.head
            while current.next:
                current = current.next
            current.next = new_item

        print(f"{quantity} {item_name}(s) added to the cart.")

    def remove_item(self, item_name):
        if not self.head:
            print("Cart is empty.")
            return

        current = self.head
        prev = None
        found = False

        while current and not found:
            if current.item_name == item_name:
                found = True
            else:
                prev = current
                current = current.next

        if not found:
            print(f"{item_name} not found in the cart.")
            return

        if prev is None:
            self.head = current.next
        else:
            prev.next = current.next

        print(f"{item_name} removed from the cart.")

    def display_items(self):
        if not self.head:
            print("Cart is empty.")
            return

        current = self.head
        print("Items in the cart:")
        while current:
            print(f"{current.quantity} {current.item_name}(s) - ${current.price} each")
            current = current.next

    def calculate_total_amount(self):
        total_amount = 0
        current = self.head
        while current:
            total_amount += current.quantity * current.price
            current = current.next
        return total_amount


if __name__ == "__main__":
    cart = ShoppingCart()

    cart.add_item("Apple", 3, 1.25)
    cart.add_item("Banana", 5, 0.75)
    cart.add_item("Mango", 2, 2.50)

    cart.display_items()

    cart.remove_item("Banana")

    cart.display_items()

    total_amount = cart.calculate_total_amount()
    print(f"Total amount in the cart: ${total_amount}")
