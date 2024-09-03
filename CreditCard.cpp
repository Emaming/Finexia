#include "CreditCard.h"
#include <iostream>

CreditCard::CreditCard(const std::string& name, double limit, double rate)
        : Card(name), creditLimit(limit), interestRate(rate) {
}

// Getters and setters
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

bool CreditCard::canProcessTransaction(double amount) const {
    return (amount <= creditLimit - getAmount()); // Il saldo può essere negativo fino al limite di credito
}

void CreditCard::addOperation(const std::shared_ptr<Operation>& op) {
    if (op->getType() == OperationType::Withdrawal && !canProcessTransaction(op->getAmount())) {
        std::cerr << "Error: Exceeds credit limit" << std::endl;
        return;
    }
    // Aggiorna l'ammontare della carta
    if (op->getType() == OperationType::Deposit) {
        setAmount(getAmount() + op->getAmount());
    } else if (op->getType() == OperationType::Withdrawal) {
        setAmount(getAmount() - op->getAmount());
    }
    // Aggiungi l'operazione alla lista delle operazioni della carta
    Card::addOperation(op);
}
