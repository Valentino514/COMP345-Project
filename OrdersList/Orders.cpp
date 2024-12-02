#include <algorithm>
#include <cstdlib> // for random
#include <ctime> // for random
#include <iostream>
#include <fstream>
#include <limits>  // for numeric_limits
#include <algorithm>  // for max

#include "Orders.h"
#include "../Map/Map.h"
#include "../Player/Player.h"
#include"../PlayerStrategies/PlayerStrategies.h"
using namespace std;

/**
 * @class Order
 * @brief Represents a generic order in the game.
 * 
 * The Order class is the base class for various types of orders, such as deploy, attack, or move.
 * It provides common functionality for order name handling, assignment, and notification.
 */


// Default Constructor
/**
 * @brief Constructs an Order object.
 * 
 * @details Initializes the order with no name (nullptr).
 */
Order::Order() : orderName(nullptr) {}

// Destructor
/**
 * @brief Destroys the Order object and releases its allocated memory.
 * 
 * @details Deletes the dynamically allocated `orderName` string.
 */
Order::~Order() {
    delete orderName;  
}

// Copy Constructor
/**
 * @brief Creates a deep copy of another Order object.
 * 
 * @param other The Order object to copy.
 */
Order::Order(const Order& other) {
    orderName = new string(*other.orderName);
}

// Assignment Operator
/**
 * @brief Assigns the state of another Order object to this one.
 * 
 * @param other The Order object to copy.
 * @return A reference to the updated Order object.
 */
Order& Order::operator=(const Order& other) {
    if (this == &other) return *this; 
    delete orderName; 
    orderName = new string(*other.orderName); 
    return *this;
}

// Stream Insertion Operator
/**
 * @brief Outputs the details of the Order object to an output stream.
 * 
 * @param os The output stream.
 * @param order The Order object to output.
 * @return The modified output stream.
 */
ostream& operator<<(ostream& os, const Order& order) {
    order.print(os);  
    return os;
}

// Notify method
/**
 * @brief Notifies the associated orders list about the current state of the order.
 * 
 * @details If the order has an associated orders list, this method calls its `notify` method.
 */
void Order::notify() {
    if (ordersList) {
        ordersList->notify();
    }
}

/**
 * @class Deploy
 * @brief Represents a deploy order, which involves sending reinforcements to a territory.
 * 
 * The Deploy class is a specific type of order that allows a player to deploy troops to
 * one of their territories. It checks ownership and executes the deployment of soldiers.
 */

// Default Constructor
/**
 * @brief Constructs a default Deploy object.
 * 
 * @details Initializes an empty deploy order with no target territory, no reinforcement amount, and no player.
 */
Deploy::Deploy() : Order(), targetTerritory(nullptr), reinforcementAmount(nullptr), player(nullptr) {}

// Parameterized Constructor
/**
 * @brief Constructs a Deploy object with the specified target territory, reinforcement amount, and player.
 * 
 * @param target The target territory for deployment.
 * @param reinforcementAmount The number of units to deploy.
 * @param player The player who is deploying the troops.
 */
Deploy::Deploy(Territory* target, int reinforcementAmount, Player* player)
    : Order(), targetTerritory(target), reinforcementAmount(new int(reinforcementAmount)), player(player) {}

// Copy Constructor
/**
 * @brief Creates a deep copy of another Deploy object.
 * 
 * @param other The Deploy object to copy.
 */
Deploy::Deploy(const Deploy& other)
    : Order(other), targetTerritory(other.targetTerritory), reinforcementAmount(new int(*other.reinforcementAmount)) {}

// Assignment Operator
/**
 * @brief Assigns the state of another Deploy object to this one.
 * 
 * @param other The Deploy object to copy.
 * @return A reference to the updated Deploy object.
 */
Deploy& Deploy::operator=(const Deploy& other) {
    if (this == &other) return *this;
    Order::operator=(other);
    delete reinforcementAmount;
    reinforcementAmount = new int(*other.reinforcementAmount);
    targetTerritory = other.targetTerritory;
    return *this;
}

// Destructor
/**
 * @brief Destroys the Deploy object and releases its allocated memory.
 * 
 * @details Deletes the dynamically allocated reinforcement amount.
 */
Deploy::~Deploy() {
    delete reinforcementAmount;
}

// Validate method
/**
 * @brief Validates the deploy order.
 * 
 * @details Checks if the player owns the target territory and if the deployment amount is valid.
 * 
 * @return true if the deploy order is valid, false otherwise.
 */
bool Deploy::validate() {
    const vector<Territory*>* player_trt = player->getTerritories();
    //checks if players owns the territory
    bool isTerritoryOwned = any_of(player_trt->begin(), player_trt->end(), [this](Territory *t){return t == this->targetTerritory;});
    //bool haveEnoughUnits = 
    return isTerritoryOwned;// && haveEnoughUnits;
}

