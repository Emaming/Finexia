#ifndef SCHEDULEDOPERATION_H
#define SCHEDULEDOPERATION_H

#include <chrono>
#include <memory>
#include <string>
#include "Operation.h"  // Assicurati di includere la dichiarazione della classe Operation

enum class Frequency { One, Daily, Weekly, Monthly, Yearly };

class ScheduledOperation : public Operation{
private:
    std::chrono::system_clock::time_point scheduledExecutionDate;
    Frequency frequency;

public:
    ScheduledOperation(double amount,OperationType type, std::chrono::system_clock::time_point date, Frequency freq)
            : Operation(amount,type), scheduledExecutionDate(date), frequency(freq) {}

    const std::chrono::system_clock::time_point& getScheduledExecutionDate() const;
    Frequency getFrequency() const;
    std::string frequencyToString() const;
    void setScheduledExecutionDate(std::chrono::system_clock::time_point newDate);

};

#endif // SCHEDULEDOPERATION_H