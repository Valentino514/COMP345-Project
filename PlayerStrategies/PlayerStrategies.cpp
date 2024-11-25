#include "PlayerStrategies.h"
#include "../Player/Player.h"
#include <iostream>
#include <algorithm>
#include <unordered_set>
#include <limits>

// HumanPlayerStrategy Implementation
void HumanPlayerStrategy::issueOrder(Player* player,const std::vector<Player*>& playerList) {

       const std::vector<Card*>* cards = &(player->getCards());
    OrdersList* orders = player->getOrdersList();

    
    // Retrieve territories to defend and attack
     vector<Territory*> toDefend = player->toDefend();
    vector<Territory*> toAttack = player->toAttack();

    // Reinforcement phase
    cout<< "Player " << player->getName() << " - Select territories to reinforce.\n" << endl;
    for (size_t i = 0; i < toDefend.size(); ++i) {
        cout << i + 1 << ". " << toDefend[i]->getName() << ": " << toDefend[i]->getArmyAmount() << " armies." << endl;
    }

    while (player->getArmyAmount() > 0) { // Dosent allow the player to go to the next step without finishing reinforcements
        string territoryName;
        int reinforcementAmount = 0;
        
        cout << "\nYou have " << player->getArmyAmount() << " armies remaining. Enter territory name for reinforcement: ";
        cin >> territoryName;
        std::cout <<"\n";

        if (territoryName == "exit" && player->getArmyAmount()==0) break;

        // Locate the territory to deploy reinforcements
        Territory* selectedTerritory = nullptr;
        for (Territory* t : toDefend) {
            if (t->getName() == territoryName) {
                selectedTerritory = t;
                break;
            }
        }

        if (!selectedTerritory) {
            cout << "Territory not found. Try again.\n" << endl;
            continue;
        }

        // Validates and adds Reinforcement amount for the territory chosen 
        cout << "Enter the number of armies to reinforce " << selectedTerritory->getName() << ": ";
        while (!(cin >> reinforcementAmount) || reinforcementAmount < 0 || reinforcementAmount > player->getArmyAmount()) {
            cout << "Invalid amount. Enter a number between 0 and " << player->getArmyAmount() << ": ";
            cin.clear();
            cin.ignore(numeric_limits<streamsize>::max(), '\n');
        }

        // Create a Deploy order and add to orders list
         Order* deployOrder = new Deploy(selectedTerritory, reinforcementAmount,player);
         orders->addOrder(deployOrder);  
        player->setArmyAmount(player->getArmyAmount() - reinforcementAmount);
        cout << "Deploy order issued to add " << reinforcementAmount << " armies to " << selectedTerritory->getName() << ".\n" << endl;
    }

    cout << "\n------------------------------------------------\n         Reinforcement phase completed\n------------------------------------------------\n\n" << endl;

  bool issuingAdvanceOrders = true;
while (issuingAdvanceOrders) {
    toDefend=this->toDefend(player);
    toAttack = this->toAttack(player);

    cout << "Territories available to move/attack from:" << endl;
    cout << "------------------------------------------------\n" << endl;
    for (size_t i = 0; i < toDefend.size(); ++i) {
        cout << i + 1 << ". " << toDefend[i]->getName() << ": " << toDefend[i]->getArmyAmount() << " armies available." << endl;
    }

    // Gather source territory
    Territory* source = nullptr;
    while (true) {
        string sourceName;
        cout << "\nEnter the name of the territory to move troops from: ";
        cin >> sourceName;

        //Validate maybe will comment out
        for (Territory* t : toDefend) {
            if (t->getName() == sourceName) {
                source = t;
                break;
            }
        }
        if (source) break; // Exit Loop if player owns territory
        cout << "Source territory not found. Try again." << endl;
    }

    
    cout << "\nChoose a destination territory:" << endl;
    cout << "-----------------------------------------------" << endl;

    // Display player's own territories (for defensive moves)
    cout << "Your territories (to reinforce):" << endl;
    for (Territory* t : toDefend) {
        for(Territory* s: *source->getAdjacentTerritories()){
            if (t->getName()==s->getName())
            {
               cout << "- " << t->getName() << ": " << t->getArmyAmount() << " armies." << endl;
            }
            
        }
    }
    

    // Display enemy territories (for attack)
    cout << "\nEnemy territories (to attack):" << endl;
    for (Territory* t : toAttack) {
        for(Territory* s: *source->getAdjacentTerritories()){
            if (t->getName()==s->getName())
            {
               cout << "- " << t->getName() << ": " << t->getArmyAmount() << " armies." << endl;
            }
            
        }
    }

    // Get destination territory
    Territory* destination = nullptr;
    while (true) {
        string destinationName;
        cout << "\nEnter the name of the destination territory: ";
        cin >> destinationName;

        // Locate the destination in either toDefend or toAttack            (maybe comment out later )
        for (Territory* t : toDefend) {
            if (t->getName() == destinationName) {
                destination = t;
                break;
            }
        }
        if (!destination) { // If not found in toDefend, check in toAttack
            for (Territory* t : toAttack) {
                if (t->getName() == destinationName) {
                    destination = t;
                    break;
                }
            }
        }
        if (destination) break; // Exit loop if a valid destination is found
        cout << "Invalid destination. Try again." << endl;
    }

    // Choose army amount to move or attack
    int numArmies = 0;
    while (true) {
        cout << "Enter number of armies to move: ";
        if (cin >> numArmies && numArmies > 0) break;
        cout << "Invalid number. Enter a positive integer: ";
        cin.clear();
        cin.ignore(numeric_limits<streamsize>::max(), '\n');
    }

    
    Order* order = new Advance(player, source, destination, numArmies);  
    orders->addOrder(order); // Add the order to the player's order list

    cout << "Advance order issued to move " << numArmies << " armies from " << source->getName()
         << " to " << destination->getName() << "." << endl;

    // Ask if the player wants to issue another Advance order
     string decision;
    cout << "Would you like to issue another Advance order? (Y/N): ";
    cin >> decision;
    issuingAdvanceOrders = (decision == "Y" || decision == "y");
}

cout << "Orders Issuing phase for Advance orders completed.\n------------------------------------------------" << endl;


 while (!cards->empty()) {
        cout << player->getName() << " - Cards in hand:" << endl;
        for (size_t i = 0; i < cards->size(); ++i) {
            cout << i + 1 << ". " << cards->at(i)->getCardTypeName() << endl;
        }
        cout << "Would you like to play a card? (Y/N): ";
        char playCardDecision;
        cin >> playCardDecision;
        if (playCardDecision != 'Y' && playCardDecision != 'y') {
            break;
        }

        int cardChoice = 0;
        cout << "Select a card to play (enter the number): ";
        while (!(cin >> cardChoice) || cardChoice < 1 || cardChoice > cards->size()) {
            cout << "Invalid choice. Please select a valid card number: ";
            cin.clear();
            cin.ignore(numeric_limits<streamsize>::max(), '\n');
        }
       Card* selectedCard = cards->at(cardChoice - 1);

        Order* cardOrder = nullptr;
        if (selectedCard->getCardTypeName() == "Bomb") {
            Territory* target = player->selectTargetFromAttackList();
            cardOrder = new Bomb(target,player); // Bomb order targets enemy territory

            cout << "Bomb order created targeting " << target->getName() << "." << endl;
        } else if (selectedCard->getCardTypeName() == "Airlift") {
            Territory* source = player->selectSourceTerritory();
            Territory* destination = player->selectDestinationTerritory();
            int armyAmount =player-> selectArmyAmount(source);
            cardOrder = new Airlift(source, destination, armyAmount,player); // Airlift within own territories

            cout << "Airlift order created to move " << armyAmount << " armies from " << source->getName()
                 << " to " << destination->getName() << "." << endl;
        } else if (selectedCard->getCardTypeName() == "Blockade") {
            Territory* target = player->selectTargetFromDefendList();
            cardOrder = new Blockade(player, target); // Blockade on own territory

            cout << "Blockade order created on " << target->getName() << "." << endl;
        } else if (selectedCard->getCardTypeName() == "Diplomacy") {
            //Player* targetPlayer = selectPlayerToNegotiate(*playerList);
            Player* targetPlayer = player->selectPlayerToNegotiate(playerList);

            cardOrder = new Negociate(player, targetPlayer); // Negotiate with enemy player
            cout << "Negotiate order created with " << *(targetPlayer->getName()) << "." << endl;
         }

        if (cardOrder) {
            orders->addOrder(cardOrder);
        }
        player->removeCard(selectedCard);
    }

    cout << "Orders Issuing phase completed for player " << player->getName() << ".\n------------------------------------------------" << endl;}




