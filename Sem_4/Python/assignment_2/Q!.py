from threading import Thread, Lock

class Bank:
    def __init__(self):
        self.accounts = {}
        self.lock = Lock()

    def checkBalance(self, accountNumber):
        with self.lock:
            return self.accounts.get(accountNumber, 0)

    def depositMoney(self, accountNumber, amount):
        with self.lock:
            current_balance = self.accounts.get(accountNumber, 0)
            self.accounts[accountNumber] = current_balance + amount
            print(f"Deposited {amount} into account {accountNumber}")

    def withdrawMoney(self, accountNumber, amount):
        with self.lock:
            current_balance = self.accounts.get(accountNumber, 0)
            if current_balance >= amount:
                self.accounts[accountNumber] = current_balance - amount
                print(f"{amount} withdrawn successfully. Current balance is {self.accounts.get(accountNumber)}")
            else:
                print("Not enough balance. Current balance is ", current_balance)

    def simulateTransaction(self, accountNumber, numberOfTransactionsPerThread):
        deposit_amount = 1000
        withdrawal_amount = 200
        
        for i in range(numberOfTransactionsPerThread):
            self.depositMoney(accountNumber, deposit_amount)
            self.withdrawMoney(accountNumber, withdrawal_amount)

if __name__ == "__main__":
    bank = Bank()
    accountNumber = 395795739474
    numberOfThreads = 5
    numberOfTransactionsPerThread = 2
    threads = []

    for _ in range(numberOfThreads):
        t = Thread(target=bank.simulateTransaction, args=(accountNumber, numberOfTransactionsPerThread))
        threads.append(t)
        t.start()

    for t in threads:
        t.join()

    print("Total current balance: ", bank.checkBalance(accountNumber))
