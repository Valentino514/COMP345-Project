#include <iostream>
#include <array>
#include "GameEngine.h"
#include "../Map/Map.h"
#include "../Player/Player.h"
#include "../CardsDeckHand/Cards.h"
#include <string>
#include <vector>
#include <filesystem> 
#include <algorithm>
#include <random>
#include "../CommandProcessing/CommandProcessing.h"



using namespace std;


GameEngine::GameEngine() {
    playerList = new std::vector<Player*>;
}

GameEngine::GameEngine(GameEngine& other){ }

GameEngine::~GameEngine() {
    if (playerList) {
        for (Player* player : *playerList) {
                std::cout<<"Player "<<(*player->getName())<< " has been eliminated \n";


            delete player; // Deletes each Player object

        }
        delete playerList; // Deletes the vector itself
        playerList = nullptr;
    }
    delete[] map;
    delete[] commands;
}


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
//Assignment operator
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

void GameEngine::navigate() {
    string* command = new string;
    int* currentIndex = new int(0);

    while (true) {
        //display current state
        cout << "current state: " << map[*currentIndex] << endl;

        //check for win state
        if ((*currentIndex) == 7) {
            cout << "type 'play' to begin a new game, or end to finish the game: ";
        cin >> ws;  
        getline(cin, *command); 

        if(*command == "end"){
            cout<<"exiting..";
            return;
        }else if(*command == commands[9]){
            (*currentIndex) = 0;
            cout<<"starting new game..."<<endl;
            cout<<"current state: "<<map[*currentIndex]<<endl;
        }
        }

        //next move
        cout << "Type '" << commands[*currentIndex] << "' to move to the next state, or type 'exit' to end: ";
        cin >> ws;  // Clear leading whitespaces
        getline(cin, *command);  // Get full command including spaces

        //exit
        if (*command == "exit") {
            cout << "Exiting..." << endl;
            return;
        }

        //next state
        if (*command == commands[*currentIndex]) {
            (*currentIndex)++;
            //looping orders
            if ((*currentIndex) == 1 || (*currentIndex) == 3 || (*currentIndex) == 5 || (*currentIndex) == 6) {
                while (true) {
                    cout << "current state: " << map[*currentIndex] << endl;

                    //execute order
                    if (*currentIndex == 6) {
                        cout <<"Type '"<<commands[*currentIndex]<< "' to loop,\n type exit to terminate,\n type '" //loop
                             << commands[(*currentIndex)+2] << "' to move to "<<map[*currentIndex+1]<< " state\n or type '" // win
                             <<commands[(*currentIndex)+1]<< "' to go back to "<<map[(*currentIndex)-2]<<": "; //assign rein.

                    } else {
                        cout << "Type the state again to loop, type exit to terminate, or '"
                             << commands[*currentIndex] << "' to move to the next state: ";
                    }
                    
                    cin >> ws;  
                    getline(cin, *command); 

                    //execute order logic
                    if(*currentIndex == 6){
                        if(*command == commands[*currentIndex]){
                            continue;
                        }
                        else if(*command == commands[(*currentIndex)+2]){
                            (*currentIndex)++;
                            break;
                        }
                        else if(*command == commands[(*currentIndex)+1]){
                            *currentIndex = (*currentIndex)-2;
                            break;
                        }
                    }
                    if (*command == commands[*currentIndex-1]) {
                       //loop
                        continue;
                    } else if (*command == commands[*currentIndex]) {
                        (*currentIndex)++;
                        if(*currentIndex==6){
                            continue;
                        }
                        break;

                    }else if (*command == "exit") {
                        cout << "Exiting..." << endl;
                        return;
                    } else {
                        cout << "Invalid input, try again." << endl;
                    }
                }
            }
        } else {
            cout << "Invalid input, try again." << endl;
        }
    }

    delete command;
    delete currentIndex;
    command = nullptr;
    currentIndex = nullptr;
}


void GameEngine::startupPhase() {
    std::cout << "Welcome to the Game Startup Phase.\n";
    MapLoader x;
    std::vector<std::string> mapFiles;
    std::string mapsDirectory = "./Map/maps";  // Directory where the map files are stored
    CommandProcessor cp;

    while (true) {
        std::cout << "Please enter your command:\n";
        std::string command = cp.readCommand1();  // Read the entire command as a single string
        std::cout << std::endl;

        // Check the current state and validate command
        if (cp.currentState == "start" && command == "loadmap") {
            // List available map files
            std::cout << "Here is a list of maps, please choose one:\n";
            for (const auto& entry : std::filesystem::directory_iterator(mapsDirectory)) {
                if (entry.is_regular_file()) {
                    std::string filename = entry.path().filename().string();
                    std::cout << "  " << filename << "\n";
                    mapFiles.push_back(filename);  // Store filenames in a vector
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
                cp.currentState = "maploaded"; // Update state after successful map loading
            } else {
                std::cout << "Invalid map name. Please choose from the available maps.\n";
            }
        } 
        else if (cp.currentState == "maploaded" && command == "validatemap") {
            if (Cmap != nullptr) {
                if (Cmap->validate()) {
                    cp.currentState = "mapvalidated"; // Update state after successful validation
                } else {
                    std::cout << "Map is invalid.\n";
                }
            } else {
                std::cout << "No map loaded. Please load a map first.\n";
            }
        } 
        else if (cp.currentState == "mapvalidated" && command == "addplayer") {
            addplayer();
            std::cout << std::endl;
            cp.currentState = "playersadded"; // Update state after adding players
        } 
        else if (cp.currentState == "playersadded" && command == "gamestart") {
            if (Cmap != nullptr) {
                std::cout << std::endl;
                DistributeTerritories(*Cmap->Territories, *playerList);
                shufflePlayers();
                assignArmyAmount(50);
                DrawTwoCards();
                cp.currentState = "assignreinforcement"; // Update state to assign reinforcements
                break;  // Exit the loop once the game starts
            } else {
                std::cout << "No map loaded. Please load and validate a map before starting the game.\n";
            }
        } else {
            std::cout << "Unknown command or command not allowed in the current state. Please try again.\n";
        }
    }

    mainGameLoop();
}


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

void GameEngine::mainGameLoop() {
    bool gameRunning = true;

    do {
        std::cout << "------------------- Starting Reinforcement Phase -------------------\n" << std::endl;
        
        // Execute the game phases
        reinforcementPhase();
        issueOrdersPhase();
        executeOrdersPhase();

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
    } while (gameRunning);
}


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
    }
}


void GameEngine::executeOrdersPhase() {
    cout << "Starting Orders Execution Phase...\n";

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
                    delete nextOrder;      
                    ordersRemaining = true; // Set to true if there are more orders
                }
            }
        }

        // Eliminate players who no longer control any territories
        for (auto it = playerList->begin(); it != playerList->end();) {
            if ((*it)->getTerritoryCount() == 0) { 
                cout << (*it)->getName() << " has been eliminated.\n";
                delete *it;              // Free memory for eliminated player
                it = playerList->erase(it); // Remove player from the list
            } else {
                ++it;
            }
        }
    }

    cout << "Orders Execution Phase completed.\n";
}