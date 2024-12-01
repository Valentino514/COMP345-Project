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

// Base Order Class
Order::Order() : orderName(nullptr) {}

//order destructor
Order::~Order() {
    delete orderName;  
}
//Order copy constructor
Order::Order(const Order& other) {
    orderName = new string(*other.orderName);
}
//order = operator
Order& Order::operator=(const Order& other) {
    if (this == &other) return *this; 
    delete orderName; 
    orderName = new string(*other.orderName); 
    return *this;
}
//order << operator
ostream& operator<<(ostream& os, const Order& order) {
    order.print(os);  
    return os;
}
//notify order fucntion
void Order::notify() {
    if (ordersList) {
        ordersList->notify();
    }
}

// Deploy Class
Deploy::Deploy() : Order(), targetTerritory(nullptr), reinforcementAmount(nullptr), player(nullptr) {}
//deploy constructor
Deploy::Deploy(Territory* target, int reinforcementAmount, Player* player)
    : Order(), targetTerritory(target), reinforcementAmount(new int(reinforcementAmount)), player(player) {}
//deploy copy constructor
Deploy::Deploy(const Deploy& other)
    : Order(other), targetTerritory(other.targetTerritory), reinforcementAmount(new int(*other.reinforcementAmount)) {}
//deploy == operator
Deploy& Deploy::operator=(const Deploy& other) {
    if (this == &other) return *this;
    Order::operator=(other);
    delete reinforcementAmount;
    reinforcementAmount = new int(*other.reinforcementAmount);
    targetTerritory = other.targetTerritory;
    return *this;
}
//deploy destuctor
Deploy::~Deploy() {
    delete reinforcementAmount;
}
// validate method for deploy order
bool Deploy::validate() {
    const vector<Territory*>* player_trt = player->getTerritories();
    //checks if players owns the territory
    bool isTerritoryOwned = any_of(player_trt->begin(), player_trt->end(), [this](Territory *t){return t == this->targetTerritory;});
    //bool haveEnoughUnits = 
    return isTerritoryOwned;// && haveEnoughUnits;
}
//execute method for deploy
void Deploy::execute() {
    if (validate()){

    int newArmyAmmount = (targetTerritory->getArmyAmount() + (*reinforcementAmount));
    targetTerritory->setArmyAmount(newArmyAmmount);

    }
    else
     cout<<"error deploying troops, player does not seem to own the territoy"<<"\n";

    
}
//print method for deploy
void Deploy::print(ostream& os) const {
    os << "deploy: " << *reinforcementAmount << " soldiers to " << targetTerritory->getName() << "\n";
}
//string to log method for deploy
string Deploy::stringToLog() const {
    string reinforcementStr = to_string(*reinforcementAmount);
    return "Deploy : " + reinforcementStr + " soldiers to " + targetTerritory->getName();
}

// Implementation of Advance member functions

// Default constructor
Advance::Advance()
    : Order(), isDestinationOwned(false), player(nullptr), sourceTerritory(nullptr), destinationTerritory(nullptr), reinforcementAmount(0) {}

// Parameterized constructor
Advance::Advance(Player* player, Territory* source, Territory* destination, int armyCount)
    : Order(), isDestinationOwned(false), player(player), sourceTerritory(source), destinationTerritory(destination), reinforcementAmount(armyCount) {}

// Copy constructor
Advance::Advance(const Advance& other)
    : Order(other),
      isDestinationOwned(other.isDestinationOwned),
      player(other.player),
      sourceTerritory(other.sourceTerritory),
      destinationTerritory(other.destinationTerritory),
      reinforcementAmount(other.reinforcementAmount) {}


// Assignment operator
// Assignment operator
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
//destructor
Advance::~Advance() {

}
//validate method for advance order
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

/// advance order execute method
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

//print method for advance order
void Advance::print(ostream& os) const {
    os << "advance: move " << reinforcementAmount << " armies from " << sourceTerritory->getName() << " to " << destinationTerritory->getName() << "\n";
}