// Execute method
/**
 * @brief Executes the deploy order.
 * 
 * @details Adds the specified reinforcement amount to the target territory's army count.
 */
void Deploy::execute() {

    int newArmyAmmount = (targetTerritory->getArmyAmount() + (*reinforcementAmount));
    targetTerritory->setArmyAmount(newArmyAmmount);
    
}

// Print method
/**
 * @brief Prints the details of the deploy order to an output stream.
 * 
 * @param os The output stream.
 */
void Deploy::print(ostream& os) const {
    os << "deploy: " << *reinforcementAmount << " soldiers to " << targetTerritory->getName() << "\n";
}

// String to Log method
/**
 * @brief Converts the deploy order to a string suitable for logging.
 * 
 * @return A string representing the deploy order.
 */
string Deploy::stringToLog() const {
    string reinforcementStr = to_string(*reinforcementAmount);
    return "Deploy : " + reinforcementStr + " soldiers to " + targetTerritory->getName();
}

/**
 * @class Advance
 * @brief Represents an advance order, where a player moves troops from one territory to another.
 * 
 * The Advance class allows a player to move troops from a source territory to an adjacent destination
 * territory. It handles validation, execution of the order (including combat if the destination is owned by an enemy),
 * and logging of the result.
 */

// Default Constructor
/**
 * @brief Constructs an empty Advance order.
 * 
 * @details Initializes an advance order with no specified player, source or destination territories, and army count.
 */
Advance::Advance()
    : Order(), isDestinationOwned(false), player(nullptr), sourceTerritory(nullptr), destinationTerritory(nullptr), reinforcementAmount(0) {}

// Parameterized Constructor
/**
 * @brief Constructs an Advance order with the specified player, source territory, destination territory, and army count.
 * 
 * @param player The player initiating the advance.
 * @param source The source territory to move armies from.
 * @param destination The destination territory to move armies to.
 * @param armyCount The number of armies to move.
 */
Advance::Advance(Player* player, Territory* source, Territory* destination, int armyCount)
    : Order(), isDestinationOwned(false), player(player), sourceTerritory(source), destinationTerritory(destination), reinforcementAmount(armyCount) {}

// Copy Constructor
/**
 * @brief Creates a deep copy of another Advance order.
 * 
 * @param other The Advance order to copy.
 */
Advance::Advance(const Advance& other)
    : Order(other),
      isDestinationOwned(other.isDestinationOwned),
      player(other.player),
      sourceTerritory(other.sourceTerritory),
      destinationTerritory(other.destinationTerritory),
      reinforcementAmount(other.reinforcementAmount) {}

// Assignment Operator
/**
 * @brief Assigns the state of another Advance order to this one.
 * 
 * @param other The Advance order to copy.
 * @return A reference to the updated Advance order.
 */
Advance& Advance::operator=(const Advance& other) {
    if (this == &other) return *this;
    Order::operator=(other);
    player = other.player;  
    sourceTerritory = other.sourceTerritory;
    destinationTerritory = other.destinationTerritory;
    reinforcementAmount = other.reinforcementAmount;
    isDestinationOwned = other.isDestinationOwned; // Added this line
    return *this;
}

// Destructor
/**
 * @brief Destroys the Advance order.
 * 
 * @details The destructor is trivial as there are no dynamic resources to clean up for this class.
 */
Advance::~Advance() {}

// Validate method
/**
 * @brief Validates the advance order.
 * 
 * @details Checks if the source territory is owned by the player, if the destination is adjacent to the source territory,
 * and if the player has enough armies in the source territory. Additional checks for existing truces with the destination owner
 * are also included.
 * 
 * @return true if the advance order is valid, false otherwise.
 */
bool Advance::validate() {
    //cout << "validating advance order\n";
    const vector<Territory*>* player_trt = player->getTerritories();
    const vector<Territory*>* adjacentTerritories = sourceTerritory->getAdjacentTerritories();

    bool isSourceOwned = any_of(player_trt->begin(), player_trt->end(), [this](Territory* t) { return t == this->sourceTerritory; });
    this->isDestinationOwned = any_of(player_trt->begin(), player_trt->end(), [this](Territory* t) { return t == this->destinationTerritory; });
    bool isDestinationAdjacent = any_of(adjacentTerritories->begin(), adjacentTerritories->end(), [this](Territory* t) { return t == this->destinationTerritory; });

    // New validation check
    bool hasEnoughArmies = sourceTerritory->getArmyAmount() >= reinforcementAmount;
    if (!hasEnoughArmies) {
        cout << "Advance order validation failed: Not enough armies in source territory.\n";
    }

    // Existing truce check
    if (!isDestinationOwned) {
        Player* territoryOwner = destinationTerritory->getLandOccupier();
        if (player->isNegotiatedWith(territoryOwner)) {
            cout << "Advance order validation failed: Truce with " << *(territoryOwner->getName()) << " prevents the attack.\n";
            return false;
        }
    }
    //if source territory not owned
    if (!isSourceOwned) {
        cout << "Source territory not owned\n";
    } else if (!isDestinationAdjacent) {
        cout << "Target territory not adjacent\n";
    }

    return (isSourceOwned && isDestinationAdjacent && hasEnoughArmies);
}

