#include "CardOperation.h"
#include <iomanip>
#include <random>
#include <sstream>
#include <iostream>
#include <cstdint> // For uint64_t

// Constructor with all parameters
CardOperation::CardOperation(std::shared_ptr<Operation> op,
                             std::variant<std::monostate, std::shared_ptr<CreditCard>, std::shared_ptr<DebitCard>> card,
                             const std::string& id,
                             const std::string& number,
                             const std::string& cvv,
                             std::chrono::system_clock::time_point expDate)
        : operation(op), card(card), cardId(id), cardNumber(number), cvv(cvv), expirationDate(expDate) {
}

// Constructor for a new card operation with random values
CardOperation::CardOperation(const std::string& name)
        : operation(nullptr), card(std::monostate()), cardId(name), cardNumber(""), cvv(""), expirationDate(std::chrono::system_clock::now()) {
    // Generate random card details
    std::random_device rd;
    std::mt19937 gen(rd());
    std::uniform_int_distribution<std::uint64_t> dist(1000000000000000ULL, 9999999999999999ULL); // 16 digit card number
    std::uniform_int_distribution<> cvvDist(100, 999); // CVV
    std::uniform_int_distribution<> yearDist(1, 5); // Validity period in years

    // Generate card number and CVV
    cardNumber = std::to_string(dist(gen));
    cvv = std::to_string(cvvDist(gen));

    // Generate expiration date
    auto future = std::chrono::system_clock::now() + std::chrono::duration_cast<std::chrono::system_clock::duration>(std::chrono::years(yearDist(gen)));
    expirationDate = future;

    std::cout << "Generated Card ID: " << cardId
              << ", Card Number: " << cardNumber
              << ", CVV: " << cvv
              << ", Expiration Date: " << timePointToString(expirationDate) << std::endl;
}

std::string CardOperation::printCardString() const {
    std::stringstream cardStream;
    cardStream << "Card Name: " << cardId << ", Card Number: " << cardNumber
               << ", CVV: " << cvv << ", Expiration Date: " << timePointToString(expirationDate) << " \n";
    return cardStream.str();
}

bool CardOperation::isCreditCard() const {
    return std::holds_alternative<std::shared_ptr<CreditCard>>(card);
}

bool CardOperation::isDebitCard() const {
    return std::holds_alternative<std::shared_ptr<DebitCard>>(card);
}

std::string CardOperation::timePointToString(std::chrono::system_clock::time_point tp) const {
    std::time_t time = std::chrono::system_clock::to_time_t(tp);
    std::string timeStr = std::ctime(&time);
    if (!timeStr.empty() && timeStr.back() == '\n') {
        timeStr.pop_back(); // Remove newline character if present
    }
    return timeStr;
}

// Implement getters
std::string CardOperation::getCardId() const {
    return cardId;
}

std::string CardOperation::getCardNumber() const {
    return cardNumber;
}

std::string CardOperation::getCvv() const {
    return cvv;
}

std::chrono::system_clock::time_point CardOperation::getExpirationDate() const {
    return expirationDate;
}

std::shared_ptr<Operation> CardOperation::getOperation() const {
    return operation;
}

std::variant<std::monostate, std::shared_ptr<CreditCard>, std::shared_ptr<DebitCard>> CardOperation::getCard() const {
    return card;
}
