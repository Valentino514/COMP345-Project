#include "CommandProcessing.h"

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
}

void CommandProcessor::readCommand(int argc, char* argv[]) {
    for (int i = 1; i < argc; ++i) {
        Command* cmd = new Command(argv[i]);
        saveCommand(cmd);
    }
}

void CommandProcessor::saveCommand(Command* command) {
    commands->push_back(command);
}

void CommandProcessor::validate() {
    // Implement validation logic
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
