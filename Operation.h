#ifndef OPERATION_H
#define OPERATION_H

#include <string>
#include <chrono>

enum class OperationType {
    Deposit,
    Withdrawal,
    Transfer
};

class Operation {
public:
    Operation(double amount, OperationType type, std::chrono::system_clock::time_point date);
    Operation(double amount, OperationType type);
    Operation(const std::string& description);
    Operation(double amount, OperationType type, const std::string& description);

    double getAmount() const;
    OperationType getType() const;
    std::chrono::system_clock::time_point getDate() const;
    std::string getDescription() const;
    void setDescription(const std::string &description);
    std::string printOperationType() const;

private:
    double amount;
    OperationType type;
    std::chrono::system_clock::time_point date;
    std::string description;
};

#endif // OPERATION_H