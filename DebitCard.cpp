#include "DebitCard.h"
#include "iostream"

DebitCard::DebitCard(const std::string& name)
        : Card(name,isCreditCard) {
    this->isCreditCard= false;
}


bool DebitCard::canProcessTransaction(double amount) const {
    return (amount <= getAmount());
}

void DebitCard::addOperation(const std::shared_ptr<Operation>& op) {
    if (op->getType() == OperationType::Withdrawal && !canProcessTransaction(op->getAmount())) {
        std::cerr << "Error: Insufficient funds" << std::endl;
        return;
    }
    Card::addOperation(op);
}