string Advance::stringToLog() const {
    string reinforcementStr = to_string(reinforcementAmount);
    return "Advance Order: " + reinforcementStr + " armies from " + sourceTerritory->getName() + " to " + destinationTerritory->getName();
}

// Bomb Class
Bomb::Bomb() : Order(), targetTerritory(nullptr), player(nullptr) {}
//constructor for bomb class
Bomb::Bomb(Territory* target, Player* player) : Order(), targetTerritory(target), player(player) {}
//copy constrcutor for bomb class
Bomb::Bomb(const Bomb& other) : Order(other), targetTerritory(other.targetTerritory), player(other.player) {}
// operator = for bomb class
Bomb& Bomb::operator=(const Bomb& other) {
    if (this == &other) return *this;
    Order::operator=(other);
    targetTerritory = other.targetTerritory;
    player = other.player;
    return *this;
}
//desturcor for bomb class
Bomb::~Bomb() {}
//validate method for bomb class
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
//execute bomb order method
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
//print merhod for bomb class
void Bomb::print(ostream& os) const {
    os << "bomb: drop bombs on " << targetTerritory->getName() << "\n";
}
//string to log method for bomb
string Bomb::stringToLog() const {
    return "Bomb Order: drop bombs on " + targetTerritory->getName();
}

// Blockade Class
Blockade::Blockade() : Order(), targetTerritory(nullptr) {}
//blockade order constructor
Blockade::Blockade(Player* player, Territory* target)
    : Order(), targetTerritory(target) {
    issuingPlayer = player;
}
//copy constructor ffor blockade order
Blockade::Blockade(const Blockade& other) : Order(other), targetTerritory(other.targetTerritory) {}
//equals operator for blockade class
Blockade& Blockade::operator=(const Blockade& other) {
    if (this == &other) return *this;
    Order::operator=(other);
    targetTerritory = other.targetTerritory;
    return *this;
}
//destructor for blockade
Blockade::~Blockade() {}
//validate order for blockade
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

//method to ccreate or get the neutral player 
Player* Blockade::getOrCreateNeutralPlayer() {
    if (!neutralPlayer) {
        string neutralName = "Neutral";
        int neutralArmy = 0;  // Neutral player starts with no armies
        neutralPlayer = new Player(&neutralName, &neutralArmy);  // Dynamically allocate neutral player(make sure to deallocate at the end of the game )
        //cout << "Neutral player created.\n";
    }
    return neutralPlayer;
}
//exevute the blockade order method
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

//print method for blockade order
void Blockade::print(ostream& os) const {
    os << "blockade: on " << targetTerritory->getName() << "\n";
}
//string to log method for blockade
string Blockade::stringToLog() const {
    return "Blockade Order: blockade on " + targetTerritory->getName();
}

// Airlift Class
Airlift::Airlift() : Order(), sourceTerritory(nullptr), destinationTerritory(nullptr), reinforcementAmount(nullptr),player(nullptr){}
//airlift order constructor
Airlift::Airlift(Territory* source, Territory* destination, int armyCount, Player* player)
    : Order(), sourceTerritory(source), destinationTerritory(destination), reinforcementAmount(new int(armyCount)),player(player) {}
//airlift destructor
Airlift::~Airlift() {
    delete reinforcementAmount;
}
//validate method for the airlift method
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

// execute method for the airlift order 
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

//print method for airlift order
void Airlift::print(ostream& os) const {
    os << "airlift: move " << *reinforcementAmount << " armies from " << sourceTerritory->getName() << " to " << destinationTerritory->getName() << "\n";
}
//string to log method for airlift order
string Airlift::stringToLog() const {
    string reinforcementStr = to_string(*reinforcementAmount);
    return "Airlift Order: move " + reinforcementStr + " armies from " + sourceTerritory->getName() + " to " + destinationTerritory->getName();
}
//airlift order copy constructor
Airlift::Airlift(const Airlift& other)
    : Order(other),
      sourceTerritory(other.sourceTerritory),
      destinationTerritory(other.destinationTerritory),
      reinforcementAmount(new int(*other.reinforcementAmount)) {}



