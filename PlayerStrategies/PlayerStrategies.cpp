#include "PlayerStrategies.h"
#include "../Player/Player.h"
#include <iostream>
#include <algorithm>

// HumanPlayerStrategy Implementation
void HumanPlayerStrategy::issueOrder(Player* player) {
    std::cout << "HumanPlayerStrategy: Issuing orders based on user input.\n";
    // Directly call the player's issueOrder method (it already handles user interaction).
    std::vector<Player*> emptyPlayerList; // Create an empty vector to satisfy the parameter requirement.
    player->issueOrder(emptyPlayerList);
}

std::vector<Territory*> HumanPlayerStrategy::toDefend(const Player* player) const {
    return player->toDefend(); // Reuse Player's logic
}

std::vector<Territory*> HumanPlayerStrategy::toAttack(const Player* player) const {
    return player->toAttack(); // Reuse Player's logic
}


// NeutralPlayerStrategy Implementation
void NeutralPlayerStrategy::issueOrder(Player* player) {
    std::cout << "NeutralPlayer: Doing nothing.\n";
    // Does not issue any orders
}

std::vector<Territory*> NeutralPlayerStrategy::toDefend(const Player* player) const {
    return {}; // Does not defend
}

std::vector<Territory*> NeutralPlayerStrategy::toAttack(const Player* player) const {
    return {}; // Does not attack
}

// CheaterPlayerStrategy Implementation
void CheaterPlayerStrategy::issueOrder(Player* player) {
    std::cout << "CheaterPlayerStrategy: Automatically conquering all adjacent enemy territories!\n";
    std::vector<Territory*> attackList = player->toAttack(); // Get territories that can be attacked

    for (Territory* territory : attackList) {
        // Change ownership of each territory to the cheater player
        std::cout << "CheaterPlayerStrategy: Conquered " << territory->getName() << "!\n";
        territory->setLandOccupier(player); // Set the cheater as the owner
    }
}


std::vector<Territory*> CheaterPlayerStrategy::toDefend(const Player* player) const {
    return player->toDefend(); // Defends all owned territories
}

std::vector<Territory*> CheaterPlayerStrategy::toAttack(const Player* player) const {
    auto attackList = player->toAttack();
    std::cout << "CheaterPlayer considers these territories to attack:\n";
    for (Territory* t : attackList) {
        std::cout << "- " << t->getName() << "\n";
    }
    return attackList;
}

