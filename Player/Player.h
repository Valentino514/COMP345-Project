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

    void printOwnedTerritories() const;

    const std::vector<Card*>& getCards() const { return *cards; }
    void addCard(Card* card);

    std::string* getName() const { return name; }

     const std::vector<Territory*>* getTerritories() const {
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