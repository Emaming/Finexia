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
    std::cout << "6. Save to File" << std::endl;
    std::cout << "7. Load from File" << std::endl;
    std::cout << "8. Schedule Transaction" << std::endl;
    std::cout << "9. Print Planned Transactions" << std::endl;
    std::cout << "0. Exit" << std::endl;
}

void handleAddTransaction(BankAccount& account) {
    int transactionType;
    do {
        std::cout << "Select transaction type (or 0 to return to main menu):" << std::endl;
        std::cout << "1. Deposit" << std::endl;
        std::cout << "2. Withdrawal" << std::endl;
        std::cout << "3. Transfer" << std::endl;
        std::cout << "0. Return to main menu" << std::endl;
        std::cin >> transactionType;

        if (transactionType == 0) return;  // Esce dalla funzione se l'utente sceglie 0

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
                continue;  // Torna al menu di selezione del tipo di transazione
        }

        auto transaction = std::make_shared<Operation>(1, amount, type, now);
        account.addTransaction(transaction);
        std::cout << "Transaction added successfully." << std::endl;
        break;
    } while (true);
}

void handleSearchOperations(BankAccount& account) {
    int searchMethod;
    do {
        std::cout << "Select search method (or 0 to return to main menu):" << std::endl;
        std::cout << "1. Amount" << std::endl;
        std::cout << "2. Type" << std::endl;
        std::cout << "3. Date" << std::endl;
        std::cout << "0. Return to main menu" << std::endl;
        std::cin >> searchMethod;

        if (searchMethod == 0) return;

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
                        continue;  // Torna al menu di selezione del tipo di operazione
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
                continue;  // Torna al menu di selezione del metodo di ricerca
        }
        break;
    } while (true);
}

void handleCancelTransactions(BankAccount& account) {
    int transactionType;
    do {
        std::cout << "Select transaction type to cancel (or 0 to return to main menu):" << std::endl;
        std::cout << "1. Regular transaction" << std::endl;
        std::cout << "2. Planned transaction" << std::endl;
        std::cout << "0. Return to main menu" << std::endl;
        std::cin >> transactionType;

        if (transactionType == 0) return;

        switch (transactionType) {
            case 1: {
                int searchMethod;
                do {
                    std::cout << "Select search method:" << std::endl;
                    std::cout << "1. Amount" << std::endl;
                    std::cout << "2. Type" << std::endl;
                    std::cout << "3. Date" << std::endl;
                    std::cout << "0. Return to previous menu" << std::endl;
                    std::cin >> searchMethod;

                    if (searchMethod == 0) break;

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
                                    continue;  // Torna al menu di selezione del tipo di operazione
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
                            continue;  // Torna al menu di selezione del metodo di ricerca
                    }

                    account.cancelOperations(toCancel);
                    std::cout << "Selected transactions have been canceled." << std::endl;
                    break;
                } while (true);
                break;
            }
            case 2: {
                int searchMethod;
                do {
                    std::cout << "Select search method for planned transactions:" << std::endl;
                    std::cout << "1. Amount" << std::endl;
                    std::cout << "2. Date" << std::endl;
                    std::cout << "0. Return to previous menu" << std::endl;
                    std::cin >> searchMethod;

                    if (searchMethod == 0) break;

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
                            continue;  // Torna al menu di selezione del metodo di ricerca
                    }


                    std::cout << "Selected planned transactions have been canceled." << std::endl;
                    break;
                } while (true);
                break;
            }
            default:
                std::cerr << "Invalid choice!" << std::endl;
                continue;  // Torna al menu principale
        }
        break;
    } while (true);
}

void handleShareTransactionHistory(BankAccount& account) {
    std::string history = account.getTransactionHistory();
    std::cout << "Transaction History:" << std::endl;
    std::cout << history << std::endl;
}

void handleCardOperations(BankAccount& account) {
    int cardOption;
    do {
        std::cout << "Select card operation (or 0 to return to main menu):" << std::endl;
        std::cout << "1. Add Card" << std::endl;
        std::cout << "2. Print Cards" << std::endl;
        std::cin >> cardOption;

        if (cardOption == 0) return;

        switch (cardOption) {
            case 1: {
                std::string cardName;
                bool isCreditCard;
                std::cout << "Enter card name: ";
                std::cin.ignore();  // Ignora il newline residuo
                std::getline(std::cin, cardName);
                std::cout << "Is it a credit card? (1 for yes, 0 for no): ";
                std::cin >> isCreditCard;
                account.addCard(cardName, isCreditCard);
                std::cout << "Card added successfully." << std::endl;
                break;
            }
            case 2:
                account.printCards();
                break;
            default:
                std::cerr << "Invalid choice!" << std::endl;
                continue;  // Torna al menu di operazioni delle carte
        }
        break;
    } while (true);
}

