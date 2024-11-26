#ifndef PLAYERSTRATEGIES_H
#define PLAYERSTRATEGIES_H
#include <vector>
#include "../OrdersList/Orders.h"
#include "../Map/Map.h"
#include "../Player/Player.h"

using namespace std;


class PlayerStrategy {
public:
    virtual ~PlayerStrategy() = default;

    virtual void issueOrder(Player* player,const vector<Player*>& playerList) = 0;
    virtual vector<Territory*> toDefend(const Player* player) const = 0;
    virtual vector<Territory*> toAttack( const Player* player) const = 0;
};

class HumanPlayerStrategy : public PlayerStrategy {
public:
    void issueOrder(Player* player,const vector<Player*>& playerList) ;
    vector<Territory*> toDefend(const Player* player) const ;
    vector<Territory*> toAttack(const Player* player) const ;
};

class AggresivePlayerStrategy: public PlayerStrategy{
public:
    void issueOrder(Player* player, const vector<Player*>& playerList) ;
    vector<Territory*> toDefend(const Player* player) const ;
    vector<Territory*> toAttack(const Player* player) const ;
};


class BenevolentPlayerStrategy: public PlayerStrategy{
    public:
    void issueOrder(Player* player,const vector<Player*>& playerList) override;
    vector<Territory*> toDefend(const Player* player) const override ;
    vector<Territory*> toAttack( const Player* player) const override ;

};

// NeutralPlayerStrategy class
class NeutralPlayerStrategy : public PlayerStrategy {
public:
    void issueOrder(Player* player,const vector<Player*>& playerList) override;
    vector<Territory*> toDefend(const Player* player) const override ;
    vector<Territory*> toAttack( const Player* player) const override ;
};

// CheaterPlayerStrategy class
class CheaterPlayerStrategy : public PlayerStrategy {
public:
    void issueOrder(Player* player,const vector<Player*>& playerList) override;
    vector<Territory*> toDefend(const Player* player) const ;
    vector<Territory*> toAttack(const Player* player) const ;
};




#endif
