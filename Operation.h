//
// Created by user on 13/08/24.
//

#ifndef FINEXIA_OPERATION_H
#define FINEXIA_OPERATION_H

#include <string>
#include <ctime>
#include <iostream>
#include "chrono"

enum class OperationType {
    Deposit, Withdrawal, Transfer
};

class Operation {
protected:
    double amount;
    std::chrono::system_clock::time_point date;
    OperationType type;
public:

    virtual void printOperation() const;

    virtual std::string printOperationString() const;

    const std::chrono::system_clock::time_point &getDate() const;

    std::string printOperationType() const;

    static std::string printOperationTypes();

    std::string printDateTime() const;

explicit Operation(double a,OperationType type);

~Operation() = default;

    double getAmount() const;

    OperationType getType() const;



};


#endif //FINEXIA_OPERATION_H
