#include "LoggingObserver.h"
#include <fstream>
#include <iostream>
#include "../OrdersList/Orders.h"


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

LogObserver::LogObserver() : ordersList(nullptr) {
    logFile.open("gamelog.txt", std::ios::app);  // Open the log file in append mode
    if (!logFile) {
        std::cerr << "Error opening log file!" << std::endl;
    }
}

void LogObserver::update() const {
    const OrdersList* ordersList = dynamic_cast<const OrdersList*>(subject);
    if (ordersList) {
        Order* currentOrder = ordersList->getCurrentOrder();
        if (currentOrder) {
            std::ofstream logFile("gamelog.txt", std::ios::app);
            if (logFile.is_open()) {
                logFile << "Executing order: " << *currentOrder << std::endl;
                logFile.flush();
                logFile.close();
            } else {
                std::cerr << "Error: Unable to open log file." << std::endl;
            }
            //mutable
             logData.push_back("Executed order: " + currentOrder->stringToLog());
        }
    }
}

void LogObserver::printLog() const {
    for (const auto& logEntry : logData) {
        std::cout << logEntry << std::endl;
    }
}

void LogObserver::setSubject(Subject* newSubject) {
    subject = newSubject;
    std::cout << "Subject set to: " << subject << std::endl;  //debug
}