#ifndef CARDOPERATION_H
#define CARDOPERATION_H

#include "Operation.h"
#include <memory>
#include <string>

class CardOperation {
private:
    std::shared_ptr<Operation> operation;
    std::string cardId;
    std::string cardNumber; // Aggiunto
    int cvv;                // Aggiunto
    int pin;                // Aggiunto
    int maximum;            // Aggiunto
    std::string cardName;   // Aggiunto

public:
    CardOperation(std::shared_ptr<Operation> op, const std::string& id);
    CardOperation(const std::string& name); // Aggiunto

    std::string getCardId() const { return cardId; }
    std::shared_ptr<Operation> getOperation() const { return operation; }

    std::string printCardString() const; // Aggiunto
};

#endif // CARDOPERATION_H
