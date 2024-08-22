#include <iostream>
#include <memory>
#include <vector>
#include <chrono>
#include <string>
#include <sstream>
#include "BankAccount.h"
#include "Operation.h"
#include "ScheduledOperation.h"
#include "CardOperation.h"

void printMenu() {
    std::cout << "Select an option:" << std::endl;
    std::cout << "1. Add Transaction" << std::endl;
    std::cout << "2. Search Operations" << std::endl;
    std::cout << "3. Cancel Transactions" << std::endl;
    std::cout << "4. Share Transaction History" << std::endl;
    std::cout << "5. Card Operations" << std::endl;
    std::cout << "6. Execute Planned Transactions" << std::endl;
    std::cout << "7. Save to File" << std::endl;
    std::cout << "8. Load from File" << std::endl;
    std::cout << "9. Schedule Transaction" << std::endl;
    std::cout << "10. Print Planned Transactions" << std::endl;
    std::cout << "0. Exit" << std::endl;
}

void handleAddTransaction(BankAccount& account) {
    int transactionType;
    std::cout << "Select transaction type:" << std::endl;
    std::cout << "1. Deposit" << std::endl;
    std::cout << "2. Withdrawal" << std::endl;
    std::cout << "3. Transfer" << std::endl;
    std::cin >> transactionType;

    double amount;
    std::cout << "Enter amount: ";
    std::cin >> amount;

    auto now = std::chrono::system_clock::now();
    OperationType type;
    switch (transactionType) {
        case 1: type = OperationType::Deposit; break;
        case 2: type = OperationType::Withdrawal; break;
        case 3: type = OperationType::Transfer; break;
        default:
            std::cerr << "Invalid choice!" << std::endl;
            return;
    }

    auto transaction = std::make_shared<Operation>(1, amount, type, now);
    account.addTransaction(transaction);
    std::cout << "Transaction added successfully." << std::endl;
}

void handleSearchOperations(BankAccount& account) {
    int searchMethod;
    std::cout << "Select search method:" << std::endl;
    std::cout << "1. Amount" << std::endl;
    std::cout << "2. Type" << std::endl;
    std::cout << "3. Date" << std::endl;
    std::cin >> searchMethod;

    switch (searchMethod) {
        case 1: {
            double amount;
            std::cout << "Enter amount: ";
            std::cin >> amount;
            auto results = account.findOperationByAmount(amount);
            account.printOperations(results);
            break;
        }
        case 2: {
            int typeChoice;
            std::cout << "Select operation type:" << std::endl;
            std::cout << "1. Deposit" << std::endl;
            std::cout << "2. Withdrawal" << std::endl;
            std::cout << "3. Transfer" << std::endl;
            std::cin >> typeChoice;

            OperationType type;
            switch (typeChoice) {
                case 1: type = OperationType::Deposit; break;
                case 2: type = OperationType::Withdrawal; break;
                case 3: type = OperationType::Transfer; break;
                default:
                    std::cerr << "Invalid choice!" << std::endl;
                    return;
            }

            auto results = account.findOperationByType(type);
            account.printOperations(results);
            break;
        }
        case 3: {
            int year, month, day;
            std::cout << "Enter year: ";
            std::cin >> year;
            std::cout << "Enter month (1-12): ";
            std::cin >> month;
            std::cout << "Enter day (1-31): ";
            std::cin >> day;
            std::tm tm = {};
            tm.tm_year = year - 1900;
            tm.tm_mon = month - 1;
            tm.tm_mday = day;
            auto date = std::chrono::system_clock::from_time_t(std::mktime(&tm));
            auto results = account.findOperationByDate(date);
            account.printOperations(results);
            break;
        }
        default:
            std::cerr << "Invalid choice!" << std::endl;
            break;
    }
}

