#include "BankAccount.h"
#include <fstream>
#include <sstream>
#include <iostream>
#include <random>
#include <chrono>
#include <iomanip>

//costruttore
BankAccount::BankAccount() : balance(0.0) {
    generateIBAN(); // Genera l'IBAN quando l'oggetto è creato
}
void BankAccount::generateIBAN() {
    // Codice paese (esempio: IT per Italia)
    std::string countryCode = "IT";

    // Generazione sicura del numero di controllo
    std::random_device rd;
    std::mt19937 gen(rd());
    std::uniform_int_distribution<int> dist(10, 99);
    int controlDigits = dist(gen);

    // Generazione sicura del numero di conto bancario
    std::uniform_int_distribution<std::uint64_t> distAccount(0, 999999999999);
    std::ostringstream accountNumber;
    accountNumber << std::setw(12) << std::setfill('0') << distAccount(gen);

    // Componi l'IBAN
    IBAN = countryCode + std::to_string(controlDigits) + accountNumber.str();
}

// Metodi di salvataggio e caricamento
void BankAccount::saveToFile(const std::string& filename) const {
    std::ofstream file(filename);

    if (!file) {
        std::cerr << "Error opening file for writing: " << filename << std::endl;
        return;
    }

    // Salvataggio saldo e IBAN
    file << balance << '\n' << IBAN << '\n';

    // Salvataggio operazioni
    file << operations.size() << '\n';
    for (const auto& op : operations) {
        file << op->getAmount() << ' '
             << static_cast<int>(op->getType()) << ' '
             << std::chrono::system_clock::to_time_t(op->getDate()) << '\n';
    }

    // Salvataggio operazioni pianificate
    file << scheduledOperations.size() << '\n';
    for (const auto& schedOp : scheduledOperations) {
        file
                << schedOp->getAmount() << ' '
                << static_cast<int>(schedOp->getType()) << ' '
                << std::chrono::system_clock::to_time_t(schedOp->getDate()) << ' '
                << std::chrono::system_clock::to_time_t(schedOp->getScheduledExecutionDate()) << ' '
                << static_cast<int>(schedOp->getFrequency()) << '\n';
    }

    // Salvataggio carte e relative operazioni
    file << cards.size() << '\n';
    for (const auto& card : cards) {
        file
                << card->getCardName() << ' '
                << card->getCardNumber() << ' '
                << card->getCvv() << ' '
                << std::chrono::system_clock::to_time_t(card->getExpirationDate()) << ' '
                << card->getAmount() << ' '
            << card->isCreditCardBool() <<'\n';

        // Salvataggio delle operazioni associate alla carta
        file << card->getOperations().size() << '\n';
        for (const auto& op : card->getOperations()) {
            file << op->getAmount() << ' '
                 << static_cast<int>(op->getType()) << ' '
                 << std::chrono::system_clock::to_time_t(op->getDate()) << ' '
                 << op->getDescription() << '\n';
        }
    }

    file.close();
}

