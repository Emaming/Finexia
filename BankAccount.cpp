#include "BankAccount.h"
#include "CardOperation.h"
#include <fstream>
#include <sstream>
#include <iostream>
#include <random>
#include <chrono>
#include "memory"
#include "Card.h"
#include "CreditCard.h"
#include "DebitCard.h"


// Costruttore
BankAccount::BankAccount() : balance(0.0) {}

void BankAccount::saveToFile(const std::string& filename) const {
    std::ofstream file(filename);

    if (!file) {
        std::cerr << "Error opening file for writing: " << filename << std::endl;
        return;
    }

    // Save balance and IBAN
    file << balance << '\n' << IBAN << '\n';

    // Save operations
    file << operations.size() << '\n';
    for (const auto& op : operations) {
        file << op->getId() << ' '
             << op->getAmount() << ' '
             << static_cast<int>(op->getType()) << ' '
             << std::chrono::system_clock::to_time_t(op->getDate()) << '\n';
    }

    // Save scheduled operations
    file << scheduledOperations.size() << '\n';
    for (const auto& schedOp : scheduledOperations) {
        file << schedOp->getOperation()->getId() << ' '
             << schedOp->getOperation()->getAmount() << ' '
             << static_cast<int>(schedOp->getOperation()->getType()) << ' '
             << std::chrono::system_clock::to_time_t(schedOp->getOperation()->getDate()) << ' '
             << std::chrono::system_clock::to_time_t(schedOp->getScheduledExecutionDate()) << ' '
             << static_cast<int>(schedOp->getFrequency()) << '\n';
    }

    // Save card operations
    file << cardsOperations.size() << '\n';
    for (const auto& cardOp : cardsOperations) {
        file << cardOp->getCardId() << ' '
             << cardOp->getOperation()->getId() << ' '
             << cardOp->getOperation()->getAmount() << ' '
             << static_cast<int>(cardOp->getOperation()->getType()) << ' '
             << (cardOp->isCreditCard() ? 0 : 1) << '\n'; // 0 for CreditCard, 1 for DebitCard
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
        int id;
        double amount;
        int type;
        std::time_t date;
        file >> id >> amount >> type >> date;

        auto op = std::make_shared<Operation>(id, amount, static_cast<OperationType>(type), std::chrono::system_clock::from_time_t(date));
        operations.push_back(op);
    }

    // Load scheduled operations
    size_t numScheduledOps;
    file >> numScheduledOps;
    scheduledOperations.clear();
    for (size_t i = 0; i < numScheduledOps; ++i) {
        int id;
        double amount;
        int type;
        std::time_t opDate;
        std::time_t schedDate;
        int freq;
        file >> id >> amount >> type >> opDate >> schedDate >> freq;

        auto op = std::make_shared<Operation>(id, amount, static_cast<OperationType>(type), std::chrono::system_clock::from_time_t(opDate));
        auto schedOp = std::make_shared<ScheduledOperation>(op, std::chrono::system_clock::from_time_t(schedDate), static_cast<Frequency>(freq));
        scheduledOperations.push_back(schedOp);
    }

    // Load card operations
    size_t numCardOps;
    file >> numCardOps;
    cardsOperations.clear();
    for (size_t i = 0; i < numCardOps; ++i) {
        std::string cardId;
        int opId;
        double amount;
        int type;
        int cardType; // 0 for CreditCard, 1 for DebitCard

        file >> cardId >> opId >> amount >> type >> cardType;

        auto op = std::make_shared<Operation>(opId, amount, static_cast<OperationType>(type), std::chrono::system_clock::now());  // Dummy date

        std::variant<std::monostate, std::shared_ptr<CreditCard>, std::shared_ptr<DebitCard>> cardVariant;
        if (cardType == 0) {
            cardVariant = std::make_shared<CreditCard>();
        } else if (cardType == 1) {
            cardVariant = std::make_shared<DebitCard>();
        } else {
            std::cerr << "Unknown card type: " << cardType << std::endl;
            continue; // Handle unknown type if necessary
        }

        auto cardOp = std::make_shared<CardOperation>(op, cardVariant, cardId, "", "", std::chrono::system_clock::now());  // Dummy data
        cardsOperations.push_back(cardOp);
    }

    file.close();
}



#include <cstdint> // For uint64_t

