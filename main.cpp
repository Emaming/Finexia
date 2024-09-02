#include <iostream>
#include <memory>
#include <vector>
#include <chrono>
#include <string>
#include "BankAccount.h"
#include "Operation.h"
#include "ScheduledOperation.h"


void printMenu() {
    std::cout << "Select an option:" << std::endl;
    std::cout << "---- Non-scheduled Transactions ----" << std::endl;
    std::cout << "1. Add Transaction" << std::endl;
    std::cout << "2. Search Transactions" << std::endl;
    std::cout << "3. Cancel Transactions" << std::endl;
    std::cout << "4. Print Transaction History" << std::endl;
    std::cout << "5. Card Operations" << std::endl;

    std::cout << "---- Scheduled Transactions ----" << std::endl;
    std::cout << "6. Schedule Transaction" << std::endl;
    std::cout << "7. Print Planned Transactions History" << std::endl;
    std::cout << "8. Cancel Planned Transactions" << std::endl;


    std::cout << "9. Save to File" << std::endl;
    std::cout << "10. Load from File" << std::endl;
    std::cout << "11. Print Balance" << std::endl;
    std::cout << "0. Exit" << std::endl;
}


void handleAddTransaction(BankAccount& account) {
    int transactionType;
    do {
        std::cout << "Select transaction type" << std::endl;
        std::cout << "1. Deposit" << std::endl;
        std::cout << "2. Withdrawal" << std::endl;
        std::cout << "3. Transfer" << std::endl;
        std::cout << "0. Return to main menu" << std::endl;
        std::cin >> transactionType;

        switch (transactionType) {
            case 0:
                return;

            case 1:  // Deposit
            case 2:  // Withdrawal
            case 3:  // Transfer
                break;

            default:
                std::cerr << "Invalid choice!" << std::endl;
                continue;
        }

        double amount;
        std::cout << "Enter amount (or 0 to return to previous menu): " << std::endl;
        std::cin >> amount;

        switch (static_cast<int>(amount > 0 ? 1 : (amount < 0 ? -1 : 0))) {
            case 0:
                return;

            case -1:
                std::cerr << "Error: The amount cannot be negative. Please enter a valid amount." << std::endl;
                continue;

            case 1:
                break;
        }

        auto now = std::chrono::system_clock::now();
        OperationType type;
        switch (transactionType) {
            case 1: type = OperationType::Deposit; break;
            case 2: type = OperationType::Withdrawal; break;
            case 3: type = OperationType::Transfer; break;
        }

        auto transaction = std::make_shared<Operation>(amount, type, now);
        account.addTransaction(transaction);
        std::cout << "Transaction added successfully." << std::endl;
        break;
    } while (true);
}

