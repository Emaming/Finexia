//
// Created by user on 13/08/24.
//

#ifndef FINEXIA_SCHEDULEDOPERATION_H
#define FINEXIA_SCHEDULEDOPERATION_H
#include "Operation.h"
#include "chrono"

enum class Frequency {
    One, Daily, Weekly, Monthly, Yearly
};

class ScheduledOperation : public Operation{
private:
    std::chrono::system_clock::time_point scheduledExecutionDate;
    Frequency frequency;
public:
    ScheduledOperation(float amount, OperationType type,std::chrono::system_clock::time_point scheduledExecutionDate,Frequency frequency)
    : Operation(amount,type),scheduledExecutionDate(scheduledExecutionDate),frequency(frequency) {}

    const std::chrono::system_clock::time_point &getScheduledExecutionDate() const;

    Frequency getFrequency() const;

    std::string printFrequency() const;

    std::string printOperationString() const override;

    void updateNextExecutionDate();
};


#endif //FINEXIA_SCHEDULEDOPERATION_H
