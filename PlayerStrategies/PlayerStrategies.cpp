#include "PlayerStrategies.h"
#include "../Player/Player.h"
#include <iostream>
#include <algorithm>
#include <unordered_set>
#include <limits>
#include <climits>

using namespace std;

/**
 * @brief Handles the order issuance process for a human player.
 * 
 * @details This method provides a user interface for the human player to issue orders. The player can:
 *          - Reinforce their territories by selecting a territory and assigning available army units.
 *          - Issue advance orders by selecting a source territory, a destination territory, and the number of armies to move.
 *          - Play cards from their hand, including Bomb, Airlift, Blockade, or Diplomacy cards.
 *          - Each action is logged to the "gamelog.txt" file for tracking purposes.
 * 
 * @param player The player issuing the orders.
 * @param playerList The list of all players in the game.
 * @param test A flag to indicate if the method is running in test mode (unused in this implementation).
 */
void HumanPlayerStrategy::issueOrder(Player* player,const vector<Player*>& playerList, bool test) {

    const vector<Card*>* cards = &(player->getCards());
    OrdersList* orders = player->getOrdersList();

    // Open gamelog.txt in append mode to log the executed orders
    ofstream logFile("gamelog.txt", ios::app);  
    if (!logFile) {
        cerr << "Error opening log file!" << endl;
        return;
    }

    // Retrieve territories to defend and attack
     vector<Territory*> toDefend = player->toDefend();
    vector<Territory*> toAttack = player->toAttack();

    // Reinforcement phase
    cout<< "Player " << *player->getName() << " - Select territories to reinforce.\n" << endl;
    for (size_t i = 0; i < toDefend.size(); ++i) {
        cout << i + 1 << ". " << toDefend[i]->getName() << ": " << toDefend[i]->getArmyAmount() << " armies." << endl;
    }

    while (player->getArmyAmount() > 0) { // Dosent allow the player to go to the next step without finishing reinforcements
        string territoryName;
        int reinforcementAmount = 0;
        
        cout << "\nYou have " << player->getArmyAmount() << " armies remaining. Enter territory name for reinforcement: ";
        cin >> territoryName;
        cout <<"\n";

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
         if(deployOrder->validate()){
        orders->addOrder(deployOrder);  
        player->setArmyAmount(player->getArmyAmount() - reinforcementAmount);
        cout << "Deploy order issued to add " << reinforcementAmount << " armies to " << selectedTerritory->getName() << ".\n" << endl;
         }

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
        if (source) break;// Exit Loop if player owns territory
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
    cout << "\nEnemy territories to attack:" << endl;
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

        // Locate the destination in either toDefend or toAttack  (maybe comment out later )
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

    
    Advance* order = new Advance(player, source, destination, numArmies);  
    if(order->validate()){
    orders->addOrder(order); // Add the order to the player's order list
    cout << "Advance order issued to move " << numArmies << " armies from " << source->getName()
         << " to " << destination->getName() << "." << endl;
    }
    logFile << "Advance order issued to move " << numArmies << " armies from " << source->getName()
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
            logFile << "Bomb order created targeting " << target->getName() << "." << endl;
        } else if (selectedCard->getCardTypeName() == "Airlift") {
            Territory* source = player->selectSourceTerritory();
            Territory* destination = player->selectDestinationTerritory();
            int armyAmount =player-> selectArmyAmount(source);
            cardOrder = new Airlift(source, destination, armyAmount,player); // Airlift within own territories

            cout << "Airlift order created to move " << armyAmount << " armies from " << source->getName()
                 << " to " << destination->getName() << "." << endl;
            logFile << "Airlift order created to move " << armyAmount << " armies from " << source->getName()
                 << " to " << destination->getName() << "." << endl;
        } else if (selectedCard->getCardTypeName() == "Blockade") {
            Territory* target = player->selectTargetFromDefendList();
            cardOrder = new Blockade(player, target); // Blockade on own territory

            cout << "Blockade order created on " << target->getName() << "." << endl;
            logFile << "Blockade order created on " << target->getName() << "." << endl;
        } else if (selectedCard->getCardTypeName() == "Diplomacy") {
            //Player* targetPlayer = selectPlayerToNegotiate(*playerList);
            Player* targetPlayer = player->selectPlayerToNegotiate(playerList);

            cardOrder = new Negociate(player, targetPlayer); // Negotiate with enemy player
            cout << "Negotiate order created with " << *(targetPlayer->getName()) << "." << endl;
            logFile << "Negotiate order created with " << *(targetPlayer->getName()) << "." << endl;
         }

        if (cardOrder) {
            orders->addOrder(cardOrder);
        }
        player->removeCard(selectedCard->getType());
    }

    cout << "Orders Issuing phase completed for player " << player->getName() << ".\n------------------------------------------------" << endl;
    logFile << "Orders Issuing phase completed for player " << player->getName() << ".\n" << endl;
    logFile.close();
}

