#ifndef LOGGINGOBSERVER_H
#define LOGGINGOBSERVER_H

#include <vector>
#include <string>
#include <algorithm>
#include <fstream>
#include "ILoggable.h"
#include "../CommandProcessing/CommandProcessing.h"

class OrdersList;
class Subject;

class Observer {
public:
    virtual ~Observer() = default;
    virtual void update() const = 0;
    virtual void setSubject(Subject* subject) = 0;
};

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
    std::vector<Observer*> observers;

};

class LogObserver : public Observer {
public:
    LogObserver();
    void update() const override;
    void printLog() const;
    void setSubject(Subject* newSubject);

private:
    mutable std::vector<std::string> logData;
    Subject* subject;
    OrdersList* ordersList;
    std::ofstream logFile;
};

#endif // LOGGINGOBSERVER_H
