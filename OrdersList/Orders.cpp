#include "Orders.h"
#include "../Map/Map.h"
#include "../Player/Player.h"
#include <iostream>



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
Deploy::Deploy() : Order(), targetTerritory(nullptr), numArmies(nullptr) {}

Deploy::Deploy(Player* player, Territory* target, int reinforcementAmount)
    : Order(), targetTerritory(target), numArmies(new int(reinforcementAmount)) {
    issuingPlayer = player;
}

Deploy::Deploy(Territory* target, int reinforcementAmount)
    : Order(), targetTerritory(target), numArmies(new int(reinforcementAmount)) {}

Deploy::Deploy(const Deploy& other)
    : Order(other), targetTerritory(other.targetTerritory), numArmies(new int(*other.numArmies)) {}

Deploy& Deploy::operator=(const Deploy& other) {
    if (this == &other) return *this;
    Order::operator=(other);
    delete numArmies;
    numArmies = new int(*other.numArmies);
    targetTerritory = other.targetTerritory;
    return *this;
}

Deploy::~Deploy() {
    delete numArmies;
}

void Deploy::validate() {
    cout << "Validating deploy order\n";
}

void Deploy::execute() {
    validate();
    cout << "Executing deploy order: " << *numArmies << " armies deployed to " << targetTerritory->getName() << "\n";
}

void Deploy::print(ostream& os) const {
    os << "deploy: " << *numArmies << " soldiers to " << targetTerritory->getName() << "\n";
}

// Advance Class
Advance::Advance() : Order(), sourceTerritory(nullptr), destinationTerritory(nullptr), numArmies(nullptr) {}

Advance::Advance(Player* player, Territory* source, Territory* destination, int armyCount)
    : Order(), sourceTerritory(source), destinationTerritory(destination), numArmies(new int(armyCount)) {
    issuingPlayer = player;
}
Advance::Advance(Territory* source, Territory* destination, int armyCount)
    : Order(), sourceTerritory(source), destinationTerritory(destination), numArmies(new int(armyCount)) {}

Advance::Advance(const Advance& other)
    : Order(other), sourceTerritory(other.sourceTerritory), destinationTerritory(other.destinationTerritory), numArmies(new int(*other.numArmies)) {}

Advance& Advance::operator=(const Advance& other) {
    if (this == &other) return *this;
    Order::operator=(other);
    delete numArmies;
    numArmies = new int(*other.numArmies);
    sourceTerritory = other.sourceTerritory;
    destinationTerritory = other.destinationTerritory;
    return *this;
}

Advance::~Advance() {
    delete numArmies;
}

void Advance::validate() {
    cout << "Validating advance order\n";
}

void Advance::execute() {
    validate();
    cout << "Executing advance order: " << *numArmies << " armies from " << sourceTerritory->getName() << " to " << destinationTerritory->getName() << "\n";
}

void Advance::print(ostream& os) const {
    os << "advance: move " << *numArmies << " armies from " << sourceTerritory->getName() << " to " << destinationTerritory->getName() << "\n";
}

// Bomb Class
Bomb::Bomb() : Order(), targetTerritory(nullptr) {}

Bomb::Bomb(Player* player, Territory* target)
    : Order(), targetTerritory(target) {
    issuingPlayer = player;
}

Bomb::Bomb(const Bomb& other) : Order(other), targetTerritory(other.targetTerritory) {}

Bomb& Bomb::operator=(const Bomb& other) {
    if (this == &other) return *this;
    Order::operator=(other);
    targetTerritory = other.targetTerritory;
    return *this;
}

Bomb::~Bomb() {}

void Bomb::validate() {
    cout << "Validating bomb order\n";
}

void Bomb::execute() {
    validate();
    cout << "Executing bomb order on " << targetTerritory->getName() << "\n";
}

void Bomb::print(ostream& os) const {
    os << "bomb: drop bombs on " << targetTerritory->getName() << "\n";
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

void Blockade::validate() {
    cout << "Validating blockade order\n";
}

void Blockade::execute() {
    validate();
    cout << "Executing blockade on " << targetTerritory->getName() << "\n";
}

void Blockade::print(ostream& os) const {
    os << "blockade: on " << targetTerritory->getName() << "\n";
}

// Airlift Class
Airlift::Airlift() : Order(), sourceTerritory(nullptr), destinationTerritory(nullptr), numArmies(nullptr) {}

Airlift::Airlift(Player* player, Territory* source, Territory* destination, int armyCount)
    : Order(), sourceTerritory(source), destinationTerritory(destination), numArmies(new int(armyCount)) {
    issuingPlayer = player;
}

Airlift::~Airlift() {
    delete numArmies;
}

void Airlift::validate() {
    cout << "Validating airlift order\n";
}

void Airlift::execute() {
    validate();
    cout << "Executing airlift of " << *numArmies << " armies from " << sourceTerritory->getName() << " to " << destinationTerritory->getName() << "\n";
}

void Airlift::print(ostream& os) const {
    os << "airlift: move " << *numArmies << " armies from " << sourceTerritory->getName() << " to " << destinationTerritory->getName() << "\n";
}

Airlift::Airlift(const Airlift& other)
    : Order(other),
      sourceTerritory(other.sourceTerritory),
      destinationTerritory(other.destinationTerritory),
      numArmies(new int(*other.numArmies)) {}

// Negotiate Class
Negociate::Negociate() : Order(), player1(nullptr), player2(nullptr) {}

Negociate::Negociate(Player* issuer, Player* targetPlayer)
    : Order(), player1(issuer), player2(targetPlayer) {
    issuingPlayer = issuer;
}

Negociate::~Negociate() {}

Negociate::Negociate(const Negociate& other)
    : Order(other),
      player1(other.player1),
      player2(other.player2) {}

void Negociate::validate() {
    cout << "Validating negotiate order\n";
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