void BankAccount::loadFromFile(const std::string& filename) {
    std::ifstream file(filename);

    if (!file) {
        std::cerr << "Error opening file for reading: " << filename << std::endl;
        return;
    }

    // Caricamento del saldo e dell'IBAN
    file >> balance;
    file.ignore();  // Ignora il carattere newline dopo il saldo
    std::getline(file, IBAN);

    // Caricamento delle operazioni
    size_t operationsCount;
    file >> operationsCount;
    file.ignore();  // Ignora il carattere newline

    operations.clear();
    for (size_t i = 0; i < operationsCount; ++i) {
        double amount;
        int typeInt;
        std::time_t date;

        file >> amount >> typeInt >> date;
        file.ignore();  // Ignora il carattere newline

        OperationType type = static_cast<OperationType>(typeInt);
        auto operation = std::make_shared<Operation>(amount, type, std::chrono::system_clock::from_time_t(date));

        operations.push_back(operation);
    }

    // Caricamento delle operazioni pianificate
    size_t scheduledOperationsCount;
    file >> scheduledOperationsCount;
    file.ignore();  // Ignora il carattere newline

    scheduledOperations.clear();
    for (size_t i = 0; i < scheduledOperationsCount; ++i) {
        double amount;
        int typeInt;
        std::time_t operationDate, scheduledDate;
        int frequencyInt;

        file >> amount >> typeInt >> operationDate >> scheduledDate >> frequencyInt;
        file.ignore();  // Ignora il carattere newline

        OperationType type = static_cast<OperationType>(typeInt);
        Frequency frequency = static_cast<Frequency>(frequencyInt);

        auto scheduledOperation = std::make_shared<ScheduledOperation>(
                amount, type,
                std::chrono::system_clock::from_time_t(operationDate),
                frequency
        );
        scheduledOperation->setScheduledExecutionDate(std::chrono::system_clock::from_time_t(scheduledDate));

        scheduledOperations.push_back(scheduledOperation);
    }

    // Caricamento delle carte e delle relative operazioni
    size_t cardsCount;
    file >> cardsCount;
    file.ignore();  // Ignora il carattere newline

    cards.clear();
    for (size_t i = 0; i < cardsCount; ++i) {
        std::string cardName, cardNumber;
        std::string cvv;
        std::time_t expirationDate;
        double cardAmount;
        bool isCreditCard;  // Aggiungi la variabile per leggere il tipo di carta

        file >> cardName >> cardNumber >> cvv >> expirationDate >> cardAmount >> isCreditCard;
        file.ignore();  // Ignora il carattere newline

        auto card = std::make_shared<Card>(cardName, isCreditCard);
        card->setCardNumber(cardNumber);
        card->setCvv(cvv);
        card->setExpirationDate(std::chrono::system_clock::from_time_t(expirationDate));
        card->setAmount(cardAmount);

        // Caricamento delle operazioni associate alla carta
        size_t cardOperationsCount;
        file >> cardOperationsCount;
        file.ignore();  // Ignora il carattere newline

        for (size_t j = 0; j < cardOperationsCount; ++j) {
            double amount;
            int typeInt;
            std::time_t date;
            std::string description;

            file >> amount >> typeInt >> date;
            file.ignore();  // Ignora il carattere newline
            std::getline(file, description);

            OperationType type = static_cast<OperationType>(typeInt);
            auto operation = std::make_shared<Operation>(amount, type, std::chrono::system_clock::from_time_t(date));
            operation->setDescription(description);

            card->addOperation(operation);
        }

        cards.push_back(card);
    }

    file.close();
}

// Metodi per le operazioni normali
void BankAccount::addTransaction(const std::shared_ptr<Operation>& transaction) {
    // Aggiungi l'operazione alla lista
    operations.push_back(transaction);

    // Aggiorna il saldo
    if (transaction->getType() == OperationType::Deposit) {
        balance += transaction->getAmount();
    } else if (transaction->getType() == OperationType::Withdrawal) {
        balance -= transaction->getAmount();
    }
    else if (transaction->getType() == OperationType::Transfer) {
        balance -= transaction->getAmount();
    }
}

void BankAccount::removeOperations(const std::vector<std::shared_ptr<Operation>>& operationsToCancel) {
    for (const auto& op : operationsToCancel) {
        auto it = std::remove_if(operations.begin(), operations.end(),
                                 [&op](const std::shared_ptr<Operation>& o) {
                                     return o == op;
                                 });
        operations.erase(it, operations.end());
    }
}

std::vector<std::shared_ptr<Operation>> BankAccount::findOperationByAmount(double amount) const {
    std::vector<std::shared_ptr<Operation>> result;
    for (const auto& op : operations) {
        if (op->getAmount() == amount) {
            result.push_back(op);
        }
    }
    return result;
}

std::vector<std::shared_ptr<Operation>> BankAccount::findOperationByType(OperationType type) const {
    std::vector<std::shared_ptr<Operation>> result;
    for (const auto& op : operations) {
        if (op->getType() == type) {
            result.push_back(op);
        }
    }
    return result;
}

std::vector<std::shared_ptr<Operation>> BankAccount::findOperationByDate(std::chrono::system_clock::time_point date) const {
    std::vector<std::shared_ptr<Operation>> result;
    for (const auto& op : operations) {
        if (op->getDate() == date) {
            result.push_back(op);
        }
    }
    return result;
}

std::string BankAccount::getTransactionHistory() const {
    std::ostringstream oss;
    for (const auto& op : operations) {
        oss << "Amount: " << op->getAmount()
            << ", Type: " << operationTypeToString(op->getType())
            << ", Date: " << timePointToString(op->getDate())
                << ", Description: " << op->getDescription()
            << '\n';
    }
    return oss.str();
}

void BankAccount::printOperations(const std::vector<std::shared_ptr<Operation>>& operations) const {
    for (const auto& op : operations) {
        std::cout << "Amount: " << op->getAmount()
                  << ", Type: " << operationTypeToString(op->getType())
                  << ", Date: " << timePointToString(op->getDate())
                  << ", Description: " << op->getDescription()
                  << '\n';
    }
}

