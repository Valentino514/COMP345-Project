#include <iostream>
#include <array>
#include "testOrders.h"
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


testOrders::testOrders() {
    playerList = new vector<Player*>;
}

testOrders::testOrders(testOrders& other){ }

testOrders::~testOrders() {
    if (playerList) {
        for (Player* player : *playerList) {
                cout<<"Player "<<(*player->getName())<< " has been eliminated \n";


            delete player; // Deletes each Player object

        }
        delete playerList; // Deletes the vector itself
        playerList = nullptr;
    }
    delete[] map;
    delete[] commands;
}


void testOrders::navigate() {
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


void testOrders::testTheOrders() {
    cout << "Welcome to the Game Startup Phase.\n";
    MapLoader x;
    vector<string> mapFiles;
    string mapsDirectory = "./Map/maps";  // Directory where the map files are stored
    CommandProcessor cp;

    while (true) {
        cout << "Please enter your command:\n";
        string command = cp.readCommand1();  // Read the entire command as a single string

        if (command == "loadmap") {
            cout << "Here is a list of maps, please choose one:\n";
            for (const auto& entry : filesystem::directory_iterator(mapsDirectory)) {
                if (entry.is_regular_file()) {
                    string filename = entry.path().filename().string();
                    cout << "  " << filename << "\n";
                    mapFiles.push_back(filename);  // Store filenames in a vector
                }
            }

            string chosenMap = "map1.txt";

            if (find(mapFiles.begin(), mapFiles.end(), chosenMap) != mapFiles.end()) {
                cout << "You have selected: " << chosenMap << "\n";
                string fullPath = mapsDirectory + "/" + chosenMap;
                Cmap = x.loadMap(fullPath);
            } else {
                cout << "Invalid map name. Please choose from the available maps.\n";
            }
        } else if (command == "validatemap") {
            if (Cmap != nullptr) {
                if (Cmap->validate()) {
                    cout << "Map is valid.\n";
                } else {
                    cout << "Map is invalid.\n";
                }
            } else {
                cout << "No map loaded. Please load a map first.\n";
            }
        } else if (command == "addplayer") {
            addplayer();
        } else if (command == "gamestart") {
            if (Cmap != nullptr) {
                DistributeTerritories(*Cmap->Territories, *playerList);
                //shufflePlayers();
                assignArmyAmount(50);
                DrawTwoCards();
                break;  // Exit the loop once the game starts
            } else {
                cout << "No map loaded. Please load and validate a map before starting the game.\n";
            }
        } else {
            cout << "Unknown command. Please try again.\n";
        }
    }

    mainGameLoop();
}

void testOrders::addplayer() {
    string player1 = "player1";
    string player2 = "player2";
    playerList->push_back(new Player(player1)); 
    playerList->push_back(new Player(player2)); 

    cout <<"added player "<< player1 << ", "<< player2;
}

void testOrders::DistributeTerritories(unordered_map<string, Territory*> m,vector<Player*> p){

    if (p.empty() || m.empty()) {
        cout << "No players or no territories to distribute." << endl;
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


void testOrders::assignArmyAmount(int amount) {
    if (!playerList || playerList->empty()) {
        cout << "No players to assign armies." << endl;
        return;
    }

    // Iterate through each player in the list and assign them the given army amount
    for (Player* player : *playerList) {
        player->setArmyAmount(amount);  
    }

    cout << "Each player has been assigned " << amount << " armies." << endl;
}

void testOrders::DrawTwoCards() {
    if (!playerList || playerList->empty()) {
        cout << "No players available to draw cards." << endl;
        return;
    }

    Deck deck;  

    for (Player* player : *playerList) {
        
        Card* card1 = deck.draw();
        Card* card2 = deck.draw();

        cout<<player<< "drew "<< card1->getCardTypeName()<< "and "<<card2->getCardTypeName()<<endl;

        if (card1) player->addCard(card1);  
        if (card2) player->addCard(card2);

        cout << "Drew two cards for player: " << *(player->getName()) << endl;
    }
}

void testOrders::mainGameLoop() {
    bool gameRunning = true;

    do {
        cout<<" " << endl;
        
        // Check if any player owns all territories in the loaded map
        for (Player* player : *playerList) {
            vector<Territory*> ter = *player->getTerritories();
            cout<<"initial count: "<<ter[0]->getArmyAmount()<<endl;

            Deploy* deploy = new Deploy(ter[0],50,player);

            deploy->execute();


            cout<<"final count: "<<ter[0]->getArmyAmount()<<endl;
            
        }
    } while (gameRunning);
}


void testOrders::reinforcementPhase() {
    for (Player* player : *playerList) {
        int territoryCount = player->getTerritories()->size();
        int reinforcementArmies = max(territoryCount / 3, 3);  // Minimum reinforcement rule

        // Check for continent name and add bonus if player owns a continent
        for (const auto& continentPair : *Cmap->Continents) {
            const string& continentName = continentPair.first;
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
        cout << "Player " << *player->getName() << " receives " << reinforcementArmies 
                  << " reinforcement armies." << endl;
    }
}


void testOrders::issueOrdersPhase() {
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


void testOrders::executeOrdersPhase() {
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