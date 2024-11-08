#include "CommandProcessing.h"
#include "../Map/Map.h"

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

// CommandProcessor class implementation

CommandProcessor::CommandProcessor(Map* gameMap) : map(gameMap) {
    commands = new std::vector<Command*>();
    currentState = "start";
}
CommandProcessor::CommandProcessor()  {
    commands = new std::vector<Command*>();
    currentState = "start";
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

void CommandProcessor::processInput() {
    readCommand();
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
}

bool CommandProcessor::validate(Command* command) {
    std::string cmd = command->getCommand();
    bool isValid = false;

    if (currentState == "start" && cmd.rfind("loadmap", 0) == 0) {
        currentState = "maploaded";
        isValid = true;
        std::cout << "The command: loadmap is valid" << std::endl;
    } else if (currentState == "maploaded" && cmd == "validatemap") {
        currentState = "mapvalidated";
        isValid = true;
        std::cout << "The command: validatemap is valid" << std::endl;
    } else if (currentState == "mapvalidated" && cmd.rfind("addplayer", 0) == 0) {
        currentState = "playersadded";
        isValid = true;
        std::cout << "The command: addplayer is valid" << std::endl;
    } else if (currentState == "playersadded" && cmd == "gamestart") {
        currentState = "assignreinforcement";
        isValid = true;
        std::cout << "The command: gamestart is valid" << std::endl;
    } else if (currentState == "win" && (cmd == "replay" || cmd == "quit")) {
        currentState = (cmd == "replay") ? "start" : "exit";
        isValid = true;
    }

    command->saveEffect(isValid ? "Command valid." : "Invalid command for the current state.");
    return isValid;
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

std::string* FileCommandProcessorAdapter::readCommand() {
    static std::string result[2];
    if (commandFile.is_open() && !commandFile.eof()) {
        std::string line;
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
            validate(cmd);
            saveCommand(cmd);
        } else {
            std::cerr << "Error: Unable to read command from file." << std::endl;
        }
    } else {
        std::cerr << "Error: File is not open or has been fully read." << std::endl;
    }
    return result;
}