// Metodi per le operazioni programmate
void BankAccount::addScheduleOperation(std::shared_ptr<ScheduledOperation> schedOp) {
    scheduledOperations.push_back(schedOp);
}

void BankAccount::removeScheduledOperation(const std::vector<std::shared_ptr<ScheduledOperation>>& scheduledOperationsToRemove) {
    for (const auto& schedOp : scheduledOperationsToRemove) {
        auto it = std::remove_if(scheduledOperations.begin(), scheduledOperations.end(),
                                 [&schedOp](const std::shared_ptr<ScheduledOperation>& so) {
                                     return so == schedOp;
                                 });
        scheduledOperations.erase(it, scheduledOperations.end());
    }
}

std::vector<std::shared_ptr<ScheduledOperation>> BankAccount::findScheduledByAmount(double amount) const {
    std::vector<std::shared_ptr<ScheduledOperation>> result;
    for (const auto& schedOp : scheduledOperations) {
        if (schedOp->getAmount() == amount) {
            result.push_back(schedOp);
        }
    }
    return result;
}

std::vector<std::shared_ptr<ScheduledOperation>> BankAccount::findScheduledByDate(std::chrono::system_clock::time_point date) const {
    std::vector<std::shared_ptr<ScheduledOperation>> result;
    for (const auto& schedOp : scheduledOperations) {
        if (schedOp->getScheduledExecutionDate() == date) {
            result.push_back(schedOp);
        }
    }
    return result;
}

std::vector<std::shared_ptr<ScheduledOperation>> BankAccount::findScheduledByType(OperationType type) const {
    std::vector<std::shared_ptr<ScheduledOperation>> result;
    for (const auto& schedOp : scheduledOperations) {
        if (schedOp->getType() == type) {
            result.push_back(schedOp);
        }
    }
    return result;
}

void BankAccount::printPlannedTransactions() const {
    for (const auto& schedOp : scheduledOperations) {
        std::cout << "Operation Amount: " << schedOp->getAmount()
                  << ", Type: " << operationTypeToString(schedOp->getType())
                  << ", Operation Date: " << timePointToString(schedOp->getDate())
                  << ", Scheduled Execution Date: " << timePointToString(schedOp->getScheduledExecutionDate())
                  << ", Frequency: " << frequencyToString(schedOp->getFrequency())
                  << '\n';
    }
}

void BankAccount::executeScheduledOperations() {
    using namespace std::chrono;

    auto now = system_clock::now();

    // Trova e esegui le operazioni pianificate che devono essere eseguite oggi
    std::vector<std::shared_ptr<Operation>> executedOperations;  // Cambiato a Operation
    for (auto& schedOp : scheduledOperations) {
        if (schedOp->getScheduledExecutionDate() <= now) {
            // Esegui l'operazione pianificata
            auto operation = std::make_shared<Operation>(
                    schedOp->getAmount(),
                    schedOp->getType(),
                    now
            );

            // Aggiungi l'operazione all'account
            addTransaction(operation);

            // Aggiungi l'operazione alla lista di operazioni eseguite
            executedOperations.push_back(operation);  // Aggiungi Operation non ScheduledOperation

            // Aggiorna la data di esecuzione pianificata per la prossima esecuzione
            switch (schedOp->getFrequency()) {
                case Frequency::Daily:
                    schedOp->setScheduledExecutionDate(schedOp->getScheduledExecutionDate() + days(1));
                    break;
                case Frequency::Weekly:
                    schedOp->setScheduledExecutionDate(schedOp->getScheduledExecutionDate() + weeks(1));
                    break;
                case Frequency::Monthly:
                    schedOp->setScheduledExecutionDate(schedOp->getScheduledExecutionDate() + months(1));
                    break;
            }
        }
    }

    // Rimuovi le operazioni pianificate che sono state eseguite
    removeOperations(executedOperations);
}

// Metodi per le carte
void BankAccount::addCard(const std::string& cardName, bool isCredit) {
    auto card = std::make_shared<Card>(cardName,isCredit);
    cards.push_back(card);

}

void BankAccount::removeCard(const std::string& cardName) {
    auto it = std::remove_if(cards.begin(), cards.end(),
                             [&cardName](const std::shared_ptr<Card>& card) {
                                 return card->getCardName() == cardName;
                             });
    cards.erase(it, cards.end());
}

