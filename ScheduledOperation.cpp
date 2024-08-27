#include "ScheduledOperation.h"
#include "Operation.h"
#include <sstream>
#include <iomanip>
#include <chrono>
#include <ctime>




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
