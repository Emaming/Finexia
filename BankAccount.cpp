    #include "BankAccount.h"
    #include "CardOperation.h"
    #include <fstream>
    #include <sstream>
    #include <iostream>
    #include <random>
    #include <chrono>
    #include "iomanip"
    #include "memory"
    #include "CreditCard.h"
    #include "DebitCard.h"
#include <unordered_set>

    // Costruttore
    BankAccount::BankAccount() : balance(0.0) {}


    void BankAccount::saveToFile(const std::string& filename) const {
        std::ofstream file(filename);

        if (!file) {
            std::cerr << "Error opening file for writing: " << filename << std::endl;
            return;
        }

        // Salvataggio saldo e IBAN
        file << balance << '\n' << IBAN << '\n';

        // Salvataggio operazioni
        file << operations.size() << '\n';
        for (const auto& op : operations) {
            file << op->getId() << ' '
                 << op->getAmount() << ' '
                 << static_cast<int>(op->getType()) << ' '
                 << std::chrono::system_clock::to_time_t(op->getDate()) << '\n';
        }

        // Salvataggio operazioni pianificate
        file << scheduledOperations.size() << '\n';
        for (const auto& schedOp : scheduledOperations) {
            file << schedOp->getOperation()->getId() << ' '
                 << schedOp->getOperation()->getAmount() << ' '
                 << static_cast<int>(schedOp->getOperation()->getType()) << ' '
                 << std::chrono::system_clock::to_time_t(schedOp->getOperation()->getDate()) << ' '
                 << std::chrono::system_clock::to_time_t(schedOp->getScheduledExecutionDate()) << ' '
                 << static_cast<int>(schedOp->getFrequency()) << '\n';
        }

        // Salvataggio operazioni con le carte
        file << cardsOperations.size() << '\n';
        for (const auto& cardOp : cardsOperations) {
            file << cardOp->getCardId() << ' '
                 << cardOp->getOperation()->getId() << ' '
                 << cardOp->getOperation()->getAmount() << ' '
                 << static_cast<int>(cardOp->getOperation()->getType()) << ' '
                 << (cardOp->isCreditCard() ? 0 : 1) << ' '
                 << cardOp->getCardNumber() << ' '
                 << cardOp->getCvv() << ' '
                 << std::chrono::system_clock::to_time_t(cardOp->getExpirationDate()) << '\n';
        }

        file.close();
    }



    void BankAccount::loadFromFile(const std::string& filename) {
        std::ifstream file(filename);

        if (!file) {
            std::cerr << "Error opening file for reading: " << filename << std::endl;
            return;
        }

        // Load balance and IBAN
        file >> balance;
        file.ignore();  // Ignore newline character
        std::getline(file, IBAN);

        // Load operations
        size_t numOperations;
        file >> numOperations;
        operations.clear();
        for (size_t i = 0; i < numOperations; ++i) {
            int id;
            double amount;
            int type;
            std::time_t date;
            file >> id >> amount >> type >> date;

            auto op = std::make_shared<Operation>(id, amount, static_cast<OperationType>(type), std::chrono::system_clock::from_time_t(date));
            operations.push_back(op);
        }

        // Load scheduled operations
        size_t numScheduledOps;
        file >> numScheduledOps;
        scheduledOperations.clear();
        for (size_t i = 0; i < numScheduledOps; ++i) {
            int id;
            double amount;
            int type;
            std::time_t opDate;
            std::time_t schedDate;
            int freq;
            file >> id >> amount >> type >> opDate >> schedDate >> freq;

            auto op = std::make_shared<Operation>(id, amount, static_cast<OperationType>(type), std::chrono::system_clock::from_time_t(opDate));
            auto schedOp = std::make_shared<ScheduledOperation>(op, std::chrono::system_clock::from_time_t(schedDate), static_cast<Frequency>(freq));
            scheduledOperations.push_back(schedOp);
        }

        // Load card operations
        size_t numCardOps;
        file >> numCardOps;
        cardsOperations.clear();
        for (size_t i = 0; i < numCardOps; ++i) {
            std::string cardId;
            int opId;
            double amount;
            int type;
            int cardType; // 0 for CreditCard, 1 for DebitCard
            std::string cardNumber;
            std::string cvv;
            std::time_t expDate;

            file >> cardId >> opId >> amount >> type >> cardType >> cardNumber >> cvv >> expDate;

            std::cout << "Loaded card operation: " << cardId << ", type: " << cardType << ", number: " << cardNumber << ", CVV: " << cvv << ", expDate: " << std::ctime(&expDate) << std::endl;

            auto op = std::make_shared<Operation>(opId, amount, static_cast<OperationType>(type), std::chrono::system_clock::now());  // Dummy date

            std::variant<std::monostate, std::shared_ptr<CreditCard>, std::shared_ptr<DebitCard>> cardVariant;
            if (cardType == 0) {
                cardVariant = std::make_shared<CreditCard>();
            } else if (cardType == 1) {
                cardVariant = std::make_shared<DebitCard>();
            } else {
                std::cerr << "Unknown card type: " << cardType << std::endl;
                continue; // Handle unknown type if necessary
            }

            auto cardOp = std::make_shared<CardOperation>(op, cardVariant, cardId, cardNumber, cvv, std::chrono::system_clock::from_time_t(expDate));
            cardsOperations.push_back(cardOp);
        }

        file.close();
    }



    std::vector<std::shared_ptr<CardOperation>> BankAccount::getCardsOperations() const {
        return cardsOperations;
    }


    #include <cstdint> // For uint64_t

    void BankAccount::addCard(const std::string& cardName, bool isCredit) {
        auto now = std::chrono::system_clock::now(); // Current time
        int id = static_cast<int>(cardsOperations.size()) + 1; // Generate a new ID based on the number of existing operations
        double amount = 0.0; // Default amount

        // Create an Operation object with the given parameters
        auto operation = std::make_shared<Operation>(id, amount,
                                                     isCredit ? OperationType::CreditCard : OperationType::DebitCard, now);

        // Create a Card object (CreditCard or DebitCard) based on the isCredit flag
        std::variant<std::monostate, std::shared_ptr<CreditCard>, std::shared_ptr<DebitCard>> cardVariant;
        if (isCredit) {
            cardVariant = std::make_shared<CreditCard>(); // Create a CreditCard and wrap in variant
        } else {
            cardVariant = std::make_shared<DebitCard>(); // Create a DebitCard and wrap in variant
        }

        // Generate random card number and CVV
        std::random_device rd;
        std::mt19937 gen(rd());
        std::uniform_int_distribution<std::uint64_t> cardNumberDist(1000000000000000ULL, 9999999999999999ULL); // 16-digit card number
        std::uniform_int_distribution<> cvvDist(100, 999); // 3-digit CVV
        std::uniform_int_distribution<> yearDist(1, 5); // Validity period in years

        std::string cardNumber = std::to_string(cardNumberDist(gen));
        std::string cvv = std::to_string(cvvDist(gen));

        // Generate expiration date
        auto future = now + std::chrono::duration_cast<std::chrono::system_clock::duration>(std::chrono::years(yearDist(gen)));
        std::chrono::system_clock::time_point expirationDate = future;

        // Create a CardOperation object with all the gathered information
        auto cardOperation = std::make_shared<CardOperation>(operation, cardVariant, cardName, cardNumber, cvv, expirationDate);

        // Add the new CardOperation to the list of card operations
        cardsOperations.push_back(cardOperation);
    }




    void BankAccount::addTransaction(const std::shared_ptr<Operation>& transaction) {
        double amount = transaction->getAmount();
        OperationType type = transaction->getType();

        // Aggiorna il saldo in base al tipo di operazione
        if (type == OperationType::Deposit) {
            balance += amount;
        } else if (type == OperationType::Withdrawal) {
            balance -= amount;  // Sottrai l'importo per i prelievi
        }

        // Aggiungi l'operazione alla lista
        operations.push_back(transaction);
    }



    void BankAccount::cancelOperations(const std::vector<std::shared_ptr<Operation>>& operationsToCancel) {
        // Creiamo un unordered_set per un accesso veloce durante la rimozione
        std::unordered_set<std::shared_ptr<Operation>> cancelSet(operationsToCancel.begin(), operationsToCancel.end());

        // Rimuoviamo tutte le operazioni che sono nel cancelSet
        operations.remove_if([&cancelSet](const std::shared_ptr<Operation>& op) {
            return cancelSet.find(op) != cancelSet.end();
        });
    }

    void BankAccount::removeScheduledOperation(const std::vector<std::shared_ptr<ScheduledOperation>>& operationsToRemove) {
        // Creiamo un unordered_set per un accesso veloce durante la rimozione
        std::unordered_set<std::shared_ptr<ScheduledOperation>> cancelSet(
                operationsToRemove.begin(), operationsToRemove.end());

        // Rimuoviamo tutte le operazioni pianificate che sono nel cancelSet
        scheduledOperations.remove_if([&cancelSet](const std::shared_ptr<ScheduledOperation>& op) {
            return cancelSet.find(op) != cancelSet.end();
        });
    }

    std::vector<std::shared_ptr<Operation>> BankAccount::findOperationByAmount(double amount) const {
        std::vector<std::shared_ptr<Operation>> result;
        for (const auto& op : operations) {
            if (op->getAmount() == amount) {
                result.push_back(op);
            }
        }
        return result;
    }

    std::vector<std::shared_ptr<Operation>> BankAccount::findOperationByType(OperationType type) const {
        std::vector<std::shared_ptr<Operation>> result;
        for (const auto& op : operations) {
            if (op->getType() == type) {
                result.push_back(op);
            }
        }
        return result;
    }

    std::vector<std::shared_ptr<Operation>> BankAccount::findOperationByDate(std::chrono::system_clock::time_point date) const {
        std::vector<std::shared_ptr<Operation>> result;

        // Convertiamo la data di input in std::tm per confrontare solo l'anno, mese e giorno
        std::time_t inputTimeT = std::chrono::system_clock::to_time_t(date);
        std::tm* inputTm = std::localtime(&inputTimeT);
        int inputYear = inputTm->tm_year;
        int inputMonth = inputTm->tm_mon;
        int inputDay = inputTm->tm_mday;

        for (const auto& op : operations) {
            std::time_t opTimeT = std::chrono::system_clock::to_time_t(op->getDate());
            std::tm* opTm = std::localtime(&opTimeT);

            // Confrontiamo solo anno, mese e giorno
            if (opTm->tm_year == inputYear && opTm->tm_mon == inputMonth && opTm->tm_mday == inputDay) {
                result.push_back(op);
            }
        }
        return result;
    }


    std::vector<std::shared_ptr<ScheduledOperation>> BankAccount::findScheduledByAmount(double amount) const {
        std::vector<std::shared_ptr<ScheduledOperation>> result;
        for (const auto& schedOp : scheduledOperations) {
            if (schedOp->getOperation()->getAmount() == amount) {
                result.push_back(schedOp);
            }
        }
        return result;
    }
    std::vector<std::shared_ptr<ScheduledOperation>> BankAccount::findScheduledByType(OperationType type) const {
        std::vector<std::shared_ptr<ScheduledOperation>> result;

        for (const auto& schedOp : scheduledOperations) {
            if (schedOp->getOperation()->getType() == type) {
                result.push_back(schedOp);
            }
        }

        return result;
    }

    std::vector<std::shared_ptr<ScheduledOperation>> BankAccount::findScheduledByDate(std::chrono::system_clock::time_point date) const {
        std::vector<std::shared_ptr<ScheduledOperation>> result;
        for (const auto& schedOp : scheduledOperations) {
            if (schedOp->getScheduledExecutionDate() == date) {
                result.push_back(schedOp);
            }
        }
        return result;
    }


    void BankAccount::printOperations(const std::vector<std::shared_ptr<Operation>>& ops) const {
        for (const auto& op : ops) {
            std::cout << "Amount: " << op->getAmount()
                      << ", Type: " << op->printOperationType()  // Usa il metodo per ottenere il nome del tipo
                      << ", Date: " << timePointToString(op->getDate()) << std::endl;
        }
    }



    void BankAccount::printCards() const {
        for (const auto& card : cardsOperations) {
            std::cout << card->printCardString();

        }
    }

    std::string BankAccount::getTransactionHistory() const {
        std::ostringstream oss;
        for (const auto& op : operations) {
            oss
                    << "Amount: " << op->getAmount()
                    << ", Type: " << op->printOperationType()  // Usa il metodo per ottenere il nome del tipo
                    << ", Date: " << timePointToString(op->getDate()) << '\n';
        }
        return oss.str();
    }


    std::string BankAccount::timePointToString(std::chrono::system_clock::time_point tp) const {
        std::time_t time = std::chrono::system_clock::to_time_t(tp);
        std::string timeStr = std::ctime(&time);
        if (!timeStr.empty() && timeStr.back() == '\n') {
            timeStr.pop_back(); // Remove newline character if present
        }
        return timeStr;
    }

    // Implementazione del metodo per pianificare un'operazione
    void BankAccount::scheduleOperation(const std::shared_ptr<Operation>& operation, std::chrono::system_clock::time_point startDate, Frequency frequency) {
        auto schedOp = std::make_shared<ScheduledOperation>(operation, startDate, frequency);
        scheduledOperations.push_back(schedOp);
    }


    std::string BankAccount::printBalance() const {
        std::ostringstream oss;
        oss << std::fixed << std::setprecision(6) << balance;
        return oss.str();
    }


    // Stampa le transazioni pianificate
    void BankAccount::printPlannedTransactions() const {
        std::cout << "Planned Transactions:" << std::endl;
        for (const auto& schedOp : scheduledOperations) {
            std::cout << std::fixed << std::setprecision(2)  // Imposta la precisione a due decimali
                      << "Amount: " << schedOp->getOperation()->getAmount()
                      << ", Type: " << schedOp->getOperation()->printOperationType()
                      << ", Operation Date: " << timePointToString(schedOp->getOperation()->getDate())
                      << ", Scheduled Date: " << timePointToString(schedOp->getScheduledExecutionDate())
                      << ", Frequency: " << schedOp->frequencyToString() << std::endl;
        }
    }

    void BankAccount::removeCard(const std::string& cardName) {
        auto it = std::remove_if(cardsOperations.begin(), cardsOperations.end(),
                                 [&cardName](const std::shared_ptr<CardOperation>& cardOp) {
                                     return cardOp->getCardId() == cardName;
                                 });
        if (it != cardsOperations.end()) {
            cardsOperations.erase(it, cardsOperations.end());
            std::cout << "Card with name " << cardName << " has been removed." << std::endl;
        } else {
            std::cout << "No card found with name " << cardName << "." << std::endl;
        }
    }