void BankAccount::addOperationToCard(const std::string& cardName, const std::shared_ptr<Operation>& operation) {
    auto cardIt = std::find_if(cards.begin(), cards.end(),
                               [&cardName](const std::shared_ptr<Card>& card) {
                                   return card->getCardName() == cardName;
                               });

    if (cardIt != cards.end()) {
        auto& card = *cardIt;
        double operationAmount = operation->getAmount();

        if (operation->getType() == OperationType::Deposit) {
            if (balance >= operationAmount) {
                balance -= operationAmount;
                card->setAmount(card->getAmount() + operationAmount);
                card->addOperation(operation);
            } else {
                std::cerr << "Error: Insufficient funds in the bank account for this deposit." << std::endl;
            }
        } else if (operation->getType() == OperationType::Withdrawal) {
            double newCardAmount = card->getAmount() - operationAmount;

            if (card->isCreditCardBool() || newCardAmount >= 0) {
                card->setAmount(newCardAmount);
                card->addOperation(operation);

                // Se è una carta di debito, sottrai l'importo dal saldo dell'account
                if (!card->isCreditCardBool()) {
                    if (balance >= operationAmount) {
                        balance -= operationAmount;
                    } else {
                        std::cerr << "Error: Insufficient funds in the bank account for this withdrawal." << std::endl;
                        card->setAmount(card->getAmount() + operationAmount); // Rollback
                        card->removeLastOperation(); // Rollback
                    }
                }
            } else {
                std::cerr << "Error: Insufficient funds on the debit card for this withdrawal." << std::endl;
            }
        } else {
            std::cerr << "Error: Unsupported operation type." << std::endl;
        }
    } else {
        std::cerr << "Error: Card not found. Cannot add operation." << std::endl;
    }
}

void BankAccount::printCardOperations(const std::string& cardName) const {
    auto cardIt = std::find_if(cards.begin(), cards.end(),
                               [&cardName](const std::shared_ptr<Card>& card) {
                                   return card->getCardName() == cardName;
                               });

    if (cardIt != cards.end()) {
        std::cout << "Operations for card " << cardName << ":\n";
        for (const auto& op : (*cardIt)->getOperations()) {
            std::cout << "Amount: " << op->getAmount()
                      << ", Type: " << operationTypeToString(op->getType())
                      << ", Date: " << timePointToString(op->getDate())
                      << ", Description: " << op->getDescription()  // Stampa la descrizione
                      << '\n';
        }
    } else {
        std::cerr << "Card not found." << std::endl;
    }
}

void BankAccount::printCards() const {
    if (cards.empty()) {
        std::cout << "No cards available." << std::endl;
        return;
    }

    std::cout << "List of Cards:\n";
    for (const auto& card : cards) {
        std::cout << "Card Name: " << card->getCardName()
                  << ", Number: " << card->getCardNumber()
                  << ", CVV: " << card->getCvv()
                  << ", Expiration Date: " << timePointToString(card->getExpirationDate())
                  << ", Amount: " << card->getAmount()
                  << '\n';
    }
}


// Metodi di stampa e formattazione
std::string BankAccount::printBalance() const {
    std::ostringstream oss;
    oss << "Balance: " << balance;
    return oss.str();
}

std::string BankAccount::timePointToString(std::chrono::system_clock::time_point tp) const {
    std::time_t time = std::chrono::system_clock::to_time_t(tp);
    std::ostringstream oss;
    oss << std::put_time(std::localtime(&time), "%Y-%m-%d %H:%M:%S");
    return oss.str();
}

// Nuovi metodi per contare le operazioni
int BankAccount::getNormalOperationCount() const {
    return operations.size();
}

int BankAccount::getScheduledOperationCount() const {
    return scheduledOperations.size();
}

int BankAccount::getCardOperationCount() const {
    int cardOps = 0;
    for (const auto& card : cards) {
        cardOps += card->getOperations().size();
    }
    return cardOps;
}

std::string BankAccount::getIBAN() const {
    return IBAN;
}

double BankAccount::getBalance() const {
    return balance;
}

std::vector<std::shared_ptr<Card>> BankAccount::getCardsOperations() const {
    return cards;
}

int BankAccount::getCardOperationsSize() {
    return cards.size();
}
// Conversione di OperationType in stringa
std::string BankAccount::operationTypeToString(OperationType type) const {
    switch (type) {
        case OperationType::Deposit:
            return "Deposit";
        case OperationType::Withdrawal:
            return "Withdrawal";
        case OperationType::Transfer:
            return "Trasfer";
        default:
            return "Unknown";
    }
}

// Conversione di Frequency in stringa
std::string BankAccount::frequencyToString(Frequency freq) const {
    switch (freq) {
        case Frequency::Daily:
            return "Daily";
        case Frequency::Weekly:
            return "Weekly";
        case Frequency::Monthly:
            return "Monthly";
        case Frequency::Yearly:
            return "Yearly";

        default:
            return "Unknown";
    }
}