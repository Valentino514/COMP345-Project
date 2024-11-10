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
class Command : public ILoggable {
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
    CommandProcessor();
    CommandProcessor(Map* gameMap);                    // Constructor with map pointer
    CommandProcessor(const CommandProcessor& other);   // Copy constructor
    CommandProcessor& operator=(const CommandProcessor& other); // Assignment operator
    virtual ~CommandProcessor();                       // Destructor
    
    std::string readCommand1();
    Command* getCommand();                             // Retrieves the latest command
    virtual void processInput();                       // Processes user input
    std::string currentState;                          // Tracks the current state of command processing
    void saveCommand(Command* command);
    void validate();
    void notify();
    void attach(Observer* observer);
    std::string stringToLog() const override;                    // Processes user input


    friend std::ostream& operator<<(std::ostream& os, const CommandProcessor& cp); // Stream insertion operator for printing

protected:
    std::vector<Command*>* commands;                   // Stores commands issued to the processor
    std::vector<Observer*> observers;

    virtual std::string* readCommand();                // Reads and splits command input into two parts (command and argument)
    bool validate(Command* command);
    void copy(const CommandProcessor& other);          // Copies command data
    void clear();                                      // Clears commands from memory

private:
    Map* map;                                          // Pointer to the Map instance
    std::vector<Player*> players;                      // Container to store created Player objects

    void createPlayer(const std::string& playerName);  // Helper method to create a Player
    void clearPlayers();                               // Helper method to clear Player instances
};

// FileCommandProcessorAdapter Class
class FileCommandProcessorAdapter : public CommandProcessor {
public:
    FileCommandProcessorAdapter(Map* gameMap, const std::string& filename); // Constructor with map and filename
    ~FileCommandProcessorAdapter();                                         // Destructor to close file
    std::ifstream commandFile;                                              // File stream for reading commands
protected:
    std::string* readCommand() override;                                    // Reads command from file and splits it



};

#endif // COMMANDPROCESSING_H
