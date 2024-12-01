#ifndef LOGGINGOBSERVER_H
#define LOGGINGOBSERVER_H

#include <vector>
#include <string>
#include <algorithm>
#include <fstream>

#include "ILoggable.h"

// Forward declarations
class Subject;
class CommandProcessor;

// Abstract observer class
class Observer {
public:
    virtual ~Observer() = default;
    virtual void update() const = 0;
    virtual void setSubject(Subject* subject) = 0;
};

// Subject being observed by the observer
class Subject : public ILoggable  {
public:
    virtual ~Subject() = default;
    virtual std::string stringToLog() const = 0;
    void attach(Observer* observer);
    void detach(Observer* observer);
    void notify() const;
    void addObserver(Observer* observer);
    void removeObserver(Observer* observer);

protected:
    std::vector<Observer*>& getObservers();
    std::vector<Observer*> observers;   // list of observers attached to the subject

};

// Observer observing subject and logging data
class LogObserver : public Observer {
public:
    LogObserver();  // constructor
    void update() const override;   // called when subject changes
    void printLog() const;  // print log data to console
    void setSubject(Subject* newSubject) override;  // set subject being observed

private:
    mutable std::vector<std::string> logData;   // stores log data
    Subject* subject;   // pointer to subject being observed
    std::ofstream logFile;  // file stream to write logs to a file
};

#endif // LOGGINGOBSERVER_H
