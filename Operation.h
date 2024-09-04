#ifndef OPERATION_H
#define OPERATION_H

#include <string>
#include <chrono>

// Enum per i tipi di operazione
enum class OperationType {
    Deposit,
    Withdrawal,
    Transfer
};

class Operation {
public:
    // Costruttori
    Operation(double amount, OperationType type, std::chrono::system_clock::time_point date);
    Operation(double amount, OperationType type);
    Operation(const std::string& description);
    Operation(double amount, OperationType type, const std::string& description);

    // Metodi getter
    double getAmount() const;
    OperationType getType() const;
    std::chrono::system_clock::time_point getDate() const;
    std::string getDescription() const;

    // Metodo setter
    void setDescription(const std::string &description);

    // Metodo per stampare il tipo di operazione come stringa
    std::string printOperationType() const;

private:
    // Attributi della classe
    double amount;
    OperationType type;
    std::chrono::system_clock::time_point date;
    std::string description;
};

#endif // OPERATION_H
