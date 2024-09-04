#include "ScheduledOperation.h"
#include "Operation.h"
#include <sstream>
#include <iomanip>
#include <chrono>
#include <ctime>

// Getter per la data di esecuzione programmata
const std::chrono::system_clock::time_point& ScheduledOperation::getScheduledExecutionDate() const {
    return scheduledExecutionDate;
}

// Getter per la frequenza dell'operazione programmata
Frequency ScheduledOperation::getFrequency() const {
    return frequency;
}

// Metodo per convertire la frequenza in stringa
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
            return "Unknown";  // Gestione caso di frequenza non riconosciuta
    }
}

// Setter per la data di esecuzione programmata
void ScheduledOperation::setScheduledExecutionDate(std::chrono::system_clock::time_point newDate) {
    scheduledExecutionDate = newDate;
}