/**
 * @brief Returns a list of territories that the player can defend.
 * 
 * @param player The player for whom the territories are being retrieved.
 * @return A vector of territories that the player owns and can defend.
 */
vector<Territory*> HumanPlayerStrategy::toDefend(const Player* player) const {
    vector<Territory*> defendList;
    // Use player's territories
    const vector<Territory*>* territories = player->getTerritories();
    for (Territory* territory : *territories) {
        defendList.push_back(territory);
    }
    return defendList;
}

/**
 * @brief Returns a list of territories that the player can attack.
 * 
 * @param player The player for whom the territories are being retrieved.
 * @return A vector of enemy territories that the player can attack.
 */
vector<Territory*> HumanPlayerStrategy::toAttack(const Player* player) const {
    vector<Territory*> attackList;
    unordered_set<Territory*> addedTerritories;

    // Use player's territories
    const vector<Territory*>* territories = player->getTerritories();
    for (Territory* territory : *territories) {
        const vector<Territory*>* adjacent = territory->getAdjacentTerritories();
        for (Territory* adj : *adjacent) {
            if (adj->getLandOccupier() != player && addedTerritories.find(adj) == addedTerritories.end()) {
                attackList.push_back(adj);
                addedTerritories.insert(adj);
            }
        }
    }
    return attackList;
}


/**
 * @brief Does nothing, as neutral players do not issue orders.
 * 
 * @param player The neutral player (unused).
 * @param playerList List of all players (unused).
 * @param test A flag indicating if the method is running in test mode.
 */
void NeutralPlayerStrategy::issueOrder(Player* player,const vector<Player*>& playerList, bool test) {
    cout << "NeutralPlayer: Doing nothing.\n";
    // Does not issue any orders
}

/**
 * @brief Returns an empty list, as neutral players do not defend territories.
 * 
 * @param player The neutral player (unused).
 * @return An empty vector, as neutral players do not defend.
 */
vector<Territory*> NeutralPlayerStrategy::toDefend(const Player* player) const {
    return {}; //neutral does not  not defend
}

/**
 * @brief Returns an empty list, as neutral players do not attack.
 * 
 * @param player The neutral player (unused).
 * @return An empty vector, as neutral players do not attack.
 */
vector<Territory*> NeutralPlayerStrategy::toAttack(const Player* player) const {
    return {}; // neutral Does not attack
}

/**
 * @brief Automatically conquers all adjacent enemy territories.
 * 
 * @param player The cheater player issuing the orders.
 * @param playerList List of all players (unused).
 * @param test A flag indicating if the method is running in test mode.
 */
void CheaterPlayerStrategy::issueOrder(Player* player,const vector<Player*>& playerList, bool test) {
//    cout << "CheaterPlayerStrategy: Automatically conquering all adjacent enemy territories!\n";
    vector<Territory*> attackList = player->toAttack(); // Get territories that can be attacked

    for (Territory* territory : attackList) {
        // Change ownership of each territory to the cheater player
        cout << "CheaterPlayerStrategy: Conquered " << territory->getName() << "!\n";
        territory->setLandOccupier(player); // Set the cheater as the owner
    }
}

