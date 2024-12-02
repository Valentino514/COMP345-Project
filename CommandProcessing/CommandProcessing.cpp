#include "CommandProcessing.h"
#include "../Map/Map.h"
#include <algorithm>
#include <sstream>
#include <utility>

/**
 * @class Command
 * @brief Represents a game command with associated effects.
 * 
 * The Command class encapsulates a single command issued by the player or system,
 * along with its execution logic and effect on the game's state.
 */

// Constructor
/**
 * @brief Constructs a Command with the specified text.
 * 
 * @param commandText The command text representing the action to perform.
 */
Command::Command(const std::string& commandText) {
    this->commandText = new std::string(commandText);
    this->effectText = new std::string("");
}

// Copy constructor
/**
 * @brief Creates a deep copy of an existing Command object.
 * 
 * @param other The Command object to copy.
 */
Command::Command(const Command& other) {
    copy(other);
}

// Assignment operator
/**
 * @brief Assigns the state of one Command to another.
 * 
 * @param other The Command object to copy.
 * @return A reference to the updated Command object.
 */
Command& Command::operator=(const Command& other) {
    if (this != &other) {
        delete commandText;
        delete effectText;
        copy(other);
    }
    return *this;
}

// Destructor
/**
 * @brief Releases resources allocated by the Command object.
 */
Command::~Command() {
    delete commandText;
    delete effectText;
}

// Get the command text
/**
 * @brief Retrieves the text of the command.
 * 
 * @return A string representing the command text.
 */
std::string Command::getCommand() const {
    return *commandText;
}

// Save the effect of the command
/**
 * @brief Associates an effect description with the command.
 * 
 * @param effect The effect text to associate with the command.
 */
void Command::saveEffect(const std::string& effect) {
    *effectText = effect;
}

// Get the effect text
/**
 * @brief Retrieves the effect associated with the command.
 * 
 * @return A string representing the command's effect.
 */
std::string Command::getEffect() const {
    return *effectText;
}

// Copy helper function
/**
 * @brief Copies the state of another Command into this object.
 * 
 * @param other The Command object to copy.
 */
void Command::copy(const Command& other) {
    commandText = new std::string(*other.commandText);
    effectText = new std::string(*other.effectText);
}

// Stream insertion operator
/**
 * @brief Outputs the Command's details to a stream.
 * 
 * @param os The output stream.
 * @param command The Command to output.
 * @return The modified output stream.
 */
std::ostream& operator<<(std::ostream& os, const Command& command) {
    os << "Command: " << *command.commandText << ", Effect: " << *command.effectText;
    return os;
}

// Execute the command
/**
 * @brief Executes the command and updates its effect.
 * 
 * @param processor A pointer to the CommandProcessor managing this command (optional).
 */
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

// String representation for logging
/**
 * @brief Converts the command and its effect into a loggable string.
 * 
 * @return A string representation of the command for logging.
 */
std::string Command::stringToLog() const {
    return "Command: " + *commandText + ", Effect: " + *effectText;
}


/**
 * @class CommandProcessor
 * @brief Handles the processing and validation of game commands.
 * 
 * The CommandProcessor class manages the lifecycle of commands, including
 * reading, validating, saving, and executing them in relation to the current game state.
 */

// Constructor with map reference
/**
 * @brief Initializes a CommandProcessor with a reference to the game's map.
 * 
 * @param gameMap A pointer to the Map object associated with the game.
 */
CommandProcessor::CommandProcessor(Map* gameMap) : map(gameMap) {
    commands = new std::vector<Command*>();
    currentState = new std::string("start"); // Initialize currentState to avoid segmentation faults
}

// Default constructor
/**
 * @brief Constructs a CommandProcessor with no map reference.
 */
CommandProcessor::CommandProcessor() {
    commands = new std::vector<Command*>();
    currentState = new std::string("start");
}

// Copy constructor
/**
 * @brief Creates a deep copy of another CommandProcessor.
 * 
 * @param other The CommandProcessor to copy.
 */
CommandProcessor::CommandProcessor(const CommandProcessor& other) {
    commands = new std::vector<Command*>();
    copy(other);
}