// Execute method
/**
 * @brief Executes the advance order, moving armies from the source territory to the destination territory.
 * 
 * @details If the destination is owned by the player, the reinforcement amount is transferred to the destination. 
 * If the destination is owned by an enemy, a battle occurs, and the outcome is determined based on random chances.
 */
void Advance::execute() {
        if (isDestinationOwned) {
            int newSourceArmy = sourceTerritory->getArmyAmount() - reinforcementAmount;
            sourceTerritory->setArmyAmount(newSourceArmy);

            int newTargetArmy = destinationTerritory->getArmyAmount() + reinforcementAmount;
            destinationTerritory->setArmyAmount(newTargetArmy);

            //cout<<"moving troops to different owned territory\n";

        } else {
            // If player is attacking another player
            Player* enemy = destinationTerritory->getLandOccupier();
            int enemyTroops = destinationTerritory->getArmyAmount();
            int attackerTroops = reinforcementAmount;

            srand(static_cast<unsigned>(time(0))); // Seed for random number generator

            while (attackerTroops > 0 && enemyTroops > 0) {
                // Attacking units have a 60% chance of killing a defending unit
                for (int i = 0; i < attackerTroops && enemyTroops > 0; ++i) {
                    if (rand() % 100 < 60) {  // 60% chance
                        enemyTroops--;
                    }
                }

                // Defending units have a 70% chance of killing an attacking unit
                for (int i = 0; i < enemyTroops && attackerTroops > 0; ++i) {
                    if (rand() % 100 < 70) {  // 70% chance
                        attackerTroops--;
                    }
                }
            }

            // Check outcome
            if (enemyTroops == 0 && !isDestinationOwned) {
                // Attacker wins and captures the territory
                //cout << "Player " << *(player->getName()) << " managed to eliminate the enemy units\n";
                destinationTerritory->setLandOccupier(player); 
                destinationTerritory->setArmyAmount(attackerTroops);  // Surviving attacking troops occupy territory
                player->addTerritory(destinationTerritory); 
                enemy->removeTerritory(destinationTerritory);

                Card* newCard = new Card(new Card::CardType(Card::getRandomCard().getType()));  // Create a new random card
                cout << *(player->getName())<<" received a new card " << newCard->getCardTypeName() << endl;
                player->addCard(newCard); // Add the new card to the player's hand

            } else if (attackerTroops==0 && !isDestinationOwned){
                // Defender wins, update remaining enemy troops in the territory
                //cout << "Player failed to eliminate the enemy units\n";
                destinationTerritory->setArmyAmount(enemyTroops);  // Set remaining defending troops
                //cout << "Enemy has " << destinationTerritory->getArmyAmount() << " remaining troops\n";
            }
            // Trigger strategy change if defender is neutral
                if (dynamic_cast<NeutralPlayerStrategy*>(enemy->getStrategy())) {
                    cout << "Neutral player's territory attacked. Switching to AggressivePlayerStrategy.\n";
                    enemy->setStrategy(new AggresivePlayerStrategy());// placeholder for aggresiveplayerstartgey
                }

            // Update the source territory's army count after the battle
            int remainingSourceArmy = sourceTerritory->getArmyAmount() - reinforcementAmount;
            if (remainingSourceArmy < 0) remainingSourceArmy = 0;  // Ensure army count doesn't go negative
            sourceTerritory->setArmyAmount(remainingSourceArmy);
            //cout << "Player has " << remainingSourceArmy << " remaining in the territory he moved/attacked from\n";
        }
}

// Print method
/**
 * @brief Prints the details of the advance order.
 * 
 * @param os The output stream to print to.
 */
void Advance::print(ostream& os) const {
    os << "advance: move " << reinforcementAmount << " armies from " << sourceTerritory->getName() << " to " << destinationTerritory->getName() << "\n";
}

// String to Log method
/**
 * @brief Converts the advance order to a loggable string format.
 * 
 * @return A string representation of the advance order for logging.
 */
string Advance::stringToLog() const {
    string reinforcementStr = to_string(reinforcementAmount);
    return "Advance Order: " + reinforcementStr + " armies from " + sourceTerritory->getName() + " to " + destinationTerritory->getName();
}

