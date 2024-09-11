#ifndef CARD_H
#define CARD_H

#include "Operation.h"
#include <string>
#include <chrono>
#include <list>
#include <memory>

class Card {
protected:
    // Attributi della carta
    std::string cardName;
    std::string cardNumber;
    std::string cvv;
    std::string pin;
    std::chrono::system_clock::time_point expirationDate;
    double amount;
    std::list<std::shared_ptr<Operation>> cardOperations;

    // Metodi per generare i dettagli della carta
    std::string generateCardNumber();
    std::string generateCvv();
    std::string generatePin();
    std::chrono::system_clock::time_point generateExpirationDate();

public:
    // Costruttore
    Card(const std::string& name, bool isCredit);

    // Metodi di set e get
    void setAmount(double newAmount);
    const std::string& getCardName() const;
    const std::string& getCardNumber() const;
    const std::string& getCvv() const;
    const std::string& getPin() const;
    double getAmount() const;
    std::chrono::system_clock::time_point getExpirationDate() const;

    // Metodi di gestione delle operazioni
    std::list<std::shared_ptr<Operation>> getOperations() const;
    void removeLastOperation();

    // Metodo virtuale puro per aggiungere operazioni (classe astratta)
    virtual void addOperation(const std::shared_ptr<Operation>& op) = 0;

    // Metodi di set per i dettagli della carta
    void setCardNumber(const std::string& number);
    void setCvv(const std::string& cvv);
    void setExpirationDate(std::chrono::system_clock::time_point expDate);

    // Altri metodi
    bool isCreditCardBool() const;
    int getOperationSize();

    // Distruttore virtuale
    virtual ~Card() = default;

    bool isCreditCard;
};

#endif // CARD_H
