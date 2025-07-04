#include <iostream>
#include <vector>
#include <string>
#include <ctime>
using namespace std;

class Transaction {
public:
    string type;
    double amount;
    string timestamp;

    Transaction(string t, double a) {
        type = t;
        amount = a;

        // Get current time
        time_t now = time(0);
        timestamp = ctime(&now);
        timestamp.pop_back(); // remove newline
    }

    void display() {
        cout << type << " of ₹" << amount << " on " << timestamp << endl;
    }
};

class Account {
private:
    static int nextAccNo;
public:
    int accountNumber;
    double balance;
    vector<Transaction> history;

    Account() {
        accountNumber = ++nextAccNo;
        balance = 0.0;
    }

    void deposit(double amount) {
        balance += amount;
        history.push_back(Transaction("Deposit", amount));
        cout << "₹" << amount << " deposited successfully.\n";
    }

    bool withdraw(double amount) {
        if (amount <= balance) {
            balance -= amount;
            history.push_back(Transaction("Withdrawal", amount));
            cout << "₹" << amount << " withdrawn successfully.\n";
            return true;
        } else {
            cout << "Insufficient balance.\n";
            return false;
        }
    }

    void transfer(Account &receiver, double amount) {
        if (withdraw(amount)) {
            receiver.deposit(amount);
            history.push_back(Transaction("Transfer to Acc " + to_string(receiver.accountNumber), amount));
            receiver.history.push_back(Transaction("Received from Acc " + to_string(accountNumber), amount));
        }
    }

    void showHistory() {
        cout << "Transaction History for Account " << accountNumber << ":\n";
        for (auto &t : history) {
            t.display();
        }
    }

    void showDetails() {
        cout << "\n--- Account Info ---\n";
        cout << "Account Number: " << accountNumber << endl;
        cout << "Balance: ₹" << balance << endl;
        showHistory();
    }
};
int Account::nextAccNo = 1000;

class Customer {
public:
    string name;
    Account account;

    Customer(string n) {
        name = n;
        cout << "Customer " << name << " created with Account No: " << account.accountNumber << "\n";
    }

    void showCustomerDetails() {
        cout << "Customer Name: " << name << endl;
        account.showDetails();
    }
};

// Main function
int main() {
    vector<Customer> customers;

    int choice;
    while (true) {
        cout << "\n---- Banking System Menu ----\n";
        cout << "1. Create Customer\n2. Deposit\n3. Withdraw\n4. Transfer\n5. View Account Info\n6. Exit\n";
        cout << "Enter your choice: ";
        cin >> choice;

        if (choice == 1) {
            string name;
            cout << "Enter customer name: ";
            cin >> name;
            customers.push_back(Customer(name));
        }
        else if (choice >= 2 && choice <= 5) {
            if (customers.empty()) {
                cout << "No customers available. Create one first.\n";
                continue;
            }

            int accNo;
            cout << "Enter account number: ";
            cin >> accNo;

            Account* selected = nullptr;
            for (auto &cust : customers) {
                if (cust.account.accountNumber == accNo) {
                    selected = &cust.account;
                    break;
                }
            }

            if (!selected) {
                cout << "Account not found!\n";
                continue;
            }

            if (choice == 2) {
                double amount;
                cout << "Enter amount to deposit: ₹";
                cin >> amount;
                selected->deposit(amount);
            } 
            else if (choice == 3) {
                double amount;
                cout << "Enter amount to withdraw: ₹";
                cin >> amount;
                selected->withdraw(amount);
            } 
            else if (choice == 4) {
                int toAcc;
                cout << "Enter receiver's account number: ";
                cin >> toAcc;
                Account* receiver = nullptr;

                for (auto &cust : customers) {
                    if (cust.account.accountNumber == toAcc) {
                        receiver = &cust.account;
                        break;
                    }
                }

                if (!receiver) {
                    cout << "Receiver account not found!\n";
                    continue;
                }

                double amount;
                cout << "Enter amount to transfer: ₹";
                cin >> amount;
                selected->transfer(*receiver, amount);
            } 
            else if (choice == 5) {
                selected->showDetails();
            }
        } 
        else if (choice == 6) {
            cout << "Thank you for using the Banking System.\n";
            break;
        } 
        else {
            cout << "Invalid choice!\n";
        }
    }

    return 0;
}