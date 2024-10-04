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
    os << "create a blockade on selected territory\n";
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
    os << "airlifts selected soldiers to selected territory\n";
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
    os << "create a negotiation contract with selected territory\n";
}

//OrdersList Class
OrdersList::OrdersList() {}

OrdersList::~OrdersList() {
    for (Order* order : orders) {
        delete order;  //delete orderlists
    }
    orders.clear();  //clear list
}

void OrdersList::addOrder(Order* orderName) {
    orders.push_back(orderName); //add new order to list
}

void OrdersList::move() {
    int position = 0;
    int next;

    if (orders.empty()) {
        cout << "Orders are empty" << endl;
        return;
    }

    // Display first order
    cout << "Current order selected: " << *orders[position] << endl;

    cout << "Press 1 to see next order or 0 to exit: ";
    cin >> next;

    while (next != 0) {
        position++; 

        // Check the bounds
        if (position >= orders.size()) {
            position = 0;  // Loop back to the beginning if the end is reached
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
            delete *it;  // Properly delete the order
            orders.erase(it);  // Remove from the list
            return;
        }
    }
    cout << "Order not found." << endl;
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
