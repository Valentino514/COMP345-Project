#include <iostream>
#include <algorithm>
#include "Player.h"
#include <fstream>
#include <utility>
#include <sstream>
#include <vector>
#include <limits>
#include <unordered_set>

using namespace std;


Player::Player(string* name, int* armyamount) {
    cout << "Generating player..." << *name << " with " << *armyamount << " armies\n";
    this->name = new string(*name);
    this->armyamount = new int(*armyamount);
    orders = new OrdersList();
    cards = new vector<Card*>;
    territories = new vector<Territory*>;
    strategy = new HumanPlayerStrategy(); // Default strategy
}

Player::Player(string& name) {
    cout << "Generating player... " << name << " with 0 armies (default)\n";
    this->name = new string(name);
    this->armyamount = new int(0);
    orders = new OrdersList();
    cards = new vector<Card*>;
    territories = new vector<Territory*>;
    strategy = new HumanPlayerStrategy(); // Default strategy
}


Player::Player(const Player& other) {
    name = new string(*other.name);
    armyamount = new int(*other.armyamount);

    // Deep copy territories
    territories = new vector<Territory*>;
    for (Territory* t : *(other.territories)) {
        territories->push_back(new Territory(*t));
    }

    // Deep copy cards
    cards = new vector<Card*>;
    for (Card* c : *(other.cards)) {
        cards->push_back(new Card(*c));
    }

    orders = new OrdersList(*(other.orders));

    // Copy strategy using setStrategy
    strategy = nullptr; // Initialize strategy
    if (other.strategy) {
        setStrategy(other.strategy); // Use setStrategy to assign the strategy
    } else {
        strategy = new HumanPlayerStrategy(); // Default to HumanPlayerStrategy
    }
}


Player::Player(string* name, int* armyamount, PlayerStrategy* customStrategy) {
    cout << "Generating player..." << *name << " with " << *armyamount << " armies\n";
    this->name = new string(*name);
    this->armyamount = new int(*armyamount);
    orders = new OrdersList();
    cards = new vector<Card*>;
    territories = new vector<Territory*>;
    strategy = customStrategy ? customStrategy : new HumanPlayerStrategy(); // Use custom or default
}


void Player::setArmyAmount(int amount) { 
    if (armyamount) {
        *armyamount = amount;  // Update the existing armyamount
    } else {
        armyamount = new int(amount);  
    }
}

// Assignment operator 
Player& Player::operator=(const Player& other) {
    if (this == &other) return *this;  // Self-assignment check

    delete this->name;
    delete this->armyamount;
    for (Territory* t : *territories) {
        delete t;
    }
    territories->clear();
    for (Card* c : *cards) {
        delete c;
    }
    cards->clear();

    this->name = new string(*other.name);
    this->armyamount = new int(*other.armyamount);

    for (Territory* t : *(other.territories)) {
        territories->push_back(new Territory(*t));
    }

    for (Card* c : *(other.cards)) {
        cards->push_back(new Card(*c));
    }

    *orders = *(other.orders);

    return *this;
}


Player::~Player() {
    delete name;
    name = nullptr;

    delete armyamount;
    armyamount = nullptr;

    delete strategy;
    strategy = nullptr;

    negotiatedPlayers.clear();

    // Do not delete territories here
    if (territories) {
        territories->clear();  // Just clear the vector
        delete territories;
        territories = nullptr;
    }

    // Assuming cards are owned by the player and can be deleted
    if (cards) {
        for (Card* c : *cards) {
            delete c;
        }
        delete cards;
        cards = nullptr;
    }

    delete orders;
    orders = nullptr;
}


// Adds a player to the negotiated list if not already present
void Player::addNegotiatedPlayer(Player* player) {
    if (std::find(negotiatedPlayers.begin(), negotiatedPlayers.end(), player) == negotiatedPlayers.end()) {
        negotiatedPlayers.push_back(player);
    }
}

