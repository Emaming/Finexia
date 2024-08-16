//
// Created by user on 13/08/24.
//

#include "Operation.h"

double Operation::getAmount() const {
    return amount;
}

OperationType Operation::getType() const {
    return type;
}

std::string Operation::printDateTime() const {
    return std::string();
}

Operation::Operation(double a, OperationType type) {

}

std::string Operation::printOperationTypes() {
    return std::string();
}

std::string Operation::printOperationType() const {
    return std::string();
}

std::string Operation::printOperationString() const {
    return std::string();
}

void Operation::printOperation() const {

}

const std::chrono::system_clock::time_point &Operation::getDate() const {
    return date;
}

