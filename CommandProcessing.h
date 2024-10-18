#ifndef COMMANDPROCESSING_H
#define COMMANDPROCESSING_H

#include <string>
#include <vector>
#include <iostream>

// Command Class
class Command {
public:
    Command(const std::string& commandText = "");
    Command(const Command& other); // Copy constructor
    Command& operator=(const Command& other); // Assignment operator
    ~Command(); // Destructor

    std::string getCommand() const;
    void saveEffect(const std::string& effect);
    std::string getEffect() const;

    friend std::ostream& operator<<(std::ostream& os, const Command& command); // Stream insertion operator

private:
    std::string* commandText;
    std::string* effectText;
    void copy(const Command& other);
};

// CommandProcessor Class
class CommandProcessor {
public:
    CommandProcessor();
    CommandProcessor(const CommandProcessor& other); // Copy constructor
    CommandProcessor& operator=(const CommandProcessor& other); // Assignment operator
    ~CommandProcessor(); // Destructor

    Command* getCommand();
    void processInput(int argc, char* argv[]);

    friend std::ostream& operator<<(std::ostream& os, const CommandProcessor& cp); // Stream insertion operator

private:
    std::vector<Command*>* commands;

    void readCommand(int argc, char* argv[]);
    void saveCommand(Command* command);
    void validate();
    void copy(const CommandProcessor& other);
    void clear();
};

#endif // COMMANDPROCESSING_H
