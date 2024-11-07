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
    // clearPlayers();
    delete commands;
}

Command* CommandProcessor::getCommand() {
    return commands->empty() ? nullptr : commands->back();
}

void CommandProcessor::processInput() {
    readCommand();
}

void CommandProcessor::readCommand() {
    std::string input;
    std::cout << "Enter a command: ";
    std::getline(std::cin, input);
    Command* cmd = new Command(input);
    validate(cmd);
    saveCommand(cmd);
}

void CommandProcessor::saveCommand(Command* command) {
    commands->push_back(command);
}

bool CommandProcessor::validate(Command* command) {
    std::string cmd = command->getCommand();
    bool isValid = false;

    if (currentState == "start" && cmd.rfind("loadmap", 0) == 0) {
        // Extract the map filename from the command
        currentState="maploaded";
         isValid=true;
        cout<<"the command : loadmap is valid";
            } else if (currentState == "maploaded" && cmd == "validatemap") {
                currentState="mapvalidated";
         isValid=true;
        cout<<"the command : validatemap is valid";
    } else if (currentState == "mapvalidated" && cmd.rfind("addplayer", 0) == 0) {
                        currentState="playersadded";

         isValid=true;
        cout<<" the command : addplayer is valid"<<endl;
    } else if (currentState == "playersadded" && cmd == "gamestart") {
        currentState = "assignreinforcement";
        isValid = true;
    } else if (currentState == "win" && (cmd == "replay" || cmd == "quit")) {
        currentState = (cmd == "replay") ? "start" : "exit";
        isValid = true;
    }

    command->saveEffect(isValid ? "Command valid." : "Invalid command for the current state.");
    return isValid;
}


// void CommandProcessor::createPlayer(const std::string& playerName) {
//     std::string* namePtr = new std::string(playerName);  // Allocate a new string for the player's name
//     Player* newPlayer = new Player(namePtr);  // Create a new Player with the name
//     players.push_back(newPlayer);  // Add the new Player to the players vector
//     std::cout << "Player " << playerName << " created and added to the game." << std::endl;
// }

// void CommandProcessor::clearPlayers() {
//     for (Player* player : players) {
//         delete player;
//     }
//     players.clear();
// }

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

void FileCommandProcessorAdapter::readCommand() {
    if (commandFile.is_open() && !commandFile.eof()) {
        std::string line;
        if (std::getline(commandFile, line)) {
            Command* cmd = new Command(line);
            validate(cmd);
            saveCommand(cmd);
        } else {
            std::cerr << "Error: Unable to read command from file." << std::endl;
        }
    } else {
        std::cerr << "Error: File is not open or has been fully read." << std::endl;
    }
}