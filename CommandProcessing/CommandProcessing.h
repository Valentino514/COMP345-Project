#ifndef COMMANDPROCESSING_H
#define COMMANDPROCESSING_H

#include <string>
#include <vector>
#include <iostream>
#include <fstream>

#include "../Map/Map.h"
#include "../Player/Player.h"
#include "../Log/ILoggable.h"
#include "../Log/LoggingObserver.h"

class CommandProcessor;
class Observer;
class Map;
class Player;

// Command Class

struct TournamentParams {
    std::vector<std::string> maps;       // List of map file names
    std::vector<std::string> strategies; // List of player strategies
    int games;                           // Number of games per map
    int maxTurns;                        // Maximum number of turns per game
};


class Command : public Subject{
public:
    Command(const std::string& commandText = "");      // Constructor with default parameter
    Command(const Command& other);                     // Copy constructor
    Command& operator=(const Command& other);          // Assignment operator
    ~Command();                                        // Destructor

    std::string getCommand() const;                    // Returns the command text
    void saveEffect(const std::string& effect);        // Saves the effect of the command
    std::string getEffect() const;                     // Returns the effect text
    std::string stringToLog() const override;
    void execute(CommandProcessor* processor = nullptr);

    friend std::ostream& operator<<(std::ostream& os, const Command& command); // Stream insertion operator for printing

private:
    std::string* commandText;                          // Command text
    std::string* effectText;                           // Command effect
    std::vector<Observer*> observers;
    void copy(const Command& other);                   // Helper method to copy command data
};

// CommandProcessor Class
class CommandProcessor : public ILoggable {
public:
bool validate1(Command* command);
    CommandProcessor(Map* gameMap);                    // Constructor with map pointer
    CommandProcessor();
    CommandProcessor(const CommandProcessor& other);   // Copy constructor
    CommandProcessor& operator=(const CommandProcessor& other); // Assignment operator
    virtual ~CommandProcessor();                       // Destructor
    
    std::string readCommand1();
    std::string readCommand2();
    Command* getCommand();                             // Retrieves the latest command
    virtual void processInput();                       // Processes user input
    std::string * currentState;                          // Tracks the current state of command processing
    void validate();
    void notify();
    void attach(Observer* observer);
    std::string stringToLog() const override; 
    TournamentParams parseTournamentCommand(const std::string& command);

    friend std::ostream& operator<<(std::ostream& os, const CommandProcessor& cp); // Stream insertion operator for printing

protected:
    std::vector<Command*>* commands;                   // Stores commands issued to the processor
    std::vector<Observer*> observers;

    virtual std::string* readCommand();                // Reads and splits command input into two parts (command and argument)
    void saveCommand(Command* command);                // Saves a command to the command list
    bool validate(Command* command);                   // Validates command based on current state
    void copy(const CommandProcessor& other);          // Copies command data
    void clear();                                      // Clears commands from memory

private:
    Map* map;                                          // Pointer to the Map instance
    std::vector<Player*>* players;                     //  a pointer to a vector of players

    void createPlayer(const std::string& playerName);  // Helper method to create a Player
    void clearPlayers();                               // Helper method to clear Player instances
};

// FileCommandProcessorAdapter Class
class FileCommandProcessorAdapter : public CommandProcessor {
public:
    FileCommandProcessorAdapter(Map* gameMap, const std::string& filename); // Constructor with map and filename
    ~FileCommandProcessorAdapter();                                         // Destructor to close file
    std::string* readCommand() override;                                    // Reads command from file and splits it
    std::ifstream commandFile;                                              // File stream for reading commands

protected:

private:
};

#endif // COMMANDPROCESSING_H
