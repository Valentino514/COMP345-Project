#include "CommandProcessing.h"
#include <algorithm>
#include <sstream>

// Command Class Implementation

Command::Command(const std::string& commandText) {
    this->commandText = new std::string(commandText);
    this->effectText = new std::string("");
}

Command::Command(const Command& other) {
    copy(other);
}

Command& Command::operator=(const Command& other) {
    if (this != &other) {
        delete commandText;
        delete effectText;
        copy(other);
    }
    return *this;
}

Command::~Command() {
    delete commandText;
    delete effectText;
}

std::string Command::getCommand() const {
    return *commandText;
}

void Command::saveEffect(const std::string& effect) {
    *effectText = effect;
}

std::string Command::getEffect() const {
    return *effectText;
}

std::string Command::stringToLog() const {
    return "Command: " + *commandText + ", Effect: " + *effectText;
}

void Command::execute(CommandProcessor* processor) {
    std::cout << "Executing command: " << *commandText << std::endl;
    saveEffect("Executed"); 
    if (processor) {
        processor->notify();
    }
}

void Command::copy(const Command& other) {
    commandText = new std::string(*other.commandText);
    effectText = new std::string(*other.effectText);
}

std::ostream& operator<<(std::ostream& os, const Command& command) {
    os << "Command: " << *command.commandText << ", Effect: " << *command.effectText;
    return os;
}

// CommandProcessor Class Implementation

CommandProcessor::CommandProcessor() {
    commands = new std::vector<Command*>();
    attach(new LogObserver());
}

CommandProcessor::CommandProcessor(const CommandProcessor& other) {
    commands = new std::vector<Command*>();
    copy(other);
}

CommandProcessor& CommandProcessor::operator=(const CommandProcessor& other) {
    if (this != &other) {
        clear();
        copy(other);
    }
    return *this;
}

CommandProcessor::~CommandProcessor() {
    clear();
    delete commands;
}

Command* CommandProcessor::getCommand() {
    return commands->empty() ? nullptr : commands->back();
}

void CommandProcessor::processInput(int argc, char* argv[]) {
    readCommand(argc, argv);
    Command* command = new Command("EmptyCommand");
    command->execute(this);
    commands->push_back(command);
    notify();
}

void CommandProcessor::readCommand(int argc, char* argv[]) {
    for (int i = 1; i < argc; ++i) {
        Command* cmd = new Command(argv[i]);
        saveCommand(cmd);
    }
}

void CommandProcessor::saveCommand(Command* command) {
    commands->push_back(command);
    command->execute(this);
}

void CommandProcessor::validate() {
    // Implement validation logic
}

void CommandProcessor::notify() {
    for (Observer* observer : observers) {
        observer->update();
    }
}

std::string CommandProcessor::stringToLog() const {
    std::ostringstream oss;
    oss << "CommandProcessor with " << commands->size() << " commands.";
    // You can add more details here as needed
    return oss.str();
}

void CommandProcessor::attach(Observer* observer) {
    observers.push_back(observer);
}

void CommandProcessor::copy(const CommandProcessor& other) {
    for (Command* cmd : *other.commands) {
        commands->push_back(new Command(*cmd));
    }
}

void CommandProcessor::clear() {
    for (Command* cmd : *commands) {
        delete cmd;
    }
    commands->clear();
}


std::ostream& operator<<(std::ostream& os, const CommandProcessor& cp) {
    for (const Command* cmd : *cp.commands) {
        os << *cmd << std::endl;
    }
    return os;
}
