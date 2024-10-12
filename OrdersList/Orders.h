#ifndef ORDERS_H_
#define ORDERS_H_

#include <iostream>
#include <vector>
#include <string>

using namespace std;

// Base Order Class
class Order {
public:
    string* orderName;  
    Order();//default constructor
    virtual ~Order(); //virtual destructor 
    Order(const Order& other);// Copy constructor 
    Order& operator=(const Order& other); // Assignment operator

   
    virtual void validate() = 0;
    virtual void execute() = 0;

    
    friend ostream& operator<<(ostream& os, const Order& order);

protected:
    virtual void print(ostream& os) const = 0; 
};

//subclasses:
class Deploy : public Order {//
public:
    Deploy();
    Deploy(const Deploy& other);
    Deploy& operator=(const Deploy& other);
    void validate() override;
    void execute() override;
    void print(ostream& os) const override;
};


class Advance : public Order {
public:
    Advance();
    Advance& operator=(const Advance& other);
    void validate() override;
    void execute() override;
    void print(ostream& os) const override;
};


class Bomb : public Order {
public:
    Bomb();
    Bomb& operator=(const Bomb& other);
    void validate() override;
    void execute() override;
    void print(ostream& os) const override;
};


class Blockade : public Order {
public:
    Blockade();
    Blockade& operator=(const Blockade& other);
    void validate() override;
    void execute() override;
    void print(ostream& os) const override;
};


class Airlift : public Order {
public:
    Airlift();
    Airlift& operator=(const Airlift& other);
    void validate() override;
    void execute() override;
    void print(ostream& os) const override;
};

class Negociate : public Order {
public:
    Negociate();
    Negociate& operator=(const Negociate& other);
    void validate() override;
    void execute() override;
    void print(ostream& os) const override;
};


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
};

#endif
