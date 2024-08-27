cmake_minimum_required(VERSION 3.28)

set(CMAKE_CXX_STANDARD 23)

#aggiungo una sottocartella dove inserirò google test
add_subdirectory(./lib/googletest)
#definisco la cartella dove si trova google test (ovvero la sottocartella)
set(gtest_SOURCE_DIR ./lib/googletest/)
#includo gli header della libreria di google test cercandoli nella sottocartella appena creata
include_directories(${gtest_SOURCE_DIR}/include ${gtest_SOURCE_DIR})

set(TEST_SOURCE_FILES runAllTests.cpp BankAccountTest.cpp OperationTest.cpp
        ScheduledOperationTest.cpp
)

#crea l'eseguibile con i file cpp che contengono i test (riga 14)
add_executable(runAllTests ${TEST_SOURCE_FILES})
#crea l'eseguibile con il singolo file cpp di test di BankAccount
add_executable(runBankAccountTest BankAccountTest.cpp)
#crea l'eseguibile con il singolo file cpp di test di TransactionTest
add_executable(runOperationTest OperationTest.cpp)
#crea l'eseguibile con il singolo file cpp di test di PlannedTransactionTest
add_executable(runScheduledOperationTest ScheduledOperationTest.cpp)
#crea l'eseguibile con il singolo file cpp di test di DateTest
#collega l'eseguibile di test (riga 17) con la libreria creata nell'altro CMakeLists.txt che contiene i SOURCE_FILES da testare
target_link_libraries(runAllTests gtest gtest_main ${CMAKE_PROJECT_NAME}_lib)
#collega l'eseguibile di test (riga 19) con la libreria creata nell'altro CMakeLists.txt che contiene i SOURCE_FILES da testare
target_link_libraries(runBankAccountTest gtest gtest_main ${CMAKE_PROJECT_NAME}_lib)
#collega l'eseguibile di test (riga 21) con la libreria creata nell'altro CMakeLists.txt che contiene i SOURCE_FILES da testare
target_link_libraries(runOperationTest gtest gtest_main ${CMAKE_PROJECT_NAME}_lib)
#collega l'eseguibile di test (riga 23) con la libreria creata nell'altro CMakeLists.txt che contiene i SOURCE_FILES da testare
target_link_libraries(runScheduledOperationTest gtest gtest_main ${CMAKE_PROJECT_NAME}_lib)
