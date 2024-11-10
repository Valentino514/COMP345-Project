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
    std::cout << "==========================" << std::endl;
    std::cout << "     ORDERS TEST      " << std::endl;
    std::cout << "==========================" << std::endl;

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
