#ifndef CREDITCARD_H
#define CREDITCARD_H

#include "Card.h"

class CreditCard : public Card {
private:
    double creditLimit;
    double interestRate;

public:
    // Costruttore
    CreditCard(const std::string& name, double limit, double rate);

    // Getters e setters
    double getCreditLimit() const;
    void setCreditLimit(double limit);

    double getInterestRate() const;
    void setInterestRate(double rate);

    // Metodo per verificare se una transazione può essere processata
    bool canProcessTransaction(double amount) const;

    // Override del metodo addOperation (implementazione del metodo virtuale puro)
    bool addOperation(const std::shared_ptr<Operation>& op) override;

};

#endif // CREDITCARD_H
