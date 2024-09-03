#ifndef BANKACCOUNT_H
#define BANKACCOUNT_H

#include <vector>
#include <memory>
#include <string>
#include <chrono>
#include <list>
#include <iostream>
#include "Operation.h"
#include "ScheduledOperation.h"
#include "Card.h"

class BankAccount {
private:
    double balance;
    std::string IBAN;
    std::list<std::shared_ptr<Operation>> operations;
    std::list<std::shared_ptr<ScheduledOperation>> scheduledOperations;
    std::vector<std::shared_ptr<Card>> cardsOperations;

public:
    // Costruttore
    BankAccount();
    void generateIBAN();

    // Getter
    double getBalance() const;
    std::vector<std::shared_ptr<Card>> getCardsOperations() const;

    // Metodi di salvataggio e caricamento
    void saveToFile(const std::string& filename) const;
    void loadFromFile(const std::string& filename);

    // Metodi per le operazioni normali
    void addTransaction(const std::shared_ptr<Operation>& transaction);
    void removeOperations(const std::vector<std::shared_ptr<Operation>>& operationsToCancel);
    std::vector<std::shared_ptr<Operation>> findOperationByAmount(double amount) const;
    std::vector<std::shared_ptr<Operation>> findOperationByType(OperationType type) const;
    std::vector<std::shared_ptr<Operation>> findOperationByDate(std::chrono::system_clock::time_point date) const;
    std::string getTransactionHistory() const;
    void printOperations(const std::vector<std::shared_ptr<Operation>>& operations) const;

    // Metodi per le operazioni programmate
    void addScheduleOperation(const std::shared_ptr<Operation>& operation, std::chrono::system_clock::time_point startDate, Frequency frequency);
    void removeScheduledOperation(const std::vector<std::shared_ptr<ScheduledOperation>>& scheduledOperationsToRemove);
    std::vector<std::shared_ptr<ScheduledOperation>> findScheduledByAmount(double amount) const;
    std::vector<std::shared_ptr<ScheduledOperation>> findScheduledByDate(std::chrono::system_clock::time_point date) const;
    std::vector<std::shared_ptr<ScheduledOperation>> findScheduledByType(OperationType type) const;
    void printPlannedTransactions() const;
    void executeScheduledOperations();

    // Metodi per le carte
    void addCard(const std::string& cardName, bool isCredit = false);
    void removeCard(const std::string& cardName);
    void addOperationToCard(const std::string& cardName, const std::shared_ptr<Operation>& operation);
    void printCardOperations(const std::string& cardName) const;
    void printCards() const;

    // Metodi di conversione
    std::string operationTypeToString(OperationType type) const;
    std::string frequencyToString(Frequency freq) const;

    // Metodi di stampa e formattazione
    std::string printBalance() const;
    std::string timePointToString(std::chrono::system_clock::time_point tp) const;

    // Nuovi metodi per contare le operazioni
    int getNormalOperationCount() const;
    int getScheduledOperationCount() const;
    int getCardOperationCount() const;
    std::string getIBAN() const;
};

#endif // BANKACCOUNT_H
