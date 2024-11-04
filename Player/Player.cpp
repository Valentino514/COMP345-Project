#include <iostream>
#include <algorithm>
#include "Player.h"
#include <fstream>
#include <utility>
#include <sstream>
#include <vector>
#include <limits>
using namespace std;

Player::Player(string* name, int* armyamount) {
    cout << "Generating player..." << *name << " with " << *armyamount << " armies\n";
    this->name = new string(*name);  
    this->armyamount = new int(*armyamount);  
    orders = new OrdersList();
    cards = new vector<Card*>;
    territories = new vector<Territory*>;
}
Player::Player(string* name) {
    cout << "Generating player..." << *name << " with 0 armies (default)\n";
    this->name = new string(*name);  
    this->armyamount = new int(0);  
    orders = new OrdersList();
    cards = new vector<Card*>;
    territories = new vector<Territory*>;
}

Player::Player(const Player& other) {
    name = new string(*other.name);
    armyamount = new int(*other.armyamount);  

    // Deep copy territories
    territories = new vector<Territory*>;
    for (Territory* t : *(other.territories)) {
        territories->push_back(new Territory(*t));
    }

    // Deep copy Card
    cards = new vector<Card*>;
    for (Card* c : *(other.cards)) {
        cards->push_back(new Card(*c));
    }

    
    orders = new OrdersList(*(other.orders));
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


// Destructor
Player::~Player() {
    delete name;
    delete armyamount;  

    for (Territory* t : *territories) {
        delete t;
    }
    territories->clear();
    delete territories;

    for (Card* c : *cards) {
        delete c;
    }
    cards->clear();
    delete cards;

    delete orders;  
}


// Return a List of Territories to Defend
std::vector<Territory*> Player::toDefend() const {

    std::vector<Territory*> defendList;
    for (Territory* territory : *territories) {
        defendList.push_back(territory);
    }
    return defendList;

}

// toAttack returns a list of adjacent territories not owned by the player
std::vector<Territory*> Player::toAttack() const {
    std::vector<Territory*> attackList;

    // Loop through each territory owned by the player to find adjacent ones not owned by the player
    for (Territory* territory : *territories) {
        const std::vector<Territory*>* adjacent = territory->getAdjacentTerritories();

        for (Territory* adj : *adjacent) {
            if (adj->getLandOccupier() != this) {  // Only add if the territory is not owned by the player
                attackList.push_back(adj);
            }
        }
    }
    return attackList;
}


void Player::issueOrder() {
    // Retrieve territories to defend and attack
     vector<Territory*> toDefend = this->toDefend();
    vector<Territory*> toAttack = this->toAttack();

    // Reinforcement phase
    cout << *name << " - Select territories to reinforce." << endl;
    for (size_t i = 0; i < toDefend.size(); ++i) {
        cout << i + 1 << ". " << toDefend[i]->getName() << ": " << toDefend[i]->getArmyAmount() << " armies." << endl;
    }

    while (getArmyAmount() > 0) { // Dosent allow the player to go to the next step without finishing reinforcements
        string territoryName;
        int reinforcementAmount = 0;
        
        cout << "You have " << getArmyAmount() << " armies remaining. Enter territory name for reinforcement (or type 'exit' to finish): ";
        cin >> territoryName;

        if (territoryName == "exit") break;

        // Locate the territory to deploy reinforcements
        Territory* selectedTerritory = nullptr;
        for (Territory* t : toDefend) {
            if (t->getName() == territoryName) {
                selectedTerritory = t;
                break;
            }
        }

        if (!selectedTerritory) {
            cout << "Territory not found. Try again." << endl;
            continue;
        }

        // Validates and adds Reinforcement amount for the territory chosen 
        cout << "Enter the number of armies to reinforce " << selectedTerritory->getName() << ": ";
        while (!(cin >> reinforcementAmount) || reinforcementAmount < 0 || reinforcementAmount > getArmyAmount()) {
            cout << "Invalid amount. Enter a number between 0 and " << getArmyAmount() << ": ";
            cin.clear();
            cin.ignore(numeric_limits<streamsize>::max(), '\n');
        }

        // Create a Deploy order and add to orders list
         Order* deployOrder = new Deploy(selectedTerritory, reinforcementAmount, this);
         deployOrder->execute();
         orders->addOrder(deployOrder);  
        setArmyAmount(getArmyAmount() - reinforcementAmount);
        cout << "Deploy order issued to add " << reinforcementAmount << " armies to " << selectedTerritory->getName() << "." << endl;
    }

    cout << "Reinforcement phase completed.\n------------------------------------------------" << endl;

  bool issuingAdvanceOrders = true;
while (issuingAdvanceOrders) {
    cout << "Territories available to move/attack from:" << endl;
    cout << "------------------------------------------------" << endl;
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
        cout << "- " << t->getName() << ": " << t->getArmyAmount() << " armies." << endl;
    }

    // Display enemy territories (for attack)
    cout << "\nEnemy territories (to attack):" << endl;
    for (Territory* t : toAttack) {
        cout << "- " << t->getName() << ": " << t->getArmyAmount() << " armies." << endl;
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
    int reinforcementAmount = 0;
    while (true) {
        cout << "Enter number of armies to move: ";
        if (cin >> reinforcementAmount && reinforcementAmount > 0) break;
        cout << "Invalid number. Enter a positive integer: ";
        cin.clear();
        cin.ignore(numeric_limits<streamsize>::max(), '\n');
    }
//Territory* source, Territory* destination, int armyCount, Player* player
     
    Order* order = new Advance(this,source,destination,reinforcementAmount);
    orders->addOrder(order); // Add the order to the player's order list

    cout << "Advance order issued to move " << reinforcementAmount << " armies from " << source->getName()
         << " to " << destination->getName() << "." << endl;

    // Ask if the player wants to issue another Advance order
    string decision;
    cout << "Would you like to issue another order? (Y/N): ";
    cin >> decision;
    issuingAdvanceOrders = (decision == "Y" || decision == "y");
}

this->resetCardReceived();// player can only get one card per advancing phase

cout << "Orders Issuing phase for Advance orders completed.\n------------------------------------------------" << endl;

}



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

void Player::removeTerritory(Territory* territory) {
    auto it = find(territories->begin(), territories->end(), territory); // Find the territory in the list
    if (it != territories->end()) { // If found
        territories->erase(it); // Remove the territory
    }
}

void Player::printOwnedTerritories() const {
    cout << "Player " << *name << " owns the following territories:\n";
    for (auto t : *territories) {
        std::cout << t->getName() << std::endl;  
    }
}

void Player::addCard(Card* card) {
    cards->push_back(card);  // Adds a card to the player's hand
}

void Player::removeCard(Card* card) {
    auto it = find(cards->begin(), cards->end(), card); // Find the card in the hand
    if (it != cards->end()) { // If found
        delete *it; // Delete the card to free memory
        cards->erase(it); // Remove the card from the hand
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
