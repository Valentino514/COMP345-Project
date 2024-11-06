#ifndef COMMANDPROCESSING_H
#define COMMANDPROCESSING_H

#include <string>
#include <vector>
#include <iostream>
#include "../Log/ILoggable.h"
#include "../Log/LoggingObserver.h"

class CommandProcessor;
class Observer;

// Command Class
class Command : public ILoggable {
public:
    Command(const std::string& commandText = "");
    Command(const Command& other); // Copy constructor
    Command& operator=(const Command& other); // Assignment operator
    ~Command(); // Destructor

    std::string getCommand() const;
    void saveEffect(const std::string& effect);
    std::string getEffect() const;
    std::string stringToLog() const override;
    void execute(CommandProcessor* processor = nullptr);

    friend std::ostream& operator<<(std::ostream& os, const Command& command); // Stream insertion operator

private:
    std::string* commandText;
    std::string* effectText;
    std::vector<Observer*> observers;

    void copy(const Command& other);
};

// CommandProcessor Class
class CommandProcessor : public ILoggable {
public:
    CommandProcessor();
    CommandProcessor(const CommandProcessor& other); // Copy constructor
    CommandProcessor& operator=(const CommandProcessor& other); // Assignment operator
    ~CommandProcessor(); // Destructor

    Command* getCommand();
    void processInput(int argc, char* argv[]);
    void saveCommand(Command* command);
    void validate();
    void notify();
    void attach(Observer* observer);
    std::string stringToLog() const override;

    friend std::ostream& operator<<(std::ostream& os, const CommandProcessor& cp); // Stream insertion operator

private:
    std::vector<Command*>* commands;
    std::vector<Observer*> observers;

    void readCommand(int argc, char* argv[]);
    void copy(const CommandProcessor& other);
    void clear();
};

#endif // COMMANDPROCESSING_H
