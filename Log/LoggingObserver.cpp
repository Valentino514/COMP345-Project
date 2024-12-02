#include <fstream>
#include <iostream>

#include "LoggingObserver.h"
#include "../OrdersList/Orders.h"
#include "../GameEngine/GameEngine.h"

/**
 * @class Subject
 * @brief Represents an entity that can be observed by observers for state changes.
 * 
 * The Subject class manages a list of observers and provides methods to attach, detach,
 * and notify them of state changes.
 */


// Attach an observer to the subject
/**
 * @brief Adds an observer to the list of observers monitoring this subject.
 * 
 * @param observer A pointer to the observer object to be added.
 */
void Subject::attach(Observer* observer) {
    observers.push_back(observer);
}

// Detach an observer from the subject
/**
 * @brief Removes an observer from the list of observers.
 * 
 * @param observer A pointer to the observer object to be removed.
 */
void Subject::detach(Observer* observer) {
    for (auto it = observers.begin(); it != observers.end(); ) {
        if (*it == observer) {
            it = observers.erase(it);
        } else {
            ++it;
        }
    }
}

// Notify all observers about a change
/**
 * @brief Notifies all registered observers of a change in the subject's state.
 */
void Subject::notify() const {
    for (const auto& observer : observers) {
        observer->update();
    }
}

// Add an observer to the list
/**
 * @brief Adds an observer to the subject's observer list.
 * 
 * @param observer A pointer to the observer to be added.
 */
void Subject::addObserver(Observer* observer) {
    observers.push_back(observer);
}

// Remove an observer from the list
/**
 * @brief Removes an observer from the subject's observer list.
 * 
 * @param observer A pointer to the observer to be removed.
 */
void Subject::removeObserver(Observer* observer) {
    observers.erase(std::remove(observers.begin(), observers.end(), observer), observers.end());
}

// Get the list of observers
/**
 * @brief Provides access to the list of observers monitoring this subject.
 * 
 * @return A reference to the vector containing pointers to observers.
 */
std::vector<Observer*>& Subject::getObservers() {
    return observers;
}

/**
 * @class LogObserver
 * @brief Observes changes in a Subject and logs them to a file or memory.
 * 
 * The LogObserver class listens to state changes in associated subjects and logs
 * these changes to a file and an internal data structure for future reference.
 */


// Constructor for LogObserver
/**
 * @brief Initializes a LogObserver and opens the log file for writing.
 */
LogObserver::LogObserver() {
    logFile.open("gamelog.txt", std::ios::app);  // Open the log file in append mode
    if (!logFile) {
        std::cerr << "Error opening log file!" << std::endl;
    }
}

// Log changes for GameEngine state or OrdersList actions
/**
 * @brief Logs changes observed in the associated subject, such as GameEngine state
 * changes or orders executed in an OrdersList.
 * 
 * @details Depending on the subject type (GameEngine or OrdersList), this method logs
 * the appropriate actions to the `gamelog.txt` file and the internal log data.
 */
void LogObserver::update() const {
    // Logging for state change
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

        // Ensure entry is unique
        std::string logEntry = "Game state changed to: " + currentState;
        if (logData.empty() || logData.back() != logEntry) {
            logData.push_back(logEntry);
        }
        return;
    }

    // Logging for orders
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

            // Ensure entry is unique
            std::string logEntry = "Executed order: " + currentOrder->stringToLog();
            if (logData.empty() || logData.back() != logEntry) {
                logData.push_back(logEntry);
            }
        }
    } else {
        std::cerr << "Error: Subject is not of type OrdersList or GameEngine.\n";
    }
}

// Print log to console
/**
 * @brief Outputs the contents of the internal log data to the console.
 */
void LogObserver::printLog() const {
    for (const auto& logEntry : logData) {
        std::cout << logEntry << std::endl;
    }
}

// Link observer to subject
/**
 * @brief Sets the subject that the LogObserver will monitor.
 * 
 * @param newSubject A pointer to the Subject to be observed.
 */
void LogObserver::setSubject(Subject* newSubject) {
    subject = newSubject;
    if (subject) {
        std::cout << "Subject set to: " << subject->stringToLog() << std::endl;
    } else {
        std::cout << "Subject is null" << std::endl;
    }
}
