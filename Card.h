#ifndef CARD_H
#define CARD_H

#include "Operation.h"
#include <string>
#include <chrono>
#include <list>
#include <memory>

class Card {
protected:
    std::string cardName;
    std::string cardNumber;
    std::string cvv;
    std::string pin;
    std::chrono::system_clock::time_point expirationDate;
    double amount;
    std::list<std::shared_ptr<Operation>> cardOperations;
    bool isCreditCard;

    // Metodi per generare i dettagli della carta
    std::string generateCardNumber();
    std::string generateCvv();
    std::string generatePin();
    std::chrono::system_clock::time_point generateExpirationDate();

public:
    // Costruttore che accetta solo il nome
    Card(const std::string& name,bool isCredit);
    void setAmount(double newAmount);
    const std::string& getCardName() const;
    const std::string& getCardNumber() const;
    const std::string& getCvv() const;
    double getAmount() const;
    const std::string& getPin() const;
    std::chrono::system_clock::time_point getExpirationDate() const;

    std::list<std::shared_ptr<Operation>> getOperations() const;

    virtual void addOperation(const std::shared_ptr<Operation>& op);
    void setCardNumber(const std::string& number);
    void setCvv(const std::string& cvv);
    void setExpirationDate(std::chrono::system_clock::time_point expDate);

    bool isCreditCardBool() const;

    virtual ~Card() = default;
};

#endif // CARD_H