// Negociate Class
Negociate::Negociate() : Order(), player1(nullptr), player2(nullptr) {}
//negociate order constructor
Negociate::Negociate(Player* issuer, Player* targetPlayer)
    : Order(), player1(issuer), player2(targetPlayer) {
    issuingPlayer = issuer;
}
//negociate order copy constructor
Negociate::Negociate(const Negociate& other)
    : Order(other), player1(other.player1), player2(other.player2) {}
//equals operator for negociate method 
Negociate& Negociate::operator=(const Negociate& other) {
    if (this == &other) return *this;
    Order::operator=(other);
    player1 = other.player1;
    player2 = other.player2;
    return *this;
}
//destructor for negociate method
Negociate::~Negociate() {}
//validate method for negociate method
bool Negociate::validate() {
    if (player1 == player2) {
        cout << "Invalid negotiate order: Cannot negotiate with oneself.\n";
        return false;
    } else {
        cout << "Negotiate order is valid.\n";
        return true;
    }
}
//method to execute negociate order
void Negociate::execute() {
        if (player1 != player2) {
            // Adds the negotiation effect (truce) for the current turn
            player1->addNegotiatedPlayer(player2);  // Adds player2 to player1's negotiated list
            player2->addNegotiatedPlayer(player1);  // Adds player1 to player2's negotiated list
            cout << "Executing negotiation: " << *(player1->getName()) << " and " << *(player2->getName())
                      << " cannot attack each other this turn.\n";
        }
}
//print method for negociate order
void Negociate::print(ostream& os) const {
    os << "negotiate: between " << *(player1->getName()) << " and " << *(player2->getName()) << "\n";
}
//string to log method for negociate
string Negociate::stringToLog() const {
    return "Negociate Order: negotiate between " + *(player1->getName()) + " and " + *(player2->getName());
}
//orderlist default constructor
OrdersList::OrdersList() {}
//orderlist destructor
OrdersList::~OrdersList() {
    for (Order* order : orders) {
        delete order;
    }
    orders.clear();
}
//equals operate for orderlist
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
//add order method for orderlist
void OrdersList::addOrder(Order* orderName) {
    orders.push_back(orderName);
    orderName->setOrdersList(this);
}
//notify method for orderlist
void OrdersList::notify() {
    for (auto& observer : observers) {
        observer->update();
    }
}
//add a new observer for orderlist class
void OrdersList::addObserver(Observer* observer) {
    observers.push_back(observer);
    observer->setSubject(this);  // Set the subject (OrdersList) for the observer
}

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

//remove and order from orderlist method
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
//get the current order 
Order* OrdersList::getCurrentOrder() const {
    if (position >= 0 && position < orders.size()) {
        return orders[position];
    }
    return nullptr;
}
//string to log method
string OrdersList::stringToLog() const {
    return "Logging OrdersList...";
}
//check if thers a deploy order in orderlist
bool OrdersList::hasDeployOrder() const {
    for (const Order* order : orders) {
        if (dynamic_cast<const Deploy*>(order)) {
            return true;
        }
    }
    return false;
}
//get the next deploy order in orderlist
Order* OrdersList::getNextDeployOrder() {
    for (auto it = orders.begin(); it != orders.end(); ++it) {
        if (Deploy* deployOrder = dynamic_cast<Deploy*>(*it)) {
            orders.erase(it);
            return deployOrder;
        }
    }
    return nullptr;
}
//get next order from orderlist
Order* OrdersList::getNextOrder() {
    if (!orders.empty()) {
        Order* nextOrder = orders.front();
        orders.erase(orders.begin());
        return nextOrder;
    }
    return nullptr;
}
//check if orderlist is empty
bool OrdersList::isEmpty() const {
    return orders.empty();
}