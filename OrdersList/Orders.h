#ifndef ORDERS_H_
#define ORDERS_H_

#include <iostream>
#include <vector>
#include <string>

using namespace std;

// Forward declarations to avoid circular dependencies
class Territory;
class Player;

// Base Order Class
class Order {
public:
    string* orderName;  
    Order(); // Default constructor
    virtual ~Order(); // Virtual destructor 
    Order(const Order& other); // Copy constructor 
    Order& operator=(const Order& other); // Assignment operator

    virtual bool validate() = 0;
    virtual void execute() = 0;

    friend ostream& operator<<(ostream& os, const Order& order);

protected:
    virtual void print(ostream& os) const = 0; 
};

// Deploy Order
class Deploy : public Order {
private:
    Territory* targetTerritory;
    int* reinforcementAmount;
    Player* player;

public:
    Deploy();  // Default constructor
    Deploy(Territory* targetTerritory, int reinforcementAmount,Player* player);  // Parameterized constructor
    Deploy(const Deploy& other);
    Deploy& operator=(const Deploy& other);
    ~Deploy();

    bool validate() override;
    void execute() override;
    void print(ostream& os) const override;
};

// Advance Order
class Advance : public Order {
private:
    Player* player;
    Territory* sourceTerritory;
    Territory* destinationTerritory;
    int reinforcementAmount;
    bool isDestinationOwned;

public:
    Advance();  // Default constructor
    Advance(Territory* source, Territory* destination, int reinforcementAmount, Player* player);  // Parameterized constructor
    Advance(const Advance& other);
    Advance& operator=(const Advance& other);
    ~Advance();

    bool validate() override;
    void execute() override;
    void print(ostream& os) const override;
};

// Bomb Order
class Bomb : public Order {
private:
    Territory* targetTerritory;
    Player* player;

public:
    Bomb();  // Default constructor
    Bomb(Territory* target);  // Parameterized constructor
    Bomb(const Bomb& other);
    Bomb& operator=(const Bomb& other);
    ~Bomb();

    bool validate() override;
    void execute() override;
    void print(ostream& os) const override;
};

// Blockade Order
class Blockade : public Order {
private:
    Territory* targetTerritory;

public:
    Blockade();  // Default constructor
    Blockade(Territory* target);  // Parameterized constructor
    Blockade(const Blockade& other);
    Blockade& operator=(const Blockade& other);
    ~Blockade();

    bool validate() override;
    void execute() override;
    void print(ostream& os) const override;
};

class Airlift : public Order {
private:
    Territory* sourceTerritory;
    Territory* destinationTerritory;
    int* reinforcementAmount;
    Player* player;

public:
    Airlift();  // Default constructor
    Airlift(Territory* source, Territory* destination, int reinforcementAmount, Player* player);  // Parameterized constructor
    Airlift(const Airlift& other);  // Copy constructor
    Airlift& operator=(const Airlift& other);
    ~Airlift();

    bool validate() override;
    void execute() override;
    void print(ostream& os) const override;
};

// Negotiate Order
class Negociate : public Order {
private:
    Player* player1;
    Player* player2;

public:
    Negociate();  // Default constructor
    Negociate(Player* p1, Player* p2);  // Parameterized constructor
    Negociate(const Negociate& other);  // Copy constructor
    Negociate& operator=(const Negociate& other);
    ~Negociate();

    bool validate() override;
    void execute() override;
    void print(ostream& os) const override;
};

// OrdersList Class
class OrdersList {
private:
    vector<Order*> orders;

public:
    OrdersList();  
    ~OrdersList(); 
    OrdersList& operator=(const OrdersList& other);  
    void move();  
    void remove(Order* order);  
    void addOrder(Order* orderName); 
    bool hasDeployOrder() const;
    Order* getNextDeployOrder();
    Order* getNextOrder();
    bool isEmpty() const;
};

#endif
