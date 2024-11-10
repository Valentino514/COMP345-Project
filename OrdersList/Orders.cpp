#include <algorithm>
#include <cstdlib> // for random
#include <ctime> // for random
#include <iostream>

#include "Orders.h"
#include "../Map/Map.h"
#include "../Player/Player.h"

using namespace std;

// Base Order Class
Order::Order() : orderName(nullptr) {}

Order::~Order() {
    delete orderName;  
}

Order::Order(const Order& other) {
    orderName = new string(*other.orderName);
}

Order& Order::operator=(const Order& other) {
    if (this == &other) return *this; 
    delete orderName; 
    orderName = new string(*other.orderName); 
    return *this;
}

ostream& operator<<(ostream& os, const Order& order) {
    order.print(os);  
    return os;
}

void Order::notify() {
    if (ordersList) {
        ordersList->notify();
    }
}

// Deploy Class
Deploy::Deploy() : Order(), targetTerritory(nullptr), reinforcementAmount(nullptr), player(nullptr) {}

Deploy::Deploy(Territory* target, int reinforcementAmount, Player* player)
    : Order(), targetTerritory(target), reinforcementAmount(new int(reinforcementAmount)), player(player) {}

Deploy::Deploy(const Deploy& other)
    : Order(other), targetTerritory(other.targetTerritory), reinforcementAmount(new int(*other.reinforcementAmount)) {}

Deploy& Deploy::operator=(const Deploy& other) {
    if (this == &other) return *this;
    Order::operator=(other);
    delete reinforcementAmount;
    reinforcementAmount = new int(*other.reinforcementAmount);
    targetTerritory = other.targetTerritory;
    return *this;
}

Deploy::~Deploy() {
    delete reinforcementAmount;
}

bool Deploy::validate() {
    cout<<"validating deploy order\n";
    const vector<Territory*>* player_trt = player->getTerritories();
    //checks if players owns the territory
    bool isTerritoryOwned = any_of(player_trt->begin(), player_trt->end(), [this](Territory *t){return t == this->targetTerritory;});
    //bool haveEnoughUnits = 
    return isTerritoryOwned;// && haveEnoughUnits;
}

void Deploy::execute() {
    if (validate()){
    cout<<"validation successful, player owns that territory. updating target territory army count\nold count:"<<targetTerritory->getArmyAmount()<<"\n";
    int* newArmyAmmount = (targetTerritory->getArmyAmount() + reinforcementAmount);
    targetTerritory->setArmyAmount(*newArmyAmmount);
    cout<<"new count:"<<targetTerritory->getArmyAmount()<<"\n";

    }
    else
     cout<<"error deploying troops, player does not seem to own the territoy"<<"\n";

    
}

void Deploy::print(ostream& os) const {
    os << "deploy: " << *reinforcementAmount << " soldiers to " << targetTerritory->getName() << "\n";
}

