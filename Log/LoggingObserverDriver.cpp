#include "LoggingObserver.h"
#include "../OrdersList/Orders.h"
#include <iostream>

void testLoggingObserver() {
    OrdersList ordersList;

    Order* deployOrder = new Deploy();
    Order* advanceOrder = new Advance();
    Order* bombOrder = new Bomb();
    Order* blockadeOrder = new Blockade();
    Order* airliftOrder = new Airlift();
    Order* negociateOrder = new Negociate();

    deployOrder->orderName = new std::string("Deploy Soldiers");
    advanceOrder->orderName = new std::string("Advance Troops");
    bombOrder->orderName = new std::string("Bomb Enemy Base");
    blockadeOrder->orderName = new std::string("Blockade Supply Route");
    airliftOrder->orderName = new std::string("Airlift Supplies");
    negociateOrder->orderName = new std::string("Negotiate Treaty");

    ordersList.addOrder(deployOrder);
    ordersList.addOrder(advanceOrder);
    ordersList.addOrder(bombOrder);
    ordersList.addOrder(blockadeOrder);
    ordersList.addOrder(airliftOrder);
    ordersList.addOrder(negociateOrder);

    LogObserver logObserver;
    ordersList.addObserver(&logObserver);

    std::cout << "Executing orders...\n";
    ordersList.move();

    std::cout << "\nLogging Results:\n";
    logObserver.printLog(); 
}
