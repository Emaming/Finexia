#include <iostream>
#include <memory>
#include <chrono>
#include "BankAccount.h"
#include "Operation.h"
#include "ScheduledOperation.h"
#include "CardOperation.h"

int main() {
    // Creazione di un nuovo conto bancario
    BankAccount myAccount;

    // Creazione di una carta e aggiunta al conto
    auto cardOperation = std::make_shared<CardOperation>("Visa");
    myAccount.addCard(cardOperation);

    // Creazione di una transazione di deposito
    auto deposit = std::make_shared<Operation>(1, 500.0, OperationType::Deposit, std::chrono::system_clock::now());
    myAccount.addTransaction(deposit);

    // Creazione di una transazione di prelievo
    auto withdrawal = std::make_shared<Operation>(2, 200.0, OperationType::Withdrawal, std::chrono::system_clock::now());
    myAccount.addTransaction(withdrawal);

    // Creazione di un'operazione pianificata (bonifico) da eseguire in futuro
    auto futureDate = std::chrono::system_clock::now() + std::chrono::hours(24); // Domani
    auto scheduledTransfer = std::make_shared<ScheduledOperation>(deposit, futureDate, Frequency::One); // Usare `deposit` come esempio di operazione
    myAccount.addPScheduledOperation(scheduledTransfer);

    // Esecuzione delle operazioni pianificate (se la data programmata è passata)
    myAccount.executePlannedTransactions();

    // Stampa delle operazioni eseguite
    myAccount.printOperations(myAccount.findOperationByType(OperationType::Deposit)); // Esempio di ricerca operazioni di deposito

    // Stampa delle operazioni pianificate
    myAccount.printPlannedTransactions(myAccount.findPlannedDate(futureDate));

    // Stampa delle carte
    myAccount.printCards();

    // Stampa dell'IBAN
    std::cout << myAccount.printIban() << std::endl;

    // Stampa del saldo attuale
    std::cout << myAccount.printBalance() << std::endl;

    return 0;
}
