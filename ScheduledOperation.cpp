//
// Created by user on 13/08/24.
//

#include "ScheduledOperation.h"

void ScheduledOperation::updateNextExecutionDate() {

}

std::string ScheduledOperation::printOperationString() const {
    return Operation::printOperationString();
}

std::string ScheduledOperation::printFrequency() const {
    return std::string();
}

const std::chrono::system_clock::time_point &ScheduledOperation::getScheduledExecutionDate() const {
    return scheduledExecutionDate;
}

Frequency ScheduledOperation::getFrequency() const {
    return frequency;
}
