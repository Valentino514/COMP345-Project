#include "Orders.h"
#include <iostream>

//Base Class
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

void Order::notify() {
    if (ordersList) {
        ordersList->notify();
    }
}

ostream& operator<<(ostream& os, const Order& order) {
    order.print(os);  
    return os;
}

//Deploy Class
Deploy::Deploy() : Order() {}

Deploy::Deploy(const Deploy& other) : Order(other) {}

Deploy& Deploy::operator=(const Deploy& other) {
    if (this == &other) return *this;
    Order::operator=(other);  // Use base class assignment
    return *this;
}

void Deploy::validate() {
    cout << "Validating deploy order\n";
}

void Deploy::execute() {
    validate();
    cout << "Executing deploy order\n";
}

void Deploy::print(ostream& os) const {
    os << "deploy: new soldiers are deployed on selected territory\n";
}

string Deploy::stringToLog() const {
    return "Deploy Order: " + *orderName;
}

//Advance Class
Advance::Advance() : Order() {}

Advance& Advance::operator=(const Advance& other) {
    if (this == &other) return *this;
    Order::operator=(other);
    return *this;
}

void Advance::validate() {
    cout << "Validating advance order\n";
}

void Advance::execute() {
    validate();
    cout << "Executing advance order\n";
}

void Advance::print(ostream& os) const {
    os << "advance: selected soldiers move into selected territory\n";
}

string Advance::stringToLog() const {
    return "Advance Order: " + *orderName;
}

//Bomb Class
Bomb::Bomb() : Order() {}

Bomb& Bomb::operator=(const Bomb& other) {
    if (this == &other) return *this;
    Order::operator=(other);
    return *this;
}

void Bomb::validate() {
    cout << "Validating bomb order\n";
}

void Bomb::execute() {
    validate();
    cout << "Executing bomb order\n";
}

void Bomb::print(ostream& os) const {
    os << "bomb: drop bombs on selected territory\n";
}

string Bomb::stringToLog() const {
    return "Bomb Order: " + *orderName;
}

//Blockade Class
Blockade::Blockade() : Order() {}

Blockade& Blockade::operator=(const Blockade& other) {
    if (this == &other) return *this;
    Order::operator=(other);
    return *this;
}

void Blockade::validate() {
    cout << "Validating blockade order\n";
}

void Blockade::execute() {
    validate();
    cout << "Executing blockade order\n";
}

void Blockade::print(ostream& os) const {
    os << "blockade: create a blockade on selected territory\n";
}

string Blockade::stringToLog() const {
    return "Blockade Order: " + *orderName;
}

//Airlift Class
Airlift::Airlift() : Order() {}

Airlift& Airlift::operator=(const Airlift& other) {
    if (this == &other) return *this;
    Order::operator=(other);
    return *this;
}

void Airlift::validate() {
    cout << "Validating airlift order\n";
}

void Airlift::execute() {
    validate();
    cout << "Executing airlift order\n";
}

void Airlift::print(ostream& os) const {
    os << "airlift: airlifts selected soldiers to selected territory\n";
}

string Airlift::stringToLog() const {
    return "Airlift Order: " + *orderName;
}

//Negociate Class
Negociate::Negociate() : Order() {}

Negociate& Negociate::operator=(const Negociate& other) {
    if (this == &other) return *this;
    Order::operator=(other);
    return *this;
}

void Negociate::validate() {
    cout << "Validating negotiate order\n";
}

void Negociate::execute() {
    validate();
    cout << "Executing negotiate order\n";
}

void Negociate::print(ostream& os) const {
    os << "negotiate: create a negotiation contract with selected territory\n";
}

string Negociate::stringToLog() const {
    return "Negociate Order: " + *orderName;
}

//OrdersList Class

OrdersList::~OrdersList() {
    for (Order* order : orders) {
        delete order;  //delete orderlists
    }
    orders.clear();  //clear list
}

void OrdersList::addOrder(Order* orderName) {
    orders.push_back(orderName); //add new order to list
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
    position = 0;  // Start from the first order

    std::ofstream logFile("gamelog.txt", std::ios::app);
    if (!logFile) {
        std::cerr << "Error opening log file!" << std::endl;
        return;
    }

    if (orders.empty()) {
        std::cout << "Orders are empty" << std::endl;
        return;
    }

    std::cout << "Executing orders..." << std::endl;
    while (position < orders.size()) {
        std::cout << "Current order selected: " << *orders[position] << std::endl;
        orders[position]->execute();
        notify();  // Notify observers with the updated position

        logFile << "Executing order: " << *orders[position] << std::endl;

        int next;
        std::cout << "Press 1 to see next order or 0 to exit: ";
        std::cin >> next;
        if (next == 0) break;

        ++position;  // Move to the next order
        if (position >= orders.size()) {
            std::cout << "All orders have been processed!" << std::endl;
            break;
        }
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
            delete *it;  // Properly delete the order
            orders.erase(it);  // Remove from the list
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

OrdersList& OrdersList::operator=(const OrdersList& other) {
    if (this != &other) {
        for (Order* order : orders) {
            delete order;
        }
        orders.clear();

        //copying them all
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
