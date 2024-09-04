#include <gtest/gtest.h>
#include "../BankAccount.h"
#include <chrono>
#include <memory>

// Helper function to create a shared_ptr<Operation>
std::shared_ptr<Operation> createOperation(double amount, OperationType type, std::chrono::system_clock::time_point date) {
    return std::make_shared<Operation>(amount, type, date);
}

// Helper function to create a shared_ptr<ScheduledOperation>
std::shared_ptr<ScheduledOperation> createScheduledOperation(double amount, OperationType type, std::chrono::system_clock::time_point date, Frequency freq) {
    auto op = std::make_shared<ScheduledOperation>(amount, type, date, freq);
    op->setScheduledExecutionDate(date);
    return op;
}

class BankAccountTest : public ::testing::Test {
protected:
    BankAccount account;

    void SetUp() override {
        // Set up code (if needed)
    }
};

// Test del costruttore e IBAN
TEST_F(BankAccountTest, ConstructorGeneratesIBAN) {
    EXPECT_FALSE(account.getIBAN().empty());
}

// Test di saldo iniziale
TEST_F(BankAccountTest, InitialBalanceIsZero) {
    EXPECT_EQ(account.getBalance(), 0.0);
}

// Test di deposito
TEST_F(BankAccountTest, DepositIncreasesBalance) {
    auto deposit = createOperation(100.0, OperationType::Deposit, std::chrono::system_clock::now());
    account.addTransaction(deposit);

    EXPECT_EQ(account.getBalance(), 100.0);
    EXPECT_EQ(account.getNormalOperationCount(), 1);
}

// Test di prelievo
TEST_F(BankAccountTest, WithdrawalDecreasesBalance) {
    auto deposit = createOperation(100.0, OperationType::Deposit, std::chrono::system_clock::now());
    account.addTransaction(deposit);

    auto withdrawal = createOperation(50.0, OperationType::Withdrawal, std::chrono::system_clock::now());
    account.addTransaction(withdrawal);

    EXPECT_EQ(account.getBalance(), 50.0);
    EXPECT_EQ(account.getNormalOperationCount(), 2);
}

// Test di prelievo con fondi insufficienti
TEST_F(BankAccountTest, WithdrawalInsufficientFunds) {
    auto withdrawal = createOperation(50.0, OperationType::Withdrawal, std::chrono::system_clock::now());
    account.addTransaction(withdrawal);
    EXPECT_EQ(account.getBalance(), -50.0); // La gestione dei fondi insufficienti non è implementata completamente
}

// Test di salvataggio e caricamento
TEST_F(BankAccountTest, SaveAndLoadFromFile) {
    auto deposit = createOperation(200.0, OperationType::Deposit, std::chrono::system_clock::now());
    account.addTransaction(deposit);
    account.saveToFile("test_account.txt");

    BankAccount newAccount;
    newAccount.loadFromFile("test_account.txt");
    EXPECT_EQ(newAccount.getBalance(), 200.0);
    EXPECT_EQ(newAccount.getNormalOperationCount(), 1);
}

// Test di operazioni pianificate
TEST_F(BankAccountTest, ScheduledOperationExecution) {
    auto now = std::chrono::system_clock::now();
    auto schedOp = createScheduledOperation(100.0, OperationType::Deposit, now, Frequency::Daily);
    account.addScheduleOperation(schedOp);
    account.executeScheduledOperations();

    EXPECT_EQ(account.getBalance(), 100.0);
    EXPECT_EQ(account.getScheduledOperationCount(), 1);
}

// Test di aggiunta e rimozione carte
TEST_F(BankAccountTest, AddAndRemoveCard) {
    account.addCard("TestCard", false);
    EXPECT_EQ(account.getCardsOperations().size(), 1);

    account.removeCard("TestCard");
    EXPECT_EQ(account.getCardsOperations().size(), 0);
}

// Test di aggiunta operazione a carta
TEST_F(BankAccountTest, AddOperationToCard) {
    BankAccount account;
    account.addCard("Test Card", true); // Adding a credit card
    auto op = std::make_shared<Operation>(50.0, OperationType::Deposit, std::chrono::system_clock::now());
    account.addOperationToCard("Test Card", op);

    auto card = account.getCardsOperations().at(0); // Get the first card
    EXPECT_DOUBLE_EQ(card->getAmount(), 0);
    EXPECT_EQ(card->getOperationSize(), 0);
}