string Deploy::stringToLog() const {
    std::string reinforcementStr = std::to_string(*reinforcementAmount);
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
Advance::~Advance() {

}

bool Advance::validate() {//check if source and destination location is valid
    cout << "validating advance order\n";
    const vector<Territory*>* player_trt = player->getTerritories();
    const vector<Territory*>* adjacentTerritories = sourceTerritory->getAdjacentTerritories();

    bool isSourceOwned = any_of(player_trt->begin(), player_trt->end(), [this](Territory* t) { return t == this->sourceTerritory; });
    cout << "does player own the source territory: " << isSourceOwned << '\n';
    this->isDestinationOwned = any_of(player_trt->begin(), player_trt->end(), [this](Territory* t) { return t == this->destinationTerritory; });
    cout << "does player own the target territory: " << this->isDestinationOwned << '\n';
    bool isDestinationAdjacent = any_of(adjacentTerritories->begin(), adjacentTerritories->end(), [this](Territory* t) { return t == this->destinationTerritory; });
    cout << "is territory to advance adjacent?: " << isDestinationAdjacent << endl;

    // Check if the destination territory belongs to a player with whom a truce has been negotiated
    if (!isDestinationOwned) {
        Player* territoryOwner = destinationTerritory->getLandOccupier();
        if (player->isNegotiatedWith(territoryOwner)) {
            cout << "Advance order validation failed: Truce with " << *(territoryOwner->getName()) << " prevents the attack.\n";
            return false;
        }
    }

    return (isSourceOwned && isDestinationAdjacent);
}


void Advance::execute() {
    if (validate()){
        cout<<"advance order validated, will the user be moving troops to a destination he owns?: "<<isDestinationOwned<<"\n";
            if(isDestinationOwned){
                int* newDestinationArmy = new int((sourceTerritory->getArmyAmount() - reinforcementAmount));
                sourceTerritory->setArmyAmount(*newDestinationArmy);
                delete newDestinationArmy;
                newDestinationArmy = nullptr;
                int* newTargetArmy = new int((destinationTerritory->getArmyAmount() + reinforcementAmount));
                destinationTerritory->setArmyAmount(*newTargetArmy);
                delete newTargetArmy;
                newTargetArmy = nullptr;
        }
             else {
            // If player is attacking another player
            Player* enemy = destinationTerritory->getLandOccupier();
            int enemyTroops = destinationTerritory->getArmyAmount();
            int attackerTroops = reinforcementAmount;

            srand(static_cast<unsigned>(time(0))); // Seed for random number generator

            while (attackerTroops > 0 && enemyTroops > 0) {
                // Each attacking unit has a 60% chance of killing a defending unit
                for (int i = 0; i < attackerTroops && enemyTroops > 0; ++i) {
                    if (rand() % 100 < 60) { // 60% chance
                        enemyTroops--;
                    }
                }

                // Each defending unit has a 70% chance of killing an attacking unit
                for (int i = 0; i < enemyTroops && attackerTroops > 0; ++i) {
                    if (rand() % 100 < 70) { // 70% chance
                        attackerTroops--;
                    }
                }
            }


            // If all defending units are eliminated, the attacker captures the territory
            if (enemyTroops == 0) {
                cout<<"player "<<*(player->getName())<<" managed to elimate the enemy units\n";
                destinationTerritory->setLandOccupier(player); 
                destinationTerritory->setArmyAmount(attackerTroops); 
                player->addTerritory(destinationTerritory); 
                //cout<<"updated player territories:\n";
                //player->printOwnedTerritories();
                enemy->removeTerritory(destinationTerritory);
               // cout<<"updated enemy territories:\n";
                //enemy->printOwnedTerritories();
                if(player->canReceiveCard()){
                    Card* newCard = new Card(new Card::CardType(Card::getRandomCard().getType())); // Create a new random card
                    cout<<"player received a new card "<<newCard->getCardTypeName()<<endl;
                    player->addCard(newCard); // Add the new card to the player's hand
                    player->cardReceived(); // Mark that the player has received a card this turn
                }else{
                    cout<<"player will already received a card for this turn"<<endl;
                }
                    
            } else {
                cout<<"player failed to elimate the enemy units\n";
                // Update defending troops if the defender held the territory
                destinationTerritory->setArmyAmount(enemyTroops);
                cout<<"enemy has "<<destinationTerritory->getArmyAmount()<<" remaining \n";

            }

            // Update the source territory's army count
            int remainingSourceArmy = sourceTerritory->getArmyAmount() - reinforcementAmount;
            sourceTerritory->setArmyAmount(remainingSourceArmy);
            cout<<"player has "<<remainingSourceArmy<<" remaining in the territory he attacked from \n";

        }
    }else{
        cout<<"validation for advance order failed"<<endl;
    }
}


void Advance::print(ostream& os) const {
    os << "advance: move " << reinforcementAmount << " armies from " << sourceTerritory->getName() << " to " << destinationTerritory->getName() << "\n";
}

string Advance::stringToLog() const {
    std::string reinforcementStr = std::to_string(reinforcementAmount);
    return "Advance Order: " + reinforcementStr + " armies from " + sourceTerritory->getName() + " to " + destinationTerritory->getName();
}

// Bomb Class
Bomb::Bomb() : Order(), targetTerritory(nullptr), player(nullptr) {}

Bomb::Bomb(Territory* target, Player* player) : Order(), targetTerritory(target), player(player) {}

Bomb::Bomb(const Bomb& other) : Order(other), targetTerritory(other.targetTerritory), player(other.player) {}

Bomb& Bomb::operator=(const Bomb& other) {
    if (this == &other) return *this;
    Order::operator=(other);
    targetTerritory = other.targetTerritory;
    player = other.player;
    return *this;
}

Bomb::~Bomb() {}

bool Bomb::validate() {
    cout << "validating bomb order\n";
    vector<Territory*> enemy_trt = player->toAttack();
    bool isTargetAdjacent = any_of(enemy_trt.begin(), enemy_trt.end(), [this](Territory* t) { return t == this->targetTerritory; });

    // Check if there’s a truce with the player who owns the target territory
    Player* territoryOwner = targetTerritory->getLandOccupier();
    if (player->isNegotiatedWith(territoryOwner)) {
        cout << "Bomb order validation failed: Truce with " << *(territoryOwner->getName()) << " prevents the bombing.\n";
        return false;
    }

    return (isTargetAdjacent && player->hasCard(Card::Bomb));
}

void Bomb::execute() {
    if (validate()){
        cout<<"bomb order validated, current army ammount in target territory:"<<targetTerritory->getArmyAmount()<<'\n';
        player->removeCard(new Card(new Card::CardType(Card::Bomb)));
        int newArmyAmmount = (targetTerritory->getArmyAmount())/2; 
        targetTerritory->setArmyAmount(newArmyAmmount);
        cout<<"new army ammount in target territory:"<<targetTerritory->getArmyAmount()<<'\n';

    }else{
        cout<<"bomb validation failed"<<endl;
        return;
    }
    
}

void Bomb::print(ostream& os) const {
    os << "bomb: drop bombs on " << targetTerritory->getName() << "\n";
}

string Bomb::stringToLog() const {
    return "Bomb Order: drop bombs on " + targetTerritory->getName();
}

// Blockade Class
Blockade::Blockade() : Order(), targetTerritory(nullptr) {}

Blockade::Blockade(Player* player, Territory* target)
    : Order(), targetTerritory(target) {
    issuingPlayer = player;
}

Blockade::Blockade(const Blockade& other) : Order(other), targetTerritory(other.targetTerritory) {}

Blockade& Blockade::operator=(const Blockade& other) {
    if (this == &other) return *this;
    Order::operator=(other);
    targetTerritory = other.targetTerritory;
    return *this;
}

Blockade::~Blockade() {}

bool Blockade::validate() {
    if (targetTerritory->getLandOccupier() != issuingPlayer) {
        cout << "Invalid blockade order: Target territory does not belong to issuing player.\n";
        return false;
    } else {
        cout << "Blockade order is valid.\n";
        return true;
    }
}

// Static variable to hold the neutral player instance
 Player* Blockade::neutralPlayer = nullptr;


Player* Blockade::getOrCreateNeutralPlayer() {
    if (!neutralPlayer) {
        string neutralName = "Neutral";
        int neutralArmy = 0;  // Neutral player starts with no armies
        neutralPlayer = new Player(&neutralName, &neutralArmy);  // Dynamically allocate neutral player(make sure to deallocate at the end of the game )
        cout << "Neutral player created.\n";
    }
    return neutralPlayer;
}

void Blockade::execute() {
    if(validate()){
    if (targetTerritory->getLandOccupier() == issuingPlayer) {
        // Double the army units on the target territory
        targetTerritory->setArmyAmount(targetTerritory->getArmyAmount() * 2);
        
        // Transfer ownership to the neutral player
        Player* neutral = getOrCreateNeutralPlayer();
        targetTerritory->setLandOccupier(neutral);
        
        cout << "Executing blockade: Army units doubled, territory ownership transferred to Neutral player.\n";
    }
}else{
    cout<<"error validating order blockade";
}
}


void Blockade::print(ostream& os) const {
    os << "blockade: on " << targetTerritory->getName() << "\n";
}

string Blockade::stringToLog() const {
    return "Blockade Order: blockade on " + targetTerritory->getName();
}

// Airlift Class
Airlift::Airlift() : Order(), sourceTerritory(nullptr), destinationTerritory(nullptr), reinforcementAmount(nullptr),player(nullptr){}

Airlift::Airlift(Territory* source, Territory* destination, int armyCount, Player* player)
    : Order(), sourceTerritory(source), destinationTerritory(destination), reinforcementAmount(new int(armyCount)),player(player) {}

Airlift::~Airlift() {
    delete reinforcementAmount;
}

bool Airlift::validate() {  
    cout<<"validating airlift order\n";
    const vector<Territory*>* player_trt = player->getTerritories();
    bool isTargetOwned = any_of(player_trt->begin(), player_trt->end(), [this](Territory *t){return t == this->destinationTerritory;});    
    bool isSourceOwned = any_of(player_trt->begin(), player_trt->end(), [this](Territory *t){return t == this->sourceTerritory;});
    return (isSourceOwned && isTargetOwned && player->hasCard(Card::Airlift));
}

void Airlift::execute() {
    if(validate()){
    cout<<"airlift order validated, current army ammount in source territory: "<<sourceTerritory->getArmyAmount();
    cout<<"\ncurrent army ammount in target territory: "<<destinationTerritory->getArmyAmount()<<endl;
    cout<<"deploying "<<reinforcementAmount<<" units"<<endl;
    player->removeCard(new Card(new Card::CardType(Card::Airlift))); //looks bad because we are passing by pointer
    int newSourceArmy = (sourceTerritory->getArmyAmount() - *reinforcementAmount);
    sourceTerritory->setArmyAmount(newSourceArmy);
    int newDestinationArmy =  (destinationTerritory->getArmyAmount() + *reinforcementAmount);
    destinationTerritory->setArmyAmount(newDestinationArmy);
    cout<<" new army ammount in source territory: "<<sourceTerritory->getArmyAmount();
    cout<<"\nnew army ammount in target territory: "<<destinationTerritory->getArmyAmount()<<endl;
    }else{
        cout<<"airlift order failed"<<endl;
    }
}

void Airlift::print(ostream& os) const {
    os << "airlift: move " << *reinforcementAmount << " armies from " << sourceTerritory->getName() << " to " << destinationTerritory->getName() << "\n";
}

string Airlift::stringToLog() const {
    std::string reinforcementStr = std::to_string(*reinforcementAmount);
    return "Airlift Order: move " + reinforcementStr + " armies from " + sourceTerritory->getName() + " to " + destinationTerritory->getName();
}

Airlift::Airlift(const Airlift& other)
    : Order(other),
      sourceTerritory(other.sourceTerritory),
      destinationTerritory(other.destinationTerritory),
      reinforcementAmount(new int(*other.reinforcementAmount)) {}



// Negociate Class
Negociate::Negociate() : Order(), player1(nullptr), player2(nullptr) {}

Negociate::Negociate(Player* issuer, Player* targetPlayer)
    : Order(), player1(issuer), player2(targetPlayer) {
    issuingPlayer = issuer;
}

Negociate::Negociate(const Negociate& other)
    : Order(other), player1(other.player1), player2(other.player2) {}

Negociate& Negociate::operator=(const Negociate& other) {
    if (this == &other) return *this;
    Order::operator=(other);
    player1 = other.player1;
    player2 = other.player2;
    return *this;
}

Negociate::~Negociate() {}

bool Negociate::validate() {
    if (player1 == player2) {
        cout << "Invalid negotiate order: Cannot negotiate with oneself.\n";
        return false;
    } else {
        cout << "Negotiate order is valid.\n";
        return true;
    }
}

void Negociate::execute() {
    if(validate()){
    if (player1 != player2) {
        // Adds the negotiation effect (truce) for the current turn
        player1->addNegotiatedPlayer(player2);  // Assuming this will add player2 to a truce list for player1
        player2->addNegotiatedPlayer(player1);  // Assuming this will add player1 to a truce list for player2
        cout << "Executing negotiation: " << *(player1->getName()) << " and " << *(player2->getName())
                  << " cannot attack each other this turn.\n";
    }
} else{
    cout<<"error validating order";
}
}

void Negociate::print(ostream& os) const {
    os << "negotiate: between " << *(player1->getName()) << " and " << *(player2->getName()) << "\n";
}

string Negociate::stringToLog() const {
    return "Negociate Order: negotiate between " + *(player1->getName()) + " and " + *(player2->getName());
}

OrdersList::OrdersList() {}

OrdersList::~OrdersList() {
    for (Order* order : orders) {
        delete order;
    }
    orders.clear();
}

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
void OrdersList::addOrder(Order* orderName) {
    orders.push_back(orderName);
    orderName->setOrdersList(this);
}

void OrdersList::notify() {
    for (auto& observer : observers) {
        observer->update();
    }
}

void OrdersList::addObserver(Observer* observer) {
    observers.push_back(observer);
    observer->setSubject(this);  // Set the subject (OrdersList) for the observer
}

void OrdersList::move() {
    int position = 0;
    int next;

    std::ofstream logFile("gamelog.txt", std::ios::app);
    if (!logFile) {
        std::cerr << "Error opening log file!" << std::endl;
        return;
    }

    if (orders.empty()) {
        std::cout << "Orders are empty" << std::endl;
        return;
    }

    // Display the initial order
    std::cout << "Current order selected: " << *orders[position] << std::endl;
    std::cout << "Press any number to see next order or 0 to exit: ";
    std::cin >> next;

    // Process orders as long as the user wants to continue
    while (next != 0) {
        // Execute the current order, notify observers, and log the action
        orders[position]->execute();
        notify();  // Notify after executing the order
        logFile << "Executing order: " << *orders[position] << std::endl;

        // Move to the next order
        position++;
        if (position >= orders.size()) {
            position = 0;  // Loop back to the start if we reach the end
        }

        // Display the next order and ask for user input again
        std::cout << "Order selected: " << *orders[position] << std::endl;
        std::cout << "Press 1 to see next order or 0 to exit: ";
        std::cin >> next;
    }

    logFile.close();
}

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

Order* OrdersList::getCurrentOrder() const {
    if (position >= 0 && position < orders.size()) {
        return orders[position];
    }
    return nullptr;
}

std::string OrdersList::stringToLog() const {
    return "Logging OrdersList...";
}

bool OrdersList::hasDeployOrder() const {
    for (const Order* order : orders) {
        if (dynamic_cast<const Deploy*>(order)) {
            return true;
        }
    }
    return false;
}

Order* OrdersList::getNextDeployOrder() {
    for (auto it = orders.begin(); it != orders.end(); ++it) {
        if (Deploy* deployOrder = dynamic_cast<Deploy*>(*it)) {
            orders.erase(it);
            return deployOrder;
        }
    }
    return nullptr;
}

Order* OrdersList::getNextOrder() {
    if (!orders.empty()) {
        Order* nextOrder = orders.front();
        orders.erase(orders.begin());
        return nextOrder;
    }
    return nullptr;
}

bool OrdersList::isEmpty() const {
    return orders.empty();
}
