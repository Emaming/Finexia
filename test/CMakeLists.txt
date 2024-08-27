cmake_minimum_required(VERSION 3.10)
project(BankAccountTests)

# Imposta la versione C++ richiesta
set(CMAKE_CXX_STANDARD 17)
set(CMAKE_CXX_STANDARD_REQUIRED ON)

# Aggiungi il percorso ai file di intestazione del progetto principale
include_directories(${PROJECT_SOURCE_DIR}/src)

# Trova GoogleTest
find_package(GTest REQUIRED)
include_directories(${GTEST_INCLUDE_DIRS})

# Aggiungi i file sorgenti del progetto principale ai test
set(SOURCES
        ${PROJECT_SOURCE_DIR}/src/BankAccount.cpp
        ${PROJECT_SOURCE_DIR}/src/Operation.cpp
        ${PROJECT_SOURCE_DIR}/src/ScheduledOperation.cpp
        ${PROJECT_SOURCE_DIR}/src/CardOperation.cpp
        ${PROJECT_SOURCE_DIR}/src/Card.cpp
)

# Aggiungi i file di test
set(TEST_SOURCES
        ${PROJECT_SOURCE_DIR}/test/BankAccountTest.cpp
        ${PROJECT_SOURCE_DIR}/test/OperationTest.cpp
        ${PROJECT_SOURCE_DIR}/test/ScheduledOperationTest.cpp
)

# Crea un eseguibile per tutti i test
add_executable(RunAllTests ${TEST_SOURCES} ${SOURCES})

# Collega le librerie GoogleTest e pthread all'eseguibile di test
target_link_libraries(RunAllTests ${GTEST_LIBRARIES} pthread)

# Usa la funzione add_test per registrare i test
enable_testing()
add_test(NAME RunAllTests COMMAND RunAllTests)
