#include "ScheduledOperation.h"
#include "Operation.h"
#include <sstream>
#include <iomanip>
#include <chrono>
#include <ctime>

// Funzione libera per convertire il time_point in una stringa
std::string timePointToString(const std::chrono::system_clock::time_point& timePoint) {
    std::time_t time = std::chrono::system_clock::to_time_t(timePoint);
    std::stringstream ss;
    ss << std::put_time(std::localtime(&time), "%Y-%m-%d %H:%M:%S");
    return ss.str();
}

// Funzione di supporto per convertire OperationType in stringa
std::string operationTypeToString(OperationType type) {
    switch (type) {
        case OperationType::Deposit:
            return "Deposit";
        case OperationType::Withdrawal:
            return "Withdrawal";
        case OperationType::Transfer:
            return "Transfer";
        default:
            return "Unknown";
    }
}

const std::chrono::system_clock::time_point& ScheduledOperation::getScheduledExecutionDate() const {
    return scheduledExecutionDate;
}

Frequency ScheduledOperation::getFrequency() const {
    return frequency;
}


std::string ScheduledOperation::frequencyToString() const {
    switch (frequency) {
        case Frequency::Daily:
            return "Daily";
        case Frequency::Weekly:
            return "Weekly";
        case Frequency::Monthly:
            return "Monthly";
        case Frequency::Yearly:
            return "Yearly";
        default:
            return "Unknown";
    }
}
