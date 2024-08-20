#ifndef BANKACCOUNT_H
#define BANKACCOUNT_H

#include <vector>
#include <memory>
#include <string>
#include <chrono>

#include "Operation.h"
#include "ScheduledOperation.h"
#include "CardOperation.h"

class BankAccount {
private:
    double balance;
    std::string IBAN;
    std::vector<std::shared_ptr<Operation>> operations;
    std::vector<std::shared_ptr<ScheduledOperation>> scheduledOperations;
    std::vector<std::shared_ptr<CardOperation>> cardsOperations;

public:
    BankAccount();

    void saveToFile(const std::string& filename) const;
    void loadFromFile(const std::string& filename);

    void addCard(const std::string& cardName, bool isCredit = false);  // isCredit defaults to false
    void addTransaction(const std::shared_ptr<Operation>& transaction);
    void addScheduledOperation(const std::shared_ptr<ScheduledOperation>& operation);
    void cancelOperations(const std::vector<std::shared_ptr<Operation>>& operationsToCancel);
    void removeScheduledOperation(const std::vector<std::shared_ptr<ScheduledOperation>>& scheduledOperationsToRemove);

    std::vector<std::shared_ptr<Operation>> findOperationByAmount(double amount) const;
    std::vector<std::shared_ptr<Operation>> findOperationByType(OperationType type) const;
    std::vector<std::shared_ptr<Operation>> findOperationByDate(std::chrono::system_clock::time_point date) const;
    std::vector<std::shared_ptr<ScheduledOperation>> findScheduledByAmount(double amount) const;
    std::vector<std::shared_ptr<ScheduledOperation>> findScheduledByDate(std::chrono::system_clock::time_point date) const;
    std::vector<std::shared_ptr<ScheduledOperation>> findNextExecutionDate(std::chrono::system_clock::time_point date) const;

    void executePlannedTransactions();
    void printOperations(const std::vector<std::shared_ptr<Operation>>& operations) const;
    void printPlannedTransactions(const std::vector<std::shared_ptr<ScheduledOperation>>& operations) const;
    void printCards() const;  // Updated method name

    std::string printIban() const;
    std::string printBalance() const;
    std::string getTransactionHistory() const;

private:
    std::string timePointToString(std::chrono::system_clock::time_point tp) const;
};

#endif // BANKACCOUNT_H