std::vector<Territory*> HumanPlayerStrategy::toDefend(const Player* player) const {
    std::vector<Territory*> defendList;
    // Use player's territories
    const std::vector<Territory*>* territories = player->getTerritories();
    for (Territory* territory : *territories) {
        defendList.push_back(territory);
    }
    return defendList;
}

std::vector<Territory*> HumanPlayerStrategy::toAttack(const Player* player) const {
    std::vector<Territory*> attackList;
    std::unordered_set<Territory*> addedTerritories;

    // Use player's territories
    const std::vector<Territory*>* territories = player->getTerritories();
    for (Territory* territory : *territories) {
        const std::vector<Territory*>* adjacent = territory->getAdjacentTerritories();
        for (Territory* adj : *adjacent) {
            if (adj->getLandOccupier() != player && addedTerritories.find(adj) == addedTerritories.end()) {
                attackList.push_back(adj);
                addedTerritories.insert(adj);
            }
        }
    }
    return attackList;
}


// NeutralPlayerStrategy Implementation
void NeutralPlayerStrategy::issueOrder(Player* player,const std::vector<Player*>& playerList) {
    std::cout << "NeutralPlayer: Doing nothing.\n";
    // Does not issue any orders
}

std::vector<Territory*> NeutralPlayerStrategy::toDefend(const Player* player) const {
    return {}; // Does not defend
}