// Assignment operator
/**
 * @brief Assigns the state of one CommandProcessor to another.
 * 
 * @param other The CommandProcessor to copy.
 * @return A reference to the updated CommandProcessor.
 */
CommandProcessor& CommandProcessor::operator=(const CommandProcessor& other) {
    if (this != &other) {
        clear();
        copy(other);
    }
    return *this;
}

// Destructor
/**
 * @brief Releases resources allocated by the CommandProcessor.
 */
CommandProcessor::~CommandProcessor() {
    clear();
    delete commands;
}

// Get the most recent command
/**
 * @brief Retrieves the most recently added command.
 * 
 * @return A pointer to the last Command object, or nullptr if no commands exist.
 */
Command* CommandProcessor::getCommand() {
    return commands->empty() ? nullptr : commands->back();
}

// Process user input
/**
 * @brief Reads, validates, and saves user input as a command.
 */
void CommandProcessor::processInput() {
    readCommand();
    Command* command = new Command("EmptyCommand");
    command->execute(this);
    commands->push_back(command);
    notify();
}

// Read a command from input
/**
 * @brief Reads a command from standard input and validates it.
 * 
 * @return An array containing the command name and arguments.
 */
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
    std::getline(std::cin, input);

    // Create and validate the Command
    Command* cmd = new Command(input);
    validate(cmd);
    saveCommand(cmd);

    return input;  // Return the entire command as a single string
}

std::string CommandProcessor::readCommand2() {
    std::string input;

    // Directly read the input without extra clearing or ignoring
    std::cin >> input;

    // Create and validate the Command
    Command* cmd = new Command(input);
    validate(cmd);
    saveCommand(cmd);

    return input;  // Return the entire command as a single string
}

// Save a command
/**
 * @brief Saves a command to the command list and executes it.
 * 
 * @param command A pointer to the Command object to save.
 */
void CommandProcessor::saveCommand(Command* command) {
    commands->push_back(command);
    command->execute(this);
}

// Validate a command
/**
 * @brief Validates a command based on the current game state.
 * 
 * @param command A pointer to the Command to validate.
 * @return true if the command is valid; false otherwise.
 */
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
               *currentState = "gamestart" ;

        return true;
    }else if (*currentState == "win" && (cmd == "replay" || cmd == "quit")) {
        command->saveEffect("Command valid.");

        return true;
      
    }

    command->saveEffect("Invalid command for the current state.");
    return false;
}

/**
 * @brief Copies the state of another CommandProcessor object into this one.
 * 
 * @details This function performs a deep copy of all commands in the other
 * CommandProcessor, ensuring new Command objects are created for each entry.
 * 
 * @param other The CommandProcessor object to copy.
 */
void CommandProcessor::copy(const CommandProcessor& other) {
    for (Command* cmd : *other.commands) {
        commands->push_back(new Command(*cmd));
    }
    currentState = other.currentState;
}

/**
 * @brief Clears all commands stored in the CommandProcessor.
 * 
 * @details Frees the memory for each Command object and empties the commands list.
 */
void CommandProcessor::clear() {
    for (Command* cmd : *commands) {
        delete cmd;
    }
    commands->clear();
}

/**
 * @brief Outputs all commands stored in the CommandProcessor to an output stream.
 * 
 * @param os The output stream to write to.
 * @param cp The CommandProcessor whose commands will be written.
 * @return The modified output stream.
 */
std::ostream& operator<<(std::ostream& os, const CommandProcessor& cp) {
    for (const Command* cmd : *cp.commands) {
        os << *cmd << std::endl;
    }
    return os;
}

/**
 * @brief Notifies all observers of changes in the CommandProcessor.
 * 
 * @details Calls the `update()` method for every attached observer.
 */
void CommandProcessor::notify() {
    for (Observer* observer : observers) {
        observer->update();
    }
}

/**
 * @brief Converts the CommandProcessor's state into a loggable string.
 * 
 * @return A string summarizing the current state of the CommandProcessor.
 */