/**
 * @class Bomb
 * @brief Represents a bomb order to drop bombs on an enemy territory.
 * 
 * The Bomb class allows a player to drop bombs on a target territory, reducing its army count by half. It also includes 
 * functionality for validation, execution, and logging of the bombing action.
 */


// Default Constructor
/**
 * @brief Constructs an empty Bomb order.
 * 
 * @details Initializes a Bomb order with no target territory or player.
 */
Bomb::Bomb() : Order(), targetTerritory(nullptr), player(nullptr) {}

// Parameterized Constructor
/**
 * @brief Constructs a Bomb order with the specified target territory and player.
 * 
 * @param target The target territory to bomb.
 * @param player The player issuing the bomb order.
 */
Bomb::Bomb(Territory* target, Player* player) : Order(), targetTerritory(target), player(player) {}

// Copy Constructor
/**
 * @brief Creates a deep copy of another Bomb order.
 * 
 * @param other The Bomb order to copy.
 */
Bomb::Bomb(const Bomb& other) : Order(other), targetTerritory(other.targetTerritory), player(other.player) {}

// Assignment Operator
/**
 * @brief Assigns the state of another Bomb order to this one.
 * 
 * @param other The Bomb order to copy.
 * @return A reference to the updated Bomb order.
 */
Bomb& Bomb::operator=(const Bomb& other) {
    if (this == &other) return *this;
    Order::operator=(other);
    targetTerritory = other.targetTerritory;
    player = other.player;
    return *this;
}

// Destructor
/**
 * @brief Destroys the Bomb order.
 * 
 * @details The destructor is trivial as there are no dynamic resources to clean up for this class.
 */
Bomb::~Bomb() {}

// Validate method
/**
 * @brief Validates the Bomb order.
 * 
 * @details Checks if the target territory is adjacent to one of the player's territories and whether there is a truce with 
 * the player who owns the target territory.
 * 
 * @return true if the Bomb order is valid, false otherwise.
 */
bool Bomb::validate() {
    //cout << "validating bomb order\n";
    vector<Territory*> enemy_trt = player->toAttack();
    bool isTargetAdjacent = any_of(enemy_trt.begin(), enemy_trt.end(), [this](Territory* t) { return t == this->targetTerritory; });

    // Check if there’s a truce with the player who owns the target territory
    Player* territoryOwner = targetTerritory->getLandOccupier();
    if (player->isNegotiatedWith(territoryOwner)) {
        //cout << "Bomb order validation failed: Truce with " << *(territoryOwner->getName()) << " prevents the bombing.\n";
        return false;
    }
    
    //return if the target territory is adjacent
    return (isTargetAdjacent);
}

// Execute method
/**
 * @brief Executes the Bomb order.
 * 
 * @details Reduces the army count of the target territory by half. If the target territory is owned by a neutral player,
 * their strategy is changed to aggressive.
 */
void Bomb::execute() { //checks if its validated first
        //cout<<"bomb order validated"<<endl;
        player->removeCard(Card::Bomb);
        int newArmyAmmount;
        if(targetTerritory->getArmyAmount() <= 0){
            newArmyAmmount = 0;
            }
            else{
               newArmyAmmount = (targetTerritory->getArmyAmount())/2; 
            }
        targetTerritory->setArmyAmount(newArmyAmmount);
        //<<"new army ammount in target territory:"<<targetTerritory->getArmyAmount()<<'\n';

        // Trigger strategy change if the target belongs to a neutral player
        Player* owner = targetTerritory->getLandOccupier();
        if (dynamic_cast<NeutralPlayerStrategy*>(owner->getStrategy())) {
            cout << "Neutral player's territory bombed. Switching to AggressivePlayerStrategy.\n";
            owner->setStrategy(new AggresivePlayerStrategy()); // Switch to aggressive behavior
        }
    
}

// Print method
/**
 * @brief Prints the details of the Bomb order.
 * 
 * @param os The output stream to print to.
 */
void Bomb::print(ostream& os) const {
    os << "bomb: drop bombs on " << targetTerritory->getName() << "\n";
}

// String to Log method
/**
 * @brief Converts the Bomb order to a loggable string format.
 * 
 * @return A string representation of the Bomb order for logging.
 */
string Bomb::stringToLog() const {
    return "Bomb Order: drop bombs on " + targetTerritory->getName();
}

/**
 * @class Blockade
 * @brief Represents a blockade order to double the army in a player's territory and transfer ownership to a neutral player.
 * 
 * The Blockade class allows a player to double the army count in one of their territories and transfer ownership of that 
 * territory to a neutral player. It includes functionality for validation, execution, and logging of the blockade action.
 */

// Default Constructor
/**
 * @brief Constructs an empty Blockade order.
 * 
 * @details Initializes a Blockade order with no target territory or player.
 */
