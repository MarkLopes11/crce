import queue
import time

class Order:
    def __init__(self, order_id, item):
        self.order_id = order_id
        self.item = item
        self.status = "Placed"  # Possible statuses: Placed, Preparing, Delivered


def place_order(order_queue, order_id, item):
    order = Order(order_id, item)
    order_queue.put(order)
    print(f"Order {order_id} for {item} has been placed.")

def prepare_orders(order_queue, preparing_queue):
    print("Preparing orders...")
    while not order_queue.empty():
        order = order_queue.get()
        order.status = 'Preparing'
        preparing_queue.put(order)
        print(f"Order {order.order_id} for {order.item} is being prepared.")
        time.sleep(2)  # Simulate preparation time

def deliver_orders(preparing_queue, delivered_orders):
    print("Delivering orders...")
    while not preparing_queue.empty():
        order = preparing_queue.get()
        order.status = 'Delivered'
        delivered_orders.append(order)
        print(f"Order {order.order_id} for {order.item} has been delivered.")

def display_order_status(order_queue, preparing_queue, delivered_orders):
    print("---- Current Order Status ----")
    print("Orders Being Prepared:")
    for order in list(preparing_queue.queue):
        print(f"Order ID: {order.order_id}, Item: {order.item}, Status: {order.status}")
    print("Orders Waiting for Preparation:")
    for order in list(order_queue.queue):
        print(f"Order ID: {order.order_id}, Item: {order.item}, Status: {order.status}")
    print("Delivered Orders:")
    for order in delivered_orders:
        print(f"Order ID: {order.order_id}, Item: {order.item}, Status: {order.status}")
    print("-------------------------------")


if __name__ == "__main__":
    order_queue = queue.Queue()
    preparing_queue = queue.Queue()
    delivered_orders = []

    place_order(order_queue, 1, "Samosa")
    place_order(order_queue, 2, "Vada Pav")
    place_order(order_queue, 3, "Biryani")

    prepare_orders(order_queue, preparing_queue)

    deliver_orders(preparing_queue, delivered_orders)

    display_order_status(order_queue, preparing_queue, delivered_orders)
