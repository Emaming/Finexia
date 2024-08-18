#include "CardOperation.h"
#include <random>
#include <sstream>
#include <iostream>

CardOperation::CardOperation(std::shared_ptr<Operation> op, const std::string& id)
        : operation(op), cardId(id), cardNumber(""), cvv(0), pin(0), maximum(1000), cardName("") {
}

CardOperation::CardOperation(const std::string& name)
        : operation(nullptr), cardId(""), cardNumber(""), cvv(0), pin(0), maximum(1000), cardName(name) {
    // Initialization of random number generators
    std::random_device rd;
    std::mt19937 gen(rd());

    // Generation of the cardID (16 digits, spaced every 4)
    std::uniform_int_distribution<int> cardIDModuleGenerator(1000, 9999);
    std::stringstream cardIDStream;
    cardIDStream << cardIDModuleGenerator(gen) << " " << cardIDModuleGenerator(gen) << " " << cardIDModuleGenerator(gen)
                 << " " << cardIDModuleGenerator(gen);
    cardNumber = cardIDStream.str();

    // Generation of the cvv (3 digits)
    std::uniform_int_distribution<int> cvvGenerator(100, 999);
    cvv = cvvGenerator(gen);

    // Generation of the pin (5 digits)
    std::uniform_int_distribution<int> pinGenerator(10000, 99999);
    pin = pinGenerator(gen);
}

std::string CardOperation::printCardString() const {
    std::stringstream card;
    card << "Card Name: " << cardName << ", card number: " << cardNumber << ", cvv: " << cvv << ", pin: " << pin
         << ", maximum: " << maximum << std::endl;
    return card.str();
}
