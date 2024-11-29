#ifndef PLAYERSTRATEGIES_H
#define PLAYERSTRATEGIES_H

#include <vector>
#include "../OrdersList/Orders.h"
#include "../Map/Map.h"
#include "../Player/Player.h"

using namespace std;

// Abstract base class defining the Strategy interface for Player behaviors
class PlayerStrategy {
public:
    virtual ~PlayerStrategy() = default;

    // Pure virtual method for issuing orders based on the player's strategy
    virtual void issueOrder(Player* player, const vector<Player*>& playerList) = 0;

    // Pure virtual method to determine which territories the player should defend
    virtual vector<Territory*> toDefend(const Player* player) const = 0;

    // Pure virtual method to determine which territories the player should attack
    virtual vector<Territory*> toAttack(const Player* player) const = 0;
};

// Concrete strategy for Human-controlled player behavior
class HumanPlayerStrategy : public PlayerStrategy {
public:
    // Issue orders interactively with user input
    void issueOrder(Player* player, const vector<Player*>& playerList) override;

    // Determine territories to defend based on user input
    vector<Territory*> toDefend(const Player* player) const override;

    // Determine territories to attack based on user input
    vector<Territory*> toAttack(const Player* player) const override;
};

// Concrete strategy for Aggressive player behavior
class AggresivePlayerStrategy : public PlayerStrategy {
public:
    // Issue orders to focus on attacking enemy territories
    void issueOrder(Player* player, const vector<Player*>& playerList) override;

    // Determine the strongest territory for defense (if needed)
    vector<Territory*> toDefend(const Player* player) const override;

    // Determine territories to attack based on aggressive behavior
    vector<Territory*> toAttack(const Player* player) const override;
};

// Concrete strategy for Benevolent player behavior
class BenevolentPlayerStrategy : public PlayerStrategy {
public:
    // Issue orders to focus on defending the weakest territories
    void issueOrder(Player* player, const vector<Player*>& playerList) override;

    // Determine the weakest territories to prioritize for defense
    vector<Territory*> toDefend(const Player* player) const override;

    // Benevolent player does not attack; returns an empty list
    vector<Territory*> toAttack(const Player* player) const override;
};

// Concrete strategy for Neutral player behavior
class NeutralPlayerStrategy : public PlayerStrategy {
public:
    // Neutral player does not issue orders
    void issueOrder(Player* player, const vector<Player*>& playerList) override;

    // Neutral player does not defend; returns an empty list
    vector<Territory*> toDefend(const Player* player) const override;

    // Neutral player does not attack; returns an empty list
    vector<Territory*> toAttack(const Player* player) const override;
};

// Concrete strategy for Cheater player behavior
class CheaterPlayerStrategy : public PlayerStrategy {
public:
    // Automatically conquer all adjacent enemy territories
    void issueOrder(Player* player, const vector<Player*>& playerList) override;

    // Determine which territories to defend (all owned territories)
    vector<Territory*> toDefend(const Player* player) const override;

    // Automatically identify all adjacent enemy territories to attack
    vector<Territory*> toAttack(const Player* player) const override;
};

#endif