void handleSearchOperations(BankAccount& account) {
    int searchMethod;
    do {
        std::cout << "Select search method" << std::endl;
        std::cout << "1. Amount" << std::endl;
        std::cout << "2. Type" << std::endl;
        std::cout << "3. Date" << std::endl;
        std::cout << "0. Return to main menu" << std::endl;
        std::cin >> searchMethod;

        if (searchMethod == 0) return;

        switch (searchMethod) {
            case 1: {
                double amount;
                std::cout << "Enter amount (or 0 to return to previous menu): ";

                std::cin >> amount;
                if (amount == 0) return;


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
                std::cout << "0. Return to main menu" << std::endl;
                std::cin >> typeChoice;
                if (typeChoice == 0) return;

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
                std::cout << "Enter year (or 0 to return to previous menu): ";
                std::cin >> year;
                if(year == 0) return;
                std::cout << "Enter month (1-12) (or 0 to return to previous menu): ";
                std::cin >> month;
                if (month == 0) return;
                std::cout << "Enter day (1-31) (or 0 to return to previous menu): ";
                std::cin >> day;
                if (day == 0) return;
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

void handleCancelRegularTransactions(BankAccount& account) {
    int searchMethod;
    std::cout << "Select search method for regular transactions:" << std::endl;
    std::cout << "1. Amount" << std::endl;
    std::cout << "2. Type" << std::endl;
    std::cout << "3. Date" << std::endl;
    std::cout << "0. Return to main menu" << std::endl;
    std::cin >> searchMethod;

    std::vector<std::shared_ptr<Operation>> toCancel;

    switch (searchMethod) {
        case 0: return;
        case 1: {
            double amount;
            std::cout << "Enter amount (or 0 to return to previous menu): ";
            std::cin >> amount;
            if (amount == 0) return;
            toCancel = account.findOperationByAmount(amount);
            break;
        }
        case 2: {
            int typeChoice;
            std::cout << "Select operation type:" << std::endl;
            std::cout << "1. Deposit" << std::endl;
            std::cout << "2. Withdrawal" << std::endl;
            std::cout << "3. Transfer" << std::endl;
            std::cout << "0. Return to main menu" << std::endl;
            std::cin >> typeChoice;
            if (typeChoice == 0) return;

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
            std::cout << "Enter year (or 0 to return to previous menu): ";
            std::cin >> year;
            if (year == 0) return;
            std::cout << "Enter month (1-12) (or 0 to return to previous menu): ";
            std::cin >> month;
            if (month == 0) return;
            std::cout << "Enter day (1-31) (or 0 to return to previous menu): ";
            std::cin >> day;
            if (day == 0) return;
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

    if (toCancel.empty()) {
        std::cout << "No operations found for the given criteria." << std::endl;
        return;
    }

    account.cancelOperations(toCancel);
    std::cout << "Selected regular transactions have been canceled." << std::endl;
}

void handleCancelScheduledTransactions(BankAccount& account) {
    int searchMethod;
    std::cout << "Select search method for planned transactions:" << std::endl;
    std::cout << "1. Amount" << std::endl;
    std::cout << "2. Type" << std::endl;
    std::cout << "3. Date" << std::endl;
    std::cout << "0. Return to main menu" << std::endl;
    std::cin >> searchMethod;

    std::vector<std::shared_ptr<ScheduledOperation>> toCancel;

    switch (searchMethod) {
        case 0: return;
        case 1: {
            double amount;
            std::cout << "Enter amount (or 0 to return to previous menu): ";
            std::cin >> amount;
            if (amount == 0) return;
            toCancel = account.findScheduledByAmount(amount);
            break;
        }
        case 2: {
            int typeChoice;
            std::cout << "Select operation type:" << std::endl;
            std::cout << "1. Deposit" << std::endl;
            std::cout << "2. Withdrawal" << std::endl;
            std::cout << "3. Transfer" << std::endl;
            std::cout << "0. Return to main menu" << std::endl;
            std::cin >> typeChoice;
            if (typeChoice == 0) return;

            OperationType type;
            switch (typeChoice) {
                case 1: type = OperationType::Deposit; break;
                case 2: type = OperationType::Withdrawal; break;
                case 3: type = OperationType::Transfer; break;
                default:
                    std::cerr << "Invalid choice!" << std::endl;
                    return;
            }
            toCancel = account.findScheduledByType(type);
            break;
        }
        case 3: {
            int year, month, day;
            std::cout << "Enter year (or 0 to return to previous menu): ";
            std::cin >> year;
            if (year == 0) return;
            std::cout << "Enter month (1-12) (or 0 to return to previous menu): ";
            std::cin >> month;
            if (month == 0) return;
            std::cout << "Enter day (1-31) (or 0 to return to previous menu): ";
            std::cin >> day;
            if (day == 0) return;
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

    if (toCancel.empty()) {
        std::cout << "No scheduled operations found for the given criteria." << std::endl;
        return;
    }

    account.removeScheduledOperation(toCancel);
    std::cout << "Selected planned transactions have been canceled." << std::endl;
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
        std::cout << "3. Remove Card" << std::endl;
        std::cout << "4. Add Operation" << std::endl;
        std::cout << "5. Print Card Operations" << std::endl;
        std::cout << "0. Return to previous menu" << std::endl;
        std::cin >> cardOption;

        if (cardOption == 0) break;

        switch (cardOption) {
            case 1: {
                std::string cardName;
                bool isCreditCard;

                std::cin.ignore();  // Ignora il newline residuo
                std::cout << "Enter card name (or 0 to return to previous menu): ";
                std::getline(std::cin, cardName);

                if (cardName == "0") {
                    std::cout << "Returning to previous menu." << std::endl;
                    break;
                }

                if (cardName.empty()) {
                    std::cerr << "Card name cannot be empty. Please enter a valid card name." << std::endl;
                    continue;
                }

                std::cout << "Is it a credit card? (1 for yes, 0 for no): ";
                std::cin >> isCreditCard;
                std::cin.ignore(std::numeric_limits<std::streamsize>::max(), '\n');

                account.addCard(cardName, isCreditCard);
                std::cout << "Card added successfully." << std::endl;
                break;
            }

            case 2:
                account.printCards();
                break;

            case 3: {
                std::string cardName;
                std::cin.ignore();  // Ignora il newline residuo
                std::cout << "Enter card name to remove: ";
                std::getline(std::cin, cardName);
                account.removeCard(cardName);
                std::cout << "Card removed successfully." << std::endl;
                break;
            }

            case 4: {
                std::string cardName;
                double amount;
                std::string description;

                std::cin.ignore();  // Ignora il newline residuo
                std::cout << "Enter card name to associate operation: ";
                std::getline(std::cin, cardName);

                if (cardName.empty()) {
                    std::cerr << "Card name cannot be empty." << std::endl;
                    continue;
                }

                std::cout << "Enter operation amount: ";
                std::cin >> amount;

                std::cin.ignore();  // Ignora il newline residuo
                std::cout << "Enter operation description: ";
                std::getline(std::cin, description);

                if (description.empty()) {
                    std::cerr << "Operation description cannot be empty." << std::endl;
                    continue;
                }

                auto operation = std::make_shared<Operation>(amount, OperationType::Deposit, description);

                account.addOperationToCard(cardName, operation);
                std::cout << "Operation added to card successfully." << std::endl;
                break;
            }

            case 5: {
                std::string cardName;
                std::cin.ignore();  // Ignora il newline residuo
                std::cout << "Enter card name to print operations: ";
                std::getline(std::cin, cardName);
                account.printCardOperations(cardName);
                break;
            }

            default:
                std::cerr << "Invalid choice!" << std::endl;
                continue;
        }
    } while (true);
}

void handleSaveToFile(BankAccount& account) {
    std::string filename;
    std::cout << "Enter filename to save (or 0 to return to previous menu): ";
    std::cin >> filename;

    if (filename == "0") {
        return;  // Torna al menu precedente
    }

    account.saveToFile(filename);
    std::cout << "Data saved successfully." << std::endl;
}

void handleLoadFromFile(BankAccount& account) {
    std::string filename;
    std::cout << "Enter filename to load (or 0 to return to previous menu): ";
    std::cin >> filename;

    if (filename == "0") {
        return;  // Torna al menu precedente
    }

    account.loadFromFile(filename);
    std::cout << "Data loaded successfully." << std::endl;
}

void handleScheduleTransaction(BankAccount& account) {
    double amount;
    int transactionType;
    int freqChoice;
    int startYear, startMonth, startDay;

    while (true) {
        std::cout << "Select transaction type (or 0 to return to main menu):" << std::endl;
        std::cout << "1. Deposit" << std::endl;
        std::cout << "2. Withdrawal" << std::endl;
        std::cout << "3. Transfer" << std::endl;
        std::cout << "0. Return to main menu" << std::endl;

        std::cin >> transactionType;
        switch (transactionType) {
            case 0:
                return;  // Ritorna al menu principale

            case 1:  // Deposit
            case 2:  // Withdrawal
            case 3:  // Transfer
                break;  // Prosegue con l'inserimento dell'importo

            default:
                std::cerr << "Invalid choice!" << std::endl;
                continue;  // Torna al menu di selezione del tipo di transazione
        }

        std::cout << "Enter amount: ";
        std::cout << "0. Return to main menu" << std::endl;
        std::cin >> amount;

        switch (static_cast<int>(amount > 0 ? 1 : (amount < 0 ? -1 : 0))) {
            case 0:  // Se amount è 0, ritorna al menu principale
                return;

            case -1:  // Se amount è negativo
                std::cerr << "Error: The amount cannot be negative. Please enter a valid amount." << std::endl;
                continue;  // Torna al menu di inserimento dell'importo

            case 1:  // Se amount è positivo
                break;  // Prosegue con la creazione della transazione
        }

        OperationType type;
        switch (transactionType) {
            case 1: type = OperationType::Deposit; break;
            case 2: type = OperationType::Withdrawal; break;
            case 3: type = OperationType::Transfer; break;
        }

        std::cout << "Enter start date (year month day): ";
        std::cout << "Enter start year (or 0 to return to previous menu):";
        std::cin >> startYear;
        if (startYear == 0) return;

        std::cout << "Enter start month (1-12) (or 0 to return to previous menu):";

        std::cin >> startMonth;
        if (startMonth == 0) return;

        std::cout << "Enter start day (1-31) (or 0 to return to previous menu):";

        std::cin >> startDay;
        if (startDay == 0) return;

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
            continue;  // Permetti all'utente di reinserire la data
        }

        std::cout << "Select frequency" << std::endl;
        std::cout << "1. Daily" << std::endl;
        std::cout << "2. Weekly" << std::endl;
        std::cout << "3. Monthly" << std::endl;
        std::cout << "4. Yearly" << std::endl;
        std::cout << "0. Return to main menu" << std::endl;
        std::cin >> freqChoice;
        Frequency freq;
        switch (freqChoice) {
            case 0:
                return;  // Ritorna al menu principale

            case 1: freq = Frequency::Daily; break;
            case 2: freq = Frequency::Weekly; break;
            case 3: freq = Frequency::Monthly; break;
            case 4: freq = Frequency::Yearly; break;

            default:
                std::cerr << "Invalid choice!" << std::endl;
                continue;  // Torna al menu di selezione della frequenza
        }

        auto transaction = std::make_shared<Operation>( amount, type, startDate);
        account.scheduleOperation(transaction, startDate, freq);
        std::cout << "Transaction scheduled successfully." << std::endl;
        break;  // Esci dal ciclo se la data è valida e l'operazione è programmata
    }
}

void handlePrintPlannedTransactions(BankAccount& account) {
    account.printPlannedTransactions();
}

void handlePrintBalance(const BankAccount& account) {
    double balance = account.getBalance();
    std::cout << "Current Balance: " << balance << std::endl;
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
            case 3: handleCancelRegularTransactions(account); break;
            case 4: handleShareTransactionHistory(account); break;
            case 5: handleCardOperations(account); break;
            case 6: handleScheduleTransaction(account); break;
            case 7: handlePrintPlannedTransactions(account); break;
            case 8: handleCancelScheduledTransactions(account); break;
            case 9: handleSaveToFile(account); break;
            case 10: handleLoadFromFile(account); break;
            case 11: handlePrintBalance(account); break;  // Add this line
            case 0: std::cout << "Exiting program." << std::endl; break;
            default: std::cerr << "Invalid choice! Please try again." << std::endl; break;
        }
    } while (choice != 0);

    return 0;
}