Blockade::Blockade() : Order(), targetTerritory(nullptr) {}

// Parameterized Constructor
/**
 * @brief Constructs a Blockade order with the specified player and target territory.
 * 
 * @param player The player issuing the blockade order.
 * @param target The target territory to blockade.
 */
Blockade::Blockade(Player* player, Territory* target)
    : Order(), targetTerritory(target) {
    issuingPlayer = player;
}

// Copy Constructor
/**
 * @brief Creates a deep copy of another Blockade order.
 * 
 * @param other The Blockade order to copy.
 */
Blockade::Blockade(const Blockade& other) : Order(other), targetTerritory(other.targetTerritory) {}

// Assignment Operator
/**
 * @brief Assigns the state of another Blockade order to this one.
 * 
 * @param other The Blockade order to copy.
 * @return A reference to the updated Blockade order.
 */
Blockade& Blockade::operator=(const Blockade& other) {
    if (this == &other) return *this;
    Order::operator=(other);
    targetTerritory = other.targetTerritory;
    return *this;
}

// Destructor
/**
 * @brief Destroys the Blockade order.
 * 
 * @details The destructor is trivial as there are no dynamic resources to clean up for this class.
 */
Blockade::~Blockade() {}

// Validate method
/**
 * @brief Validates the Blockade order.
 * 
 * @details Checks if the player owns the target territory before proceeding with the blockade.
 * 
 * @return true if the Blockade order is valid, false otherwise.
 */
bool Blockade::validate() {
    if (targetTerritory->getLandOccupier() != issuingPlayer) {
        //cout << "Invalid blockade order: Target territory does not belong to issuing player.\n";
        return false;
    } else {
        //cout << "Blockade order is valid.\n";
        return true;
    }
}

// Static variable to hold the neutral player instance
 Player* Blockade::neutralPlayer = nullptr;

// Static method to create or retrieve the neutral player
/**
 * @brief Creates or retrieves the neutral player instance.
 * 
 * @return The neutral player instance.
 */
Player* Blockade::getOrCreateNeutralPlayer() {
    if (!neutralPlayer) {
        string neutralName = "Neutral";
        int neutralArmy = 0;  // Neutral player starts with no armies
        neutralPlayer = new Player(&neutralName, &neutralArmy);  // Dynamically allocate neutral player(make sure to deallocate at the end of the game )
        //cout << "Neutral player created.\n";
    }
    return neutralPlayer;
}

// Execute method
/**
 * @brief Executes the Blockade order.
 * 
 * @details Doubles the army count in the target territory and transfers ownership of the territory to the neutral player.
 */
void Blockade::execute() {
    if (targetTerritory->getLandOccupier() == issuingPlayer) {
        // Double the army units on the target territory
        targetTerritory->setArmyAmount(targetTerritory->getArmyAmount() * 2);
        
        // Transfer ownership to the neutral player
        Player* neutral = getOrCreateNeutralPlayer();
        targetTerritory->setLandOccupier(neutral);
        
        cout << "Executing blockade: Army units doubled, territory ownership transferred to Neutral player.\n";
    }
}

// Print method
/**
 * @brief Prints the details of the Blockade order.
 * 
 * @param os The output stream to print to.
 */
void Blockade::print(ostream& os) const {
    os << "blockade: on " << targetTerritory->getName() << "\n";
}

// String to Log method
/**
 * @brief Converts the Blockade order to a loggable string format.
 * 
 * @return A string representation of the Blockade order for logging.
 */
string Blockade::stringToLog() const {
    return "Blockade Order: blockade on " + targetTerritory->getName();
}

/**
 * @class Airlift
 * @brief Represents an airlift order to move armies between two territories.
 * 
 * The Airlift class allows a player to move a specified number of armies from one of their territories to another. 
 * It includes functionality for validation, execution, and logging of the airlift action.
 */

// Default Constructor
/**
 * @brief Constructs an empty Airlift order.
 * 
 * @details Initializes an Airlift order with no source or destination territories, no reinforcement amount, or player.
 */
Airlift::Airlift() : Order(), sourceTerritory(nullptr), destinationTerritory(nullptr), reinforcementAmount(nullptr),player(nullptr){}

// Parameterized Constructor
/**
 * @brief Constructs an Airlift order with the specified source territory, destination territory, reinforcement amount, and player.
 * 
 * @param source The source territory to move armies from.
 * @param destination The destination territory to move armies to.
 * @param armyCount The number of armies to move.
 * @param player The player issuing the airlift order.
 */
Airlift::Airlift(Territory* source, Territory* destination, int armyCount, Player* player)
    : Order(), sourceTerritory(source), destinationTerritory(destination), reinforcementAmount(new int(armyCount)),player(player) {}

