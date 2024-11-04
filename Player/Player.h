#ifndef PLAYER_H_
#define PLAYER_H_

#include <vector>
#include <iostream>
#include <list>
#include "../OrdersList/Orders.h"
#include "../Map/Map.h"
#include "../CardsDeckHand/Cards.h"

class Territory;

class Player
{
private:

    vector<Territory*>* territories;
    vector<Card*>* cards;
    OrdersList *orders;
    string* name;
    int* armyamount;
    bool cardReceivedinTurn;
    

public:


    Player(string* name, int* armyamount);
    Player(string* name);

    ~Player(); //destructor

    // Copy Constructor
    Player(const Player& other);

    // Assignment Operator
    Player& operator=(const Player& other);
    void setArmyAmount(int amount);
    int getArmyAmount() const { return *armyamount; }

    friend ostream& operator<<(ostream& os, const Player& player);

    void issueOrder();

    std::vector<Territory*> toDefend() const;

    vector<Territory*> toAttack() const;

    void addTerritory(Territory* territory);

    void removeTerritory(Territory* territory);

    void printOwnedTerritories() const;

    const vector<Card*>* getCards() const { return cards; }

    bool hasCard(Card::CardType type) const;

    void addCard(Card* card);

    void removeCard(Card* card) ;

    void cardReceived(){cardReceivedinTurn = true;}

    void resetCardReceived() { cardReceivedinTurn = false; } //reset for the next turn

    bool canReceiveCard() const { return !cardReceivedinTurn; } //check if player can receive a card


    string* getName() const { return name; }

     const vector<Territory*>* getTerritories() const {
        return territories;
    }

      OrdersList* getOrdersList() const {
        return orders;
    }

    // Accessor for territories list size
    size_t getTerritoryCount() const {
        return territories->size();
    }

    
};

#endif