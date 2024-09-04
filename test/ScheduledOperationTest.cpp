#include <gtest/gtest.h>
#include "../ScheduledOperation.h"
#include <chrono>
#include <string>

using namespace std::chrono;

class ScheduledOperationTest : public ::testing::Test {
protected:
    // You can define common setup code here if needed.
    void SetUp() override {
        // Common setup code, if needed
    }

    void TearDown() override {
        // Common teardown code, if needed
    }
};

// Helper function to get a fixed time point
system_clock::time_point getFixedTimePoint() {
    return system_clock::now() + std::chrono::hours(1);
}

TEST_F(ScheduledOperationTest, Constructor) {
    auto now = getFixedTimePoint();
    ScheduledOperation op(100.0, OperationType::Deposit, now, Frequency::Weekly);

    EXPECT_DOUBLE_EQ(op.getAmount(), 100.0);
    EXPECT_EQ(op.getType(), OperationType::Deposit);
    EXPECT_EQ(op.getScheduledExecutionDate(), now);
    EXPECT_EQ(op.getFrequency(), Frequency::Weekly);
}

TEST_F(ScheduledOperationTest, FrequencyToString) {
    ScheduledOperation dailyOp(100.0, OperationType::Deposit, getFixedTimePoint(), Frequency::Daily);
    ScheduledOperation weeklyOp(100.0, OperationType::Deposit, getFixedTimePoint(), Frequency::Weekly);
    ScheduledOperation monthlyOp(100.0, OperationType::Deposit, getFixedTimePoint(), Frequency::Monthly);
    ScheduledOperation yearlyOp(100.0, OperationType::Deposit, getFixedTimePoint(), Frequency::Yearly);

    EXPECT_EQ(dailyOp.frequencyToString(), "Daily");
    EXPECT_EQ(weeklyOp.frequencyToString(), "Weekly");
    EXPECT_EQ(monthlyOp.frequencyToString(), "Monthly");
    EXPECT_EQ(yearlyOp.frequencyToString(), "Yearly");
}

TEST_F(ScheduledOperationTest, SetScheduledExecutionDate) {
    auto now = getFixedTimePoint();
    ScheduledOperation op(100.0, OperationType::Deposit, now, Frequency::Weekly);

    auto newDate = now + std::chrono::days(10);
    op.setScheduledExecutionDate(newDate);

    EXPECT_EQ(op.getScheduledExecutionDate(), newDate);
}

TEST_F(ScheduledOperationTest, DefaultFrequencyToString) {
    ScheduledOperation op(100.0, OperationType::Deposit, getFixedTimePoint(), Frequency::One);

    EXPECT_EQ(op.frequencyToString(), "Unknown");
}

