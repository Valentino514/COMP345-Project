#include "CommandProcessing.h"
#include "../Map/Map.h"
#include <algorithm>
#include <sstream>

// Command class implementation

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

void Command::execute(CommandProcessor* processor) {
    if (commandText) {
        std::cout << "Executing command: " << *commandText << std::endl;
    } else {
        std::cerr << "Error: commandText is uninitialized!" << std::endl;
        return;
    }

    std::cout << "Saving effect..." << std::endl;
    saveEffect("Executed");

    if (processor) {
        std::cout << "Notifying processor observers..." << std::endl;
        processor->notify();
    } else {
        std::cerr << "Warning: processor is null." << std::endl;
    }
}

std::string Command::stringToLog() const {
    return "Command: " + *commandText + ", Effect: " + *effectText;
}


// CommandProcessor class implementation

CommandProcessor::CommandProcessor(Map* gameMap) : map(gameMap) {
    commands = new std::vector<Command*>();
    *currentState = "start";
}
CommandProcessor::CommandProcessor()  {
    commands = new std::vector<Command*>();
    currentState = new std::string("start");}


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

void CommandProcessor::processInput() {
    readCommand();
    Command* command = new Command("EmptyCommand");
    command->execute(this);
    commands->push_back(command);
    notify();
}

std::string* CommandProcessor::readCommand() {
    std::string input;
    std::cout << "Enter a command: ";
    std::getline(std::cin, input);

    // Allocate a C-style array of two strings
    static std::string result[2];
    
    size_t spacePos = input.find(' ');
    if (spacePos != std::string::npos) {
        result[0] = input.substr(0, spacePos);       // Command name
        result[1] = input.substr(spacePos + 1);      // Argument
    } else {
        result[0] = input;       // Command name only
        result[1] = "";          // No argument
    }

    
    // Create and validate the Command
    Command* cmd = new Command(input);  // You can store the entire input if needed
    validate(cmd);
    saveCommand(cmd);
    return result;
}

std::string CommandProcessor::readCommand1() {
    std::string input;

    // Directly read the input without extra clearing or ignoring
    std::cin >> input;

    // Create and validate the Command
    Command* cmd = new Command(input);
    validate(cmd);
    saveCommand(cmd);

    return input;  // Return the entire command as a single string
}
void CommandProcessor::saveCommand(Command* command) {
    commands->push_back(command);
    command->execute(this);
}

bool CommandProcessor::validate(Command* command) {
    std::string cmd = command->getCommand();

    if (*currentState == "start" && cmd == "loadmap") {
        
        command->saveEffect("Command valid.");
        return true;
    } else if (*currentState == "maploaded" && cmd == "validatemap") {
        command->saveEffect("Command valid.");
        return true;
    } else if (*currentState == "mapvalidated" && cmd == "addplayer") {
        command->saveEffect("Command valid.");
        return true;
    } else if (*currentState == "playersadded" && cmd == "gamestart") {
        command->saveEffect("Command valid.");
        return true;
    }else if (*currentState == "win" && (cmd == "replay" || cmd == "quit")) {
        command->saveEffect("Command valid.");
        return true;
      
    }

    command->saveEffect("Invalid command for the current state.");
    return false;
}

bool CommandProcessor::validate1(Command* command) {
    std::string cmd = command->getCommand();

    if (*currentState == "start" && cmd == "loadmap") {
        
        command->saveEffect("Command valid.");
        *currentState = "maploaded" ;
        return true;
    } else if (*currentState == "maploaded" && cmd == "validatemap") {
        command->saveEffect("Command valid.");
                *currentState = "mapvalidated" ;

        return true;
    } else if ((*currentState == "mapvalidated" ||* currentState == "playersadded" )  && cmd == "addplayer") {
        command->saveEffect("Command valid.");
              *  currentState = "playersadded" ;

        return true;
    } else if (*currentState == "playersadded" && cmd == "gamestart") {
        command->saveEffect("Command valid.");
               *currentState = "maploaded" ;

        return true;
    }else if (*currentState == "win" && (cmd == "replay" || cmd == "quit")) {
        command->saveEffect("Command valid.");

        return true;
      
    }

    command->saveEffect("Invalid command for the current state.");
    return false;
}

void CommandProcessor::copy(const CommandProcessor& other) {
    for (Command* cmd : *other.commands) {
        commands->push_back(new Command(*cmd));
    }
    currentState = other.currentState;
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

// FileCommandProcessorAdapter class implementation

FileCommandProcessorAdapter::FileCommandProcessorAdapter(Map* gameMap, const std::string& filename) : CommandProcessor(gameMap) {
    commandFile.open(filename);
    if (!commandFile.is_open()) {
        std::cerr << "Error: Unable to open file " << filename << std::endl;
    }
}

FileCommandProcessorAdapter::~FileCommandProcessorAdapter() {
    if (commandFile.is_open()) {
        commandFile.close();
    }
}

#include <utility>

std::string* FileCommandProcessorAdapter::readCommand() {
    // Dynamically allocate an array to hold the command and argument
    std::string* result = new std::string[2]{"", ""};

    // Check if the file is open and not at the end of file
    if (commandFile.is_open() && !commandFile.eof()) {
        std::string line;

        // Attempt to read the next line from the file
        if (std::getline(commandFile, line)) {
            size_t spacePos = line.find(' ');
            if (spacePos != std::string::npos) {
                result[0] = line.substr(0, spacePos);       // Command name
                result[1] = line.substr(spacePos + 1);      // Argument
            } else {
                result[0] = line;       // Command name only
                result[1] = "";         // No argument
            }

            // Create and validate the Command
            Command* cmd = new Command(line);
            bool isValid = validate1(cmd);
            if (isValid) {
                cmd->saveEffect("Command valid.");
                                std::cout << "Command valid.";

            } else {
                cmd->saveEffect("Invalid command for the current state.");
                     std::cout << "Invalid command for the current state.";

            }
            saveCommand(cmd);

            // Debug message to confirm the command has been processed
        } else {
                        std::cout << "end of file." << std::endl;

            exit(0);
        }
    } else {
        if (!commandFile.is_open()) {
            std::cerr << "Error: File is not open." << std::endl; 
        } else if (commandFile.eof()) {
            std::cerr << "End of file reached." << std::endl;
        }
    }
    return result;
}
