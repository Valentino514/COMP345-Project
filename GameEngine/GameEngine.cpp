#include <iostream>
#include <array>
#include "GameEngine.h"
#include "../Map/Map.h"
#include "../Player/Player.h"
#include "../CardsDeckHand/Cards.h"
#include "../CommandProcessing/CommandProcessing.h"
#include "../Log/LoggingObserver.h"

#include <string>
#include <vector>
#include <filesystem> 
#include <algorithm>
#include <random>
#include <iomanip>


using namespace std;

/**
 * @class GameEngine
 * @brief Represents the core game engine, managing the game states, players, maps, commands, and phases.
 * 
 * The GameEngine class implements the main logic for navigating through the game phases, managing game states,
 * executing commands, and running the game or tournament modes. It integrates with various components like 
 * players, maps, cards, and observers.
 */


// Default constructor
/**
 * @brief Initializes a new GameEngine object with default state and observer.
 */
GameEngine::GameEngine():currentIndex(new int(0)){
    playerList = new std::vector<Player*>;
    LogObserver* logObserver = new LogObserver();
    this->attach(logObserver);
}

// Copy constructor
/**
 * @brief Creates a copy of another GameEngine object, including observers.
 * 
 * @param other The GameEngine object to copy.
 */
GameEngine::GameEngine(GameEngine& other){
    LogObserver* logObserver = new LogObserver();
    this->attach(logObserver);
 }

// Destructor
/**
 * @brief Releases all resources used by the GameEngine, including players and observers.
 */
GameEngine::~GameEngine() {
    if (playerList) {
        for (Player* player : *playerList) {


            delete player; // Deletes each Player object

        }
        delete playerList; // Deletes the vector itself
        playerList = nullptr;
    }
    delete[] map;
    delete[] commands;
    observers.clear();

}

// Stream insertion operator
/**
 * @brief Outputs the current state of the GameEngine object to an output stream.
 * 
 * @param os The output stream.
 * @param gameEngine The GameEngine object to output.
 * @return Reference to the output stream.
 */
ostream& operator<<(ostream& os, const GameEngine& gameEngine) {
    os << "Map States:\n";
    for (int i = 0; i < 8; i++) {
        os << "  " << gameEngine.map[i] << "\n";
    }
    os << "Commands:\n";
    for (int i = 0; i < 11; i++) {
        os << "  " << gameEngine.commands[i] << "\n";
    }
    return os;
}

// Assignment operator
/**
 * @brief Assigns the state of one GameEngine object to another.
 * 
 * @param other The GameEngine object to copy.
 * @return Reference to the updated GameEngine object.
 */
GameEngine& GameEngine::operator=(const GameEngine& other) {
    if (this != &other) {  // Check for self
        delete[] map;
        delete[] commands;

        map = new string[8];
        commands = new string[11];

        for (int i = 0; i < 8; i++) {
            map[i] = other.map[i];
        }

        for (int i = 0; i < 11; i++) {
            commands[i] = other.commands[i];
        }
    }

    return *this;
}

// Logs the current state of the GameEngine
/**
 * @brief Returns a string representation of the GameEngine's state for logging.
 * 
 * @return A string describing the current state.
 */
std::string GameEngine::stringToLog() const {
    return "GameEngine State Change: New state is " + map[*currentIndex];
}

// Navigates through the game states
/**
 * @brief Allows the user to navigate through the game states based on commands.
 * 
 * This method manages the transitions between game states, processes user input, 
 * and invokes state-specific functionality. It also checks for end-game conditions.
 * 
 * @details The navigation starts with the current state and transitions based on valid commands. 
 * Special handling is applied for looping states and exit conditions.
 */
