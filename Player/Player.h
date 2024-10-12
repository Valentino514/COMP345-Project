#ifndef PLAYER_H_
#define PLAYER_H_

#include <vector>
#include <iostream>
#include <list>
#include "../OrdersList/Orders.h"
#include "../Map/Map.h"

class Territory;


class Cards{

    public:
    Cards();
};


class Player
{
private:

    vector<Territory*>* territories;
    vector<Cards*>* cards;
    OrdersList *orders;
    string* name;

public:


    Player(string* name); 

    ~Player(); //destructor

    // Copy Constructor
    Player(const Player& other);

    // Assignment Operator
    Player& operator=(const Player& other);

    friend ostream& operator<<(ostream& os, const Player& player);

    void issueOrder(Order*);

    void toDefend();

    void toAttack();

    void addTerritory(Territory* territory);

    void printOwnedTerritories() const;
    
};

#endif