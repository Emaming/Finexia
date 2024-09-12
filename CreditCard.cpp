#include "CreditCard.h"
#include <iostream>

#define DEF void

// Costruttore
CreditCard::CreditCard(const std::string& name, double limit, double rate)
        : Card(name, isCreditCard), creditLimit(limit), interestRate(rate) {
    this->isCreditCard = true;
    this->creditLimit = 1000.0;
}

// Getters e setters
double CreditCard::getCreditLimit() const {
    return creditLimit;
}

void CreditCard::setCreditLimit(double limit) {
    creditLimit = limit;
}

double CreditCard::getInterestRate() const {
    return interestRate;
}

void CreditCard::setInterestRate(double rate) {
    interestRate = rate;
}

// Gestione delle transazioni
bool CreditCard::canProcessTransaction(double amount) const {
    return (amount <= creditLimit - getAmount());
}

bool CreditCard::addOperation(const std::shared_ptr<Operation>& op) {
    try {
        double operationAmount = op->getAmount();
        if (op->getType() == OperationType::Withdrawal && !canProcessTransaction(operationAmount)) {
            throw std::runtime_error("Error: Exceeds credit limit");
        }
        // Aggiorna l'ammontare della carta
        if (op->getType() == OperationType::Deposit) {
            setAmount(getAmount() + operationAmount);
        } else if (op->getType() == OperationType::Withdrawal) {
            setAmount(getAmount() - operationAmount);
        }
        // Aggiungi l'operazione alla lista delle operazioni della carta
        Card::addOperation(op);

        return true; // L'operazione è andata a buon fine
    } catch (const std::exception& e) {
        std::cerr << e.what() << std::endl;
        return false; // Si è verificato un errore
    }
}


