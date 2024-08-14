//
// Created by user on 13/08/24.
//

#ifndef FINEXIA_BANKACCOUNT_H
#define FINEXIA_BANKACCOUNT_H

#include <iostream>
#include <vector>
#include <memory>
#include <list>
#include "chrono"
#include "Operation.h"
#include "CardOperation.h"
#include "ScheduledOperation.h"

class BankAccount {
public:
    BankAccount();
    ~BankAccount() = default;

    const std::vector<std::shared_ptr<Operation>> &getOperations() const;

    const std::vector<std::shared_ptr<ScheduledOperation>> &getScheduledOperations() const;

    const std::list<std::shared_ptr<CardOperation>> &getCardsOperations() const;

    const std::string &getIban() const;

    double getBalance() const;

    void addTransaction(const std::shared_ptr<Operation> &transaction);

    void addPScheduledOperation(const std::shared_ptr<ScheduledOperation>& transaction);

    void cancelOperations(const std::vector<std::shared_ptr<Operation>> &operationsToCancel);

    void removeScheduledOperation(const std::vector<std::shared_ptr<ScheduledOperation>> plannedTransactions);

    void addCard(std::string name);

    // Search functions

    std::vector<std::shared_ptr<Operation>> findOperationByAmount(double amount) const;

    std::vector<std::shared_ptr<ScheduledOperation>> findPlannedDate(double amount) const;

    std::vector<std::shared_ptr<Operation>> findOperationByDate(std::chrono::system_clock::time_point date) const;

    std::vector<std::shared_ptr<ScheduledOperation>> findPlannedDate(std::chrono::system_clock::time_point date) const;

    std::vector<std::shared_ptr<ScheduledOperation>> findNextExecutionDate(std::chrono::system_clock::time_point date) const;

    std::vector<std::shared_ptr<Operation>> findOperationByType(OperationType type) const;

    void executePlannedTransactions();

    void printOperations(const std::vector<std::shared_ptr<Operation>> &operations) const;

    void printPlannedTransactions(const std::vector<std::shared_ptr<ScheduledOperation>> &operations) const;

    void printCards() const;

    std::string printIban() const;

    std::string printBalance() const;

private:
    std::vector<std::shared_ptr<Operation>> operations;
    std::vector<std::shared_ptr<ScheduledOperation>> scheduledOperations;
    std::list<std::shared_ptr<CardOperation>> cardsOperations;
    std::string IBAN;
    double balance;
};


#endif //FINEXIA_BANKACCOUNT_H
