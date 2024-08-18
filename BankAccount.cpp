#include "BankAccount.h"
#include <random>
#include <sstream>
#include <iostream>
#include <iomanip>
#include <stdexcept>
#include <algorithm>

BankAccount::BankAccount() : balance(0.0) {
    std::random_device rd;
    std::mt19937 gen(rd());
    std::uniform_int_distribution<int> dis(0, 9);
    std::uniform_int_distribution<int> letter_dis(0, 25);

    std::stringstream ibanStream;
    ibanStream << "IT";
    ibanStream << dis(gen) << dis(gen);
    char letters[] = "ABCDEFGHIJKLMNOPQRSTUVWXYZ";
    ibanStream << letters[letter_dis(gen)];
    for (int i = 0; i < 22; ++i) {
        ibanStream << dis(gen);
    }
    IBAN = ibanStream.str();
}

void BankAccount::addTransaction(const std::shared_ptr<Operation>& transaction) {
    double newBalance = balance;
    if (transaction->getType() == OperationType::Deposit) {
        newBalance += transaction->getAmount();
    } else if (transaction->getType() == OperationType::Withdrawal ||
               transaction->getType() == OperationType::Transfer) {
        newBalance -= transaction->getAmount();
    }

    if (newBalance < 0) {
        throw std::runtime_error("The transaction exceeds the available balance!");
    } else {
        balance = newBalance;
        operations.push_back(transaction);
    }
}

void BankAccount::addPScheduledOperation(const std::shared_ptr<ScheduledOperation>& operation) {
    scheduledOperations.push_back(operation);
}

void BankAccount::cancelOperations(const std::vector<std::shared_ptr<Operation>>& operationsToCancel) {
    double newBalance = balance;
    for (const auto& operation : operationsToCancel) {
        auto it = std::find(operations.begin(), operations.end(), operation);
        if (it != operations.end()) {
            if (operation->getType() == OperationType::Deposit) {
                newBalance -= operation->getAmount();
            } else {
                newBalance += operation->getAmount();
            }
        }
    }

    if (newBalance < 0) {
        throw std::runtime_error("The cancellation exceeds the available balance!");
    }

    balance = newBalance;
    for (const auto& operation : operationsToCancel) {
        auto it = std::find(operations.begin(), operations.end(), operation);
        if (it != operations.end()) {
            operations.erase(it);
        }
    }
}

void BankAccount::removeScheduledOperation(const std::vector<std::shared_ptr<ScheduledOperation>>& scheduledOperationsToRemove) {
    for (const auto& operation : scheduledOperationsToRemove) {
        auto it = std::find(scheduledOperations.begin(), scheduledOperations.end(), operation);
        if (it != scheduledOperations.end()) {
            scheduledOperations.erase(it);
        }
    }
}

std::vector<std::shared_ptr<Operation>> BankAccount::findOperationByAmount(double amount) const {
    if (amount <= 0) {
        throw std::invalid_argument("Amount not valid!");
    }
    std::vector<std::shared_ptr<Operation>> result;

    for (const auto& operation : operations) {
        if (std::fabs(operation->getAmount() - amount) < 1e-5) {
            result.push_back(operation);
        }
    }
    return result;
}

std::vector<std::shared_ptr<ScheduledOperation>> BankAccount::findPlannedDate(double amount) const {
    if (amount <= 0) {
        throw std::invalid_argument("Amount not valid!");
    }
    std::vector<std::shared_ptr<ScheduledOperation>> result;

    for (const auto& operation : scheduledOperations) {
        if (std::fabs(operation->getOperation()->getAmount() - amount) < 1e-5) {
            result.push_back(operation);
        }
    }
    return result;
}

std::vector<std::shared_ptr<Operation>> BankAccount::findOperationByDate(std::chrono::system_clock::time_point date) const {
    std::vector<std::shared_ptr<Operation>> result;

    for (const auto& operation : operations) {
        if (operation->getDate() == date) {
            result.push_back(operation);
        }
    }
    return result;
}

std::vector<std::shared_ptr<ScheduledOperation>> BankAccount::findPlannedDate(std::chrono::system_clock::time_point date) const {
    std::vector<std::shared_ptr<ScheduledOperation>> result;

    for (const auto& operation : scheduledOperations) {
        if (operation->getScheduledExecutionDate() == date) {
            result.push_back(operation);
        }
    }
    return result;
}

std::vector<std::shared_ptr<ScheduledOperation>> BankAccount::findNextExecutionDate(std::chrono::system_clock::time_point date) const {
    std::vector<std::shared_ptr<ScheduledOperation>> result;

    for (const auto& operation : scheduledOperations) {
        if (operation->getScheduledExecutionDate() > date) {
            result.push_back(operation);
        }
    }
    return result;
}

std::vector<std::shared_ptr<Operation>> BankAccount::findOperationByType(OperationType type) const {
    std::vector<std::shared_ptr<Operation>> result;

    for (const auto& operation : operations) {
        if (operation->getType() == type) {
            result.push_back(operation);
        }
    }
    return result;
}

void BankAccount::executePlannedTransactions() {
    auto now = std::chrono::system_clock::now();
    std::vector<std::shared_ptr<ScheduledOperation>> operationsToExecute;

    for (const auto& scheduledOperation : scheduledOperations) {
        if (scheduledOperation->getScheduledExecutionDate() <= now) {
            try {
                addTransaction(scheduledOperation->getOperation());
                operationsToExecute.push_back(scheduledOperation);
            } catch (const std::exception& e) {
                std::cerr << "Failed to execute scheduled operation: " << e.what() << std::endl;
            }
        }
    }

    removeScheduledOperation(operationsToExecute);
}

void BankAccount::printOperations(const std::vector<std::shared_ptr<Operation>>& operations) const {
    for (const auto& operation : operations) {
        std::cout << "Operation ID: " << operation->getId() << ", Amount: " << operation->getAmount()
                  << ", Type: " << (operation->getType() == OperationType::Deposit ? "Deposit" : "Withdrawal")
                  << ", Date: " << timePointToString(operation->getDate()) << std::endl;
    }
}

void BankAccount::printPlannedTransactions(const std::vector<std::shared_ptr<ScheduledOperation>>& operations) const {
    for (const auto& operation : operations) {
        std::cout << "Scheduled Operation ID: " << operation->getOperation()->getId() << ", Amount: " << operation->getOperation()->getAmount()
                  << ", Scheduled Date: " << timePointToString(operation->getScheduledExecutionDate()) << std::endl;
    }
}

void BankAccount::printCards() const {
    for (const auto& cardOperation : cardsOperations) {
        std::cout << "Card ID: " << cardOperation->getCardId() << ", Operation Amount: " << cardOperation->getOperation()->getAmount() << std::endl;
    }
}

std::string BankAccount::printIban() const {
    return "IBAN: " + IBAN;
}

std::string BankAccount::printBalance() const {
    std::stringstream ss;
    ss << "Balance: " << balance;
    return ss.str();
}

std::string BankAccount::timePointToString(std::chrono::system_clock::time_point tp) const {
    std::time_t time = std::chrono::system_clock::to_time_t(tp);
    std::tm* tm = std::localtime(&time);
    std::stringstream ss;
    ss << std::put_time(tm, "%Y-%m-%d %H:%M:%S");
    return ss.str();
}
void BankAccount::addCard(const std::shared_ptr<CardOperation>& cardOperation) {
    cardsOperations.push_back(cardOperation);
}
