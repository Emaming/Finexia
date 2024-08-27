#include "../Operation.h"
#include <gtest/gtest.h>
#include <chrono>
#include <stdexcept>
#include <sstream>
#include <iomanip>

// Test Fixture for Operation
class OperationTest : public ::testing::Test {
protected:
    void SetUp() override {
        // Setup code if needed
    }

    void TearDown() override {
        // Cleanup code if needed
    }
};

// Test constructor with all parameters
TEST_F(OperationTest, ConstructorWithAllParameters) {
    int id = 1;
    double amount = 100.50;
    OperationType type = OperationType::Deposit;
    std::chrono::system_clock::time_point now = std::chrono::system_clock::now();

    Operation op(id, amount, type, now);

    EXPECT_EQ(op.getId(), id);
    EXPECT_EQ(op.getAmount(), amount);
    EXPECT_EQ(op.getType(), type);
    EXPECT_EQ(op.getDate(), now);
}

// Test constructor with amount and type
TEST_F(OperationTest, ConstructorWithAmountAndType) {
    double amount = 50.75;
    OperationType type = OperationType::Withdrawal;

    Operation op(amount, type);

    EXPECT_GT(op.getAmount(), 0); // Expect amount to be greater than 0
    EXPECT_EQ(op.getType(), type);
    EXPECT_NE(op.getDate(), std::chrono::system_clock::time_point()); // Ensure date is set to now

    // Test exception for invalid amount
    EXPECT_THROW(Operation op(-10.0, type), std::out_of_range);
}

// Test printOperationString method


// Test printOperationType method
TEST_F(OperationTest, PrintOperationType) {
    Operation op(1, 100.0, OperationType::Deposit, std::chrono::system_clock::now());

    EXPECT_EQ(op.printOperationType(), "Deposit");

    op = Operation(1, 100.0, OperationType::Withdrawal, std::chrono::system_clock::now());
    EXPECT_EQ(op.printOperationType(), "Withdrawal");

    op = Operation(1, 100.0, OperationType::Transfer, std::chrono::system_clock::now());
    EXPECT_EQ(op.printOperationType(), "Transfer");

    // Test unknown type
    op = Operation(1, 100.0, static_cast<OperationType>(999), std::chrono::system_clock::now());
    EXPECT_EQ(op.printOperationType(), "Unknown");
}



