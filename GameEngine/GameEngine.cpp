#include <iostream>
#include <array>
#include "GameEngine.h"
#include "../Map/Map.h"
#include "../Player/Player.h"
#include <string>
#include <vector>
#include <filesystem> 



using namespace std;

GameEngine::GameEngine(){}

GameEngine::GameEngine(GameEngine& other){ }

GameEngine::~GameEngine(){
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

GameEngine& GameEngine::operator=(const GameEngine& other) {
    if (this != &other) {  // Check for self
        delete[] map;
        delete[] commands;

        // Allocate new memory for map and commands
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
    std::string command;
    std::cout << "Welcome to the Game Startup Phase. Please enter your commands:\n";
    Map* map = nullptr;
    MapLoader x;
    std::vector<std::string> mapFiles;
    std::string mapsDirectory = "../Maps/maps";  // Directory where your map files are stored


    while (true) {
        std::cout << "> ";
        std::cin >> command;

        if (command == "loadmap") {
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
            std::cout << "Please enter the name of the map you want to load: ";
            std::cin >> chosenMap;

            // Verify if the entered map exists in the list
            if (std::find(mapFiles.begin(), mapFiles.end(), chosenMap) != mapFiles.end()) {
                std::cout << "You have selected: " << chosenMap << "\n";
                map = x.loadMap(chosenMap);
            } else {
                std::cout << "Invalid map name. Please choose from the available maps.\n";
            }
        } 
        else if (command == "validatemap") {
            if (map != nullptr) {
                if (map->validate()) {
                    std::cout << "Map is valid.\n";
                } else {
                    std::cout << "Map is invalid.\n";
                }
            } else {
                std::cout << "No map loaded. Please load a map first.\n";
            }
        } 
        else if (command == "addplayer") {
            std::string playerName;
            addplayer();  
        } 
        else if (command == "gamestart") {
            if (map != nullptr) {
                DistributeTerritories(*map->Territories,*playerList);
                break;  // Exit the loop once game starts
            } else {
                std::cout << "No map loaded. Please load and validate a map before starting the game.\n";
            }
        } 
        else {
            std::cout << "Unknown command. Please try again.\n";
        }
    }
}


void GameEngine::addplayer() {
    int playerAmount;
    while (true) {
        std::cout << "Please Enter the number of Players (2-6): ";
        std::cin >> playerAmount;

        if (playerAmount >= 2 && playerAmount <= 6) {
            break;
        } else {
            std::cout << "\nInvalid Player Amount. Please try again.\n";
        }
    }

    // Loop to add players
    for (int i = 1; i <= playerAmount; i++) {
        std::string playerName;
        std::cout << "Please enter the name of Player " << i << ": ";
        std::cin >> playerName;

        // Add player to the playerList
        playerList->push_back(new Player(&playerName)); 
    }

    std::cout << playerAmount << " players added successfully!\n";
}

void DistributeTerritories(unordered_map<std::string, Territory*> m,vector<Player*> p){

    if (p.empty() || m.empty()) {
        std::cout << "No players or no territories to distribute." << std::endl;
        return;
    }
auto it = m.begin();
int numPlayers = p.size();
    
   

    // Assign territories in round-robin fashion
    int playerIndex = 0;

    while (it != m.end()) {
        // Assign the current territory to the current player
        p[playerIndex]->addTerritory(it->second); // Assuming `addTerritory` is a method in the Player class

        // Move to the next territory
        ++it;

        // Move to the next player, wrap around if at the end of the player list
        playerIndex = (playerIndex + 1) % numPlayers;
    }

}

 