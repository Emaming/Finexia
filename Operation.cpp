#include "Operation.h"
#include <stdexcept>

// Costruttore con importo, tipo e data specificata
Operation::Operation(double amount, OperationType type, std::chrono::system_clock::time_point date)
        : amount(amount), type(type), date(date) {
    if (amount <= 0) {
        throw std::out_of_range("Amount not valid!");
    }
}

// Costruttore con importo e tipo, data corrente
Operation::Operation(double amount, OperationType type)
        : amount(amount), type(type), date(std::chrono::system_clock::now()) {
    if (amount <= 0) {
        throw std::out_of_range("Amount not valid!");
    }
}

// Costruttore con descrizione (ammontare di default a 0, tipo deposito)
Operation::Operation(const std::string& description)
        : amount(0), type(OperationType::Deposit), date(std::chrono::system_clock::now()), description(description) {}

// Costruttore con importo, tipo e descrizione, data corrente
Operation::Operation(double amount, OperationType type, const std::string& description)
        : amount(amount), type(type), date(std::chrono::system_clock::now()), description(description) {
    if (amount <= 0) {
        throw std::out_of_range("Amount not valid!");
    }
}

// Metodi get per i membri della classe
double Operation::getAmount() const {
    return amount;
}

OperationType Operation::getType() const {
    return type;
}

std::chrono::system_clock::time_point Operation::getDate() const {
    return date;
}

std::string Operation::getDescription() const {
    return description;
}

// Metodo per stampare il tipo di operazione come stringa
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

// Metodo set per la descrizione
void Operation::setDescription(const std::string &description) {
    Operation::description = description;
}