// Destructor
/**
 * @brief Destroys the Airlift order.
 * 
 * @details Deletes the reinforcement amount dynamically allocated.
 */
Airlift::~Airlift() {
    delete reinforcementAmount;
}

// Validate method
/**
 * @brief Validates the Airlift order.
 * 
 * @details Checks if both the source and destination territories are owned by the player and if the player has enough armies.
 * 
 * @return true if the Airlift order is valid, false otherwise.
 */
bool Airlift::validate() {  
    //cout << "validating airlift order\n";
    const vector<Territory*>* player_trt = player->getTerritories();
    bool isTargetOwned = any_of(player_trt->begin(), player_trt->end(), [this](Territory *t){return t == this->destinationTerritory;});    
    bool isSourceOwned = any_of(player_trt->begin(), player_trt->end(), [this](Territory *t){return t == this->sourceTerritory;});
    
    // New validation check
    bool hasEnoughArmies = sourceTerritory->getArmyAmount() >= *reinforcementAmount;
    if (!hasEnoughArmies) {
        //cout << "Airlift order validation failed: Not enough armies in source territory.\n";
    }
    //returns the validations
    return (isSourceOwned && isTargetOwned && hasEnoughArmies);
}

// Execute method
/**
 * @brief Executes the Airlift order.
 * 
 * @details Moves the specified number of armies from the source territory to the destination territory.
 */
void Airlift::execute() {
        cout << "airlift order validated, current army amount in source territory: " << sourceTerritory->getArmyAmount();
        cout << "\ncurrent army amount in target territory: " << destinationTerritory->getArmyAmount() << endl;
        cout << "deploying " << *reinforcementAmount << " units" << endl;
        player->removeCard(Card::Airlift); 

        int newSourceArmy = sourceTerritory->getArmyAmount() - *reinforcementAmount;
        if (newSourceArmy < 0) newSourceArmy = 0;  // Ensure army count doesn't go negative
        sourceTerritory->setArmyAmount(newSourceArmy);

        int newDestinationArmy = destinationTerritory->getArmyAmount() + *reinforcementAmount;
        destinationTerritory->setArmyAmount(newDestinationArmy);
        cout << "new army amount in source territory: " << sourceTerritory->getArmyAmount();
        cout << "\nnew army amount in target territory: " << destinationTerritory->getArmyAmount() << endl;
}

// Print method
/**
 * @brief Prints the details of the Airlift order.
 * 
 * @param os The output stream to print to.
 */
void Airlift::print(ostream& os) const {
    os << "airlift: move " << *reinforcementAmount << " armies from " << sourceTerritory->getName() << " to " << destinationTerritory->getName() << "\n";
}

// String to Log method
/**
 * @brief Converts the Airlift order to a loggable string format.
 * 
 * @return A string representation of the Airlift order for logging.
 */
string Airlift::stringToLog() const {
    string reinforcementStr = to_string(*reinforcementAmount);
    return "Airlift Order: move " + reinforcementStr + " armies from " + sourceTerritory->getName() + " to " + destinationTerritory->getName();
}

// Copy Constructor
/**
 * @brief Creates a deep copy of another Airlift order.
 * 
 * @param other The Airlift order to copy.
 */
Airlift::Airlift(const Airlift& other)
    : Order(other),
      sourceTerritory(other.sourceTerritory),
      destinationTerritory(other.destinationTerritory),
      reinforcementAmount(new int(*other.reinforcementAmount)) {}



/**
 * @class Negociate
 * @brief Represents a negotiation order between two players to establish a truce.
 * 
 * The Negociate class allows one player to negotiate with another player, forming a truce that prevents them from attacking each other.
 */

// Default Constructor
/**
 * @brief Constructs an empty Negociate order.
 * 
 * @details Initializes a Negociate order with no specified players.
 */
Negociate::Negociate() : Order(), player1(nullptr), player2(nullptr) {}

// Parameterized Constructor
/**
 * @brief Constructs a Negociate order between the specified players.
 * 
 * @param issuer The player initiating the negotiation.
 * @param targetPlayer The player with whom the negotiation is taking place.
 */
Negociate::Negociate(Player* issuer, Player* targetPlayer)
    : Order(), player1(issuer), player2(targetPlayer) {
    issuingPlayer = issuer;
}

// Copy Constructor
/**
 * @brief Creates a deep copy of another Negociate order.
 * 
 * @param other The Negociate order to copy.
 */
Negociate::Negociate(const Negociate& other)
    : Order(other), player1(other.player1), player2(other.player2) {}

// Assignment Operator
/**
 * @brief Assigns the state of another Negociate order to this one.
 * 
 * @param other The Negociate order to copy.
 * @return A reference to the updated Negociate order.
 */