void BankAccount::addCard(const std::string& cardName, bool isCredit) {
    auto now = std::chrono::system_clock::now(); // Current time
    int id = static_cast<int>(cardsOperations.size()) + 1; // Generate a new ID based on the number of existing operations
    double amount = 0.0; // Default amount

    // Create an Operation object with the given parameters
    auto operation = std::make_shared<Operation>(id, amount,
                                                 isCredit ? OperationType::CreditCard : OperationType::DebitCard, now);

    // Create a Card object (CreditCard or DebitCard) based on the isCredit flag
    std::variant<std::monostate, std::shared_ptr<CreditCard>, std::shared_ptr<DebitCard>> cardVariant;
    if (isCredit) {
        cardVariant = std::make_shared<CreditCard>(); // Create a CreditCard and wrap in variant
    } else {
        cardVariant = std::make_shared<DebitCard>(); // Create a DebitCard and wrap in variant
    }

    // Generate random card number and CVV
    std::random_device rd;
    std::mt19937 gen(rd());
    std::uniform_int_distribution<std::uint64_t> cardNumberDist(1000000000000000ULL, 9999999999999999ULL); // 16-digit card number
    std::uniform_int_distribution<> cvvDist(100, 999); // 3-digit CVV
    std::uniform_int_distribution<> yearDist(1, 5); // Validity period in years

    std::string cardNumber = std::to_string(cardNumberDist(gen));
    std::string cvv = std::to_string(cvvDist(gen));

    // Generate expiration date
    auto future = now + std::chrono::duration_cast<std::chrono::system_clock::duration>(std::chrono::years(yearDist(gen)));
    std::chrono::system_clock::time_point expirationDate = future;

    // Create a CardOperation object with all the gathered information
    auto cardOperation = std::make_shared<CardOperation>(operation, cardVariant, cardName, cardNumber, cvv, expirationDate);

    // Add the new CardOperation to the list of card operations
    cardsOperations.push_back(cardOperation);
}




void BankAccount::addTransaction(const std::shared_ptr<Operation>& transaction) {
    operations.push_back(transaction);
}

void BankAccount::addScheduledOperation(const std::shared_ptr<ScheduledOperation>& operation) {
    scheduledOperations.push_back(operation);
}

void BankAccount::cancelOperations(const std::vector<std::shared_ptr<Operation>>& operationsToCancel) {
    for (const auto& op : operationsToCancel) {
        operations.erase(
                std::remove_if(operations.begin(), operations.end(),
                               [&op](const std::shared_ptr<Operation>& o) { return o->getId() == op->getId(); }),
                operations.end()
        );
    }
}

void BankAccount::removeScheduledOperation(const std::vector<std::shared_ptr<ScheduledOperation>>& operationsToRemove) {
    for (const auto& op : operationsToRemove) {
        scheduledOperations.erase(
                std::remove_if(scheduledOperations.begin(), scheduledOperations.end(),
                               [&op](const std::shared_ptr<ScheduledOperation>& o) { return o->getOperation()->getId() == op->getOperation()->getId(); }),
                scheduledOperations.end()
        );
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

std::vector<std::shared_ptr<ScheduledOperation>> BankAccount::findNextExecutionDate(std::chrono::system_clock::time_point date) const {
    std::vector<std::shared_ptr<ScheduledOperation>> result;
    for (const auto& schedOp : scheduledOperations) {
        if (schedOp->getScheduledExecutionDate() >= date) {
            result.push_back(schedOp);
        }
    }
    return result;
}

void BankAccount::executePlannedTransactions() {
    auto now = std::chrono::system_clock::now();
    scheduledOperations.erase(
            std::remove_if(scheduledOperations.begin(), scheduledOperations.end(),
                           [&now, this](const std::shared_ptr<ScheduledOperation>& schedOp) {
                               if (schedOp->getScheduledExecutionDate() <= now) {
                                   addTransaction(schedOp->getOperation());
                                   return true; // Erase this element
                               }
                               return false; // Keep this element
                           }),
            scheduledOperations.end()
    );
}

void BankAccount::printOperations(const std::vector<std::shared_ptr<Operation>>& ops) const {
    for (const auto& op : ops) {
        std::cout << "ID: " << op->getId()
                  << ", Amount: " << op->getAmount()
                  << ", Type: " << static_cast<int>(op->getType())
                  << ", Date: " << timePointToString(op->getDate()) << std::endl;
    }
}

void BankAccount::printPlannedTransactions(const std::vector<std::shared_ptr<ScheduledOperation>>& ops) const {
    for (const auto& schedOp : ops) {
        std::cout << "ID: " << schedOp->getOperation()->getId()
                  << ", Amount: " << schedOp->getOperation()->getAmount()
                  << ", Type: " << static_cast<int>(schedOp->getOperation()->getType())
                  << ", Operation Date: " << timePointToString(schedOp->getOperation()->getDate())
                  << ", Scheduled Date: " << timePointToString(schedOp->getScheduledExecutionDate())
                  << ", Frequency: " << static_cast<int>(schedOp->getFrequency()) << std::endl;
    }
}

void BankAccount::printCards() const {
    for (const auto& card : cardsOperations) {
        std::cout << card->printCardString();
    }
}

std::string BankAccount::printIban() const {
    return IBAN;
}

std::string BankAccount::printBalance() const {
    std::ostringstream oss;
    oss << balance;
    return oss.str();
}

std::string BankAccount::getTransactionHistory() const {
    std::ostringstream oss;
    for (const auto& op : operations) {
        oss << "ID: " << op->getId()
            << ", Amount: " << op->getAmount()
            << ", Type: " << static_cast<int>(op->getType())
            << ", Date: " << timePointToString(op->getDate()) << '\n';
    }
    return oss.str();
}

std::string BankAccount::timePointToString(std::chrono::system_clock::time_point tp) const {
    std::time_t time = std::chrono::system_clock::to_time_t(tp);
    std::string timeStr = std::ctime(&time);
    if (!timeStr.empty() && timeStr.back() == '\n') {
        timeStr.pop_back(); // Remove newline character if present
    }
    return timeStr;
}