/**
 * @brief Returns a list of territories owned by the player that can be defended.
 * 
 * @param player The player for whom the territories are being retrieved.
 * @return A vector of territories that the player owns and can defend.
 */
vector<Territory*> CheaterPlayerStrategy::toDefend(const Player* player) const {
    return player->toDefend(); // Defends all owned territories
}

/**
 * @brief Returns a list of territories that the player can attack, ensuring they conquer all enemy territories.
 * 
 * @param player The player for whom the territories are being retrieved.
 * @return A vector of enemy territories that the player can attack and conquer.
 */
vector<Territory*> CheaterPlayerStrategy::toAttack(const Player* player) const {
    if (player == nullptr) {
        cout << "Error: Player pointer is null!" << endl;
        return {}; // Return an empty vector
    }

    auto attackList = player->toAttack1();
//    cout << "CheaterPlayer considers these territories to attack:\n";
    for (Territory* t : attackList) {
        if (t != nullptr) {
            // Change the land occupier of the territory to the cheater player
            t->setLandOccupier(const_cast<Player*>(player));
//            std::cout << "- " << t->getName() << " is now owned by the cheater player!\n";
        }
    }
    return attackList;
}

/**
 * @brief Returns a list of territories that the aggressive player can defend, prioritizing the strongest one.
 * 
 * @param player The player for whom the territories are being retrieved.
 * @return A vector of territories that the player can defend, prioritizing the strongest territory.
 */
