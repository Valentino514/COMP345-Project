#include <iostream>
#include "../Player/Player.h"
#include "Orders.h"
#include <string>
#include "testOrders.h"
#include "OrdersDriver.h"

using namespace std;

//use g++ Orders.cpp OrdersDriver.cpp to test it

void testOrderExecution(){

    testOrders test;

    // Simulate user input and actions for game startup
    std::cout << "\n--- Game Startup Phase ---\n";

    // Step 1: Load Map
    
    test.testTheOrders();  // Load map via the GameEngine's startup phase method


}