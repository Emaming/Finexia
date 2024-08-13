//
// Created by user on 13/08/24.
//

#include "BankAccount.h"

const std::vector<std::shared_ptr<Operation>> &BankAccount::getOperations() const {
    return operations;
}

const std::vector<std::shared_ptr<ScheduledOperations>> &BankAccount::getScheduledOperations() const {
    return scheduledOperations;
}

const std::list<std::shared_ptr<CardOperations>> &BankAccount::getCardsOperations() const {
    return cardsOperations;
}

const std::string &BankAccount::getIban() const {
    return IBAN;
}

double BankAccount::getBalance() const {
    return balance;
}