void handleCancelTransactions(BankAccount& account) {
    int transactionType;
    std::cout << "Select transaction type to cancel:" << std::endl;
    std::cout << "1. Regular transaction" << std::endl;
    std::cout << "2. Planned transaction" << std::endl;
    std::cin >> transactionType;

    switch (transactionType) {
        case 1: {
            int searchMethod;
            std::cout << "Select search method:" << std::endl;
            std::cout << "1. Amount" << std::endl;
            std::cout << "2. Type" << std::endl;
            std::cout << "3. Date" << std::endl;
            std::cin >> searchMethod;

            std::vector<std::shared_ptr<Operation>> toCancel;
            switch (searchMethod) {
                case 1: {
                    double amount;
                    std::cout << "Enter amount: ";
                    std::cin >> amount;
                    toCancel = account.findOperationByAmount(amount);
                    break;
                }
                case 2: {
                    int typeChoice;
                    std::cout << "Select operation type:" << std::endl;
                    std::cout << "1. Deposit" << std::endl;
                    std::cout << "2. Withdrawal" << std::endl;
                    std::cout << "3. Transfer" << std::endl;
                    std::cin >> typeChoice;

                    OperationType type;
                    switch (typeChoice) {
                        case 1: type = OperationType::Deposit; break;
                        case 2: type = OperationType::Withdrawal; break;
                        case 3: type = OperationType::Transfer; break;
                        default:
                            std::cerr << "Invalid choice!" << std::endl;
                            return;
                    }

                    toCancel = account.findOperationByType(type);
                    break;
                }
                case 3: {
                    int year, month, day;
                    std::cout << "Enter year: ";
                    std::cin >> year;
                    std::cout << "Enter month (1-12): ";
                    std::cin >> month;
                    std::cout << "Enter day (1-31): ";
                    std::cin >> day;
                    std::tm tm = {};
                    tm.tm_year = year - 1900;
                    tm.tm_mon = month - 1;
                    tm.tm_mday = day;
                    auto date = std::chrono::system_clock::from_time_t(std::mktime(&tm));
                    toCancel = account.findOperationByDate(date);
                    break;
                }
                default:
                    std::cerr << "Invalid choice!" << std::endl;
                    return;
            }

            account.cancelOperations(toCancel);
            std::cout << "Selected transactions have been canceled." << std::endl;
            break;
        }
        case 2: {
            int searchMethod;
            std::cout << "Select search method for planned transactions:" << std::endl;
            std::cout << "1. Amount" << std::endl;
            std::cout << "2. Date" << std::endl;
            std::cin >> searchMethod;

            std::vector<std::shared_ptr<ScheduledOperation>> toCancel;
            switch (searchMethod) {
                case 1: {
                    double amount;
                    std::cout << "Enter amount: ";
                    std::cin >> amount;
                    toCancel = account.findScheduledByAmount(amount);
                    break;
                }
                case 2: {
                    int year, month, day;
                    std::cout << "Enter year: ";
                    std::cin >> year;
                    std::cout << "Enter month (1-12): ";
                    std::cin >> month;
                    std::cout << "Enter day (1-31): ";
                    std::cin >> day;
                    std::tm tm = {};
                    tm.tm_year = year - 1900;
                    tm.tm_mon = month - 1;
                    tm.tm_mday = day;
                    auto date = std::chrono::system_clock::from_time_t(std::mktime(&tm));
                    toCancel = account.findScheduledByDate(date);
                    break;
                }
                default:
                    std::cerr << "Invalid choice!" << std::endl;
                    return;
            }

            account.removeScheduledOperation(toCancel);
            std::cout << "Selected planned transactions have been canceled." << std::endl;
            break;
        }
        default:
            std::cerr << "Invalid choice!" << std::endl;
            break;
    }
}

void handleCardOperations(BankAccount& account) {
    int cardOperationChoice;
    do {
        std::cout << "Select operation to perform:" << std::endl;
        std::cout << "1. Creation of a new Debit card" << std::endl;
        std::cout << "2. Creation of a new Credit card" << std::endl;
        std::cout << "3. Visualize personal cards info" << std::endl;
        std::cout << "4. Exit Card Operations" << std::endl;
        std::cin >> cardOperationChoice;

        switch (cardOperationChoice) {
            case 1: {
                std::string cardName;
                std::cout << "Select a name for the Debit card:" << std::endl;
                std::cin.ignore(); // Ignore leftover newline from previous input
                std::getline(std::cin, cardName);
                account.addCard(cardName, false); // false for Debit card
                std::cout << "Debit card created successfully." << std::endl;
                break;
            }
            case 2: {
                std::string cardName;
                std::cout << "Select a name for the Credit card:" << std::endl;
                std::cin.ignore(); // Ignore leftover newline from previous input
                std::getline(std::cin, cardName);
                account.addCard(cardName, true); // true for Credit card
                std::cout << "Credit card created successfully." << std::endl;
                break;
            }
            case 3: {
                std::cout << "Personal Card Information:" << std::endl;
                account.printCards();
                break;
            }
            case 4:
                std::cout << "Exiting Card Operations." << std::endl;
                break;
            default:
                std::cerr << "Invalid choice!" << std::endl;
                break;
        }
    } while (cardOperationChoice != 4);
}

