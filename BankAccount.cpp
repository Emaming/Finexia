#include "BankAccount.h"
#include <fstream>
#include <sstream>
#include <iostream>
#include <random>
#include <chrono>
#include <iomanip>
#include <algorithm> // Per std::find_if

BankAccount::BankAccount() : balance(0.0) {}

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

    // Salvataggio carte
    file << cardsOperations.size() << '\n';
    for (const auto& card : cardsOperations) {
        file
                << card->getCardName() << ' '
                << card->getCardNumber() << ' '
                << card->getCvv() << ' '
                << std::chrono::system_clock::to_time_t(card->getExpirationDate()) << ' '
                << card->getAmount() << '\n';
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

    // Load cards
    size_t numCards;
    file >> numCards;
    cardsOperations.clear();
    for (size_t i = 0; i < numCards; ++i) {
        std::string cardName;
        std::string cardNumber;
        std::string cvv;
        std::time_t expDate;
        double amount;

        file >> cardName >> cardNumber >> cvv >> expDate >> amount;

        auto card = std::make_shared<Card>(cardName);
        card->setCardNumber(cardNumber);
        card->setCvv(cvv);
        card->setExpirationDate(std::chrono::system_clock::from_time_t(expDate));
        card->setAmount(amount);

        cardsOperations.push_back(card);
    }

    file.close();
}

void BankAccount::removeCard(const std::string& cardName) {
    auto it = std::remove_if(cardsOperations.begin(), cardsOperations.end(),
                             [&cardName](const std::shared_ptr<Card>& card) {
                                 return card->getCardName() == cardName;
                             });
    cardsOperations.erase(it, cardsOperations.end());
}

void BankAccount::addCard(const std::string& cardName, bool isCredit) {
    auto card = std::make_shared<Card>(cardName);
    cardsOperations.push_back(card);
}

void BankAccount::addTransaction(const std::shared_ptr<Operation>& transaction) {
    // Aggiungi l'operazione alla lista
    operations.push_back(transaction);

    // Aggiorna il saldo
    if (transaction->getType() == OperationType::Deposit) {
        balance += transaction->getAmount();
    } else if (transaction->getType() == OperationType::Withdrawal) {
        balance -= transaction->getAmount();
    }
}

void BankAccount::addOperationToCard(const std::string& cardName, const std::shared_ptr<Operation>& operation) {
    auto cardIt = std::find_if(cardsOperations.begin(), cardsOperations.end(),
                               [&cardName](const std::shared_ptr<Card>& card) {
                                   return card->getCardName() == cardName;
                               });

    if (cardIt != cardsOperations.end()) {
        (*cardIt)->addOperation(operation);

        // Aggiorna il saldo del conto corrente se l'operazione è un deposito o prelievo
        if (operation->getType() == OperationType::Deposit) {
            balance -= operation->getAmount();  // Il conto corrente diminuisce
            (*cardIt)->setAmount((*cardIt)->getAmount() + operation->getAmount()); // La carta aumenta
        } else if (operation->getType() == OperationType::Withdrawal) {
            balance += operation->getAmount();  // Il conto corrente aumenta
            (*cardIt)->setAmount((*cardIt)->getAmount() - operation->getAmount()); // La carta diminuisce
        }
    } else {
        std::cerr << "Card not found. Cannot add operation." << std::endl;
    }
}

void BankAccount::cancelOperations(const std::vector<std::shared_ptr<Operation>>& operationsToCancel) {
    for (const auto& op : operationsToCancel) {
        auto it = std::remove_if(operations.begin(), operations.end(),
                                 [&op](const std::shared_ptr<Operation>& o) {
                                     return o == op;
                                 });
        operations.erase(it, operations.end());
    }
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

void BankAccount::scheduleOperation(const std::shared_ptr<Operation>& operation, std::chrono::system_clock::time_point startDate, Frequency frequency) {
    auto schedOp = std::make_shared<ScheduledOperation>(operation, startDate, frequency);
    scheduledOperations.push_back(schedOp);
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

std::vector<std::shared_ptr<ScheduledOperation>> BankAccount::findScheduledByType(OperationType type) const {
    std::vector<std::shared_ptr<ScheduledOperation>> result;
    for (const auto& schedOp : scheduledOperations) {
        if (schedOp->getOperation()->getType() == type) {
            result.push_back(schedOp);
        }
    }
    return result;
}

std::string BankAccount::printBalance() const {
    std::ostringstream oss;
    oss << "Balance: " << balance;
    return oss.str();
}

void BankAccount::printOperations(const std::vector<std::shared_ptr<Operation>>& operations) const {
    for (const auto& op : operations) {
        std::cout << "Amount: " << op->getAmount()
                  << ", Type: " << static_cast<int>(op->getType())
                  << ", Date: " << std::chrono::system_clock::to_time_t(op->getDate())
                  << '\n';
    }
}

std::vector<std::shared_ptr<Card>> BankAccount::getCardsOperations() const {
    return cardsOperations;
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
                      << ", Type: " << op->printOperationType()
                      << ", Date: " << std::chrono::system_clock::to_time_t(op->getDate())
                      << ", Description: " << op->getDescription()
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

std::string BankAccount::getTransactionHistory() const {
    std::ostringstream oss;
    for (const auto& op : operations) {
        oss << "Amount: " << op->getAmount()
            << ", Type: " << static_cast<int>(op->getType())
            << ", Date: " << std::chrono::system_clock::to_time_t(op->getDate())
            << '\n';
    }
    return oss.str();
}

void BankAccount::printPlannedTransactions() const {
    for (const auto& schedOp : scheduledOperations) {
        std::cout << "Operation Amount: " << schedOp->getOperation()->getAmount()
                  << ", Type: " << static_cast<int>(schedOp->getOperation()->getType())
                  << ", Operation Date: " << std::chrono::system_clock::to_time_t(schedOp->getOperation()->getDate())
                  << ", Scheduled Execution Date: " << std::chrono::system_clock::to_time_t(schedOp->getScheduledExecutionDate())
                  << ", Frequency: " << static_cast<int>(schedOp->getFrequency())
                  << '\n';
    }
}

std::string BankAccount::timePointToString(std::chrono::system_clock::time_point tp) const {
    std::time_t time = std::chrono::system_clock::to_time_t(tp);
    std::ostringstream oss;
    oss << std::put_time(std::localtime(&time), "%Y-%m-%d %H:%M:%S");
    return oss.str();
}

double BankAccount::getBalance() const {
    return balance;
}

