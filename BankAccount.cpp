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
                << schedOp->getOperation()->getAmount() << ' '
                << static_cast<int>(schedOp->getOperation()->getType()) << ' '
                << std::chrono::system_clock::to_time_t(schedOp->getOperation()->getDate()) << ' '
                << std::chrono::system_clock::to_time_t(schedOp->getScheduledExecutionDate()) << ' '
                << static_cast<int>(schedOp->getFrequency()) << '\n';
    }

    // Salvataggio carte e relative operazioni
    file << cardsOperations.size() << '\n';
    for (const auto& card : cardsOperations) {
        file
                << card->getCardName() << ' '
                << card->getCardNumber() << ' '
                << card->getCvv() << ' '
                << std::chrono::system_clock::to_time_t(card->getExpirationDate()) << ' '
                << card->getAmount() << '\n';

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

    // Load balance and IBAN
    file >> balance;
    file.ignore();  // Ignore newline character
    std::getline(file, IBAN);

    // Load operations
    size_t numOperations;
    file >> numOperations;
    operations.clear();
    for (size_t i = 0; i < numOperations; ++i) {
        double amount;
        int type;
        std::time_t date;
        file >> amount >> type >> date;

        auto op = std::make_shared<Operation>(amount, static_cast<OperationType>(type), std::chrono::system_clock::from_time_t(date));
        operations.push_back(op);
    }

    // Load scheduled operations
    size_t numScheduledOps;
    file >> numScheduledOps;
    scheduledOperations.clear();
    for (size_t i = 0; i < numScheduledOps; ++i) {
        double amount;
        int type;
        std::time_t opDate;
        std::time_t schedDate;
        int freq;
        file >> amount >> type >> opDate >> schedDate >> freq;

        auto op = std::make_shared<Operation>(amount, static_cast<OperationType>(type), std::chrono::system_clock::from_time_t(opDate));
        auto schedOp = std::make_shared<ScheduledOperation>(op, std::chrono::system_clock::from_time_t(schedDate), static_cast<Frequency>(freq));
        scheduledOperations.push_back(schedOp);
    }

    // Load cards and their operations
    size_t numCards;
    file >> numCards;
    cardsOperations.clear();
    for (size_t i = 0; i < numCards; ++i) {
        std::string cardName;
        std::string cardNumber;
        std::string cvv;
        std::time_t expDate;
        double amount;
        bool isCredit;

        file >> cardName >> cardNumber >> cvv >> expDate >> amount >> isCredit;

        auto card = std::make_shared<Card>(cardName,isCredit);
        card->setCardNumber(cardNumber);
        card->setCvv(cvv);
        card->setExpirationDate(std::chrono::system_clock::from_time_t(expDate));
        card->setAmount(amount);

        // Load operations associated with this card
        size_t numCardOperations;
        file >> numCardOperations;
        for (size_t j = 0; j < numCardOperations; ++j) {
            double opAmount;
            int opType;
            std::time_t opDate;
            std::string description;

            file >> opAmount >> opType >> opDate;
            file.ignore();  // Ignore the space before the description
            std::getline(file, description);  // Read the rest of the line as the description

            auto cardOp = std::make_shared<Operation>(opAmount, static_cast<OperationType>(opType), std::chrono::system_clock::from_time_t(opDate));
            cardOp->setDescription(description);
            card->addOperation(cardOp);
        }

        cardsOperations.push_back(card);
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
void BankAccount::addScheduleOperation(const std::shared_ptr<Operation>& operation, std::chrono::system_clock::time_point startDate, Frequency frequency) {
    auto schedOp = std::make_shared<ScheduledOperation>(operation, startDate, frequency);
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
        if (schedOp->getOperation()->getAmount() == amount) {
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
        if (schedOp->getOperation()->getType() == type) {
            result.push_back(schedOp);
        }
    }
    return result;
}

void BankAccount::printPlannedTransactions() const {
    for (const auto& schedOp : scheduledOperations) {
        std::cout << "Operation Amount: " << schedOp->getOperation()->getAmount()
                  << ", Type: " << operationTypeToString(schedOp->getOperation()->getType())
                  << ", Operation Date: " << timePointToString(schedOp->getOperation()->getDate())
                  << ", Scheduled Execution Date: " << timePointToString(schedOp->getScheduledExecutionDate())
                  << ", Frequency: " << frequencyToString(schedOp->getFrequency())
                  << '\n';
    }
}

void BankAccount::executeScheduledOperations() {
    auto now = std::chrono::system_clock::now();
    std::vector<std::shared_ptr<ScheduledOperation>> operationsToExecute;

    // Trova le operazioni pianificate che devono essere eseguite
    for (const auto& schedOp : scheduledOperations) {
        if (schedOp->getScheduledExecutionDate() <= now) {
            operationsToExecute.push_back(schedOp);
        }
    }

    // Esegui le operazioni e aggiorna il saldo
    for (const auto& schedOp : operationsToExecute) {
        auto operation = schedOp->getOperation();
        if (operation->getType() == OperationType::Deposit) {
            balance += operation->getAmount();
        } else if (operation->getType() == OperationType::Withdrawal) {
            if (balance >= operation->getAmount()) {
                balance -= operation->getAmount();
            } else {
                std::cerr << "Error: Insufficient funds for scheduled withdrawal." << std::endl;
                continue;
            }
        }
        operations.push_back(operation);

        // Aggiorna la data di esecuzione pianificata in base alla frequenza
        auto frequency = schedOp->getFrequency();
        auto newExecutionDate = schedOp->getScheduledExecutionDate();
        switch (frequency) {
            case Frequency::Daily:
                newExecutionDate += std::chrono::hours(24);
                break;
            case Frequency::Weekly:
                newExecutionDate += std::chrono::hours(24 * 7);
                break;
            case Frequency::Monthly:
                newExecutionDate += std::chrono::hours(24 * 30);
                break;
            case Frequency::Yearly:
                newExecutionDate += std::chrono::hours(24 * 365);  // Approximation for a year
                break;
            default:
                std::cerr << "Error: Unknown frequency type." << std::endl;
                break;
        }

        schedOp->setScheduledExecutionDate(newExecutionDate);
    }

    // Rimuovi le operazioni pianificate eseguite
    scheduledOperations.erase(std::remove_if(scheduledOperations.begin(), scheduledOperations.end(),
                                             [&operationsToExecute](const std::shared_ptr<ScheduledOperation>& schedOp) {
                                                 return std::find(operationsToExecute.begin(), operationsToExecute.end(), schedOp) != operationsToExecute.end();
                                             }), scheduledOperations.end());
}

// Metodi per le carte
void BankAccount::addCard(const std::string& cardName, bool isCredit) {
    auto card = std::make_shared<Card>(cardName,isCredit);
    cardsOperations.push_back(card);
}

void BankAccount::removeCard(const std::string& cardName) {
    auto it = std::remove_if(cardsOperations.begin(), cardsOperations.end(),
                             [&cardName](const std::shared_ptr<Card>& card) {
                                 return card->getCardName() == cardName;
                             });
    cardsOperations.erase(it, cardsOperations.end());
}

void BankAccount::addOperationToCard(const std::string& cardName, const std::shared_ptr<Operation>& operation) {
    auto cardIt = std::find_if(cardsOperations.begin(), cardsOperations.end(),
                               [&cardName](const std::shared_ptr<Card>& card) {
                                   return card->getCardName() == cardName;
                               });

    if (cardIt != cardsOperations.end()) {
        auto& card = *cardIt;
        if (operation->getType() == OperationType::Deposit) {
            // Verifica e gestisci il deposito
            if (balance >= operation->getAmount()) {
                balance -= operation->getAmount();
                card->setAmount(card->getAmount() + operation->getAmount());
                card->addOperation(operation);
            } else {
                std::cerr << "Error: Insufficient funds in the bank account for this deposit." << std::endl;
            }
        } else if (operation->getType() == OperationType::Withdrawal) {
            // Verifica e gestisci il prelievo
            double newCardAmount = card->getAmount() - operation->getAmount();
            if (card->isCreditCardBool() || newCardAmount >= 0) {
                // Se la carta è di credito o ci sono fondi sufficienti sulla carta di debito
                card->setAmount(newCardAmount);
                card->addOperation(operation);
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
    auto cardIt = std::find_if(cardsOperations.begin(), cardsOperations.end(),
                               [&cardName](const std::shared_ptr<Card>& card) {
                                   return card->getCardName() == cardName;
                               });

    if (cardIt != cardsOperations.end()) {
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
    for (const auto& card : cardsOperations) {
        std::cout << "Card Name: " << card->getCardName()
                  << ", Number: " << card->getCardNumber()
                  << ", CVV: " << card->getCvv()
                  << ", Expiration Date: " << std::chrono::system_clock::to_time_t(card->getExpirationDate())
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
    for (const auto& card : cardsOperations) {
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
    return cardsOperations;
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
            return "Monthly";

        default:
            return "Unknown";
    }
}