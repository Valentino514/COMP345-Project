#include <algorithm>
#include "Orders.h"
#include "../Map/Map.h"
#include "../Player/Player.h"
#include <cstdlib> // for rand and srand
#include <ctime> // for time

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

// Implementation of Advance member functions

// Default constructor
Advance::Advance()
    : Order(), player(nullptr), sourceTerritory(nullptr), destinationTerritory(nullptr), reinforcementAmount(0) {}

// Parameterized constructor
Advance::Advance(Player* player, Territory* source, Territory* destination, int armyCount)
    : Order(), player(player), sourceTerritory(source), destinationTerritory(destination), reinforcementAmount(armyCount) {}

// Copy constructor
Advance::Advance(const Advance& other)
    : Order(other),
      player(other.player),  
      sourceTerritory(other.sourceTerritory),
      destinationTerritory(other.destinationTerritory),
      reinforcementAmount(other.reinforcementAmount),
      isDestinationOwned(other.isDestinationOwned) {} 


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

bool Advance::validate() { //check if source and destination location is valid
    cout<<"validating advance order\n";
    const vector<Territory*>* player_trt = player->getTerritories();
    const vector<Territory*> enemy_trt = player->toAttack();

    bool isSourceOwned = any_of(player_trt->begin(), player_trt->end(), [this](Territory *t){return t == this->sourceTerritory;});
    cout<<"does player own the source territory:"<<isSourceOwned<<'\n';
    this->isDestinationOwned =  any_of(player_trt->begin(), player_trt->end(), [this](Territory *t){return t == this->destinationTerritory;});
    cout<<"does player own the target territory:"<<this->isDestinationOwned<<'\n';
    bool isDestinationAdjacent = false;
    if(!(this->isDestinationOwned)){
        const vector<Territory*> enemy_trt = player->toAttack();
        isDestinationAdjacent = any_of(enemy_trt.begin(), enemy_trt.end(), [this](Territory *t){return t == this->destinationTerritory;});
        cout<<" is the territory adjacent to the player:"<<isDestinationAdjacent<<'\n';
    }
    return (isSourceOwned && (isDestinationAdjacent || this->isDestinationOwned));
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
                cout<<"player managed to elimate the enemy units\n";
                destinationTerritory->setLandOccupier(player); 
                destinationTerritory->setArmyAmount(attackerTroops); 
                player->addTerritory(destinationTerritory); 
                cout<<"updated player territories:\n";
                player->printOwnedTerritories();
                enemy->removeTerritory(destinationTerritory);
                cout<<"updated enemy territories:\n";
                enemy->printOwnedTerritories();
                if(player->canReceiveCard()){
                    Card* newCard = new Card(new Card::CardType(Card::getRandomCard().getType())); // Create a new random card
                    player->addCard(newCard); // Add the new card to the player's hand
                    player->cardReceived(); // Mark that the player has received a card this turn
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
    }
}


void Advance::print(ostream& os) const {
    os << "advance: move " << reinforcementAmount << " armies from " << sourceTerritory->getName() << " to " << destinationTerritory->getName() << "\n";
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
    cout<<"validating bomb order";
    vector<Territory*> enemy_trt = player->toAttack();
    bool isTargetAdjacent = any_of(enemy_trt.begin(), enemy_trt.end(), [this](Territory *t){return t == this->targetTerritory;});

    return (isTargetAdjacent && player->hasCard(Card::Bomb));
}

void Bomb::execute() {
    if (validate()){
        cout<<"bomb order validated, current army ammount in target territory:"<<targetTerritory->getArmyAmount()<<'\n';
        player->removeCard(new Card(new Card::CardType(Card::Bomb))); // remove his card
        int newArmyAmmount = (targetTerritory->getArmyAmount())/2; 
        targetTerritory->setArmyAmount(newArmyAmmount);
        cout<<"new army ammount in target territory:"<<targetTerritory->getArmyAmount()<<'\n';

    }
}

void Bomb::print(ostream& os) const {
    os << "bomb: drop bombs on " << targetTerritory->getName() << "\n";
}

// Blockade Class
Blockade::Blockade() : Order(), targetTerritory(nullptr) {}

Blockade::Blockade(Territory* target) : Order(), targetTerritory(target) {}

Blockade::Blockade(const Blockade& other) : Order(other), targetTerritory(other.targetTerritory) {}

Blockade& Blockade::operator=(const Blockade& other) {
    if (this == &other) return *this;
    Order::operator=(other);
    targetTerritory = other.targetTerritory;
    return *this;
}

Blockade::~Blockade() {}

bool Blockade::validate() {
    cout << "Validating blockade order\n";
    return true;
}

void Blockade::execute() {
    validate();
    cout << "Executing blockade on " << targetTerritory->getName() << "\n";
}

void Blockade::print(ostream& os) const {
    os << "blockade: on " << targetTerritory->getName() << "\n";
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
    player->removeCard(new Card(new Card::CardType(Card::Airlift))); //looks bad because we are passing by pointer
    int newSourceArmy = (sourceTerritory->getArmyAmount() - *reinforcementAmount);
    sourceTerritory->setArmyAmount(newSourceArmy);
    int newDestinationArmy =  (destinationTerritory->getArmyAmount() + *reinforcementAmount);
    destinationTerritory->setArmyAmount(newDestinationArmy);
    cout<<" new army ammount in source territory: "<<sourceTerritory->getArmyAmount();
    cout<<"\nnew army ammount in target territory: "<<destinationTerritory->getArmyAmount()<<endl;
    }
}

void Airlift::print(ostream& os) const {
    os << "airlift: move " << *reinforcementAmount << " armies from " << sourceTerritory->getName() << " to " << destinationTerritory->getName() << "\n";
}

Airlift::Airlift(const Airlift& other)
    : Order(other),
      sourceTerritory(other.sourceTerritory),
      destinationTerritory(other.destinationTerritory),
      reinforcementAmount(new int(*other.reinforcementAmount)) {}

// Negotiate Class
Negociate::Negociate() : Order(), player1(nullptr), player2(nullptr) {}

Negociate::Negociate(Player* p1, Player* p2) : Order(), player1(p1), player2(p2) {}

Negociate::~Negociate() {}

Negociate::Negociate(const Negociate& other)
    : Order(other),
      player1(other.player1),
      player2(other.player2) {}

bool Negociate::validate() {
    cout << "Validating negotiate order\n";
    return true;
}

void Negociate::execute() {
    validate();
    cout << "Executing negotiation between " << player1->getName() << " and " << player2->getName() << "\n";
    
}

void Negociate::print(ostream& os) const {
    os << "negotiate: between " << player1->getName() << " and " << player2->getName() << "\n";
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
}

void OrdersList::move() {
    int position = 0;
    int next;

    if (orders.empty()) {
        cout << "Orders are empty" << endl;
        return;
    }

    cout << "Current order selected: " << *orders[position] << endl;
    cout << "Press any number to see next order or 0 to exit: ";
    cin >> next;

    while (next != 0) {
        position++;

        if (position >= orders.size()) {
            position = 0;
        }

        cout << "Order selected: " << *orders[position] << endl;
        cout << "Press 1 to see next order or 0 to exit: ";
        cin >> next;
    }
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