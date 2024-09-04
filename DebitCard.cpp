#include "DebitCard.h"
#include <iostream>

// Costruttore
DebitCard::DebitCard(const std::string& name)
        : Card(name, isCreditCard) {
    this->isCreditCard = false;
}

// Verifica se la transazione può essere processata
bool DebitCard::canProcessTransaction(double amount) const {
    return (amount <= getAmount());
}

// Override del metodo addOperation
void DebitCard::addOperation(const std::shared_ptr<Operation>& op) {
    // Controllo dei fondi per prelievi
    if (op->getType() == OperationType::Withdrawal && !canProcessTransaction(op->getAmount())) {
        std::cerr << "Error: Insufficient funds" << std::endl;
        return;
    }

    // Aggiungi l'operazione alla lista delle operazioni della carta
    Card::addOperation(op);
}
