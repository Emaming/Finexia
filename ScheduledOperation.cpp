//
// Created by user on 13/08/24.
//

#include "ScheduledOperation.h"
#include <sstream>
#include <iomanip>
#include <chrono>
#include <ctime>

std::string timePointToString(const std::chrono::system_clock::time_point& timePoint) {
    std::time_t time = std::chrono::system_clock::to_time_t(timePoint);
    std::stringstream ss;
    ss << std::put_time(std::localtime(&time), "%Y-%m-%d %H:%M:%S");
    return ss.str();
}

void ScheduledOperation::updateNextExecutionDate() {

    switch (frequency) {
        case Frequency::One:
            // No date update, just one time
            break;
        case Frequency::Daily:
            scheduledExecutionDate += std::chrono::hours(24);
            break;
        case Frequency::Weekly:
            scheduledExecutionDate += std::chrono::hours(24 * 7);
            break;
        case Frequency::Monthly:
            scheduledExecutionDate += std::chrono::hours(24 * 30);  // Rough approximation
            break;
        case Frequency::Yearly:
            scheduledExecutionDate += std::chrono::hours(24 * 365); // Rough approximation
            break;
    }
}

std::string ScheduledOperation::printOperationString() const {
    std::stringstream transaction;
    transaction << printOperationType() << ", amount: " << amount;
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

const std::chrono::system_clock::time_point &ScheduledOperation::getScheduledExecutionDate() const {
    return scheduledExecutionDate;
}

Frequency ScheduledOperation::getFrequency() const {
    return frequency;
}
