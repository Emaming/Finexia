#ifndef OPERATION_H
#define OPERATION_H

#include <chrono>
#include <string>

enum class OperationType { Deposit, Withdrawal, Transfer };

class Operation {
private:
    int id; // Identificativo dell'operazione
    double amount;
    OperationType type;
    std::chrono::system_clock::time_point date;

public:
    // Costruttore per creare un'operazione con tutti i dati
    Operation(int id, double amount, OperationType type, std::chrono::system_clock::time_point date);

    // Costruttore per creare un'operazione con solo amount e type
    Operation(double a, OperationType type);

    int getId() const;
    double getAmount() const;
    OperationType getType() const;
    std::chrono::system_clock::time_point getDate() const;

    void printOperation() const;
    std::string printOperationString() const;
    static std::string printOperationTypes();
    std::string printOperationType() const;
    std::string printDateTime() const;
};

#endif // OPERATION_H
