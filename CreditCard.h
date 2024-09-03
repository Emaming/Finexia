#ifndef CREDITCARD_H
#define CREDITCARD_H

#include "Card.h"

class CreditCard : public Card {
private:
    double creditLimit;
    double interestRate;

public:
    // Constructor
    CreditCard(const std::string& name, double limit, double rate);

    // Getters and setters
    double getCreditLimit() const;
    void setCreditLimit(double limit);

    double getInterestRate() const;
    void setInterestRate(double rate);

    // Method to check if a transaction can be processed
    bool canProcessTransaction(double amount) const;

    // Override methods if needed
    void addOperation(const std::shared_ptr<Operation>& op) override;
};

#endif // CREDITCARD_H
