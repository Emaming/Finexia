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
    std::cout << "7. Exit" << std::endl;
}

int main() {
    BankAccount account;

    int choice;
    do {
        printMenu();
        std::cin >> choice;

        switch (choice) {
            case 1: {
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
                        continue;
                }

                auto transaction = std::make_shared<Operation>(1, amount, type, now);
                account.addTransaction(transaction);
                break;
            }
            case 2: {
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
                                continue;
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
                        auto date = std::chrono::system_clock::now(); // Dummy date, replace with actual conversion
                        auto results = account.findOperationByDate(date);
                        account.printOperations(results);
                        break;
                    }
                    default:
                        std::cerr << "Invalid choice!" << std::endl;
                        break;
                }
                break;
            }
            case 3: {
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
                                        continue;
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
                                auto date = std::chrono::system_clock::now(); // Dummy date, replace with actual conversion
                                toCancel = account.findOperationByDate(date);
                                break;
                            }
                            default:
                                std::cerr << "Invalid choice!";
                                std::cerr << "Invalid choice!" << std::endl;
                                continue;
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
                                toCancel = account.findPlannedDate(amount);
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
                                auto date = std::chrono::system_clock::now(); // Dummy date, replace with actual conversion
                                toCancel = account.findNextExecutionDate(date);
                                break;
                            }
                            default:
                                std::cerr << "Invalid choice!" << std::endl;
                                continue;
                        }

                        account.removeScheduledOperation(toCancel);
                        std::cout << "Selected planned transactions have been canceled." << std::endl;
                        break;
                    }
                    default:
                        std::cerr << "Invalid choice!" << std::endl;
                        break;
                }
                break;
            }
            case 4: {
                std::cout << "Transaction History:" << std::endl;
                std::cout << "Operations:" << std::endl;
                auto operations = account.findOperationByAmount(0); // Dummy parameter, adjust if needed
                account.printOperations(operations);
                std::cout << "Planned Transactions:" << std::endl;
                auto planned = account.findPlannedDate(std::chrono::system_clock::now()); // Dummy parameter, adjust if needed
                account.printPlannedTransactions(planned);
                break;
            }
            case 5: {
                std::cout << "Card Operations:" << std::endl;
                account.printCards();
                break;
            }
            case 6: {
                std::cout << "Executing planned transactions..." << std::endl;
                account.executePlannedTransactions();
                std::cout << "Planned transactions executed." << std::endl;
                break;
            }
            case 7:
                std::cout << "Exiting program." << std::endl;
                break;
            default:
                std::cerr << "Invalid choice! Please select a valid option." << std::endl;
                break;
        }

    } while (choice != 7);

    return 0;
}
