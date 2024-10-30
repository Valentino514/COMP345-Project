#include <iostream>
#include "Player.h"
#include <fstream>
#include <utility>
#include <sstream>
#include <vector>
using namespace std;

Player::Player(string* name, int* armyamount) {
    cout << "Generating player..." << *name << " with " << *armyamount << " armies\n";
    this->name = new string(*name);  // Deep copy name
    this->armyamount = new int(*armyamount);  // Deep copy the value of armyamount
    orders = new OrdersList();
    cards = new vector<Card*>;
    territories = new vector<Territory*>;
}
Player::Player(string* name) {
    cout << "Generating player..." << *name << " with 0 armies (default)\n";
    this->name = new string(*name);  // Deep copy name
    this->armyamount = new int(0);  // Default armyamount to 0
    orders = new OrdersList();
    cards = new vector<Card*>;
    territories = new vector<Territory*>;
}

Player::Player(const Player& other) {
    name = new string(*other.name);
    armyamount = new int(*other.armyamount);  // Deep copy armyamount

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

    // Deep copy orders
    orders = new OrdersList(*(other.orders));
}

void Player::setArmyAmount(int amount) {
    if (armyamount) {
        *armyamount = amount;  // Update the existing armyamount
    } else {
        armyamount = new int(amount);  // Allocate memory and assign value
    }
}

// Assignment operator for deep copying
Player& Player::operator=(const Player& other) {
    if (this == &other) return *this;  // Self-assignment check

    // Clean up existing data
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

    // Deep copy name and armyamount
    this->name = new string(*other.name);
    this->armyamount = new int(*other.armyamount);

    // Deep copy territories
    for (Territory* t : *(other.territories)) {
        territories->push_back(new Territory(*t));
    }

    // Deep copy cards
    for (Card* c : *(other.cards)) {
        cards->push_back(new Card(*c));
    }

    // Deep copy orders
    *orders = *(other.orders);

    return *this;
}


// Destructor
Player::~Player() {
    delete name;
    delete armyamount;  // Free the memory for armyamount

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

    delete orders;  // Clean up the orders list
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

// Issue an order
void Player::issueOrder(Order* order) {
    orders->addOrder(order);
    cout << "Order " << *order << " added to list!" << endl;
}

//Overloaded stream insertion 
ostream& operator<<(ostream& os, const Player& player) {
    os << "Player with name " << *(player.name)
       << " has " << player.territories->size() << " territories, "
       << player.cards->size() << " cards, and "
       << *(player.armyamount) << " armies.";  // Print the value of armyamount
    return os;
}


void Player::addTerritory(Territory* territory) {
    territories->push_back(territory);  // Add territory to the player's list
}


void Player::printOwnedTerritories() const {
    std::cout << "Player " << *name << " owns the following territories:\n";
    for (auto t : *territories) {
        std::cout << t->getName() << std::endl;  // Assuming Territory class has getName() method
    }
}

void Player::addCard(Card* card) {
    cards->push_back(card);  // Adds a card to the player's hand
}
