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

        // Save account to file
        void saveAccountsToFile(ofstream &ofs) const {
            ofs << ownerName << endl;
            ofs << fixed << setprecision(2) << balance << endl;
            ofs << transactionHistory.size() << endl;
            for (const auto &t : transactionHistory) {
                ofs << t << endl;
            }
        }

        // Load account from file stream
        static Account loadAccountFromFile(ifstream &ifs) {
            string name;
            if (!getline(ifs, name)) return Account();

            string balanceLine;
            getline(ifs, balanceLine);
            double bal = stod(balanceLine);

            string countLine;
            getline(ifs, countLine);
            size_t count = stoul(countLine);

            Account acc(name, 0.0);
            acc.balance = bal;
            acc.transactionHistory.clear();
            for (size_t i = 0; i < count; ++i) {
                string t;
                getline(ifs, t);
                acc.transactionHistory.push_back(t);
            }
            return acc;
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
        cout << "2. List Accounts" << endl;
        cout << "3. Select Account" << endl;
        cout << "4. Save Accounts to File (accounts.txt)" << endl;
        cout << "5. Load Accounts from File (accounts.txt)" << endl;
        cout << "6. Exit" << endl;
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
                if (accounts.empty()) {
                    cout << "No accounts created yet." << endl;
                } else {
                    cout << "Accounts: " << endl;
                    for (size_t i = 0; i < accounts.size(); ++i) {
                        cout << i << ". " << accounts[i].getOwnerName()
                             << " - $" << fixed << setprecision(2) << accounts[i].getBalance() << endl;
                    }
                }
                break;
            }

            case 3: {
                if (accounts.empty()) {
                    cout << "No accounts available. Create an account first." << endl;
                    break;
                }
                cout << "Enter account index to select: ";
                size_t index;
                if (!(cin >> index)) { cout << "Invalid input." << endl; clearInput(); break; }
                if (index >= accounts.size()) {
                    cout << "Index out of range." << endl; break; }

                bool inAccountMenu = true;
                while (inAccountMenu) {
                    cout << "\n--- Account Menu for " << accounts[index].getOwnerName() << " ---" << endl;
                    cout << "1. Deposit" << endl;
                    cout << "2. Withdraw" << endl;
                    cout << "3. View Balance" << endl;
                    cout << "4. View Transaction History" << endl;
                    cout << "5. Back to Main Menu" << endl;
                    cout << "Select an option: ";

                    int sub;
                    if (!(cin >> sub)) { cout << "Invalid input." << endl; clearInput(); continue; }

                    switch (sub) {
                        case 1: {
                            cout << "Enter deposit amount: ";
                            double amt; if (!(cin >> amt)) { cout << "Invalid amount." << endl; clearInput(); break; }
                            accounts[index].deposit(amt);
                            break;
                        }

                        case 2: {
                            cout << "Enter withdrawal amount: ";
                            double amt; if (!(cin >> amt)) { cout << "Invalid amount." << endl; clearInput(); break; }
                            accounts[index].withdraw(amt);
                            break;
                        }

                        case 3:
                            accounts[index].displayAccount();
                            break;

                        case 4:
                            accounts[index].showTransactionHistory();
                            break;
                        
                        case 5:
                            inAccountMenu = false;
                            break;

                        default:
                            cout << "Select a valid option." << endl;
                    }
                }
                break;

                }

                case 4: {
                    ofstream ofs("accounts.txt");
                    if (!ofs) {
                        cout << "Failed to open accounts.txt for writing." << endl;
                        break;
                    }
                    ofs << accounts.size() << endl;
                    for (const auto &acc : accounts) acc.saveAccountsToFile(ofs);
                    ofs.close();
                    cout << "Accounts saved to accounts.txt" << endl;
                    break;
                }

                    case 5: {
                        ifstream ifs("accounts.txt");
                        if (!ifs) { cout << "Failed to open accounts.txt for writing." << endl; break; }
                        string countLine;
                        if (!getline(ifs, countLine)) { cout << "File empty or corrupted." << endl; break; }
                        size_t n = stoul(countLine);
                        accounts.clear();
                        for (size_t i = 0; i < n; ++i) {
                            Account acc = Account::loadAccountFromFile(ifs);
                            accounts.push_back(acc);
                        }
                        cout << "Loaded " << accounts.size() << " accounts from accounts.txt" << endl;
                        break;
                    }

                    case 6:
                        running = false;
                        cout << "Exiting program. Farewell!" << endl;
                        break;

                    default:
                        cout << "Select a valid option." << endl;
                } // end switch(choice)
        } // end while(running)
    return 0;
} // end main