std::string CommandProcessor::stringToLog() const {
    std::ostringstream oss;
    oss << "CommandProcessor with " << commands->size() << " commands.";
    // You can add more details here as needed
    return oss.str();
}

/**
 * @brief Attaches an observer to the CommandProcessor.
 * 
 * @param observer A pointer to the Observer to attach.
 */
void CommandProcessor::attach(Observer* observer) {
    observers.push_back(observer);
}

/**
 * @class FileCommandProcessorAdapter
 * @brief Extends CommandProcessor to process commands from a file.
 * 
 * The FileCommandProcessorAdapter reads commands from a file and integrates them
 * into the command processing lifecycle, including validation and execution.
 */

// Constructor
/**
 * @brief Constructs a FileCommandProcessorAdapter with a reference to a game map
 * and a file containing commands.
 * 
 * @param gameMap A pointer to the Map object used by the game.
 * @param filename The name of the file containing commands to process.
 */
FileCommandProcessorAdapter::FileCommandProcessorAdapter(Map* gameMap, const std::string& filename) 
    : CommandProcessor(gameMap) {
    commandFile.open(filename);
    if (!commandFile.is_open()) {
        std::cerr << "Error: Unable to open file " << filename << std::endl;
    }
}

// Destructor
/**
 * @brief Closes the command file and cleans up resources.
 */
FileCommandProcessorAdapter::~FileCommandProcessorAdapter() {
    if (commandFile.is_open()) {
        commandFile.close();
    }
}



// Updated readCommand method in FileCommandProcessorAdapter
/**
 * @brief Reads a command and its arguments from the file.
 * 
 * @details Validates and saves the command if it is valid. Returns the command and
 * its argument as a dynamically allocated array of strings.
 * 
 * @return A dynamically allocated array of two strings: the command name and its argument.
 *         Returns nullptr if the file is at EOF or cannot be read.
 */
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

                std::cout << "\n Command valid." << std::endl;
            } else {
                cmd->saveEffect("Invalid command for the current state.");
                std::cout << "\nInvalid command for the current state." << std::endl;
            }
            saveCommand(cmd);

        } else {
            std::cout << "End of file reacheddd." << std::endl;
            delete[] result;  // Clean up result before returning nullptr
            return nullptr;
        }
    } else {
        if (!commandFile.is_open()) {
            std::cerr << "Error: File is not open." << std::endl;
        } else if (commandFile.eof()) {
            std::cerr << "End of file reacheed." << std::endl;
            return nullptr;
        }
        delete[] result;  // Clean up result in case of errors or EOF
        return nullptr;
    }
    return result;
}

TournamentParams CommandProcessor::parseTournamentCommand(const std::string& command) {
    TournamentParams params;
    std::istringstream iss(command);
    std::string token;

    iss >> token; // Skip "tournament"
  while (iss >> token) {
    if (token == "-M") {
        std::string mapList;
        iss >> mapList;
        size_t start = 0, end = 0;

        // Loop through the comma-separated list of maps
        while ((end = mapList.find(',', start)) != std::string::npos) {
            params.maps.push_back(mapList.substr(start, end - start));
            start = end + 1; // Skip the comma
        }
        params.maps.push_back(mapList.substr(start)); // Add the last map
    } else if (token == "-P") {
        std::string strategyList;
        iss >> strategyList;
        size_t start = 0, end = 0;

        // Loop through the comma-separated list of strategies
        while ((end = strategyList.find(',', start)) != std::string::npos) {
            params.strategies.push_back(strategyList.substr(start, end - start));
            start = end + 1; // Skip the comma
        }
        params.strategies.push_back(strategyList.substr(start)); // Add the last strategy
    } else if (token == "-G") {
        iss >> params.games;
    } else if (token == "-D") {
        iss >> params.maxTurns;
    }
}

    // Validate the command parameters
    if (params.maps.size() < 1 || params.maps.size() > 5 ||
        params.strategies.size() < 2 || params.strategies.size() > 4 ||
        params.games < 1 || params.games > 5 ||
        params.maxTurns < 10 || params.maxTurns > 50) {
        params.maps.clear(); // Mark invalid parameters by clearing maps
    }

    return params;
}