void GameEngine::navigate() {
    std::string* command = new std::string;

    while (true) {
        
        std::cout << "Current state: " << map[*currentIndex] << std::endl;
        notify();

        // Check for win state
        if (*currentIndex == 7) {
            std::cout << "Type 'play' to begin a new game, or 'end' to finish the game: ";
            std::cin >> std::ws;
            getline(std::cin, *command);

            if (*command == "end") {
                std::cout << "Exiting..." << std::endl;
                break;
            } else if (*command == commands[9]) {
                *currentIndex = 0;
                std::cout << "Starting new game..." << std::endl;
                std::cout << "Current state: " << map[*currentIndex] << std::endl;
                notify();
            }
            continue;
        }

        // Prompt for next move
        std::cout << "Type '" << commands[*currentIndex] << "' to move to the next state, or 'exit' to end: ";
        std::cin >> std::ws;
        getline(std::cin, *command);

        // Exit option
        if (*command == "exit") {
            std::cout << "Exiting..." << std::endl;
            break;
        }

        // Proceed to the next state if the command matches
        if (*command == commands[*currentIndex]) {
            (*currentIndex)++;
            notify();

            // Looping states
            if (*currentIndex == 1 || *currentIndex == 3 || *currentIndex == 5 || *currentIndex == 6) {
                while (true) {
                    std::cout << "Current state: " << map[*currentIndex] << std::endl;

                    // Execute order and prompt
                    if (*currentIndex == 6) {
                        std::cout << "Type '" << commands[*currentIndex] << "' to loop,\n type 'exit' to terminate,\n type '"
                                  << commands[(*currentIndex) + 2] << "' to move to " << map[*currentIndex + 1]
                                  << " state\n or type '" << commands[(*currentIndex) + 1] << "' to go back to "
                                  << map[(*currentIndex) - 2] << ": ";
                    } else {
                        std::cout << "Type the state again to loop, 'exit' to terminate, or '"
                                  << commands[*currentIndex] << "' to move to the next state: ";
                    }

                    std::cin >> std::ws;
                    getline(std::cin, *command);

                    // Handle commands in the looped state
                    if (*currentIndex == 6) {
                        if (*command == commands[*currentIndex]) {
                            continue;  // Loop
                        } else if (*command == commands[(*currentIndex) + 2]) {
                            (*currentIndex)++;
                            break;
                        } else if (*command == commands[(*currentIndex) + 1]) {
                            *currentIndex -= 2;
                            break;
                        }
                    } else if (*command == commands[*currentIndex - 1]) {
                        continue;  // Loop
                    } else if (*command == commands[*currentIndex]) {
                        (*currentIndex)++;
                        if (*currentIndex == 6) {
                            continue;
                        }
                        break;
                    } else if (*command == "exit") {
                        std::cout << "Exiting..." << std::endl;
                        return;
                    } else {
                        std::cout << "Invalid input, try again." << std::endl;
                    }
                }
            }
        } else {
            std::cout << "Invalid input, try again." << std::endl;
        }
    }
    delete command;
    delete currentIndex;
    command = nullptr;
    currentIndex = nullptr;
}

// Adds an observer to the GameEngine
/**
 * @brief Registers an observer to monitor GameEngine state changes.
 * 
 * @param observer A pointer to the Observer object to add.
 */
void GameEngine::addObserver(Observer* observer) {
    observers.push_back(observer);
    observer->setSubject(this);
}

// Notifies all observers of a state change
/**
 * @brief Notifies all registered observers about the current GameEngine state.
 */
void GameEngine::notify() {
    for (Observer* observer : observers) {
        observer->update(); 
    }
}

// Retrieves the current game state
/**
 * @brief Gets the current state of the GameEngine as a string.
 * 
 * @return A string representing the current game state.
 */
std::string GameEngine::getState() const {
    return map[*currentIndex];
}

// Startup phase of the game
/**
 * @brief Manages the initialization phase, allowing users to set up tournament or normal game mode.
 * 
 * This phase involves loading maps, adding players, validating maps, and transitioning to gameplay.
 * 
 * @details Users can select tournament mode or normal gameplay. Based on the choice, appropriate commands 
 * are processed, and the game transitions through its setup states.
 */
