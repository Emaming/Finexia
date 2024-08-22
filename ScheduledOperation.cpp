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

void ScheduledOperation::updateNextExecutionDate() {
    switch (frequency) {
        case Frequency::One:
            // Nessun aggiornamento della data, solo una volta
            break;
        case Frequency::Daily:
            scheduledExecutionDate += std::chrono::hours(24);
            break;
        case Frequency::Weekly:
            scheduledExecutionDate += std::chrono::hours(24 * 7);
            break;
        case Frequency::Monthly:
            scheduledExecutionDate += std::chrono::hours(24 * 30);  // Approssimazione
            break;
        case Frequency::Yearly:
            scheduledExecutionDate += std::chrono::hours(24 * 365); // Approssimazione
            break;
    }
}

std::string ScheduledOperation::printOperationString() const {
    std::stringstream transaction;
    transaction << operationTypeToString(operation->getType()) << ", amount: " << operation->getAmount();
    transaction << ", scheduled execution date: " << timePointToString(scheduledExecutionDate);
    transaction << ", frequency: " << printFrequency() << std::endl;
    return transaction.str();
}

std::string ScheduledOperation::printFrequency() const {
    switch (frequency) {
        case Frequency::One:
            return "One";
        case Frequency::Daily:
            return "Daily";
        case Frequency::Weekly:
            return "Weekly";
        case Frequency::Monthly:
            return "Monthly";
        case Frequency::Yearly:
            return "Yearly";
    }
    return "";
}

const std::chrono::system_clock::time_point& ScheduledOperation::getScheduledExecutionDate() const {
    return scheduledExecutionDate;
}

Frequency ScheduledOperation::getFrequency() const {
    return frequency;
}
#include "ScheduledOperation.h"

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
