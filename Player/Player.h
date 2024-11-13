#ifndef PLAYER_H_
#define PLAYER_H_

#include <vector>
#include <iostream>
#include <list>
#include "../OrdersList/Orders.h"
#include "../Map/Map.h"
#include "../CardsDeckHand/Cards.h"

class Territory;
class GameEngine;

class Player
{
private:
    
    std::vector<Player*> negotiatedPlayers; // Players with whom this player has a truce
    vector<Territory*>* territories;
    vector<Card*>* cards;
    OrdersList *orders;
    string* name;
    int* armyamount;
    

public:


    void clearNegotiations(); // Clears the list of negotiated players
    Player(string* name, int* armyamount);
    Player(string& name);

    ~Player(); //destructor

    // Copy Constructor
    Player(const Player& other);

    // Assignment Operator
    Player& operator=(const Player& other);
    void setArmyAmount(int amount);
    int getArmyAmount() const { return *armyamount; }

    friend ostream& operator<<(ostream& os, const Player& player);

    void issueOrder(const std::vector<Player*>& playerList);

    std::vector<Territory*> toDefend() const;

    vector<Territory*> toAttack() const;

    void addTerritory(Territory* territory);

    void printOwnedTerritories() const;

    const std::vector<Card*>& getCards() const { return *cards; }

    void addCard(Card* card);

    void removeCard(Card* card) ;

    bool hasCard(Card::CardType type) const;

    std::string* getName() const { return name; }

     const std::vector<Territory*>* getTerritories() const {
        return territories;
    }

    void removeTerritory(Territory* territory);

      OrdersList* getOrdersList() const {
        return orders;
    }


    // Accessor for territories list size
    size_t getTerritoryCount() const {
        return territories->size();
    }
    // Methods to manage negotiated players
    void addNegotiatedPlayer(Player* player);

    bool isNegotiatedWith(const Player* player) const;

    Territory* selectTargetFromAttackList();

    Territory* selectTargetFromDefendList();

    Player* selectPlayerToNegotiate(const vector<Player*>& playerList) ;

    Territory* selectSourceTerritory();

    Territory* selectDestinationTerritory();

    int selectArmyAmount(Territory* sourceTerritory);
};


#endif