void GameEngine::startupPhase() {
    std::cout << "Welcome to the Game Startup Phase.\n";

    // Open gamelog.txt in append mode to log the executed orders
    ofstream logFile("gamelog.txt", ios::app);  
    if (!logFile) {
        cerr << "Error opening log file!" << endl;
        return;
    }

    // Ask the user if they want to enter tournament mode
    std::string choice;
    std::cout << "Do you want to enter a tournament mode? (Y/N): ";
    std::cin >> choice;
    std::cin.ignore(); // Clear the newline left in the buffer

    CommandProcessor cp;
    MapLoader x;
    std::vector<std::string> mapFiles;
    std::string mapsDirectory = "./Map/maps"; // Directory for map files

    if (choice == "Y" || choice == "y") {
        // Tournament mode
        while (true) {
            std::cout << "Please enter the tournament command in this format:\ntournament -M <listofmapfiles> -P <listofplayerstrategies> -G <numberofgames> -D <maxnumberofturns>\n";
            std::string command = cp.readCommand1(); // Use readCommand1 for tournament input
            std::cout << std::endl;

            if (command.substr(0, 10) == "tournament") {
                TournamentParams params = cp.parseTournamentCommand(command);
                if (params.maps.empty()) {
                    std::cout << "Invalid tournament command. Exiting.\n";
                    logFile << "Exiting program due to invalid tournament command.\n";
                    return;
                }

                // Add players to the GameEngine's playerList based on strategies
                addPlayersToGameEngine(params.strategies);

                // Execute the tournament
                logFile << "Executing tournament mode.\n";
                executeTournament(params);
                exit(0);
            } else {
                std::cout << "Invalid tournament command. Please try again.\n";
            }
        }
    } else if (choice == "N" || choice == "n") {
        // Normal game mode
        while (true) {
            std::cout << "Please enter your command:\n";
            std::string command = cp.readCommand2(); // Use readCommand for normal game input
            std::cout << std::endl;

            // Check the current state and validate command
            if (*cp.currentState == "start" && command == "loadmap") {
                // List available map files
                std::cout << "Here is a list of maps, please choose one:\n";
                for (const auto& entry : std::filesystem::directory_iterator(mapsDirectory)) {
                    if (entry.is_regular_file()) {
                        std::string filename = entry.path().filename().string();
                        std::cout << "  " << filename << "\n";
                        mapFiles.push_back(filename); // Store filenames in a vector
                    }
                }

                // Ask the user to select a map
                std::string chosenMap;
                std::cout << "\nPlease enter the name of the map you want to load: ";
                std::cin >> chosenMap;

                if (std::find(mapFiles.begin(), mapFiles.end(), chosenMap) != mapFiles.end()) {
                    std::cout << "You have selected: " << chosenMap << "\n\n";
                    std::string fullPath = mapsDirectory + "/" + chosenMap;
                    Cmap = x.loadMap(fullPath);
                    *cp.currentState = "maploaded"; // Update state after successful map loading
                    (*currentIndex)++;
                    notify();
                } else {
                    std::cout << "Invalid map name. Please choose from the available maps.\n";
                }
            } else if (*cp.currentState == "maploaded" && command == "validatemap") {
                if (Cmap != nullptr) {
                    if (Cmap->validate()) {
                        *cp.currentState = "mapvalidated"; // Update state after successful validation
                        (*currentIndex)++;
                        notify();
                    } else {
                        std::cout << "Map is invalid.\n";
                    }
                } else {
                    std::cout << "No map loaded. Please load a map first.\n";
                }
            } else if (*cp.currentState == "mapvalidated" && command == "addplayer") {
                addplayer();
                std::cout << std::endl;
                *cp.currentState = "playersadded"; // Update state after adding players
                (*currentIndex)++;
                notify();
            } else if (*cp.currentState == "playersadded" && command == "gamestart") {
                if (Cmap != nullptr) {
                    std::cout << std::endl;
                    DistributeTerritories(*Cmap->Territories, *playerList);
                    shufflePlayers();
                    assignArmyAmount(50);
                    DrawTwoCards();
                    *cp.currentState = "assignreinforcement"; // Update state to assign reinforcements
                    (*currentIndex)++;
                    notify();
                    break; // Exit the loop once the game starts
                } else {
                    std::cout << "No map loaded. Please load and validate a map before starting the game.\n";
                }
            } else {
                std::cout << "Unknown command or command not allowed in the current state. Please try again.\n";
            }
        }
    } else {
        std::cout << "Invalid choice. Please restart the game and enter Y or N.\n";
    }
}

// Adds players to the GameEngine
/**
 * @brief Adds players to the game and assigns strategies to them.
 * 
 * @param strategies A vector of strings representing player strategies.
 */