// Checks if this player has a truce with the specified player
bool Player::isNegotiatedWith(const Player* player) const {
    return std::find(negotiatedPlayers.begin(), negotiatedPlayers.end(), player) != negotiatedPlayers.end();
}

void Player::clearNegotiations() {
    negotiatedPlayers.clear();
}

// Issues an order using the player's strategy or logs an error if none is assigned.
void Player::issueOrder(const std::vector<Player*>& playerList) {
    if (strategy) {
        strategy->issueOrder(this, playerList);
    } else {
        std::cerr << "No strategy assigned to player " << *name << ". Cannot issue orders.\n";
    }
}

// Returns the territories to defend using the player's strategy or an empty list if none is assigned.
std::vector<Territory*> Player::toDefend() const {
    if (strategy) {
        return strategy->toDefend(this);
    } else {
        std::cerr << "No strategy assigned to player " << *name << ". Returning empty defense list.\n";
        return {};
    }
}

// Returns the territories to attack using the player's strategy or an empty list if none is assigned.
std::vector<Territory*> Player::toAttack() const {
    if (strategy) {
        return strategy->toAttack(this);
    } else {
        std::cerr << "No strategy assigned to player " << *name << ". Returning empty attack list.\n";
        return {};
    }
}


// // Return a List of Territories to Defend
// std::vector<Territory*> Player::toDefend() const {

//     std::vector<Territory*> defendList;
//     for (Territory* territory : *territories) {
//         defendList.push_back(territory);
//     }
//     return defendList;

// }


// toAttack returns a list of adjacent territories not owned by the player
 std::vector<Territory*> Player::toAttack1() const {
     std::vector<Territory*> attackList;
     std::unordered_set<Territory*> addedTerritories;

     // Loop through each territory owned by the player to find adjacent ones not owned by the player
     for (Territory* territory : *territories) {
         const std::vector<Territory*>* adjacent = territory->getAdjacentTerritories();

         for (Territory* adj : *adjacent) {
             // Only add if the territory is not owned by the player and hasn't been added before
             if (adj->getLandOccupier() != this && addedTerritories.find(adj) == addedTerritories.end()) {
                 attackList.push_back(adj);
                 addedTerritories.insert(adj);  // Mark this territory as added
             }
         }
     }
     return attackList;
 }


// void Player::issueOrder(const std::vector<Player*>& playerList) {

//     if (strategy) {
//         // Check if the strategy is NOT a HumanPlayerStrategy
//         if (!dynamic_cast<HumanPlayerStrategy*>(strategy)) {
//             std::cout << "Non-HumanPlayerStrategy logic being executed for " << *name << "\n";
//             strategy->issueOrder(this); // Call the non-human strategy logic
//         }
//         }else {
//     // Retrieve territories to defend and attack
//      vector<Territory*> toDefend = this->toDefend();
//     vector<Territory*> toAttack = this->toAttack();

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

//     cout << "\n------------------------------------------------\n         Reinforcement phase completed\n------------------------------------------------\n\n" << endl;

//   bool issuingAdvanceOrders = true;
// while (issuingAdvanceOrders) {
//     toDefend=this->toDefend();
//     toAttack = this->toAttack();

//     cout << "Territories available to move/attack from:" << endl;
//     cout << "------------------------------------------------\n" << endl;
//     for (size_t i = 0; i < toDefend.size(); ++i) {
//         cout << i + 1 << ". " << toDefend[i]->getName() << ": " << toDefend[i]->getArmyAmount() << " armies available." << endl;
//     }

//     // Gather source territory
//     Territory* source = nullptr;
//     while (true) {
//         string sourceName;
//         cout << "\nEnter the name of the territory to move troops from: ";
//         cin >> sourceName;

//         //Validate maybe will comment out
//         for (Territory* t : toDefend) {
//             if (t->getName() == sourceName) {
//                 source = t;
//                 break;
//             }
//         }
//         if (source) break; // Exit Loop if player owns territory
//         cout << "Source territory not found. Try again." << endl;
//     }

    
//     cout << "\nChoose a destination territory:" << endl;
//     cout << "-----------------------------------------------" << endl;

