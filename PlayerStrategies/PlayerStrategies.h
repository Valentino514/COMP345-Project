#ifndef PLAYERSTRATEGIES_H
#define PLAYERSTRATEGIES_H
#include <vector>
#include "../OrdersList/Orders.h"
#include "../Map/Map.h"

class Player; // Forward declaration of Player class

class PlayerStrategy {
public:
    virtual ~PlayerStrategy() = default;

    virtual void issueOrder(Player* player,const std::vector<Player*>& playerList) = 0;
    virtual std::vector<Territory*> toDefend(const Player* player) const = 0;
    virtual std::vector<Territory*> toAttack( const Player* player) const = 0;
};

class HumanPlayerStrategy : public PlayerStrategy {
public:
    void issueOrder(Player* player,const std::vector<Player*>& playerList) ;
    std::vector<Territory*> toDefend(const Player* player) const ;
    std::vector<Territory*> toAttack(const Player* player) const ;
};

class AggresivePlayerStrategy: public PlayerStrategy{
public:
    void issueOrder(Player* player,const std::vector<Player*>& playerList) ;
    std::vector<Territory*> toDefend(const Player* player) const ;
    std::vector<Territory*> toAttack(const Player* player) const ;
};


class BenevolentPlayerStrategy: public PlayerStrategy{

};

// NeutralPlayerStrategy class
class NeutralPlayerStrategy : public PlayerStrategy {
public:
    void issueOrder(Player* player,const std::vector<Player*>& playerList) override;
    std::vector<Territory*> toDefend(const Player* player) const override ;
    std::vector<Territory*> toAttack( const Player* player) const override ;
};

// CheaterPlayerStrategy class
class CheaterPlayerStrategy : public PlayerStrategy {
public:
    void issueOrder(Player* player,const std::vector<Player*>& playerList) override;
    std::vector<Territory*> toDefend(const Player* player) const ;
    std::vector<Territory*> toAttack(const Player* player) const ;
};




#endif
