#ifndef DEBITCARD_H
#define DEBITCARD_H

#include "Card.h"

class DebitCard : public Card {
public:
    // Constructor
    DebitCard(const std::string& name);

    // Method to check if a transaction can be processed
    bool canProcessTransaction(double amount) const;

    // Override methods if needed
    void addOperation(const std::shared_ptr<Operation>& op) override;
};

#endif // DEBITCARD_H