//     // Display player's own territories (for defensive moves)
//     cout << "Your territories (to reinforce):" << endl;
//     for (Territory* t : toDefend) {
//         for(Territory* s: *source->getAdjacentTerritories()){
//             if (t->getName()==s->getName())
//             {
//                cout << "- " << t->getName() << ": " << t->getArmyAmount() << " armies." << endl;
//             }
            
//         }
//     }
    

//     // Display enemy territories (for attack)
//     cout << "\nEnemy territories (to attack):" << endl;
//     for (Territory* t : toAttack) {
//         for(Territory* s: *source->getAdjacentTerritories()){
//             if (t->getName()==s->getName())
//             {
//                cout << "- " << t->getName() << ": " << t->getArmyAmount() << " armies." << endl;
//             }
            
//         }
//     }

//     // Get destination territory
//     Territory* destination = nullptr;
//     while (true) {
//         string destinationName;
//         cout << "\nEnter the name of the destination territory: ";
//         cin >> destinationName;

//         // Locate the destination in either toDefend or toAttack            (maybe comment out later )
//         for (Territory* t : toDefend) {
//             if (t->getName() == destinationName) {
//                 destination = t;
//                 break;
//             }
//         }
//         if (!destination) { // If not found in toDefend, check in toAttack
//             for (Territory* t : toAttack) {
//                 if (t->getName() == destinationName) {
//                     destination = t;
//                     break;
//                 }
//             }
//         }
//         if (destination) break; // Exit loop if a valid destination is found
//         cout << "Invalid destination. Try again." << endl;
//     }

//     // Choose army amount to move or attack
//     int numArmies = 0;
//     while (true) {
//         cout << "Enter number of armies to move: ";
//         if (cin >> numArmies && numArmies > 0) break;
//         cout << "Invalid number. Enter a positive integer: ";
//         cin.clear();
//         cin.ignore(numeric_limits<streamsize>::max(), '\n');
//     }

    
//     Order* order = new Advance(this, source, destination, numArmies);  
//     orders->addOrder(order); // Add the order to the player's order list

//     cout << "Advance order issued to move " << numArmies << " armies from " << source->getName()
//          << " to " << destination->getName() << "." << endl;

//     // Ask if the player wants to issue another Advance order
//      string decision;
//     cout << "Would you like to issue another Advance order? (Y/N): ";
//     cin >> decision;
//     issuingAdvanceOrders = (decision == "Y" || decision == "y");
// }

// cout << "Orders Issuing phase for Advance orders completed.\n------------------------------------------------" << endl;


//  while (!cards->empty()) {
//         cout << *name << " - Cards in hand:" << endl;
//         for (size_t i = 0; i < cards->size(); ++i) {
//             cout << i + 1 << ". " << cards->at(i)->getCardTypeName() << endl;
//         }
//         cout << "Would you like to play a card? (Y/N): ";
//         char playCardDecision;
//         cin >> playCardDecision;
//         if (playCardDecision != 'Y' && playCardDecision != 'y') {
//             break;
//         }

//         int cardChoice = 0;
//         cout << "Select a card to play (enter the number): ";
//         while (!(cin >> cardChoice) || cardChoice < 1 || cardChoice > cards->size()) {
//             cout << "Invalid choice. Please select a valid card number: ";
//             cin.clear();
//             cin.ignore(numeric_limits<streamsize>::max(), '\n');
//         }
//        Card* selectedCard = cards->at(cardChoice - 1);

//         Order* cardOrder = nullptr;
//         if (selectedCard->getCardTypeName() == "Bomb") {
//             Territory* target = selectTargetFromAttackList();
//             cardOrder = new Bomb(target,this); // Bomb order targets enemy territory

