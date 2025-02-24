import time
from multiprocessing import Process, Lock, Value

def deposit_without_lock(balance, amount):
    for i in range(100):
        time.sleep(0.01)
        balance.value += round(amount / 100)

def withdraw_without_lock(balance, amount):
    for i in range(100):
        time.sleep(0.01)
        balance.value -= round(amount / 100)

if __name__ == '__main__':
    initial_balance = int(input("Enter initial balance: "))
    balance = Value('i', initial_balance)
    lock = Lock()

    deposit_amount = int(input("Enter the deposit amount: "))
    withdraw_amount = int(input("Enter the withdrawal amount: "))

    # Transaction without locks
    print("\nTransaction without locks:")
    deposit_proc_without_lock = Process(target=deposit_without_lock, args=(balance, deposit_amount))
    withdraw_proc_without_lock = Process(target=withdraw_without_lock, args=(balance, withdraw_amount))
    deposit_proc_without_lock.start()
    withdraw_proc_without_lock.start()
    deposit_proc_without_lock.join()
    withdraw_proc_without_lock.join() 

    print("Final balance without locks:", balance.value)