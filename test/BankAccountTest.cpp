#include <gtest/gtest.h>
#include "../BankAccount.h"
#include "../Operation.h"
#include "../ScheduledOperation.h"
#include "../CardOperation.h"
#include "../CreditCard.h"
#include "../DebitCard.h"

#include <chrono>
#include <memory>
#include <vector>

// Test constructor
TEST(BankAccountTest, ConstructorInitializesBalance) {
    BankAccount account;
    EXPECT_EQ(account.printBalance(), "0.000000");
}

// Test addTransaction
TEST(BankAccountTest, AddTransaction) {
    BankAccount account;
    auto op = std::make_shared<Operation>(1, 100.0, OperationType::Deposit, std::chrono::system_clock::now());
    account.addTransaction(op);

    auto operations = account.findOperationByAmount(100.0);
    EXPECT_EQ(operations.size(), 1);
    EXPECT_EQ(operations[0]->getAmount(), 100.0);
}

// Test addCard
TEST(BankAccountTest, AddCard) {
    BankAccount account;
    account.addCard("Test Credit Card", true);

    auto cardOperations = account.getCardsOperations();
    ASSERT_EQ(cardOperations.size(), 1);

    auto cardOp = cardOperations[0];
    EXPECT_EQ(cardOp->getCardId(), "Test Credit Card");
    EXPECT_TRUE(cardOp->isCreditCard());
}

TEST(BankAccountTest, PrintBalance) {
    BankAccount account;

    // Aggiungi un deposito
    account.addTransaction(std::make_shared<Operation>(1, 200.0, OperationType::Deposit, std::chrono::system_clock::now()));
    EXPECT_EQ(account.printBalance(), "200.000000");  // Verifica il saldo dopo il deposito

    // Aggiungi un prelievo
    account.addTransaction(std::make_shared<Operation>(2, 50.0, OperationType::Withdrawal, std::chrono::system_clock::now()));
    EXPECT_EQ(account.printBalance(), "150.000000");  // Verifica il saldo dopo il prelievo
}







// Test cancelOperations
TEST(BankAccountTest, CancelOperations) {
    BankAccount account;
    auto op1 = std::make_shared<Operation>(1, 100.0, OperationType::Deposit, std::chrono::system_clock::now());
    auto op2 = std::make_shared<Operation>(2, 200.0, OperationType::Withdrawal, std::chrono::system_clock::now());
    account.addTransaction(op1);
    account.addTransaction(op2);

    std::vector<std::shared_ptr<Operation>> toCancel = {op1};
    account.cancelOperations(toCancel);

    auto operations = account.findOperationByAmount(100.0);
    EXPECT_EQ(operations.size(), 0);  // Operation 1 should be canceled
    operations = account.findOperationByAmount(200.0);
    EXPECT_EQ(operations.size(), 1);  // Operation 2 should remain
}

// Test removeScheduledOperation
TEST(BankAccountTest, RemoveScheduledOperation) {
    BankAccount account;
    auto op = std::make_shared<Operation>(1, 400.0, OperationType::Deposit, std::chrono::system_clock::now());
    account.scheduleOperation(op, std::chrono::system_clock::now() + std::chrono::hours(24), Frequency::One);

    auto scheduledOps = account.findScheduledByAmount(400.0);
    ASSERT_EQ(scheduledOps.size(), 1);

    account.removeScheduledOperation(scheduledOps);
    scheduledOps = account.findScheduledByAmount(400.0);
    EXPECT_EQ(scheduledOps.size(), 0);  // Scheduled operation should be removed
}

// Test printPlannedTransactions
TEST(BankAccountTest, PrintPlannedTransactions) {
    BankAccount account;
    auto op = std::make_shared<Operation>(1, 500.0, OperationType::Deposit, std::chrono::system_clock::now());
    account.scheduleOperation(op, std::chrono::system_clock::now() + std::chrono::hours(24), Frequency::One);

    testing::internal::CaptureStdout();
    account.printPlannedTransactions();
    std::string output = testing::internal::GetCapturedStdout();

    std::cout << "Captured Output: " << output << std::endl; // Stampa per debug

    EXPECT_NE(output.find("Amount: 500.00"), std::string::npos); // Nota il formato 500.00
}


// Test removeCard
TEST(BankAccountTest, RemoveCard) {
    BankAccount account;
    account.addCard("CardToRemove", true);
    account.removeCard("CardToRemove");

    auto cardOperations = account.getCardsOperations();
    EXPECT_EQ(cardOperations.size(), 0);  // Card should be removed
}