vector<Territory*> AggresivePlayerStrategy::toAttack(const Player *player) const {
    vector<Territory*> attackList;
    unordered_set<Territory*> addedTerritories;

    // Loop through each territory owned by the player to find adjacent ones not owned by the player
    for (Territory* territory : *player->getTerritories()) {
        const vector<Territory*>* adjacent = territory->getAdjacentTerritories();

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


/**
 * @brief Returns a list of territories that the aggressive player can defend, prioritizing the strongest one.
 * 
 * @param player The player for whom the territories are being retrieved.
 * @return A vector of territories that the player can defend, prioritizing the strongest territory.
 */
vector<Territory*> AggresivePlayerStrategy::toDefend(const Player* player) const{
    vector<Territory*> defendList;
    Territory* strongestTerritory = nullptr;
    int maxArmies = -1;

    //check to find the strongest territory with adjacent enemy territories
    for (Territory* t : *player->getTerritories()) {
        if (t->getArmyAmount() > maxArmies) {
            // Check if territory has at least one adjacent enemy territory
            bool hasEnemyAdjacent = false;
            const vector<Territory*>* adjacent = t->getAdjacentTerritories();
            for (Territory* adj : *adjacent) {
                if (adj->getLandOccupier() != player) {
                    hasEnemyAdjacent = true;
                    break;
                }
            }
            if (hasEnemyAdjacent) {
                maxArmies = t->getArmyAmount();
                strongestTerritory = t;
            }
        }
    }

    // If no territory with adjacent enemy territories was found,we pick the territory with the most armies 
    if (!strongestTerritory) {
        maxArmies = -1;
        for (Territory* t : *player->getTerritories()) {
            if (t->getArmyAmount() > maxArmies) {
                maxArmies = t->getArmyAmount();
                strongestTerritory = t;
            }
        }
    }

    if (strongestTerritory) {
        defendList.push_back(strongestTerritory);
    }
    return defendList;
}


/**
 * @brief Issues orders for the aggressive player, including reinforcement and attack orders.
 * 
 * @param player The aggressive player issuing the orders.
 * @param playerList List of all players (unused).
 * @param test A flag indicating if the method is running in test mode.
 */
void AggresivePlayerStrategy::issueOrder(Player* player, const vector<Player*>& playerList, bool test) {
    // Reinforcement phase
    OrdersList* orders = player->getOrdersList();
    Territory* strongestTerritory = nullptr;

    if (player->getArmyAmount() > 0) {
        vector<Territory*> defendList = toDefend(player);
        if (defendList.empty()) {
            return;
        }

        strongestTerritory = defendList.front(); // The territory with the most armies and adjacent enemies
        int reinforcementAmount = player->getArmyAmount();

        // Create Deploy order
        Order* deployOrder = new Deploy(strongestTerritory, reinforcementAmount, player);
        if(deployOrder->validate()){
        orders->addOrder(deployOrder);
        player->setArmyAmount(0);
        }else{
            return;
        }

    }

    if(strongestTerritory == nullptr){
        cout<<" aggresive player has no territory left";
        return;
    }else
    {
        if(test == true){
            cout<<"aggresive player strongest territory: "<< strongestTerritory->getName()+"\nplacing all available units on that territory\n";
        }
    // Start attack phase for issuing orders
    Territory* attackTerritory = strongestTerritory;
    bool foundAttackTerritory = false;

    //find the strongest territory with adjacent enemy territories
    while (true) {
        if (!attackTerritory) {
            // Find a new strongest territory with adjacent enemies
            int maxArmies = -1;
            for (Territory* t : *player->getTerritories()) {
                if (t->getArmyAmount() > maxArmies) {
                    // Check if territory has at least one adjacent enemy territory
                    bool hasEnemyAdjacent = false;
                    const vector<Territory*>* adjacents = t->getAdjacentTerritories();
                    for (Territory* adj : *adjacents) {
                        if (adj->getLandOccupier() != player) {
                            hasEnemyAdjacent = true;
                            break;
                        }
                    }
                    if (hasEnemyAdjacent) {
                        maxArmies = t->getArmyAmount();
                        attackTerritory = t;
                        foundAttackTerritory = true;
                    }
                }
            }
            if (!foundAttackTerritory) {
                return;
            }
        }

        // Get adjacent enemy territories
        vector<Territory*> adjacentEnemies;
        const vector<Territory*>* adjacents = attackTerritory->getAdjacentTerritories();
        for (Territory* adj : *adjacents) {
            if (adj->getLandOccupier() != player) {
                adjacentEnemies.push_back(adj);
            }
        }

        if (!adjacentEnemies.empty()) {
            int totalArmies = attackTerritory->getArmyAmount();
            int armiesToMove = totalArmies; 
            int numEnemies = adjacentEnemies.size();
            int baseArmy = armiesToMove / numEnemies;
            int extra = armiesToMove % numEnemies;

            if(test==true){
                cout<<"attacking the following adjacent territories:\n";
            }
        for (size_t i = 0; i < adjacentEnemies.size(); ++i) {
            Player* enemyOwner = adjacentEnemies[i]->getLandOccupier();
            if (!player->isNegotiatedWith(enemyOwner)) {
                int numArmies = baseArmy + (i < extra ? 1 : 0);
                // Ensure numArmies is not greater than source's available armies
                numArmies = min(numArmies, attackTerritory->getArmyAmount());
                if (numArmies > 0) {
                    Order* advanceOrder = new Advance(player, attackTerritory, adjacentEnemies[i], numArmies);
                    if(test == true){
                            cout<<adjacentEnemies[i]->getName()+'\n';
                        }
                    if (advanceOrder->validate()) {
                        orders->addOrder(advanceOrder);
                        
                    } else {
                        delete advanceOrder; //if advance order is invalid
                    }
                }
            }
        }
            break; // Attack orders issued, exit the loop
        } else {
            // No adjacent enemy territories, find a new attack territory
            attackTerritory = nullptr;
        }
    }
    // Play Bomb card if the player has one
    if (player->hasCard(Card::Bomb)) {
        if(test == true){
            cout<<"aggresive player owns a bomb card \n";
        }
        // Find the adjacent enemy territory with the most armies next to the strongest territory
        Territory* targetToBomb = nullptr;
        int maxEnemyArmies = -1;

        const vector<Territory*>* adjacents = strongestTerritory->getAdjacentTerritories();
        for (Territory* adj : *adjacents) {
            if (adj->getLandOccupier() != player) {
                if (adj->getArmyAmount() > maxEnemyArmies) {
                    maxEnemyArmies = adj->getArmyAmount();
                    targetToBomb = adj;
                }
            }
        }

        if (targetToBomb && !player->isNegotiatedWith(targetToBomb->getLandOccupier())) {
            Order* bombOrder = new Bomb(targetToBomb, player);
            if (bombOrder->validate()) {
                cout<<"bombing "<<targetToBomb->getName()<<endl;
                orders->addOrder(bombOrder);
                player->removeCard(Card::Bomb);
            } else {
                delete bombOrder; //if order is invalid
            }
        }
    }

    // Play Airlift card if the player has one
    if (player->hasCard(Card::Airlift)) {
        if(test == true){
            cout<<"aggresive player owns an airlift card \n";
        }
        // Find the territory with second highest number of armies excluding the strongest territory
        Territory* secondStrongestTerritory = nullptr;
        int maxArmies = -1;

        for (Territory* t : *player->getTerritories()) {
            if (t != strongestTerritory && t->getArmyAmount() > maxArmies) {
                maxArmies = t->getArmyAmount();
                secondStrongestTerritory = t;
            }
        }

        if (secondStrongestTerritory && maxArmies > 0) {
            int armyCount = secondStrongestTerritory->getArmyAmount();
            if (armyCount > 0) {
                Order* airliftOrder = new Airlift(secondStrongestTerritory, strongestTerritory, armyCount, player);
                if (airliftOrder->validate()) {
                    orders->addOrder(airliftOrder);
                    player->removeCard(Card::Airlift);
                } else {
                    delete airliftOrder;  // Clean up if not valid
                }
            }
        }

        else{
            if(test == true){
                cout<<"aggresive player has no other territories with more than 1 army to airlift\n";
            }
        }
    }
    }
}

/**
 * @brief Implements the order issuing process for the benevolent player.
 * 
 * The benevolent player focuses on reinforcing their weakest territories and 
 * fortifying them by moving armies to adjacent weaker territories. Additionally, 
 * they may use a Diplomacy card to negotiate a truce with another player.
 * 
 * @param player The benevolent player issuing the orders.
 * @param playerList List of all players in the game.
 * @param test A flag indicating if the method is running in test mode.
 */
void BenevolentPlayerStrategy::issueOrder(Player* player, const vector<Player*>& playerList, bool test) {
    OrdersList* orders = player->getOrdersList();

    // Reinforcement phase: Deploy armies to weakest territories
    if (player->getArmyAmount() > 0) {
        vector<Territory*> weakestTerritories = toDefend(player);

        int numTerritories = weakestTerritories.size();
        int totalReinforcements = player->getArmyAmount();
        int baseReinforcement = totalReinforcements / numTerritories;
        int extra = totalReinforcements % numTerritories;
        if(test == true){
            cout<<"reinfocing weakest territories of benevolent player:\n";
        }
        for (size_t i = 0; i < weakestTerritories.size(); ++i) {
            int reinforcementAmount = baseReinforcement + (i < extra ? 1 : 0);
            Order* deployOrder = new Deploy(weakestTerritories[i], reinforcementAmount, player);
            if(deployOrder->validate()){
            orders->addOrder(deployOrder);
            if(test == true){
            cout<<weakestTerritories[i]->getName()+" with "+to_string(weakestTerritories[i]->getArmyAmount())+" armies\n";
            }
            }

        }

        player->setArmyAmount(0); 
    }

    // Fortification phase: Move armies between own territories
    // Find territories with armies that can be moved
    vector<Territory*> movableTerritories;
    for (Territory* t : *player->getTerritories()) {
        if (t->getArmyAmount() > 0) {
            movableTerritories.push_back(t);
        }
    }

    // Sort movable territories by army count in descending order
    sort(movableTerritories.begin(), movableTerritories.end(), [](Territory* a, Territory* b) {
        return a->getArmyAmount() > b->getArmyAmount();
    });

    // For each move, try to reinforce weaker territories
    for (Territory* sourceTerritory : movableTerritories) {
        const vector<Territory*>* adjacents = sourceTerritory->getAdjacentTerritories();

        // Find own adjacent territories with fewer armies
        vector<Territory*> weakerAdjacentTerritories;
        for (Territory* adj : *adjacents) {
            if (adj->getLandOccupier() == player && adj->getArmyAmount() < sourceTerritory->getArmyAmount()) {
                weakerAdjacentTerritories.push_back(adj);
            }
        }

        if (!weakerAdjacentTerritories.empty()) {
            int armiesToMove = sourceTerritory->getArmyAmount(); // Can move all armies

            int numTerritories = weakerAdjacentTerritories.size();
            int baseArmies = armiesToMove / numTerritories;
            int extra = armiesToMove % numTerritories;

        for (size_t i = 0; i < weakerAdjacentTerritories.size(); ++i) {
            int numArmies = baseArmies + (i < extra ? 1 : 0);
            // Ensure numArmies is not greater than source's available armies
            numArmies = std::min(numArmies, sourceTerritory->getArmyAmount());
            if (numArmies > 0) {
                Order* advanceOrder = new Advance(player, sourceTerritory, weakerAdjacentTerritories[i], numArmies);
                if (advanceOrder->validate()) {
                    orders->addOrder(advanceOrder);
                    sourceTerritory->setArmyAmount(sourceTerritory->getArmyAmount() - numArmies);
                } else {
                    delete advanceOrder;  // Clean up if not valid
                }
            }
        }
        }
    }

    // Use Diplomacy card if the player has it
    if (player->hasCard(Card::Diplomacy)) {
        if(test == true){
            cout<< "benevolent player has diplomacy card\n";
        }
        Player* targetPlayer = nullptr;
        for (Player* p : playerList) {
            if (p != player) {
                targetPlayer = p;
                break;
            }
        }
if (targetPlayer != nullptr && targetPlayer != player) {
    Order* negotiateOrder = new Negociate(player, targetPlayer);
    if (negotiateOrder->validate()) {
        cout<<"negociating with "<<*targetPlayer->getName()<<endl;
        orders->addOrder(negotiateOrder);
        player->removeCard(Card::Diplomacy);
    } else {
        delete negotiateOrder;  // Clean up if not valid
    }
}

    }
}

/**
 * @brief Returns a list of territories the benevolent player can defend, prioritizing the weakest territories.
 * 
 * @param player The player whose territories are being considered for defense.
 * @return A vector of territories the player can defend.
 */
vector<Territory*> BenevolentPlayerStrategy::toDefend(const Player* player) const {
    vector<Territory*> defendList;
    int minArmies = INT_MAX;

    // Find the minimum number of armies among the player's territories
    for (Territory* t : *player->getTerritories()) {
        if (t->getArmyAmount() < minArmies) {
            minArmies = t->getArmyAmount();
        }
    }

    // Collect all territories that have the minimum number of armies
    for (Territory* t : *player->getTerritories()) {
        if (t->getArmyAmount() == minArmies) {
            defendList.push_back(t);
        }
    }

    return defendList;
}

/**
 * @brief Benevolent player does not attack.
 * 
 * @param player The player for whom attack territories are being considered.
 * @return An empty vector, as the benevolent player does not attack.
 */
vector<Territory*> BenevolentPlayerStrategy::toAttack(const Player* player) const {
    // Benevolent player does not attack
    return vector<Territory*>();
}