Negociate& Negociate::operator=(const Negociate& other) {
    if (this == &other) return *this;
    Order::operator=(other);
    player1 = other.player1;
    player2 = other.player2;
    return *this;
}

// Destructor
/**
 * @brief Destroys the Negociate order.
 * 
 * @details The destructor is trivial as there are no dynamic resources to clean up for this class.
 */
Negociate::~Negociate() {}

// Validate method
/**
 * @brief Validates the Negociate order.
 * 
 * @details Checks if the players are different (a player cannot negotiate with themselves).
 * 
 * @return true if the Negociate order is valid, false otherwise.
 */
bool Negociate::validate() {
    if (player1 == player2) {
        cout << "Invalid negotiate order: Cannot negotiate with oneself.\n";
        return false;
    } else {
        cout << "Negotiate order is valid.\n";
        return true;
    }
}

// Execute method
/**
 * @brief Executes the Negociate order.
 * 
 * @details Adds the players to each other's negotiated list, preventing them from attacking each other.
 */
void Negociate::execute() {
        if (player1 != player2) {
            // Adds the negotiation effect (truce) for the current turn
            player1->addNegotiatedPlayer(player2);  // Adds player2 to player1's negotiated list
            player2->addNegotiatedPlayer(player1);  // Adds player1 to player2's negotiated list
            cout << "Executing negotiation: " << *(player1->getName()) << " and " << *(player2->getName())
                      << " cannot attack each other this turn.\n";
        }
}

// Print method
/**
 * @brief Prints the details of the Negociate order.
 * 
 * @param os The output stream to print to.
 */
void Negociate::print(ostream& os) const {
    os << "negotiate: between " << *(player1->getName()) << " and " << *(player2->getName()) << "\n";
}

// String to Log method
/**
 * @brief Converts the Negociate order to a loggable string format.
 * 
 * @return A string representation of the Negociate order for logging.
 */
string Negociate::stringToLog() const {
    return "Negociate Order: negotiate between " + *(player1->getName()) + " and " + *(player2->getName());
}


/**
 * @class OrdersList
 * @brief Represents a collection of orders in a game.
 * 
 * The OrdersList class manages a list of orders, such as deploying troops, advancing, or bombing. It includes methods 
 * to add orders, notify observers, execute orders, remove orders, and check for specific types of orders.
 */

// Default Constructor
/**
 * @brief Constructs an empty OrdersList.
 * 
 * @details Initializes an empty list of orders.
 */
OrdersList::OrdersList() {}

// Destructor
/**
 * @brief Destroys the OrdersList.
 * 
 * @details Deletes all orders stored in the list and clears the list.
 */
OrdersList::~OrdersList() {
    for (Order* order : orders) {
        delete order;
    }
    orders.clear();
}

// Assignment Operator
/**
 * @brief Assigns the state of another OrdersList to this one.
 * 
 * @details Creates a deep copy of the orders in the other OrdersList and assigns it to this one.
 * 
 * @param other The OrdersList to copy from.
 * @return A reference to the updated OrdersList.
 */
OrdersList& OrdersList::operator=(const OrdersList& other) {
    if (this != &other) {
        for (Order* order : orders) {
            delete order;
        }
        orders.clear();

        for (const Order* order : other.orders) {
            if (dynamic_cast<const Deploy*>(order)) {
                orders.push_back(new Deploy(*dynamic_cast<const Deploy*>(order)));
            } else if (dynamic_cast<const Advance*>(order)) {
                orders.push_back(new Advance(*dynamic_cast<const Advance*>(order)));
            } else if (dynamic_cast<const Bomb*>(order)) {
                orders.push_back(new Bomb(*dynamic_cast<const Bomb*>(order)));
            } else if (dynamic_cast<const Blockade*>(order)) {
                orders.push_back(new Blockade(*dynamic_cast<const Blockade*>(order)));
            } else if (dynamic_cast<const Airlift*>(order)) {
                orders.push_back(new Airlift(*dynamic_cast<const Airlift*>(order)));
            } else if (dynamic_cast<const Negociate*>(order)) {
                orders.push_back(new Negociate(*dynamic_cast<const Negociate*>(order)));
            }
        }
    }
    return *this;
}

// Add Order Method
/**
 * @brief Adds an order to the list.
 * 
 * @details Adds the specified order to the `orders` list and sets the OrdersList as its subject for observers.
 * 
 * @param orderName The order to add to the list.
 */
void OrdersList::addOrder(Order* orderName) {
    orders.push_back(orderName);
    orderName->setOrdersList(this);
}

// Notify Method
/**
 * @brief Notifies all observers about a change.
 * 
 * @details Notifies all observers in the `observers` list about an update.
 */
void OrdersList::notify() {
    for (auto& observer : observers) {
        observer->update();
    }
}

