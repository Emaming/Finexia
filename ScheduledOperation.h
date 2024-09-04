#ifndef SCHEDULEDOPERATION_H
#define SCHEDULEDOPERATION_H

#include <chrono>
#include <memory>
#include <string>
#include "Operation.h"  // Includi la dichiarazione della classe Operation

// Enum per la frequenza dell'operazione programmata
enum class Frequency {
    One,
    Daily,
    Weekly,
    Monthly,
    Yearly
};

class ScheduledOperation : public Operation {
private:
    // Attributi privati
    std::chrono::system_clock::time_point scheduledExecutionDate;
    Frequency frequency;

public:
    // Costruttore
    ScheduledOperation(double amount, OperationType type, std::chrono::system_clock::time_point date, Frequency freq)
            : Operation(amount, type), scheduledExecutionDate(date), frequency(freq) {}

    // Metodi getter
    const std::chrono::system_clock::time_point& getScheduledExecutionDate() const;
    Frequency getFrequency() const;

    // Metodo per convertire la frequenza in stringa
    std::string frequencyToString() const;

    // Metodo setter
    void setScheduledExecutionDate(std::chrono::system_clock::time_point newDate);
};

#endif // SCHEDULEDOPERATION_H
