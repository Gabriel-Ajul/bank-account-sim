#include <iostream>
#include <string>
#include <vector>
#include <iomanip>
#include <limits>
#include <fstream>
#include <sstream>

using namespace std;

class Account {
private:
    string ownerName;
    double balance;
    vector <string> transactionHistory;

    //Money string formatter
    static string money(double amount) {
        ostringstream oss;
        oss << fixed << setprecision(2) << amount;
        return oss.str();
    }

    public:
        Account (const string &name = "Unnamed", double initialBalance = 0.0)
            : ownerName(name), balance(0.0) {
                if (initialBalance > 0.0) {
                    balance = initialBalance;
                    transactionHistory.push_back("Initial deposit: $" + money(initialBalance));
                } else {
                    transactionHistory.push_back("Account opened with $0.00");
                }
            }
        void deposit(double amount) {
            if (amount <= 0.0) {
                cout << "Deposit amount must be greater than 0." << endl;
                return;
            }
            balance += amount;
            transactionHistory.push_back("Deposited: $" + money(amount));
            cout << "Deposited $" << money(amount) << " to " << ownerName << "'s account." << endl;
        }

        bool withdraw(double amount) {
            if (amount <= 0.0) {
                cout << "Withdrawal amount must be greater than 0." << endl;
                return false;
            }
            if (amount > balance) {
                cout << "Insufficient funds. Current balance: $" << fixed << setprecision(2) << balance << endl;
                return false;
            }
            balance -= amount;
            transactionHistory.push_back("Withdrawal: $" + money(amount));
            cout << "Withdrew $" << money(amount) << " from " << ownerName << "'s account." << endl;
            return true;
        }

        double getBalance() const {
            return balance;
        }

        string getOwnerName() const {
            return ownerName;
        }

        void displayAccount() const {
            cout << "Owner: " << ownerName << endl;
            cout << "Balance: $" << fixed << setprecision(2) << balance << endl;
        }

        void showTransactionHistory() const {
            cout << "Transaction History for " << ownerName << ":" << endl;
            for (const auto &t : transactionHistory) {
                cout << " - " << t << endl;
            }
        }
    };

//Pause and clear invalid input
    void clearInput() {
    cin.clear();
    cin.ignore(numeric_limits<streamsize>::max(), '\n');
}

int main() {
    vector<Account> accounts;
    bool running = true;
    int choice;

    while (running) {
        cout << "\n--- Bank Account Simulation Menu ---" << endl;
        cout << "1. Create Account" << endl;
        cout << "2. Exit" << endl;
        cout << "Select an option: ";

    if (!(cin >> choice)) {
        cout << "Invalid input. Please enter a number." << endl;
        clearInput();
        continue;
    }

    switch (choice) {
        case 1: {
            clearInput();
            cout << "Enter owner name: ";
            string name;
            getline(cin, name);
            cout << "Initial deposit (or 0 for none): ";
            double init;
            if (!(cin >> init)) { cout << "Invalid amount." << endl; clearInput(); break; }
            accounts.emplace_back(name, init);
            cout << "Account created for " << name << " with balance: $" << init << endl;
            break;
        }
        
        case 2: {
            running = false;
            cout << "Exiting program. Farewell!" << endl;
            break;
        }

        default:
            cout << "Select a valid option." << endl;
    }
    }
}