void GameEngine::addPlayersToGameEngine(const std::vector<std::string>& strategies) {
    // Maintain a count of players for each strategy type
    std::map<std::string, int> strategyCount;

    // Open gamelog.txt in append mode to log the executed orders
    ofstream logFile("gamelog.txt", ios::app);  
    if (!logFile) {
        cerr << "Error opening log file!" << endl;
        return;
    }

    for (const std::string& strategy : strategies) {
        // Increment the count for the current strategy
        strategyCount[strategy]++;

        // Create a name for the player based on their strategy
        std::string name = strategy + " Player " + std::to_string(strategyCount[strategy]);

        // Create a new Player object with the generated name
        Player* player = new Player(name);

        // Assign the appropriate strategy
        if (strategy == "Aggressive") {
            player->setStrategy(new AggresivePlayerStrategy());
        } else if (strategy == "Benevolent") {
            player->setStrategy(new BenevolentPlayerStrategy());
        } else if (strategy == "Neutral") {
            player->setStrategy(new NeutralPlayerStrategy());
        } else if (strategy == "Cheater") {
            player->setStrategy(new CheaterPlayerStrategy());
        } else {
            // Handle invalid strategy input
            std::cout << "Invalid strategy type: " << strategy << "\n";
            delete player; // Avoid memory leak
            continue; // Skip to the next strategy
        }

        // Add the configured player to the GameEngine's player list
        playerList->push_back(player);
        logFile <<"Added player " + name + " with strategy " + strategy << endl;

        // confirmation output
        std::cout << name << " has been added to the game.\n";
    }
}


// Executes the tournament mode
/**
 * @brief Executes a tournament mode based on provided parameters.
 * 
 * @param params A struct containing tournament configuration details, such as maps, strategies, games, and turns.
 */
void GameEngine::executeTournament(const TournamentParams& params) {
    MapLoader x ;

    // Open gamelog.txt in append mode to log the executed orders
    ofstream logFile("gamelog.txt", ios::app);  
    if (!logFile) {
        cerr << "Error opening log file!" << endl;
        return;
    }

    // Log tournament setup
    logFile << "\n\nTournament mode:\n";
    logFile << "M: ";
    for (const auto& map : params.maps) {
        logFile << map << " ";
    }
    logFile << ".\n";
    logFile << "P: ";
    for (const auto& strat : params.strategies) {
        logFile << strat << " ";
    }
    logFile << ".\n";
    logFile << "G: " << params.games << ".\n";
    logFile << "D: " << params.maxTurns << ".\n\n\n";

    // Matrix to store tournament results
    std::vector<std::vector<std::string>> results(params.maps.size(), std::vector<std::string>(params.games, "Draw"));
    std::unordered_map<std::string, int> mapIndex;
    for (size_t i = 0; i < params.maps.size(); ++i) {
        mapIndex[params.maps[i]] = i;
    }

    // Iterate through each map specified in the tournament parameters
    for (const std::string& map : params.maps) {
        for (int game = 1; game <= params.games; ++game) {

            // Display the current game and map being played
            std::string startMsg = "Playing game " + std::to_string(game) + " on map " + map + "...";
            std::cout << startMsg << "\n";
            logFile << startMsg << endl;
            std::string mapsDirectory = "./Map/maps"; 
            string fullPath = mapsDirectory + "/" + map;
            Cmap = x.loadMap(fullPath);

            // Validate the map before starting the game
            if (Cmap->validate()) {
                std::string validMsg = "Valid map: " + map + ". Starting game.";
                std::cout << validMsg << "\n";
                logFile << validMsg << endl;
            }
            else{
                std::string errorMsg = "Invalid map: " + map + ". Skipping game.";
                std::cout << errorMsg << "\n";
                logFile << errorMsg << endl;
                continue;
            }

            // Assign players and initial setup
            logFile << "Distributing territories for map " << map << endl;
            DistributeTerritories(*Cmap->Territories, *playerList);
            
            logFile << "Shuffling players..." << endl;
            shufflePlayers();
            
            logFile << "Assigning armies..." << endl;
            assignArmyAmount(50);

            logFile << "Drawing initial cards..." << endl;
            DrawTwoCards();

            // Play game
            bool gameWon = false;
            for (int turn = 1; turn <= params.maxTurns; ++turn) {
                std::string turnMsg = "Turn " + std::to_string(turn) + "...";
                std::cout << turnMsg << "\n";
                logFile << turnMsg << endl;

                //reinforcements phase to give players troops before round start
                reinforcementPhase();
                logFile << "Reinforcement phase completed.\n" << endl;

                // Issue orders for each player
                issueOrdersPhase();
                logFile << "Issue orders phase completed.\n" << endl;

                // Execute orders
                executeOrdersPhase();
                logFile << "Execute orders phase completed.\n" << endl;

                // Check for a winner
                Player* winner = checkWinner(*Cmap->Territories);
                if (winner) {
                    results[mapIndex[map]][game - 1] = *winner->getName();
                    std::string winnerMsg = "Player " + *winner->getName() + " won the game on map " + map + "!";
                    std::cout << winnerMsg << "\n";
                    logFile << winnerMsg << endl;
                    gameWon = true;
                    break;
                }
            }

            // If no winner after maxTurns, declare a draw
            if (!gameWon) {
                results[mapIndex[map]][game - 1] = "Draw";
                std::string drawMsg = "Game " + std::to_string(game) + " on map " + map + " declared a draw.";
                std::cout << drawMsg << "\n";
                logFile << drawMsg << endl;
            }
        }
    }
    //Tabulating tournament results
    logFile << "\nTournament Results:\n\n";
    logFile << std::setw(24) << std::left << " ";
    
    // Dynamically calculate the column width
    int columnWidth = 20; // Default column width
    for (const auto& row : results) {
        for (const auto& result : row) {
            columnWidth = std::max(columnWidth, static_cast<int>(result.length() + 5)); // Add a little buffer
        }
    }

    // Header row
    for (int game = 1; game <= params.games; ++game) {
        logFile << std::setw(columnWidth) << std::left <<("|Game " + std::to_string(game));
    }
    logFile << "|\n";

    // Add dashed line after header
    logFile << std::string(columnWidth, '-') << " ";  // Dash for "Map" column
    for (int game = 1; game <= params.games; ++game) {
        logFile << std::string(columnWidth, '-');  // Dash for each game column
    }
    logFile << "\n";

    // Results for each map
    for (size_t mapIdx = 0; mapIdx < params.maps.size(); ++mapIdx) {
        logFile << std::setw(columnWidth) << std::left <<("|Map |" + params.maps[mapIdx]);
        for (int game = 0; game < params.games; ++game) {
            logFile << std::setw(columnWidth) << std::left <<"|" + results[mapIdx][game];
        }
        logFile << "|\n";

        // Add dashed line after each map row
        logFile << std::string(columnWidth, '-') << " ";  // Dash for "Map" column
        for (int game = 1; game <= params.games; ++game) {
            logFile << std::string(columnWidth, '-');  // Dash for each game column
        }
    logFile << "\n";
    }

    std::cout << "Tournament completed.\n";
    logFile << "\nTournament completed!!\n" << endl;
}

    // Fn to test the Main game loop
  void GameEngine::testMainGame() {
    MapLoader x;

    while (true) {
      
    Cmap = x.loadMap("./Map/maps/map1.txt");
        

            addplayer();
            std::cout << std::endl;

   
                DistributeTerritories(*Cmap->Territories, *playerList);
                shufflePlayers();
                assignArmyAmount(50);
                DrawTwoCards();
               
       

    mainGameLoop();
  }
  }

