#include "Operation.h"
#include <sstream>
#include <iomanip>  // Per std::put_time
#include <stdexcept>
#include <iostream>

// Costruttore che inizializza tutti i membri
Operation::Operation(int id, double amount, OperationType type, std::chrono::system_clock::time_point date)
        : id(id), amount(amount), type(type), date(date) {}

// Costruttore che inizializza solo amount e type
Operation::Operation(double a, OperationType type)
        : amount(a), type(type), date(std::chrono::system_clock::now()) {
    if (a <= 0) {
        throw std::out_of_range("Amount not valid!");
    }
}

int Operation::getId() const { return id; }
double Operation::getAmount() const { return amount; }
OperationType Operation::getType() const { return type; }
std::chrono::system_clock::time_point Operation::getDate() const { return date; }



std::string Operation::printOperationType() const {
    switch (type) {
        case OperationType::Deposit:
            return "Deposit";
        case OperationType::Withdrawal:
            return "Withdrawal";
        case OperationType::Transfer:
            return "Transfer";
        default:
            return "Unknown";  // Gestione caso di tipo non riconosciuto
    }
}


