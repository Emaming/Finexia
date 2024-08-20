#ifndef CARDOPERATION_H
#define CARDOPERATION_H

#include "Operation.h"
#include "CreditCard.h"
#include "DebitCard.h"
#include <memory>
#include <string>
#include <variant>
#include <chrono>

class CardOperation {
private:
    std::shared_ptr<Operation> operation;
    std::variant<std::monostate, std::shared_ptr<CreditCard>, std::shared_ptr<DebitCard>> card;
    std::string cardId;
    std::string cardNumber;
    std::string cvv;
    std::chrono::system_clock::time_point expirationDate;

public:
    // Constructor with all parameters
    CardOperation(std::shared_ptr<Operation> op,
                  std::variant<std::monostate, std::shared_ptr<CreditCard>, std::shared_ptr<DebitCard>> card,
                  const std::string& id,
                  const std::string& number,
                  const std::string& cvv,
                  std::chrono::system_clock::time_point expDate);

    // Constructor for a new card operation with random values
    CardOperation(const std::string& name);

    std::string getCardId() const;
    std::string getCardNumber() const;
    std::string getCvv() const;
    std::chrono::system_clock::time_point getExpirationDate() const;
    std::shared_ptr<Operation> getOperation() const;
    std::variant<std::monostate, std::shared_ptr<CreditCard>, std::shared_ptr<DebitCard>> getCard() const;

    std::string printCardString() const;
    bool isCreditCard() const;
    bool isDebitCard() const;

    std::string timePointToString(std::chrono::system_clock::time_point tp) const;
};

#endif // CARDOPERATION_H
