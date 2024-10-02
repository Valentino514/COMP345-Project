#include <iostream>
#include "Player.h"
#include <fstream>
#include <utility>
#include <sstream>
#include <vector>
using namespace std;

Player::Player(string* name){

cout<<"generating player..."<<*name<<"\n";
this->name = name;
}

Player::Player(const Player& other) {
  
    name = new string(*other.name); 

    // Deep copy territories
    for (Territory* t : other.territories) {
        territories.push_back(new Territory(*t)); 
    }

    // Deep copy cards
    for (Cards* c : other.cards) {
        cards.push_back(new Cards(*c)); 
    }
}

ostream& operator<<(ostream& os, const Player& player) {
    os << "Player with name " << *(player.name) << " has  " << player.territories.size() << " territories and "<< player.cards.size() << " cards.";
    return os;
}

Player& Player::operator=(const Player &other) {
    if (this == &other) return *this; 

    delete this->name;
    for (Territory* t : this->territories) {
        delete t;
    }
    territories.clear();
    for (Cards* c : this->cards) {
        delete c;
    }
    cards.clear();

    this->name = new string(*other.name);

    for (Territory* t : other.territories) {
        this->territories.push_back(new Territory(*t));
    }

    for (Cards* c : other.cards) {
        this->cards.push_back(new Cards(*c));
    }

    return *this;
}

Player::~Player() {
   
    delete name;

    for (Territory* t : territories) {
        delete t;
    }

    for (Cards* c : cards) {
        delete c;
    }

    name = NULL;
    territories.clear();
    cards.clear();
}

void Player::toDefend(){

    cout<<"territories to defend:\n";

    for(const auto& territory: territories){

        cout<<territory<<"\n";

    }

}

void Player::toAttack(){
    cout<<"territories to attack:\n";
    
}

void Player::issueOrder(Order* order){
    
    orders.addOrder(order);
    cout<<"order "<<*order<<" added to list!";

}
