#include "Orders.h"

Order::Order() {

}

Order::~Order() {
}

Order::Order(const Order& other) {
    
}

ostream& operator<<(ostream& os, const Order& order) {
    order.print(os);
    return os;
}


Deploy::Deploy(){

}

Deploy::Deploy(const Deploy& other) : Order(other) {}

Deploy& Deploy::operator=(const Deploy& other) {
    Order::operator=(other);
    return *this;
}

void Deploy::validate() {
    cout << "validating deploy order\n";
}

void Deploy::execute() {
    validate();
    cout << "executing deploy order\n";
}

void Deploy::print(ostream& os) const {
    os << "deploy: new soldiers are deployed on selected territory\n";
}

Advance::Advance() : Order() {}

void Advance::validate() {
    cout << "validating advance order\n";
}

void Advance::execute() {
    validate();
    cout << "executing advance order\n";
}

Advance& Advance::operator=(const Advance& other) {
    Order::operator=(other);
    return *this;
}

void Advance::print(ostream& os) const {
    os << "advance: selected soldiers move into selected territory\n";
}

Bomb::Bomb() : Order() {}

void Bomb::validate() {
    cout << "validating bomb order\n";
}

void Bomb::execute() {
    validate();
    cout << "executing bomb order\n";
}

Bomb& Bomb::operator=(const Bomb& other) {
    Order::operator=(other);
    return *this;
}

void Bomb::print(ostream& os) const {
    os << "bomb: drop bombs on selected territory\n";
    
}

Blockade::Blockade() : Order() {}

void Blockade::validate() {
    cout << "validating blockade order\n";
}

void Blockade::execute() {
    validate();
    cout << "executing blockade order\n";
}

Blockade& Blockade::operator=(const Blockade& other) {
    Order::operator=(other);
    return *this;
}

void Blockade::print(ostream& os) const {
    os << "create a blockade on selected territory\n";
    
}

Airlift::Airlift() : Order() {}

void Airlift::validate() {
    cout << "validating airlift order\n";
}

void Airlift::execute() {
    validate();
    cout << "executing airlift order\n";
}

Airlift& Airlift::operator=(const Airlift& other) {
    Order::operator=(other);
    return *this;
}

void Airlift::print(ostream& os) const {
     os << "airlifts selected soldiers to selected territory\n";
    
}

Negociate::Negociate() : Order() {}

void Negociate::validate() {
    cout << "validating negociate order\n";
}

void Negociate::execute() {
    validate();
    cout << "executing negociate order\n";
}

Negociate& Negociate::operator=(const Negociate& other) {
    Order::operator=(other);
    return *this;
}

void Negociate::print(ostream& os) const {
    os << "create a negotiation contract with selected territory\n";
    
}

OrdersList::OrdersList() {}

OrdersList::~OrdersList() {
    // Clean up orders
    for (Order* order : orders) {
        delete order;
        order = NULL;
    }
}

void OrdersList::addOrder(Order* orderName) {
    orders.push_back(orderName); // Add the order to the list
}

void OrdersList::move() {
    int position = 0; // Starting position
    int next;

    // in case orders are empty if orders are empty
    if (orders.empty()) {
        cout << "orders are empty" << endl;
        return;
    }

    // display  first order
    cout << "Current order selected: " << *orders[position] << endl;

    cout << "Press 1 to see next order or 0 to exit: ";
    cin >> next;

    while (next != 0) {
        position++; 

        //check the bounds
        if (position >= orders.size()) {
            position = 0; // Loop back to the beginning is reached the end
        }

        cout << "order selected: " << *orders[position] << endl;

        cout << "press 1 to see next order or 0 to exit: ";
        cin >> next;
    }
}

// remove given Order from the list of Orders
void OrdersList::remove(Order* order) {
    if (order == nullptr) {
        cout << "error annot remove a null order." << endl;
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
    cout << "order not found" << endl; 
}


OrdersList& OrdersList::operator=(const OrdersList& other) {
    if (this != &other) {
  
        for (Order* order : orders) {
            delete order; // clear memory
        }
        orders.clear(); 

        //deep copy 
        for (const Order* order : other.orders) {
            if (dynamic_cast<const Deploy*>(order)){
                orders.push_back(new Deploy(*dynamic_cast<const Deploy*>(order)));
            } else if (dynamic_cast<const Advance*>(order)){
                orders.push_back(new Advance(*dynamic_cast<const Advance*>(order)));
            } else if (dynamic_cast<const Bomb*>(order)){
                orders.push_back(new Bomb(*dynamic_cast<const Bomb*>(order)));
            } else if (dynamic_cast<const Blockade*>(order)){
                orders.push_back(new Blockade(*dynamic_cast<const Blockade*>(order)));
            } else if (dynamic_cast<const Airlift*>(order)){
                orders.push_back(new Airlift(*dynamic_cast<const Airlift*>(order)));
            } else if (dynamic_cast<const Negociate*>(order)) {
                orders.push_back(new Negociate(*dynamic_cast<const Negociate*>(order)));

            }
        }
    }
    return *this;
}
