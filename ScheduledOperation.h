#ifndef SCHEDULEDOPERATION_H
#define SCHEDULEDOPERATION_H

#include <chrono>
#include <memory>
#include <string>
#include "Operation.h"  // Assicurati di includere la dichiarazione della classe Operation

enum class Frequency { One, Daily, Weekly, Monthly, Yearly };

class ScheduledOperation {
private:
    std::shared_ptr<Operation> operation;  // Verifica che Operation sia definito e incluso
    std::chrono::system_clock::time_point scheduledExecutionDate;
    Frequency frequency;

public:
    ScheduledOperation(std::shared_ptr<Operation> op, std::chrono::system_clock::time_point date, Frequency freq)
            : operation(op), scheduledExecutionDate(date), frequency(freq) {}

    std::shared_ptr<Operation> getOperation() const { return operation; }
    const std::chrono::system_clock::time_point& getScheduledExecutionDate() const;

    Frequency getFrequency() const;

    void updateNextExecutionDate();
    std::string printOperationString() const;

private:
    std::string printFrequency() const;
};

#endif // SCHEDULEDOPERATION_H