void handleScheduleTransaction(BankAccount& account) {
    int transactionType;
    std::cout << "Select transaction type for scheduling:" << std::endl;
    std::cout << "1. Deposit" << std::endl;
    std::cout << "2. Withdrawal" << std::endl;
    std::cout << "3. Transfer" << std::endl;
    std::cin >> transactionType;

    double amount;
    std::cout << "Enter amount: ";
    std::cin >> amount;

    int year, month, day;
    std::cout << "Enter start year: ";
    std::cin >> year;
    std::cout << "Enter start month (1-12): ";
    std::cin >> month;
    std::cout << "Enter start day (1-31): ";
    std::cin >> day;
    std::tm tm = {};
    tm.tm_year = year - 1900;
    tm.tm_mon = month - 1;
    tm.tm_mday = day;
    auto startDate = std::chrono::system_clock::from_time_t(std::mktime(&tm));

    int freqChoice;
    std::cout << "Select frequency:" << std::endl;
    std::cout << "1. Daily" << std::endl;
    std::cout << "2. Weekly" << std::endl;
    std::cout << "3. Monthly" << std::endl;
    std::cout << "4. Yearly" << std::endl;
    std::cin >> freqChoice;

    Frequency frequency;
    switch (freqChoice) {
        case 1: frequency = Frequency::Daily; break;
        case 2: frequency = Frequency::Weekly; break;
        case 3: frequency = Frequency::Monthly; break;
        case 4: frequency = Frequency::Yearly; break;
        default:
            std::cerr << "Invalid frequency choice!" << std::endl;
            return;
    }

    OperationType type;
    switch (transactionType) {
        case 1: type = OperationType::Deposit; break;
        case 2: type = OperationType::Withdrawal; break;
        case 3: type = OperationType::Transfer; break;
        default:
            std::cerr << "Invalid transaction type!" << std::endl;
            return;
    }

    auto transaction = std::make_shared<Operation>(1, amount, type, startDate);
    account.scheduleOperation(transaction, startDate, frequency);
    std::cout << "Operation scheduled successfully." << std::endl;
}

int main() {
    BankAccount account;
    int choice;

    do {
        printMenu();
        std::cin >> choice;

        switch (choice) {
            case 1:
                handleAddTransaction(account);
                break;

            case 2:
                handleSearchOperations(account);
                break;

            case 3:
                handleCancelTransactions(account);
                break;

            case 4: {
                std::cout << "Transaction History:" << std::endl;
                std::string history = account.getTransactionHistory(); // Ensure getTransactionHistory is public
                std::cout << history << std::endl;
                break;
            }

            case 5:
                handleCardOperations(account);
                break;

            case 6:
                account.executePlannedTransactions();
                std::cout << "Planned transactions have been executed." << std::endl;
                break;

            case 7: {
                std::string filename;
                std::cout << "Enter filename to save: ";
                std::cin >> filename;
                account.saveToFile(filename);
                std::cout << "Account data saved to " << filename << std::endl;
                break;
            }

            case 8: {
                std::string filename;
                std::cout << "Enter filename to load: ";
                std::cin >> filename;
                account.loadFromFile(filename);
                std::cout << "Account data loaded from " << filename << std::endl;
                break;
            }

            case 9:
                handleScheduleTransaction(account);
                break;
            case 10:
                account.printPlannedTransactions();  // Print planned transactions
                break;

            case 0:
                std::cout << "Exiting the program." << std::endl;
                break;

            default:
                std::cerr << "Invalid choice! Please select a valid option from the menu." << std::endl;
                break;
        }
    } while (choice != 0);

    return 0;
}
