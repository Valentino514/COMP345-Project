#ifndef PLAYER_H_
#define PLAYER_H_

#include <vector>
#include <iostream>
#include <list>
#include "../OrdersList/Orders.h"
#include "../Map/Map.h"
#include "../CardsDeckHand/Cards.h"

class Territory;


// class Cards{

//     public:
//     Card();
// }


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

    friend ostream& operator<<(ostream& os, const Player& player);

    void issueOrder(Order*);

    void toDefend();

    void toAttack();

    void addTerritory(Territory* territory);

    void printOwnedTerritories() const;

    void addCard(Card* card);

    std::string* getName() const { return name; }

     const std::vector<Territory*>* getTerritories() const {
        return territories;
    }

    
};

#endif