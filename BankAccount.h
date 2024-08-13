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
#include "CardOperations.h"
#include "ScheduledOperations.h"

class BankAccount {
public:
    BankAccount();
    ~BankAccount() = default;

    const std::vector<std::shared_ptr<Operation>> &getOperations() const;

    const std::vector<std::shared_ptr<ScheduledOperations>> &getScheduledOperations() const;

    const std::list<std::shared_ptr<CardOperations>> &getCardsOperations() const;

    const std::string &getIban() const;

    double getBalance() const;

    //

    // Search functions

    std::vector<std::shared_ptr<Operation>> findOperationByAmount(double amount) const;

    std::vector<std::shared_ptr<ScheduledOperations>> findPlannedDate(double amount) const;

    std::vector<std::shared_ptr<Operation>> findOperationByDate(std::chrono::system_clock::time_point date) const;

    std::vector<std::shared_ptr<ScheduledOperations>> findPlannedDate(std::chrono::system_clock::time_point date) const;

    std::vector<std::shared_ptr<ScheduledOperations>> findNextExecutionDate(std::chrono::system_clock::time_point date) const;

    std::vector<std::shared_ptr<Operation>> findOperationByType(OperationType type) const;

private:
    std::vector<std::shared_ptr<Operation>> operations;
    std::vector<std::shared_ptr<ScheduledOperations>> scheduledOperations;
    std::list<std::shared_ptr<CardOperations>> cardsOperations;
    std::string IBAN;
    double balance;
};


#endif //FINEXIA_BANKACCOUNT_H
