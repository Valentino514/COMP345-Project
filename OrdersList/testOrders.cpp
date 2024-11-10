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
                addplayer();
                DistributeTerritories(*Cmap->Territories, *playerList);
                //shufflePlayers();
                assignArmyAmount(50);
                DrawTwoCards();
            }

    mainGameLoop();
}

void testOrders::addplayer() {
    string player1 = "player1";
    string player2 = "player2";
    playerList->push_back(new Player(player1)); 
    playerList->push_back(new Player(player2)); 
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

    cout << endl<<"Each player has been assigned " << amount << " armies." << endl;
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

        cout<<*(player->getName())<< " drew "<< card1->getCardTypeName()<< " and "<<card2->getCardTypeName()<<endl;

        if (card1) player->addCard(card1);  
        if (card2) player->addCard(card2);

    }
}

void testOrders::mainGameLoop() {
    bool gameRunning = true;

        cout<<" " << endl;
        
        // Check if any player owns all territories in the loaded map
            Player* player1 = (*playerList)[0];  
            Player* player2 = (*playerList)[1];  

            player1->printOwnedTerritories();
            player2->printOwnedTerritories();
            cout<<endl;
            vector<Territory*> ter1 = *player1->getTerritories();
            vector<Territory*> ter2 = *player2->getTerritories();
            cout<<endl;
            Deploy* deploy1= new Deploy(ter1[0],250,player1);
            deploy1->execute();
            cout<<endl;
            Deploy* deploy2 = new Deploy(ter2[0],250,player2);
            deploy2->execute();
            cout<<endl;
            Deploy* deploy3= new Deploy(ter2[0],5,player1);
            deploy3->execute();
            cout<<endl;

            Advance* advance1 = new Advance(player1, ter1[0],ter2[1],5);
            advance1->execute();
            cout<<endl;
            Advance* advance2 = new Advance(player1, ter1[0],ter2[11],5);
            advance2->execute();
            cout<<endl;
            Advance* advance3 = new Advance(player2, ter1[0],ter2[17],5);
            advance3->execute();
            cout<<endl;


            Card* bombCard1 = new Card(new Card::CardType(Card::Bomb));
            player1->addCard(bombCard1);  

            Card* bombCard2 = new Card(new Card::CardType(Card::Bomb));  
            player2->addCard(bombCard2);  

            Card* bombCard3 = new Card(new Card::CardType(Card::Bomb)); 
            player2->addCard(bombCard3); 

            cout<<"bombing adjacent territory"<<endl;
            Bomb* bomb1 = new Bomb(ter2[0],player1);
            bomb1->execute();
            cout<<endl;

            cout<<"bombing own territory"<<endl;
            Bomb* bomb2 = new Bomb(ter2[0],player2);
            bomb2->execute();
            cout<<endl;

            cout<<"bombing non adjacent territory"<<endl;
            Bomb* bomb3 = new Bomb(ter1[0],player2);
            bomb3->execute();
            cout<<endl;

            Card* airCard1 = new Card(new Card::CardType(Card::Airlift));  // Create a Bomb card
            player1->addCard(airCard1);  // Add it to player1's hand

            Card* airCard2 = new Card(new Card::CardType(Card::Airlift));  // Create a Bomb card
            player1->addCard(airCard2);  // Add it to player1's hand

            cout<<"airlift between two owned territories"<<endl;
            Airlift* airlift1 = new Airlift(ter1[0],ter1[1],15,player1);
            airlift1->execute();
            cout<<endl;
            cout<<"airlift to not owned territory"<<endl;
            Airlift* airlift2 = new Airlift(ter1[0],ter2[5],15,player1);
            airlift2->execute();


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