#include "DebitCard.h"
#include <iostream>

// Costruttore
DebitCard::DebitCard(const std::string& name)
        : Card(name, false) { // Pass false direttamente al costruttore base
}

// Gestione delle transazioni
bool DebitCard::canProcessTransaction(double amount) const {
    return (amount <= getAmount());
}

bool DebitCard::addOperation(const std::shared_ptr<Operation>& op) {
    try {
        double operationAmount = op->getAmount();

        if (op->getType() == OperationType::Withdrawal && !canProcessTransaction(operationAmount)) {
            throw std::runtime_error("Error: Insufficient funds on the debit card");
        }

        // Aggiorna l'ammontare della carta
        if (op->getType() == OperationType::Deposit) {
            setAmount(getAmount() + operationAmount);
        } else if (op->getType() == OperationType::Withdrawal) {
            setAmount(getAmount() - operationAmount);
        }

        // Aggiungi l'operazione alla lista delle operazioni della carta
        Card::addOperation(op);
        return true;
    } catch (const std::exception& e) {
        std::cerr << e.what() << std::endl;
        return false;
    }
}
