#include "LoggingObserver.h"
#include "../OrdersList/Orders.h"
#include "../GameEngine/GameEngine.h"
#include "../Player/Player.h"
#include "../Map/Map.h"


#include <iostream>

void testLoggingObserver() {
    //Testing Order Logging
    OrdersList ordersList;

// Create sample Territory and Player objects
 std::string territoryName = "Sample Territory";
    std::string playerName = "Sample Player";
    int armyAmount = 10;

    Territory* sampleTerritory = new Territory();
    Player* samplePlayer = new Player(&playerName, &armyAmount);  // Use pointer constructor

    // Instantiate orders with sample objects
    Order* deployOrder = new Deploy(sampleTerritory, 10, samplePlayer);
    Order* advanceOrder = new Advance(samplePlayer, sampleTerritory, sampleTerritory, 5);
    Order* bombOrder = new Bomb(sampleTerritory, samplePlayer);
    Order* blockadeOrder = new Blockade(samplePlayer, sampleTerritory);
    Order* airliftOrder = new Airlift(sampleTerritory, sampleTerritory, 10, samplePlayer);
    Order* negociateOrder = new Negociate(samplePlayer, samplePlayer);

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


    // Testing Game Engine Logging
    std::cout << "\nTesting GameEngine state transitions...\n";

    GameEngine gameEngine;
    gameEngine.addObserver(&logObserver);

    gameEngine.navigate();

    std::cout << "\nUpdated Logging Results:\n";
    logObserver.printLog();
}