// Checks if any player has won the game
/**
 * @brief Determines if a player has won by owning all territories.
 * 
 * @param allTerritories A map of territory names to Territory objects.
 * @return A pointer to the winning Player, or nullptr if no winner exists.
 */
 Player* GameEngine::checkWinner(const std::unordered_map<std::string, Territory*>& allTerritories) const {
    for (Player* player : *playerList) {
        bool ownsAll = true;

        // Check each territory to see if the player is the occupier
        for (const auto& pair : allTerritories) {
            if (pair.second->getLandOccupier() != player) {
                ownsAll = false;
                break;
            }
        }
        // If the player owns all territories, return them as the winner
        if (ownsAll) {
            return player;
        }
    }
    return nullptr; // No player owns all territories
}


// Adds players to the game
/**
 * @brief Prompts the user to enter player details and adds them to the game.
 * 
 * @details Ensures valid player count (between 2 and 6) and collects player names.
 */
void GameEngine::addplayer() {
    int playerAmount;

    while (true) {
        std::cout << "Please Enter the number of Players (2-6): ";
        std::cin >> playerAmount;
        std::cout <<endl;

        // Check if the input is a valid integer within the range 2-6
        if (std::cin.fail() || playerAmount < 2 || playerAmount > 6) {
            std::cin.clear(); // Clear the error flag
            std::cin.ignore(std::numeric_limits<std::streamsize>::max(), '\n'); // Discard invalid input
            std::cout << "Invalid Player Amount. Please enter a number between 2 and 6.\n\n";
        } else {
            break;
        }
    }

    if (!playerList) {
        playerList = new std::vector<Player*>;
    }

    // Loop to add players
    for (int i = 1; i <= playerAmount; i++) {
        std::string playerName;
        std::cout << "Please enter the name of Player " << i << ": ";
        std::cin >> playerName;

        // Create a new Player object and add it to playerList
        playerList->push_back(new Player(playerName)); 
        std::cout <<endl;
        
    }

    std::cout << playerAmount << " players added successfully!\n";
}

