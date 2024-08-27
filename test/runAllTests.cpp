// runAllTests.cpp
#include <gtest/gtest.h>

// Includi i file di test per le classi BankAccount, Operation e ScheduledOperation
#include "BankAccountTest.cpp"
#include "OperationTest.cpp"
#include "ScheduledOperationTest.cpp"

int main(int argc, char **argv) {
    ::testing::InitGoogleTest(&argc, argv);
    return RUN_ALL_TESTS();
}
