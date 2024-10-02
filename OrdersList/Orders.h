#ifndef ORDERS_H_
#define ORDERS_H_

#include <iostream>
#include <vector>

using namespace std;

class Order{
    
public:
    string* orderName;
    Order();
    ~Order();
    Order(const Order&);

    //functions to be overriden in order's subclasses
    virtual void validate() = 0;
    virtual void execute() = 0;

    friend ostream& operator<<(ostream& os, const Order& order);
        virtual void print(ostream& os) const = 0; //used to override the operator<< for sub classes
};

//subclasses of Order class (no destructor, and os since Order already takes care of it)

class Deploy:public Order{
    public:
    Deploy();
    Deploy(const Deploy&); 
    Deploy& operator=(const Deploy&); 
    void validate();
    void execute();
    void print(ostream& os) const override;

};

class Advance:public Order{
    public:
    Advance();
    Advance& operator=(const Advance&); 
    void validate();
    void execute();
    void print(ostream& os) const override;

};

class Bomb:public Order{
    public:
    Bomb();
    Bomb& operator=(const Bomb&); 
    void execute();
    void validate();
    void print(ostream& os) const override;

};

class Blockade:public Order{
    public:
    Blockade();
    Blockade& operator=(const Blockade&); 
    void validate();
    void execute();
    void print(ostream& os) const override;


};

class Airlift:public Order{
    public:
    Airlift();
    Airlift& operator=(const Airlift&); 
    void validate();
    void execute();
    void print(ostream& os) const override;
};

class Negociate:public Order{
    public:
    Negociate();
    Negociate& operator=(const Negociate&); 
    void validate();
    void execute();
    void print(ostream& os) const override;
};

class OrdersList {
private:
    vector<Order*> orders;

public:
    OrdersList();  
    ~OrdersList(); 
    OrdersList& operator=(const OrdersList&); 
    void move();  
    void remove(Order*);  
    void addOrder(Order*);  
};


#endif