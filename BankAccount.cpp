//
// Created by user on 13/08/24.
//

#include "BankAccount.h"

const std::vector<std::shared_ptr<Operation>> &BankAccount::getOperations() const {
    return operations;
}

const std::vector<std::shared_ptr<ScheduledOperation>> &BankAccount::getScheduledOperations() const {
    return scheduledOperations;
}

const std::list<std::shared_ptr<CardOperation>> &BankAccount::getCardsOperations() const {
    return cardsOperations;
}

const std::string &BankAccount::getIban() const {
    return IBAN;
}

double BankAccount::getBalance() const {
    return balance;
}

std::string BankAccount::printBalance() const {
    return std::string();
}

std::string BankAccount::printIban() const {
    return std::string();
}

void BankAccount::printCards() const {

}

void BankAccount::printPlannedTransactions(const std::vector<std::shared_ptr<ScheduledOperation>> &operations) const {

}

void BankAccount::printOperations(const std::vector<std::shared_ptr<Operation>> &operations) const {

}

void BankAccount::executePlannedTransactions() {

}

std::vector<std::shared_ptr<Operation>> BankAccount::findOperationByType(OperationType type) const {
    return std::vector<std::shared_ptr<Operation>>();
}

std::vector<std::shared_ptr<ScheduledOperation>>
BankAccount::findNextExecutionDate(std::chrono::system_clock::time_point date) const {
    return std::vector<std::shared_ptr<ScheduledOperation>>();
}

std::vector<std::shared_ptr<ScheduledOperation>>
BankAccount::findPlannedDate(std::chrono::system_clock::time_point date) const {
    return std::vector<std::shared_ptr<ScheduledOperation>>();
}

std::vector<std::shared_ptr<Operation>>
BankAccount::findOperationByDate(std::chrono::system_clock::time_point date) const {
    return std::vector<std::shared_ptr<Operation>>();
}

std::vector<std::shared_ptr<Operation>> BankAccount::findOperationByAmount(double amount) const {
    return std::vector<std::shared_ptr<Operation>>();
}

std::vector<std::shared_ptr<ScheduledOperation>> BankAccount::findPlannedDate(double amount) const {
    return std::vector<std::shared_ptr<ScheduledOperation>>();
}

void BankAccount::addCard(std::string name) {

}

void BankAccount::removeScheduledOperation(const std::vector<std::shared_ptr<ScheduledOperation>> plannedTransactions) {

}

void BankAccount::cancelOperations(const std::vector<std::shared_ptr<Operation>> &operationsToCancel) {

}

void BankAccount::addPScheduledOperation(const std::shared_ptr<ScheduledOperation> &transaction) {

}

void BankAccount::addTransaction(const std::shared_ptr<Operation> &transaction) {

}

BankAccount::BankAccount() {

}