void handleSaveToFile(BankAccount& account) {
    std::string filename;
    std::cout << "Enter filename to save: ";
    std::cin >> filename;
    account.saveToFile(filename);
    std::cout << "Data saved successfully." << std::endl;
}

void handleLoadFromFile(BankAccount& account) {
    std::string filename;
    std::cout << "Enter filename to load: ";
    std::cin >> filename;
    account.loadFromFile(filename);
    std::cout << "Data loaded successfully." << std::endl;
}

void handleScheduleTransaction(BankAccount& account) {
    double amount;
    int transactionType;
    int freqChoice;
    int startYear, startMonth, startDay;

    while (true) {
        std::cout << "Select transaction type:" << std::endl;
        std::cout << "1. Deposit" << std::endl;
        std::cout << "2. Withdrawal" << std::endl;
        std::cout << "3. Transfer" << std::endl;

        std::cin >> transactionType;
        std::cout << "Enter amount: ";
        std::cin >> amount;

        OperationType type;
        switch (transactionType) {
            case 1: type = OperationType::Deposit; break;
            case 2: type = OperationType::Withdrawal; break;
            case 3: type = OperationType::Transfer; break;
            default:
                std::cerr << "Invalid choice!" << std::endl;
                return;
        }

        std::cout << "Enter start date (year month day): ";
        std::cout << "Enter start year: ";
        std::cin >> startYear;
        std::cout << "Enter start month (1-12): ";
        std::cin >> startMonth;
        std::cout << "Enter start day (1-31): ";
        std::cin >> startDay;

        std::tm tm = {};
        tm.tm_year = startYear - 1900;
        tm.tm_mon = startMonth - 1;
        tm.tm_mday = startDay;
        auto startDate = std::chrono::system_clock::from_time_t(std::mktime(&tm));

        // Get the current date
        auto now = std::chrono::system_clock::now();
        auto nowTimeT = std::chrono::system_clock::to_time_t(now);
        std::tm nowTm = *std::localtime(&nowTimeT);
        std::tm today = {};
        today.tm_year = nowTm.tm_year;
        today.tm_mon = nowTm.tm_mon;
        today.tm_mday = nowTm.tm_mday;
        auto todayDate = std::chrono::system_clock::from_time_t(std::mktime(&today));

        if (startDate < todayDate) {
            std::cout << "Invalid date: Start date cannot be in the past." << std::endl;
            continue; // Permetti all'utente di reinserire la data
        }

        std::cout << "Select frequency:" << std::endl;
        std::cout << "1. Daily" << std::endl;
        std::cout << "2. Weekly" << std::endl;
        std::cout << "3. Monthly" << std::endl;
        std::cout << "4. Yearly" << std::endl;
        std::cin >> freqChoice;

        Frequency freq;
        switch (freqChoice) {
            case 1: freq = Frequency::Daily; break;
            case 2: freq = Frequency::Weekly; break;
            case 3: freq = Frequency::Monthly; break;
            case 4: freq = Frequency::Yearly; break;
            default:
                std::cerr << "Invalid choice!" << std::endl;
                return;
        }

        auto transaction = std::make_shared<Operation>(1, amount, type, startDate);
        account.scheduleOperation(transaction, startDate, freq);
        std::cout << "Transaction scheduled successfully." << std::endl;
        break; // Esci dal ciclo se la data è valida e l'operazione è programmata
    }
}



void handlePrintPlannedTransactions(BankAccount& account) {
    account.printPlannedTransactions();
}

int main() {
    BankAccount account;
    int choice;

    do {
        printMenu();
        std::cin >> choice;

        switch (choice) {
            case 1: handleAddTransaction(account); break;
            case 2: handleSearchOperations(account); break;
            case 3: handleCancelTransactions(account); break;
            case 4: handleShareTransactionHistory(account); break;
            case 5: handleCardOperations(account); break;
            case 6: handleSaveToFile(account); break;
            case 7: handleLoadFromFile(account); break;
            case 8: handleScheduleTransaction(account); break;
            case 9: handlePrintPlannedTransactions(account); break;
            case 0: std::cout << "Exiting program." << std::endl; break;
            default: std::cerr << "Invalid choice! Please try again." << std::endl; break;
        }
    } while (choice != 0);

    return 0;
}