std::vector<Territory*> NeutralPlayerStrategy::toAttack(const Player* player) const {
    return {}; // Does not attack
}

// CheaterPlayerStrategy Implementation
void CheaterPlayerStrategy::issueOrder(Player* player,const std::vector<Player*>& playerList) {
    std::cout << "CheaterPlayerStrategy: Automatically conquering all adjacent enemy territories!\n";
    std::vector<Territory*> attackList = player->toAttack(); // Get territories that can be attacked

    for (Territory* territory : attackList) {
        // Change ownership of each territory to the cheater player
        std::cout << "CheaterPlayerStrategy: Conquered " << territory->getName() << "!\n";
        territory->setLandOccupier(player); // Set the cheater as the owner
    }
}


std::vector<Territory*> CheaterPlayerStrategy::toDefend(const Player* player) const {
    return player->toDefend(); // Defends all owned territories
}

std::vector<Territory*> CheaterPlayerStrategy::toAttack(const Player* player) const {
    if (player == nullptr) {
        std::cout << "Error: Player pointer is null!" << std::endl;
        return {}; // Return an empty vector
    }

    auto attackList = player->toAttack1();
    std::cout << "CheaterPlayer considers these territories to attack:\n";
    for (Territory* t : attackList) {
        std::cout << "- " << t->getName() << "\n";
    }
    return attackList;
}

std::vector<Territory*> AggresivePlayerStrategy::toAttack(const Player *player) const {
    std::vector<Territory*> attackList;
    std::unordered_set<Territory*> addedTerritories;

    // Loop through each territory owned by the player to find adjacent ones not owned by the player
    for (Territory* territory : *player->getTerritories()) {
        const std::vector<Territory*>* adjacent = territory->getAdjacentTerritories();

        for (Territory* adj : *adjacent) {
            // Only add if the territory is not owned by the player and hasn't been added before
            if (adj->getLandOccupier() != player && addedTerritories.find(adj) == addedTerritories.end()) {
                attackList.push_back(adj);
                addedTerritories.insert(adj);  // Mark this territory as added
            }
        }
    }
    return attackList;
}



    //void AggresivePlayerStrategy::issueOrder(Player* player){

//     // Reinforcement phase
//     cout<< "Player " << *name << " - Select territories to reinforce.\n" << endl;
//     for (size_t i = 0; i < toDefend.size(); ++i) {
//         cout << i + 1 << ". " << toDefend[i]->getName() << ": " << toDefend[i]->getArmyAmount() << " armies." << endl;
//     }

//     while (getArmyAmount() > 0) { // Dosent allow the player to go to the next step without finishing reinforcements
//         string territoryName;
//         int reinforcementAmount = 0;
        
//         cout << "\nYou have " << getArmyAmount() << " armies remaining. Enter territory name for reinforcement: ";
//         cin >> territoryName;
//         std::cout <<"\n";

//         if (territoryName == "exit" && getArmyAmount()==0) break;

//         // Locate the territory to deploy reinforcements
//         Territory* selectedTerritory = nullptr;
//         for (Territory* t : toDefend) {
//             if (t->getName() == territoryName) {
//                 selectedTerritory = t;
//                 break;
//             }
//         }

//         if (!selectedTerritory) {
//             cout << "Territory not found. Try again.\n" << endl;
//             continue;
//         }

//         // Validates and adds Reinforcement amount for the territory chosen 
//         cout << "Enter the number of armies to reinforce " << selectedTerritory->getName() << ": ";
//         while (!(cin >> reinforcementAmount) || reinforcementAmount < 0 || reinforcementAmount > getArmyAmount()) {
//             cout << "Invalid amount. Enter a number between 0 and " << getArmyAmount() << ": ";
//             cin.clear();
//             cin.ignore(numeric_limits<streamsize>::max(), '\n');
//         }

//         // Create a Deploy order and add to orders list
//          Order* deployOrder = new Deploy(selectedTerritory, reinforcementAmount,this);
//          orders->addOrder(deployOrder);  
//         setArmyAmount(getArmyAmount() - reinforcementAmount);
//         cout << "Deploy order issued to add " << reinforcementAmount << " armies to " << selectedTerritory->getName() << ".\n" << endl;
//     }



