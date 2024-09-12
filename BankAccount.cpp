#include "BankAccount.h"
#include "CreditCard.h"
#include "DebitCard.h"
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
    try {
        std::ofstream file(filename);

        if (!file) {
            throw std::ios_base::failure("Error opening file for writing: " + filename);
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
                    << card->isCreditCardBool() << '\n';

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
    } catch (const std::ios_base::failure& e) {
        std::cerr << "File I/O error: " << e.what() << std::endl;
    } catch (const std::exception& e) {
        std::cerr << "Error saving to file: " << e.what() << std::endl;
    }
}

void BankAccount::loadFromFile(const std::string& filename) {
    try {
        std::ifstream file(filename);

        if (!file) {
            throw std::ios_base::failure("Error opening file for reading: " + filename);
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
            std::string cardName, cardNumber, cvv;
            std::time_t expirationDate;
            double cardAmount;
            bool isCreditCard;

            file >> cardName >> cardNumber >> cvv >> expirationDate >> cardAmount >> isCreditCard;
            file.ignore();  // Ignora il carattere newline

            std::shared_ptr<Card> card;
            if (isCreditCard) {
                card = std::make_shared<CreditCard>(cardName, cardAmount, 0.0); // Default rate
            } else {
                card = std::make_shared<DebitCard>(cardName);
            }

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

                card->addOperationWithoutUpdate(operation);
            }

            cards.push_back(card);
        }

        file.close();
    } catch (const std::ios_base::failure& e) {
        std::cerr << "File I/O error: " << e.what() << std::endl;
    } catch (const std::exception& e) {
        std::cerr << "Error loading from file: " << e.what() << std::endl;
    }
}

// Metodi per le operazioni normali
bool BankAccount::addTransaction(const std::shared_ptr<Operation>& transaction) {
    try {
        if (!transaction) {
            throw std::invalid_argument("Transaction is null");
        }

        operations.push_back(transaction);

        if (transaction->getType() == OperationType::Deposit) {
            balance += transaction->getAmount();
        } else if (transaction->getType() == OperationType::Withdrawal || transaction->getType() == OperationType::Transfer) {
            balance -= transaction->getAmount();
        } else {
            throw std::invalid_argument("Unsupported operation type");
        }

        if (balance < 0) {
            throw std::runtime_error("Balance cannot be negative");
        }
        return true; // Operazione riuscita
    } catch (const std::exception& e) {
        std::cerr << "Error adding transaction: " << e.what() << std::endl;
        return false; // Fallimento
    }
}



bool BankAccount::removeOperations(const std::vector<std::shared_ptr<Operation>>& operationsToCancel) {
    try {
        for (const auto& op : operationsToCancel) {
            if (!op) {
                throw std::invalid_argument("Operation to cancel is null");
            }

            auto it = std::remove_if(operations.begin(), operations.end(),
                                     [&op](const std::shared_ptr<Operation>& o) {
                                         return o == op;
                                     });
            if (it == operations.end()) {
                throw std::runtime_error("Operation not found in list");
            }
            operations.erase(it, operations.end());
        }
        return true; // Operazione di rimozione riuscita
    } catch (const std::exception& e) {
        std::cerr << "Error removing operations: " << e.what() << std::endl;
        return false; // Fallimento
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
    try {
        if (!schedOp) {
            throw std::invalid_argument("Scheduled operation is null");
        }
        scheduledOperations.push_back(schedOp);
    } catch (const std::exception& e) {
        std::cerr << "Error adding scheduled operation: " << e.what() << std::endl;
    }
}

void BankAccount::removeScheduledOperation(const std::vector<std::shared_ptr<ScheduledOperation>>& scheduledOperationsToRemove) {
    try {
        for (const auto& schedOp : scheduledOperationsToRemove) {
            if (!schedOp) {
                throw std::invalid_argument("Scheduled operation to remove is null");
            }

            auto it = std::remove_if(scheduledOperations.begin(), scheduledOperations.end(),
                                     [&schedOp](const std::shared_ptr<ScheduledOperation>& so) {
                                         return so == schedOp;
                                     });
            if (it == scheduledOperations.end()) {
                throw std::runtime_error("Scheduled operation not found in list");
            }
            scheduledOperations.erase(it, scheduledOperations.end());
        }
    } catch (const std::exception& e) {
        std::cerr << "Error removing scheduled operations: " << e.what() << std::endl;
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
bool BankAccount::addCard(const std::string& cardName, bool isCredit) {
    try {
        if (cardName.empty()) {
            throw std::invalid_argument("Card name cannot be empty");
        }

        std::shared_ptr<Card> card;

        if (isCredit) {
            double defaultCreditLimit = 1000.0;
            double defaultInterestRate = 0.1;
            card = std::make_shared<CreditCard>(cardName, defaultCreditLimit, defaultInterestRate);
        } else {
            card = std::make_shared<DebitCard>(cardName);
        }

        cards.push_back(card);
        return true; // Carta aggiunta con successo
    } catch (const std::exception& e) {
        std::cerr << "Error adding card: " << e.what() << std::endl;
        return false; // Fallimento
    }
}




bool BankAccount::removeCard(const std::string& cardName) {
    try {
        auto it = std::remove_if(cards.begin(), cards.end(),
                                 [&cardName](const std::shared_ptr<Card>& card) {
                                     return card->getCardName() == cardName;
                                 });
        if (it == cards.end()) {
            throw std::runtime_error("Card not found");
        }
        cards.erase(it, cards.end());
        return true; // Carta rimossa con successo
    } catch (const std::exception& e) {
        std::cerr << "Error removing card: " << e.what() << std::endl;
        return false; // Fallimento
    }
}

bool BankAccount::addOperationToCard(const std::string& cardName, const std::shared_ptr<Operation>& operation) {
    try {
        auto cardIt = std::find_if(cards.begin(), cards.end(),
                                   [&cardName](const std::shared_ptr<Card>& card) {
                                       return card->getCardName() == cardName;
                                   });

        if (cardIt == cards.end()) {
            throw std::runtime_error("Card not found");
        }

        if (!operation) {
            throw std::invalid_argument("Operation is null");
        }

        auto& card = *cardIt;
        double operationAmount = operation->getAmount();

        if (operation->getType() == OperationType::Deposit) {
            if (balance >= operationAmount) {
                balance -= operationAmount;
                card->addOperation(operation);
            } else {
                return false;
            }
        } else if (operation->getType() == OperationType::Withdrawal) {
            if (card->isCreditCard) {
                card->addOperation(operation);
                balance -= operationAmount; // Riduce il saldo del conto per carte di credito
            } else {
                // Per le carte di debito
                if (card->getAmount() >= operationAmount && balance >= operationAmount) {
                    card->addOperation(operation);
                    balance -= operationAmount; // Riduce il saldo del conto
                } else {
                    return false;
                }
            }
        } else {
            throw std::invalid_argument("Unsupported operation type");
        }

        return true; // Operazione aggiunta con successo
    } catch (const std::exception& e) {
        std::cerr << "Error adding operation to card: " << e.what() << std::endl;
        return false; // Fallimento
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