cmake_minimum_required(VERSION 3.28)
project(Finexia)

set(CMAKE_CXX_STANDARD 23)

# Aggiungi le directory dei file header, se necessario
include_directories(${PROJECT_SOURCE_DIR})

# Aggiungi l'eseguibile del progetto principale
add_executable(Finexia
        main.cpp
        BankAccount.cpp
        CreditCard.cpp
        DebitCard.cpp
        Operation.cpp
        ScheduledOperation.cpp
        CardOperation.cpp
        Card.cpp
)
