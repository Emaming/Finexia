#ifndef DEBITCARD_H
#define DEBITCARD_H

#include "Card.h"

class DebitCard : public Card {
public:
    // Costruttore
    DebitCard(const std::string& name);

    // Metodo per verificare se una transazione può essere processata
    bool canProcessTransaction(double amount) const;

    // Override del metodo addOperation (implementazione del metodo virtuale puro)
    void addOperation(const std::shared_ptr<Operation>& op) override;
};

#endif // DEBITCARD_H