// Distributes territories to players
/**
 * @brief Assigns territories to players in a round-robin fashion.
 * 
 * @param m A map of territory names to Territory objects.
 * @param p A vector of Player pointers.
 */
void GameEngine::DistributeTerritories(unordered_map<std::string, Territory*> m,vector<Player*> p){

    if (p.empty() || m.empty()) {
        std::cout << "No players or no territories to distribute." << std::endl;
        return;
    }
auto it = m.begin();
int numPlayers = p.size();
    
   

    // Assign territories in round-robin fashion
    int playerIndex = 0;

    while (it != m.end()) {
        Territory* territory = it->second;
        // Assign the current territory to the current player
        p[playerIndex]->addTerritory(it->second); 

        territory->setLandOccupier(p[playerIndex]);

    
        ++it;

        // Move to the next player, wrap around if at the end of the player list
        playerIndex = (playerIndex + 1) % numPlayers;
    }

}

// Shuffles players in random order
/**
 * @brief Randomly shuffles the order of players for fairness.
 */
void GameEngine::shufflePlayers() {
    if (!playerList || playerList->empty()) {
        std::cout << "No players to shuffle." << std::endl;
        return;
    }

    // Create a random number generator 
    std::random_device rd;
    std::mt19937 g(rd());

    // Shuffle the playerList
    std::shuffle(playerList->begin(), playerList->end(), g);

    std::cout << "Players shuffled successfully!" << std::endl;
}

// Assigns a fixed number of armies to each player
/**
 * @brief Assigns a specified amount of armies to all players.
 * 
 * @param amount The number of armies to assign to each player.
 */
void GameEngine::assignArmyAmount(int amount) {
    if (!playerList || playerList->empty()) {
        std::cout << "No players to assign armies." << std::endl;
        return;
    }

    // Iterate through each player in the list and assign them the given army amount
    for (Player* player : *playerList) {
        player->setArmyAmount(amount);  
    }
    
    std::cout<<endl;
    std::cout << "Each player has been assigned " << amount << " armies.\n" << std::endl;
}

// Draws two cards for each player
/**
 * @brief Allows each player to draw two cards from the deck.
 */
void GameEngine::DrawTwoCards() {
    if (!playerList || playerList->empty()) {
        std::cout << "No players available to draw cards." << std::endl;
        return;
    }

    Deck deck;  

    for (Player* player : *playerList) {
        
        Card* card1 = deck.draw();
        Card* card2 = deck.draw();

        if (card1) player->addCard(card1);  
        if (card2) player->addCard(card2);

        std::cout << "Drew two cards for player: " << *(player->getName())<<"\n" << std::endl;
    }
}

// Main game loop
/**
 * @brief Executes the main gameplay loop, including all phases and victory conditions.
 */
void GameEngine::mainGameLoop() {
    bool gameRunning = true;

    while (gameRunning) {
        // Check if any player owns all territories in the loaded map
        for (Player* player : *playerList) {
            bool ownsAllTerritories = true;

            for (const auto& territoryPair : *Cmap->Territories) {
                Territory* territory = territoryPair.second;

                if (std::find(player->getTerritories()->begin(), player->getTerritories()->end(), territory) == player->getTerritories()->end()) {
                    ownsAllTerritories = false;
                    break;
                }
            }

            // If the player owns all territories, they win the game
            if (ownsAllTerritories) {
                std::cout << "Player " << *(player->getName()) << " owns all territories and wins the game!" << std::endl;
                gameRunning = false;
                break;
            }
        }

        // Exit loop if a player has won
        if (!gameRunning) break;

        std::cout << "------------------- Starting Reinforcement Phase -------------------\n" << std::endl;

        // Execute the game phases
        reinforcementPhase();
        issueOrdersPhase();
        executeOrdersPhase();
    }
}

