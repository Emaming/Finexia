#include "Card.h"
#include <string>
#include <chrono>
#include <random>
#include <iostream>

Card::Card(const std::string& name, bool isCredit)
        : cardName(name),
          isCreditCard(isCredit),
          cardNumber(generateCardNumber()),
          cvv(generateCvv()),
          pin(generatePin()),
          expirationDate(generateExpirationDate()),
          amount(0) {  // Default amount is 0
}


std::string Card::generateCardNumber() {
    std::random_device rd;
    std::mt19937 gen(rd());
    std::uniform_int_distribution<std::uint64_t> dist(1000000000000000ULL, 9999999999999999ULL); // 16 cifre
    return std::to_string(dist(gen));
}

std::string Card::generateCvv() {
    std::random_device rd;
    std::mt19937 gen(rd());
    std::uniform_int_distribution<> dist(100, 999); // CVV a 3 cifre
    return std::to_string(dist(gen));
}

std::string Card::generatePin() {
    std::random_device rd;
    std::mt19937 gen(rd());
    std::uniform_int_distribution<> dist(10000, 99999); // PIN a 5 cifre
    return std::to_string(dist(gen));
}

std::chrono::system_clock::time_point Card::generateExpirationDate() {
    std::random_device rd;
    std::mt19937 gen(rd());
    std::uniform_int_distribution<> yearDist(1, 5); // Periodo di validità in anni
    auto now = std::chrono::system_clock::now();
    auto future = now + std::chrono::years(yearDist(gen));
    return future;
}

void Card::setAmount(double newAmount) {
    if (isCreditCard) {
        // Le carte di credito possono avere saldo negativo
        amount = newAmount;
    } else {
        // Le carte di debito non possono andare in negativo
        if (newAmount >= 0) {
            amount = newAmount;
        } else {
            std::cerr << "Error: Insufficient funds. Debit card cannot have a negative balance." << std::endl;
        }
    }
}


const std::string &Card::getCardName() const {
    return cardName;
}

const std::string &Card::getCardNumber() const {
    return cardNumber;
}

const std::string &Card::getCvv() const {
    return cvv;
}

const std::string &Card::getPin() const {
    return pin;
}

std::chrono::system_clock::time_point Card::getExpirationDate() const {
    return expirationDate;
}

double Card::getAmount() const {
    return amount;
}

void Card::addOperation(const std::shared_ptr<Operation>& op) {
    cardOperations.push_back(op);
}

std::list<std::shared_ptr<Operation>> Card::getOperations() const {
    return cardOperations;
}

void Card::setCardNumber(const std::string& number) {
    cardNumber = number;
}

void Card::setCvv(const std::string& cvv) {
    this->cvv = cvv;
}

void Card::setExpirationDate(std::chrono::system_clock::time_point expDate) {
    expirationDate = expDate;
}

bool Card::isCreditCardBool() const {
    return isCreditCard;
}
