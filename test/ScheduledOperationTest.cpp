#include "../ScheduledOperation.h"
#include <gtest/gtest.h>
#include <chrono>
#include <sstream>
#include <iomanip>

// Funzione libera per convertire il time_point in una stringa
std::string timePointToString(const std::chrono::system_clock::time_point& timePoint) {
    std::time_t time = std::chrono::system_clock::to_time_t(timePoint);
    std::stringstream ss;
    ss << std::put_time(std::localtime(&time), "%Y-%m-%d %H:%M:%S");
    return ss.str();
}

// Test Fixture per ScheduledOperation
class ScheduledOperationTest : public ::testing::Test {
protected:
    void SetUp() override {
        // Inizializzazione prima di ogni test
        op = std::make_shared<Operation>(100.0, OperationType::Deposit);
        now = std::chrono::system_clock::now();
    }

    void TearDown() override {
        // Pulizia dopo ogni test
    }

    std::shared_ptr<Operation> op;
    std::chrono::system_clock::time_point now;
};

// Test del costruttore e dei getter
TEST_F(ScheduledOperationTest, ConstructorAndGetters) {
    ScheduledOperation schedOp(op, now, Frequency::Daily);

    EXPECT_EQ(schedOp.getOperation(), op);
    EXPECT_EQ(schedOp.getScheduledExecutionDate(), now);
    EXPECT_EQ(schedOp.getFrequency(), Frequency::Daily);
}

// Test di updateNextExecutionDate
TEST_F(ScheduledOperationTest, UpdateNextExecutionDate) {
    ScheduledOperation schedOp(op, now, Frequency::Daily);
    auto originalDate = schedOp.getScheduledExecutionDate();

    schedOp.updateNextExecutionDate();
    auto updatedDate = schedOp.getScheduledExecutionDate();

    // La data dovrebbe essere aumentata di 24 ore
    EXPECT_EQ(updatedDate, originalDate + std::chrono::hours(24));
}

// Test di updateNextExecutionDate per frequenze diverse
TEST_F(ScheduledOperationTest, UpdateNextExecutionDateForDifferentFrequencies) {
    std::chrono::system_clock::time_point originalDate;

    ScheduledOperation schedOp(op, now, Frequency::One);
    originalDate = schedOp.getScheduledExecutionDate();
    schedOp.updateNextExecutionDate();
    EXPECT_EQ(schedOp.getScheduledExecutionDate(), originalDate);

    schedOp = ScheduledOperation(op, now, Frequency::Weekly);
    originalDate = schedOp.getScheduledExecutionDate();
    schedOp.updateNextExecutionDate();
    EXPECT_EQ(schedOp.getScheduledExecutionDate(), originalDate + std::chrono::hours(24 * 7));

    schedOp = ScheduledOperation(op, now, Frequency::Monthly);
    originalDate = schedOp.getScheduledExecutionDate();
    schedOp.updateNextExecutionDate();
    EXPECT_EQ(schedOp.getScheduledExecutionDate(), originalDate + std::chrono::hours(24 * 30));  // Approssimazione

    schedOp = ScheduledOperation(op, now, Frequency::Yearly);
    originalDate = schedOp.getScheduledExecutionDate();
    schedOp.updateNextExecutionDate();
    EXPECT_EQ(schedOp.getScheduledExecutionDate(), originalDate + std::chrono::hours(24 * 365)); // Approssimazione
}

// Test di printOperationString
TEST_F(ScheduledOperationTest, PrintOperationString) {
    ScheduledOperation schedOp(op, now, Frequency::Monthly);

    std::stringstream expected;
    expected << "Deposit, amount: 100.000000, scheduled execution date: " << timePointToString(now)
             << ", frequency: Monthly\n";

    EXPECT_EQ(schedOp.printOperationString(), expected.str());
}

// Test di frequencyToString
TEST_F(ScheduledOperationTest, FrequencyToString) {
    ScheduledOperation schedOp(op, now, Frequency::Daily);
    EXPECT_EQ(schedOp.frequencyToString(), "Daily");

    schedOp = ScheduledOperation(op, now, Frequency::Weekly);
    EXPECT_EQ(schedOp.frequencyToString(), "Weekly");

    schedOp = ScheduledOperation(op, now, Frequency::Monthly);
    EXPECT_EQ(schedOp.frequencyToString(), "Monthly");

    schedOp = ScheduledOperation(op, now, Frequency::Yearly);
    EXPECT_EQ(schedOp.frequencyToString(), "Yearly");

    schedOp = ScheduledOperation(op, now, Frequency::One);
    EXPECT_EQ(schedOp.frequencyToString(), "Unknown"); // Gestione tipo sconosciuto
}
