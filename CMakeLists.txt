cmake_minimum_required(VERSION 3.28)
project(Finexia)

set(CMAKE_CXX_STANDARD 23)

add_executable(Finexia main.cpp
        BankAccount.cpp
        BankAccount.h
        CreditCard.cpp
        CreditCard.h
        DebitCard.cpp
        DebitCard.h
        Operation.cpp
        Operation.h
        ScheduledOperation.cpp
        ScheduledOperation.h
        CardOperation.cpp
        CardOperation.h
        TimeUtils.h
        Card.cpp
        Card.h)
