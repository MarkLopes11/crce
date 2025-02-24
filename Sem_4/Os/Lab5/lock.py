import time
from multiprocessing import Process, Lock, Value

def deposit_with_lock(balance, lock, amount):
    for i in range(100):
        time.sleep(0.01)
        lock.acquire()
        balance.value += round(amount / 100)
        lock.release()

def withdraw_with_lock(balance, lock, amount):
    for i in range(100):
        time.sleep(0.01)
        lock.acquire()
        balance.value -= round(amount / 100)
        lock.release()

if __name__ == '__main__':
    initial_balance = int(input("Enter initial balance: "))
    balance = Value('i', initial_balance)
    lock = Lock()

    deposit_amount = int(input("Enter the deposit amount: "))
    withdraw_amount = int(input("Enter the withdrawal amount: "))

    # Transaction with locks
    print("\nTransaction with locks:")
    deposit_proc_with_lock = Process(target=deposit_with_lock, args=(balance, lock, deposit_amount))
    withdraw_proc_with_lock = Process(target=withdraw_with_lock, args=(balance, lock, withdraw_amount))
    deposit_proc_with_lock.start()
    withdraw_proc_with_lock.start()
    deposit_proc_with_lock.join()
    withdraw_proc_with_lock.join() 

    print("Final balance with locks:", balance.value)