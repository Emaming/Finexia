#include <gtest/gtest.h>
#include "../BankAccount.h"
#include <chrono>
#include <memory>

// Test the addCard method
TEST(BankAccountTest, AddCard) {
    BankAccount account;
    account.addCard("Test Credit Card", true);
    EXPECT_EQ(account.printBalance(), "0.000000");  // Verify that balance is still 0
    // We could also verify if the card was added properly by accessing the private cardsOperations vector,
    // but typically this would require a getter or some other method to expose the card operations.
}

// Test for adding a scheduled operation
TEST(BankAccountTest, ScheduleOperation) {
    BankAccount account;
    auto operation = std::make_shared<Operation>(1, 150.0, OperationType::Deposit, std::chrono::system_clock::now());
    account.scheduleOperation(operation, std::chrono::system_clock::now() + std::chrono::hours(24), Frequency::One);

    auto scheduledOperations = account.findScheduledByAmount(150.0);
    ASSERT_EQ(scheduledOperations.size(), 1);
    EXPECT_EQ(scheduledOperations[0]->getOperation()->getAmount(), 150.0);
}

// Test for executing planned transactions
TEST(BankAccountTest, ExecutePlannedTransactions) {
    BankAccount account;
    auto operation = std::make_shared<Operation>(1, 200.0, OperationType::Deposit, std::chrono::system_clock::now());
    account.scheduleOperation(operation, std::chrono::system_clock::now(), Frequency::One);

    account.executePlannedTransactions();
    auto result = account.findOperationByAmount(200.0);
    ASSERT_EQ(result.size(), 1);
    EXPECT_EQ(result[0]->getAmount(), 200.0);
}

// Test for cancelling operations
TEST(BankAccountTest, CancelOperations) {
    BankAccount account;
    auto operation1 = std::make_shared<Operation>(1, 100.0, OperationType::Deposit, std::chrono::system_clock::now());
    auto operation2 = std::make_shared<Operation>(2, 200.0, OperationType::Withdrawal, std::chrono::system_clock::now());

    account.addTransaction(operation1);
    account.addTransaction(operation2);

    std::vector<std::shared_ptr<Operation>> opsToCancel = {operation1};
    account.cancelOperations(opsToCancel);

    auto result = account.findOperationByAmount(100.0);
    EXPECT_EQ(result.size(), 0);  // Operation 1 should be canceled
    result = account.findOperationByAmount(200.0);
    EXPECT_EQ(result.size(), 1);  // Operation 2 should remain
}

// Test for removing scheduled operations
TEST(BankAccountTest, RemoveScheduledOperation) {
    BankAccount account;
    auto operation = std::make_shared<Operation>(1, 300.0, OperationType::Deposit, std::chrono::system_clock::now());
    account.scheduleOperation(operation, std::chrono::system_clock::now() + std::chrono::hours(24), Frequency::One);

    std::vector<std::shared_ptr<ScheduledOperation>> opsToRemove = {account.findScheduledByAmount(300.0)[0]};
    account.removeScheduledOperation(opsToRemove);

    auto result = account.findScheduledByAmount(300.0);
    EXPECT_EQ(result.size(), 0);  // The scheduled operation should be removed
}

// Test for findOperationByType method
TEST(BankAccountTest, FindOperationByType) {
    BankAccount account;
    auto operation1 = std::make_shared<Operation>(1, 400.0, OperationType::Deposit, std::chrono::system_clock::now());
    auto operation2 = std::make_shared<Operation>(2, 500.0, OperationType::Withdrawal, std::chrono::system_clock::now());

    account.addTransaction(operation1);
    account.addTransaction(operation2);

    auto depositOps = account.findOperationByType(OperationType::Deposit);
    EXPECT_EQ(depositOps.size(), 1);
    EXPECT_EQ(depositOps[0]->getAmount(), 400.0);

    auto withdrawalOps = account.findOperationByType(OperationType::Withdrawal);
    EXPECT_EQ(withdrawalOps.size(), 1);
    EXPECT_EQ(withdrawalOps[0]->getAmount(), 500.0);
}

// Test for findOperationByDate method
TEST(BankAccountTest, FindOperationByDate) {
    BankAccount account;
    auto now = std::chrono::system_clock::now();
    auto operation = std::make_shared<Operation>(1, 600.0, OperationType::Deposit, now);

    account.addTransaction(operation);

    auto result = account.findOperationByDate(now);
    ASSERT_EQ(result.size(), 1);
    EXPECT_EQ(result[0]->getAmount(), 600.0);
}

// Test for findScheduledByType method
TEST(BankAccountTest, FindScheduledByType) {
    BankAccount account;
    auto operation = std::make_shared<Operation>(1, 700.0, OperationType::Deposit, std::chrono::system_clock::now());
    account.scheduleOperation(operation, std::chrono::system_clock::now() + std::chrono::hours(24), Frequency::One);

    auto scheduledOps = account.findScheduledByType(OperationType::Deposit);
    ASSERT_EQ(scheduledOps.size(), 1);
    EXPECT_EQ(scheduledOps[0]->getOperation()->getAmount(), 700.0);
}

// Test for findScheduledByDate method
TEST(BankAccountTest, FindScheduledByDate) {
    BankAccount account;
    auto now = std::chrono::system_clock::now();
    auto operation = std::make_shared<Operation>(1, 800.0, OperationType::Deposit, now);
    account.scheduleOperation(operation, now + std::chrono::hours(24), Frequency::One);

    auto scheduledOps = account.findScheduledByDate(now + std::chrono::hours(24));
    ASSERT_EQ(scheduledOps.size(), 1);
    EXPECT_EQ(scheduledOps[0]->getOperation()->getAmount(), 800.0);
}

// Test for findNextExecutionDate method
TEST(BankAccountTest, FindNextExecutionDate) {
    BankAccount account;
    auto now = std::chrono::system_clock::now();
    auto operation = std::make_shared<Operation>(1, 900.0, OperationType::Deposit, now);
    account.scheduleOperation(operation, now + std::chrono::hours(24), Frequency::One);

    auto scheduledOps = account.findNextExecutionDate(now);
    ASSERT_EQ(scheduledOps.size(), 1);
    EXPECT_EQ(scheduledOps[0]->getOperation()->getAmount(), 900.0);
}


