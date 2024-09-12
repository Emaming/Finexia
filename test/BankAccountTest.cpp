#include <gtest/gtest.h>
#include "../BankAccount.h"
#include "../CreditCard.h"
#include <chrono>
#include <memory>
#include <iostream>  // Necessario per i messaggi di output

// Funzione helper per creare un shared_ptr<Operation>
std::shared_ptr<Operation> createOperation(double amount, OperationType type, std::chrono::system_clock::time_point date) {
    return std::make_shared<Operation>(amount, type, date);
}

// Funzione helper per creare un shared_ptr<ScheduledOperation>
std::shared_ptr<ScheduledOperation> createScheduledOperation(double amount, OperationType type, std::chrono::system_clock::time_point date, Frequency freq) {
    auto op = std::make_shared<ScheduledOperation>(amount, type, date, freq);
    op->setScheduledExecutionDate(date);
    return op;
}

class BankAccountTest : public ::testing::Test {
protected:
    BankAccount account;
};

// Test del costruttore e IBAN
TEST_F(BankAccountTest, ConstructorGeneratesIBAN) {
    EXPECT_FALSE(account.getIBAN().empty()) << "Errore: IBAN non generato correttamente.";
    if (!account.getIBAN().empty()) {
        std::cout << "IBAN generato con successo." << std::endl;
    }
}

// Test di saldo iniziale
TEST_F(BankAccountTest, InitialBalanceIsZero) {
    EXPECT_EQ(account.getBalance(), 0.0) << "Errore: Il saldo iniziale non è zero.";
    if (account.getBalance() == 0.0) {
        std::cout << "Il saldo iniziale è correttamente pari a zero." << std::endl;
    }
}

// Test di deposito
TEST_F(BankAccountTest, DepositIncreasesBalance) {
    auto deposit = createOperation(100.0, OperationType::Deposit, std::chrono::system_clock::now());
    account.addTransaction(deposit);

    EXPECT_EQ(account.getBalance(), 100.0) << "Errore: Il saldo non è aumentato correttamente dopo il deposito.";
    EXPECT_EQ(account.getNormalOperationCount(), 1) << "Errore: Il numero di operazioni normali non è corretto.";

    if (account.getBalance() == 100.0 && account.getNormalOperationCount() == 1) {
        std::cout << "Deposito effettuato con successo." << std::endl;
    }
}

// Test di prelievo
TEST_F(BankAccountTest, WithdrawalDecreasesBalance) {
    auto deposit = createOperation(100.0, OperationType::Deposit, std::chrono::system_clock::now());
    account.addTransaction(deposit);

    auto withdrawal = createOperation(50.0, OperationType::Withdrawal, std::chrono::system_clock::now());
    account.addTransaction(withdrawal);

    EXPECT_EQ(account.getBalance(), 50.0) << "Errore: Il saldo non è diminuito correttamente dopo il prelievo.";
    EXPECT_EQ(account.getNormalOperationCount(), 2) << "Errore: Il numero di operazioni normali non è corretto.";

    if (account.getBalance() == 50.0 && account.getNormalOperationCount() == 2) {
        std::cout << "Prelievo effettuato con successo." << std::endl;
    }
}

// Test di prelievo con fondi insufficienti
TEST_F(BankAccountTest, WithdrawalInsufficientFundsDebitCard) {
    // Step 1: Aggiungi una carta di debito al conto
    account.addCard("DebitCard", false); // Aggiunge una carta di debito

    // Step 2: Esegui un deposito sul conto bancario
    auto bankDeposit = createOperation(200.0, OperationType::Deposit, std::chrono::system_clock::now());
    account.addTransaction(bankDeposit); // Deposita 200 nel conto bancario

    // Verifica che il saldo del conto sia 200 dopo il deposito
    EXPECT_EQ(account.getBalance(), 200.0) << "Errore: Il saldo del conto bancario dovrebbe essere 200.";

    // Step 3: Esegui un deposito sulla carta di debito
    auto cardDeposit = createOperation(100.0, OperationType::Deposit, std::chrono::system_clock::now());
    bool depositSuccess = account.addOperationToCard("DebitCard", cardDeposit); // Deposita 100 sulla carta di debito

    // Verifica che il deposito sulla carta di debito sia andato a buon fine
    EXPECT_TRUE(depositSuccess) << "Errore: Il deposito sulla carta di debito avrebbe dovuto essere completato con successo.";

    // Verifica che il saldo della carta di debito sia ora 100
    auto card = account.getCardsOperations().at(0);
    EXPECT_DOUBLE_EQ(card->getAmount(), 100.0) << "Errore: Il saldo della carta di debito dovrebbe essere 100.";

    // Verifica che il saldo del conto bancario sia ora 100, perché 100 sono stati trasferiti alla carta
    EXPECT_EQ(account.getBalance(), 100.0) << "Errore: Il saldo del conto bancario dovrebbe essere ridotto a 100 dopo il trasferimento sulla carta.";

    // Step 4: Esegui un prelievo dalla carta di debito (che supera il saldo disponibile)
    auto withdrawal = createOperation(150.0, OperationType::Withdrawal, std::chrono::system_clock::now());
    bool withdrawalSuccess = account.addOperationToCard("DebitCard", withdrawal); // Prelievo di 150 dalla carta

    // Verifica che il prelievo abbia fallito a causa di fondi insufficienti
    EXPECT_FALSE(withdrawalSuccess) << "Errore: Il prelievo avrebbe dovuto fallire per fondi insufficienti.";

    // Verifica che il saldo della carta sia ancora 100 (il prelievo non è riuscito)
    EXPECT_DOUBLE_EQ(card->getAmount(), 100.0) << "Errore: Il saldo della carta non dovrebbe cambiare dopo il fallimento del prelievo.";

    // Verifica che il saldo del conto bancario sia ancora 100
    EXPECT_EQ(account.getBalance(), 100.0) << "Errore: Il saldo del conto bancario non dovrebbe cambiare dopo il fallimento del prelievo.";

    // Verifica che ci sia solo una operazione (il deposito) sulla carta di debito
    EXPECT_EQ(card->getOperationSize(), 1) << "Errore: Dovrebbe esserci solo una operazione (il deposito) sulla carta di debito.";

    if (!withdrawalSuccess) {
        std::cout << "Prelievo fallito correttamente a causa di fondi insufficienti." << std::endl;
    }
}