// Add Observer Method
/**
 * @brief Adds an observer to the OrdersList.
 * 
 * @details Adds the given observer to the `observers` list and sets the subject of the observer to this OrdersList.
 * 
 * @param observer The observer to add to the list.
 */
void OrdersList::addObserver(Observer* observer) {
    observers.push_back(observer);
    observer->setSubject(this);  // Set the subject (OrdersList) for the observer
}

// Move Method
/**
 * @brief Executes the orders in the list, one by one.
 * 
 * @details Iterates through the orders, executing each order and logging the result. Allows the user to move through 
 * orders interactively by pressing 1 to move to the next order or 0 to stop.
 */
void OrdersList::move() {
    int next = 0;

    // Open gamelog.txt in write mode to clear its contents at the start
    ofstream logFile("gamelog.txt", ios::out);  
    if (!logFile) {
        cerr << "Error opening log file!" << endl;
        return;
    }

    if (orders.empty()) {
        cout << "Orders are empty." << endl;
        return;
    }

    do {
        Order* currentOrder = getCurrentOrder();
        if (currentOrder != nullptr) {
            cout << "Current order selected: " << *currentOrder << endl;
            cout << "Press 1 to see next order or 0 to exit: ";
            cin >> next;

            if (cin.fail() || (next != 0 && next != 1)) {
                cin.clear();
                cin.ignore(numeric_limits<streamsize>::max(), '\n');
                cout << "Invalid input, please enter 1 or 0." << endl;
                continue;
            }

            if (next == 0) {
                break;
            }

            currentOrder->execute();
            notify();  // Notify observers after executing each order
            logFile << "Executing order: " << *currentOrder << endl;  // Log the current order

            position = (position + 1) % orders.size();  // Advance position after notify
        } else {
            cout << "Invalid order at current position." << endl;
            break;
        }
    } while (next != 0);

    logFile.close();
}

// Remove Order Method
/**
 * @brief Removes an order from the list.
 * 
 * @details Searches for the specified order in the list, deletes it, and removes it from the `orders` list.
 * 
 * @param order The order to remove from the list.
 */

void OrdersList::remove(Order* order) {
    if (order == nullptr) {
        cout << "Error: Cannot remove a null order." << endl;
        return;
    }

    for (auto it = orders.begin(); it != orders.end(); ++it) {
        if (*it == order) {
            cout << "Deleting order: " << **it << endl;
            delete *it;
            orders.erase(it);
            return;
        }
    }
    cout << "Order not found." << endl;
}

// Get Current Order Method
/**
 * @brief Gets the current order.
 * 
 * @details Returns the order at the current position in the `orders` list.
 * 
 * @return The current order, or nullptr if the position is invalid.
 */ 
Order* OrdersList::getCurrentOrder() const {
    if (position >= 0 && position < orders.size()) {
        return orders[position];
    }
    return nullptr;
}

// String to Log Method
/**
 * @brief Converts the OrdersList to a loggable string format.
 * 
 * @return A string representation of the OrdersList for logging.
 */
string OrdersList::stringToLog() const {
    return "Logging OrdersList...";
}

// Check if There's a Deploy Order
/**
 * @brief Checks if the OrdersList contains a deploy order.
 * 
 * @details Searches the list for a deploy order and returns true if found, false otherwise.
 * 
 * @return true if there is a deploy order, false otherwise.
 */
bool OrdersList::hasDeployOrder() const {
    for (const Order* order : orders) {
        if (dynamic_cast<const Deploy*>(order)) {
            return true;
        }
    }
    return false;
}

// Get Next Deploy Order
/**
 * @brief Gets the next deploy order in the list.
 * 
 * @details Searches for the next deploy order, removes it from the list, and returns it.
 * 
 * @return The next deploy order, or nullptr if no deploy order exists.
 */
Order* OrdersList::getNextDeployOrder() {
    for (auto it = orders.begin(); it != orders.end(); ++it) {
        if (Deploy* deployOrder = dynamic_cast<Deploy*>(*it)) {
            orders.erase(it);
            return deployOrder;
        }
    }
    return nullptr;
}

// Get Next Order
/**
 * @brief Gets the next order from the list.
 * 
 * @details Removes and returns the next order in the list.
 * 
 * @return The next order, or nullptr if the list is empty.
 */
Order* OrdersList::getNextOrder() {
    if (!orders.empty()) {
        Order* nextOrder = orders.front();
        orders.erase(orders.begin());
        return nextOrder;
    }
    return nullptr;
}

// Check if OrdersList is Empty
/**
 * @brief Checks if the OrdersList is empty.
 * 
 * @return true if the list is empty, false otherwise.
 */
bool OrdersList::isEmpty() const {
    return orders.empty();
}