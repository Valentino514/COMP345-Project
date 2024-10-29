#ifndef COMMANDPROCESSING_H
#define COMMANDPROCESSING_H

#include <string>
#include <vector>
#include <iostream>
#include <fstream>
#include "Map.h"
#include "Player.h"

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
    CommandProcessor(Map* gameMap);
    CommandProcessor(const CommandProcessor& other); // Copy constructor
    CommandProcessor& operator=(const CommandProcessor& other); // Assignment operator
    virtual ~CommandProcessor(); // Destructor

    Command* getCommand();
    virtual void processInput();

    friend std::ostream& operator<<(std::ostream& os, const CommandProcessor& cp); // Stream insertion operator

protected:
    std::vector<Command*>* commands;
    std::string currentState;

    virtual void readCommand();
    void saveCommand(Command* command);
    void validate(Command* command);
    void copy(const CommandProcessor& other);
    void clear();

private:
    Map* map;  // Pointer to the Map instance
    std::vector<Player*> players;  // Container to store created Player objects

    void createPlayer(const std::string& playerName);  // Helper method to create a Player
    void clearPlayers();  // Helper method to clear Player instances
};

// FileCommandProcessorAdapter Class
class FileCommandProcessorAdapter : public CommandProcessor {
public:
    FileCommandProcessorAdapter(Map* gameMap, const std::string& filename);
    ~FileCommandProcessorAdapter();

protected:
    void readCommand() override;

private:
    std::ifstream commandFile;
};

#endif // COMMANDPROCESSING_H