//             cout << "Bomb order created targeting " << target->getName() << "." << endl;
//         } else if (selectedCard->getCardTypeName() == "Airlift") {
//             Territory* source = selectSourceTerritory();
//             Territory* destination = selectDestinationTerritory();
//             int armyAmount = selectArmyAmount(source);
//             cardOrder = new Airlift(source, destination, armyAmount,this); // Airlift within own territories

//             cout << "Airlift order created to move " << armyAmount << " armies from " << source->getName()
//                  << " to " << destination->getName() << "." << endl;
//         } else if (selectedCard->getCardTypeName() == "Blockade") {
//             Territory* target = selectTargetFromDefendList();
//             cardOrder = new Blockade(this, target); // Blockade on own territory

//             cout << "Blockade order created on " << target->getName() << "." << endl;
//         } else if (selectedCard->getCardTypeName() == "Diplomacy") {
//             //Player* targetPlayer = selectPlayerToNegotiate(*playerList);
//             Player* targetPlayer = selectPlayerToNegotiate(playerList);

//             cardOrder = new Negociate(this, targetPlayer); // Negotiate with enemy player
//             cout << "Negotiate order created with " << *(targetPlayer->getName()) << "." << endl;
//          }

//         if (cardOrder) {
//             orders->addOrder(cardOrder);
//         }
//         removeCard(selectedCard);
//     }

//     cout << "Orders Issuing phase completed for player " << *name << ".\n------------------------------------------------" << endl;}
// }






//Overloaded stream insertion 
ostream& operator<<(ostream& os, const Player& player) {
    os << "Player with name " << *(player.name)
       << " has " << player.territories->size() << " territories, "
       << player.cards->size() << " cards, and "
       << *(player.armyamount) << " armies.";  
    return os;
}


void Player::addTerritory(Territory* territory) {
    territories->push_back(territory);  // Add territory to the player's list
}


void Player::printOwnedTerritories() const {
    std::cout << "Player " << *name << " owns the following territories:\n";
    for (auto t : *territories) {
        std::cout << t->getName() << std::endl;  
    }
}

void Player::addCard(Card* card) {
    cards->push_back(card);  // Adds a card to the player's hand
}

// Allows the player to select a territory to attack from the list of enemy territories.
Territory* Player::selectTargetFromAttackList() {
    vector<Territory*> attackList = toAttack(); // Get the list of enemy territories to attack
    cout << "Enemy territories to target:" << endl;

    // Display all enemy territories with their details
    for (size_t i = 0; i < attackList.size(); ++i) {
        cout << i + 1 << ". " << attackList[i]->getName() << " (Armies: " << attackList[i]->getArmyAmount() << ")" << endl;
    }

    int choice = 0;
    cout << "Select a territory to target (enter the number): ";
    
    // Validate user input to ensure a valid territory is chosen
    while (!(cin >> choice) || choice < 1 || choice > attackList.size()) {
        cout << "Invalid choice. Please select a valid territory number: ";
        cin.clear();
        cin.ignore(numeric_limits<streamsize>::max(), '\n');
    }
    return attackList[choice - 1]; // Return the selected territory
}

// Allows the player to select a territory to defend from their list of owned territories.
Territory* Player::selectTargetFromDefendList() {
    vector<Territory*> defendList = toDefend(); // Get the list of territories to defend
    cout << "Your territories to target:" << endl;

    // Display all owned territories with their details
    for (size_t i = 0; i < defendList.size(); ++i) {
        cout << i + 1 << ". " << defendList[i]->getName() << " (Armies: " << defendList[i]->getArmyAmount() << ")" << endl;
    }

    int choice = 0;
    cout << "Select a territory to target (enter the number): ";

    // Validate user input to ensure a valid territory is chosen
    while (!(cin >> choice) || choice < 1 || choice > defendList.size()) {
        cout << "Invalid choice. Please select a valid territory number: ";
        cin.clear();
        cin.ignore(numeric_limits<streamsize>::max(), '\n');
    }
    return defendList[choice - 1]; // Return the selected territory
}