// Reinforcement phase of the game
/**
 * @brief Distributes reinforcement armies to players based on territories and continent bonuses.
 */
void GameEngine::reinforcementPhase() {
    for (Player* player : *playerList) {
        int territoryCount = player->getTerritories()->size();
        int reinforcementArmies = std::max(territoryCount / 3, 3);  // Minimum reinforcement rule

        // Check for continent name and add bonus if player owns a continent
        for (const auto& continentPair : *Cmap->Continents) {
            const std::string& continentName = continentPair.first; 
            int continentBonus = continentPair.second;
            bool ownsAllInContinent = true;

            // Check if player owns all territories in this continent
           for (const auto& territoryPair : *Cmap->Territories) {
            Territory* territory = territoryPair.second;
                if (territory->getContinent() == continentName) {
                    if (territory->getLandOccupier() != player) {
                        ownsAllInContinent = false;
                        break;
                    }
                }
            }

            // Add continent control bonus if the player owns all territories
            if (ownsAllInContinent) {
                reinforcementArmies += continentBonus;
            }
        }

        // Update player's army amount with reinforcements
        int currentArmies = player->getArmyAmount();
        player->setArmyAmount(currentArmies + reinforcementArmies);
        std::cout << "Player " << *player->getName() << " receives " << reinforcementArmies 
                  << " reinforcement armies.\n" << std::endl;
    }
}

// Issues orders for players
/**
 * @brief Facilitates players in issuing orders during the orders phase.
 */
void GameEngine::issueOrdersPhase() {
    bool allPlayersDone = false;

    while (!allPlayersDone) {
        allPlayersDone = true; // Assume all players are done initially

        for (Player* player : *playerList) {
            // Check if the player can still issue orders (armyamount > 0 or has cards)
            if (player->getArmyAmount() > 0 || !player->getCards().empty()) {
                allPlayersDone = false; // If any player can still issue orders, continue
                player->issueOrder(*playerList);   // Call the player's issueOrder method to add an order to their list
            }
        }
                allPlayersDone = true; // Assume all players are done initially

    }
}

// Executes all orders
/**
 * @brief Executes orders in a structured manner, eliminating players without territories.
 */
void GameEngine::executeOrdersPhase() {
    cout << "Starting Orders Execution Phase...\n";

    // Open gamelog.txt in append mode to log the executed orders
    ofstream logFile("gamelog.txt", ios::app);  
    if (!logFile) {
        cerr << "Error opening log file!" << endl;
        return;
    }

    // Execute all Deploy orders first in a round-robin fashion across all players
    bool deployOrdersRemaining = true;
    while (deployOrdersRemaining) {
        deployOrdersRemaining = false;

        for (Player* player : *playerList) {
            OrdersList* ordersList = player->getOrdersList(); 

            if (ordersList->hasDeployOrder()) {
                Order* deployOrder = ordersList->getNextDeployOrder();
                if (deployOrder) {
                    deployOrder->execute(); 
                    logFile << "Executing deploy order: " << *deployOrder << endl;
                    notify();  
                    delete deployOrder;     
                    deployOrdersRemaining = true; // Set to true if there are more deploy orders
                }
            }
        }
    }

    // Execute remaining orders in a round-robin style
    bool ordersRemaining = true;
    while (ordersRemaining) {
        ordersRemaining = false;

        for (Player* player : *playerList) {
            OrdersList* ordersList = player->getOrdersList(); 

            if (!ordersList->isEmpty()) {
                Order* nextOrder = ordersList->getNextOrder(); // Get the next available order
                if (nextOrder) {
                    nextOrder->execute();  
                    logFile << "Executing order: " << *nextOrder << endl;
                    notify();  
                    delete nextOrder;      
                    ordersRemaining = true; // Set to true if there are more orders
                }
            }
        }

        // Eliminate players who no longer control any territories
        for (auto it = playerList->begin(); it != playerList->end();) {
            if ((*it)->getTerritoryCount() == 0) { 
                cout << *(*it)->getName() << " has been eliminated.\n";
                logFile << *(*it)->getName() << " has been eliminated.\n";              
                delete *it;              // Free memory for eliminated player
                it = playerList->erase(it); // Remove player from the list
            } else {
                ++it;
            }
        }
    }
    cout << "Orders Execution Phase completed.\n";
    logFile.close();
}