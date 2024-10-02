#include <iostream>
#include "Orders.h"

using namespace std;

//use g++ Orders.cpp OrderDriver.cpp to test it
void testOrdersList();

int main() {

    testOrdersList();
    return 0;
}

void testOrdersList(){
    // Creating order objects
    Order* deploy = new Deploy();
    Order* advance = new Advance();
    Order* bomb = new Bomb();
    Order* blockade = new Blockade();
    Order* airlift = new Airlift();
    Order* negociate = new Negociate();

    // Creating orders list
    OrdersList orders;

    // Adding orders to orders list
    orders.addOrder(deploy);
    orders.addOrder(advance);
    orders.addOrder(bomb);
    orders.addOrder(blockade);
    orders.addOrder(airlift);
    orders.addOrder(negociate);

    // Validating and executing orders
    deploy->execute();
    advance->execute();
    bomb->execute();
    blockade->execute();
    airlift->execute();
    negociate->execute();

    cout<<*airlift;

    // Moving through the orders
    orders.move();

    orders.remove(bomb);
}
