#include <fstream>
#include <iostream>

#include "LoggingObserver.h"
#include "../OrdersList/Orders.h"
#include "../GameEngine/GameEngine.h"


void Subject::attach(Observer* observer) {
    observers.push_back(observer);
}

void Subject::detach(Observer* observer) {
    for (auto it = observers.begin(); it != observers.end(); ) {
        if (*it == observer) {
            it = observers.erase(it);
        } else {
            ++it;
        }
    }
}

void Subject::notify() const {
    for (const auto& observer : observers) {
        observer->update();
    }
}

void Subject::addObserver(Observer* observer) {
    observers.push_back(observer);
}

void Subject::removeObserver(Observer* observer) {
    observers.erase(std::remove(observers.begin(), observers.end(), observer), observers.end());
}

std::vector<Observer*>& Subject::getObservers() {
    return observers;
}

LogObserver::LogObserver() {
    logFile.open("gamelog.txt", std::ios::app);  // Open the log file in append mode
    if (!logFile) {
        std::cerr << "Error opening log file!" << std::endl;
    }
}


// void LogObserver::update() const {
//     const OrdersList* ordersList = dynamic_cast<const OrdersList*>(subject);
//     if (ordersList) {
//         Order* currentOrder = ordersList->getCurrentOrder();
//         if (currentOrder) {
//             std::ofstream logFile("gamelog.txt", std::ios::app);
//             if (logFile.is_open()) {
//                 logFile << "Executing order: " << *currentOrder << std::endl;
//                 logFile.close();
//             } else {
//                 std::cerr << "Error: Unable to open log file." << std::endl;
//             }

//             // Ensure each log entry is unique in `logData`
//             std::string logEntry = "Executed order: " + currentOrder->stringToLog();
//             if (logData.empty() || logData.back() != logEntry) {
//                 logData.push_back(logEntry);
//             }
//         }
//     } else {
//         std::cerr << "Error: Subject is not of type OrdersList.\n";
//     }
// }


void LogObserver::update() const {
    const GameEngine* gameEngine = dynamic_cast<const GameEngine*>(subject);
    if (gameEngine) {
        std::string currentState = gameEngine->getState();
        std::ofstream logFile("gamelog.txt", std::ios::app);

        if (logFile.is_open()) {
            logFile << "Game state changed to: " << currentState << std::endl;
            logFile.close();
        } else {
            std::cerr << "Error: Unable to open log file." << std::endl;
        }

        std::string logEntry = "Game state changed to: " + currentState;
        if (logData.empty() || logData.back() != logEntry) {
            logData.push_back(logEntry);
        }
        return;
    }

    const OrdersList* ordersList = dynamic_cast<const OrdersList*>(subject);
    if (ordersList) {
        Order* currentOrder = ordersList->getCurrentOrder();

        if (currentOrder) {
            std::ofstream logFile("gamelog.txt", std::ios::app);

            if (logFile.is_open()) {
                logFile << "Executing order: " << *currentOrder << std::endl;
                logFile.close();
            } else {
                std::cerr << "Error: Unable to open log file." << std::endl;
            }

            std::string logEntry = "Executed order: " + currentOrder->stringToLog();
            if (logData.empty() || logData.back() != logEntry) {
                logData.push_back(logEntry);
            }
        }
    } else {
        std::cerr << "Error: Subject is not of type OrdersList or GameEngine.\n";
    }
}

void LogObserver::printLog() const {
    for (const auto& logEntry : logData) {
        std::cout << logEntry << std::endl;
    }
}

void LogObserver::setSubject(Subject* newSubject) {
    subject = newSubject;
    if (subject) {
        std::cout << "Subject set to: " << subject->stringToLog() << std::endl;
    } else {
        std::cout << "Subject is null" << std::endl;
    }
}