// Allows the player to select another player to negotiate with, excluding themselves.
Player* Player::selectPlayerToNegotiate(const std::vector<Player*>& playerList) {
    std::cout << "Players to negotiate with:" << std::endl;

    int index = 1;
    // Display the list of other players available for negotiation
    for (Player* p : playerList) {
        if (p != this) { // Exclude the current player
            std::cout << index << ". " << *(p->getName()) << std::endl;
            index++;
        }
    }

    int choice = 0;
    std::cout << "Select a player to negotiate with (enter the number): ";

    // Validate user input to ensure a valid player is chosen
    while (!(std::cin >> choice) || choice < 1 || choice >= index) {
        std::cout << "Invalid choice. Please select a valid player number: ";
        std::cin.clear();
        std::cin.ignore(std::numeric_limits<std::streamsize>::max(), '\n');
    }
    return playerList[choice - 1]; // Return the selected player
}

// Allows the player to select a source territory from their list of owned territories
// to move armies from.
Territory* Player::selectSourceTerritory() {
    vector<Territory*> defendList = toDefend(); // Get the list of territories to defend
    cout << "Your territories (to move armies from):" << endl;

    // Display all owned territories with their details
    for (size_t i = 0; i < defendList.size(); ++i) {
        cout << i + 1 << ". " << defendList[i]->getName() << " (Armies: " << defendList[i]->getArmyAmount() << ")" << endl;
    }

    int choice = 0;
    cout << "Select a source territory (enter the number): ";

    // Validate user input to ensure a valid territory is chosen
    while (!(cin >> choice) || choice < 1 || choice > defendList.size()) {
        cout << "Invalid choice. Please select a valid territory number: ";
        cin.clear();
        cin.ignore(numeric_limits<streamsize>::max(), '\n');
    }

    return defendList[choice - 1]; // Return the selected territory
}

// Allows the player to select a destination territory from their list of owned territories
// to move armies to.
Territory* Player::selectDestinationTerritory() {
    vector<Territory*> defendList = toDefend(); // Get the list of territories to defend

    cout << "Your territories to move armies to:" << endl;

    // Display all owned territories with their details
    for (size_t i = 0; i < defendList.size(); ++i) {
        cout << i + 1 << ". " << defendList[i]->getName() << " (Armies: " << defendList[i]->getArmyAmount() << ")" << endl;
    }

    int choice = 0;
    cout << "Select a destination territory (enter the number): ";

    // Validate user input to ensure a valid territory is chosen
    while (!(cin >> choice) || choice < 1 || choice > defendList.size()) {
        cout << "Invalid choice. Please select a valid territory number: ";
        cin.clear();
        cin.ignore(numeric_limits<streamsize>::max(), '\n');
    }

    return defendList[choice - 1]; // Return the selected territory
}


void Player::removeTerritory(Territory* territory) {
    auto it = find(territories->begin(), territories->end(), territory); // Find the territory in the list
    if (it != territories->end()) { // If found
        territories->erase(it); // Remove the territory
    }
}

// Fn to select an army amount
int Player::selectArmyAmount(Territory* sourceTerritory) {
    int maxAmount = sourceTerritory->getArmyAmount();
    int amount = 0;
    cout << "Enter the number of armies to move (available: " << maxAmount << "): ";
    while (!(cin >> amount) || amount < 1 || amount > maxAmount) {
        cout << "Invalid amount. Enter a number between 1 and " << maxAmount << ": ";
        cin.clear();
        cin.ignore(numeric_limits<streamsize>::max(), '\n');
    }
    return amount;
}

// Fn to delete a card
void Player::removeCard(Card::CardType type) {
    for (auto it = cards->begin(); it != cards->end(); ++it) {
        if ((*it)->getType() == type) {
            delete *it; // Free memory
            cards->erase(it);
            return; // Exit after removing the first matching card
        }
    }
}


//checks if player has a specific card
bool Player::hasCard(Card::CardType type) const {
    for (const auto& card : *cards) {
        if (card->getType() == type) {
            return true; // Found the card
        }
    }
    return false; // Card not found
}