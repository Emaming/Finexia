#include <gtest/gtest.h>
#include "../Operation.h"
#include <stdexcept>
#include <chrono>

using namespace std::chrono;

class OperationTest : public ::testing::Test {
protected:
    // Codice di setup comune (se necessario)
    void SetUp() override {
        // Codice di setup, se necessario
    }

    // Codice di teardown comune (se necessario)
    void TearDown() override {
        // Codice di teardown, se necessario
    }
};

// Test del costruttore con importo, tipo e data
TEST_F(OperationTest, ConstructorWithAmountAndTypeAndDate) {
    auto now = system_clock::now();
    Operation op(100.0, OperationType::Deposit, now);

    EXPECT_DOUBLE_EQ(op.getAmount(), 100.0);
    EXPECT_EQ(op.getType(), OperationType::Deposit);
    EXPECT_EQ(op.getDate(), now);
}

// Test del costruttore con importo e tipo
TEST_F(OperationTest, ConstructorWithAmountAndType) {
    Operation op(50.0, OperationType::Withdrawal);

    EXPECT_DOUBLE_EQ(op.getAmount(), 50.0);
    EXPECT_EQ(op.getType(), OperationType::Withdrawal);
    EXPECT_NE(op.getDate(), system_clock::time_point{});
}

// Test del costruttore con descrizione
TEST_F(OperationTest, ConstructorWithDescription) {
    Operation op("Test Description");

    EXPECT_DOUBLE_EQ(op.getAmount(), 0.0);
    EXPECT_EQ(op.getType(), OperationType::Deposit);
    EXPECT_EQ(op.getDescription(), "Test Description");
}

// Test del costruttore con importo, tipo e descrizione
TEST_F(OperationTest, ConstructorWithAmountTypeAndDescription) {
    Operation op(200.0, OperationType::Transfer, "Transfer Description");

    EXPECT_DOUBLE_EQ(op.getAmount(), 200.0);
    EXPECT_EQ(op.getType(), OperationType::Transfer);
    EXPECT_EQ(op.getDescription(), "Transfer Description");
}

// Test del costruttore con importo non valido (deve lanciare un'eccezione)
TEST_F(OperationTest, ConstructorWithInvalidAmountThrowsException) {
    EXPECT_THROW(Operation op(-50.0, OperationType::Deposit), std::out_of_range);
}

// Test del metodo printOperationType
TEST_F(OperationTest, PrintOperationType) {
    Operation depositOp(100.0, OperationType::Deposit);
    Operation withdrawalOp(100.0, OperationType::Withdrawal);
    Operation transferOp(100.0, OperationType::Transfer);

    EXPECT_EQ(depositOp.printOperationType(), "Deposit");
    EXPECT_EQ(withdrawalOp.printOperationType(), "Withdrawal");
    EXPECT_EQ(transferOp.printOperationType(), "Transfer");
}

// Test del metodo setDescription
TEST_F(OperationTest, SetDescription) {
    Operation op(100.0, OperationType::Deposit);
    op.setDescription("Updated Description");

    EXPECT_EQ(op.getDescription(), "Updated Description");
}