// Test di salvataggio e caricamento
TEST_F(BankAccountTest, SaveAndLoadFromFile) {
    // Crea un'operazione di deposito per il conto
    auto deposit = createOperation(200.0, OperationType::Deposit, std::chrono::system_clock::now());
    account.addTransaction(deposit); // Esegui un deposito sul conto

    // Aggiungi una carta al conto e deposita su di essa
    account.addCard("TestCard", true); // Aggiungi una carta di credito
    auto cardDeposit = createOperation(100.0, OperationType::Deposit, std::chrono::system_clock::now());
    account.addOperationToCard("TestCard", cardDeposit); // Deposita sulla carta

    // Salva il conto e le operazioni della carta su file
    account.saveToFile("test_account.txt");

    // Crea un nuovo conto e carica i dati dal file
    BankAccount newAccount;
    newAccount.loadFromFile("test_account.txt");

    // Verifica che il saldo del conto sia corretto
    EXPECT_EQ(newAccount.getBalance(), 100.0) << "Errore: Il saldo del conto non è stato caricato correttamente.";

    // Verifica che il numero di operazioni normali sia corretto
    EXPECT_EQ(newAccount.getNormalOperationCount(), 1) << "Errore: Il numero di operazioni non è stato caricato correttamente.";

    // Verifica che la carta esista e che abbia il deposito corretto
    auto card = newAccount.getCardsOperations().at(0);
    EXPECT_EQ(card->getOperationSize(), 1) << "Errore: Il numero di operazioni della carta non è stato caricato correttamente.";
    EXPECT_DOUBLE_EQ(card->getAmount(), 100.0) << "Errore: Il saldo della carta non è stato caricato correttamente.";

    if (newAccount.getBalance() == 200.0 && newAccount.getNormalOperationCount() == 1 && card->getAmount() == 100.0) {
        std::cout << "Dati salvati e caricati correttamente, incluse le operazioni della carta." << std::endl;
    }
}


// Test di operazioni pianificate
TEST_F(BankAccountTest, ScheduledOperationExecution) {
    auto now = std::chrono::system_clock::now();
    auto schedOp = createScheduledOperation(100.0, OperationType::Deposit, now, Frequency::Daily);
    account.addScheduleOperation(schedOp);
    account.executeScheduledOperations();

    EXPECT_EQ(account.getBalance(), 100.0) << "Errore: Le operazioni pianificate non sono state eseguite correttamente.";
    EXPECT_EQ(account.getScheduledOperationCount(), 1) << "Errore: Il conteggio delle operazioni pianificate non è corretto.";

    if (account.getBalance() == 100.0 && account.getScheduledOperationCount() == 1) {
        std::cout << "Operazioni pianificate eseguite correttamente." << std::endl;
    }
}

// Test di aggiunta e rimozione carte
TEST_F(BankAccountTest, AddAndRemoveCard) {
    account.addCard("TestCard", false);
    EXPECT_EQ(account.getCardsOperations().size(), 1) << "Errore: La carta non è stata aggiunta correttamente.";

    account.removeCard("TestCard");
    EXPECT_EQ(account.getCardsOperations().size(), 0) << "Errore: La carta non è stata rimossa correttamente.";

    if (account.getCardsOperations().size() == 0) {
        std::cout << "Carta aggiunta e rimossa con successo." << std::endl;
    }
}

// Test di aggiunta operazione a carta
TEST_F(BankAccountTest, AddOperationToCard) {
    account.addCard("Test Card", true);

    auto op = std::make_shared<Operation>(50.0, OperationType::Deposit, std::chrono::system_clock::now());
    bool success = account.addOperationToCard("Test Card", op);

    auto card = account.getCardsOperations().at(0);
    EXPECT_DOUBLE_EQ(card->getAmount(), 0) << "Errore: L'importo della carta non è stato aggiornato correttamente.";
    EXPECT_EQ(card->getOperationSize(), 0) << "Errore: Il numero di operazioni associate alla carta non è corretto.";

    if (success && card->getAmount() == 0 && card->getOperationSize() == 0) {
        std::cout << "Operazione aggiunta alla carta correttamente." << std::endl;
    }
}

// Test della carta di credito
TEST(CreditCardTest, AddOperationTest) {
    CreditCard card("MyCreditCard", 1000.0, 0.05);

    auto op = std::make_shared<Operation>(100.0, OperationType::Deposit);
    bool success = card.addOperation(op);

    ASSERT_TRUE(success) << "Errore: L'operazione non è stata aggiunta correttamente.";
    if (success) {
        std::cout << "Operazione completata con successo sulla carta di credito." << std::endl;
    }
}
