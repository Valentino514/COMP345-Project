#include <iostream>
#include "Player.h"
#include <fstream>
#include <utility>
#include <sstream>
#include <vector>
using namespace std;

Player::Player(string* name){

cout<<"Generating player..."<<*name<<"\n";
this->name = name;
orders= new OrdersList();
cards = new vector<Cards*>;
territories = new vector<Territory*>;
}

Player::Player(const Player& other) {
    name = new string(*other.name);

    // Deep copy territories
    territories = new vector<Territory*>;
    for (Territory* t : *(other.territories)) {
        territories->push_back(new Territory(*t));
    }

    // Deep copy cards
    cards = new vector<Cards*>;
    for (Cards* c : *(other.cards)) {
        cards->push_back(new Cards(*c));
    }

    // Deep copy orders
    orders = new OrdersList(*(other.orders)); 
}

// Assignment operator for deep copying
Player& Player::operator=(const Player& other) {
    if (this == &other) return *this;

    // Clean up existing data
    delete this->name;
    for (Territory* t : *territories) {
        delete t;
    }
    territories->clear();
    for (Cards* c : *cards) {
        delete c;
    }
    cards->clear();

    // Deep copy name
    this->name = new string(*other.name);

    // Deep copy territories
    for (Territory* t : *(other.territories)) {
        territories->push_back(new Territory(*t));
    }

    // Deep copy cards
    for (Cards* c : *(other.cards)) {
        cards->push_back(new Cards(*c));
    }

    // Deep copy orders
    *orders = *(other.orders); 

    return *this;
}

// Destructor
Player::~Player() {
    delete name;

    for (Territory* t : *territories) {
        delete t;
    }
    territories->clear();
    delete territories;  
    for (Cards* c : *cards) {
        delete c;
    }
    cards->clear();
    delete cards;  // 

    delete orders;  // Clean up the orders list
}

// Print territories to defend
void Player::toDefend() {
    cout << "Territories to defend:\n";
    for (const auto& territory : *territories) {
        cout << territory << "\n";  
    }
}

// Print territories to attack
void Player::toAttack() {
    cout << "Territories to attack:\n";
}

// Issue an order
void Player::issueOrder(Order* order) {
    orders->addOrder(order);
    cout << "Order " << *order << " added to list!" << endl;
}

//Overloaded stream insertion 
ostream& operator<<(ostream& os, const Player& player) {
    os << "Player with name " << *(player.name) << " has " << player.territories->size() << " territories and " << player.cards->size() << " cards.";
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