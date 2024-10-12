#include <iostream>
#include <string>
#include <vector>
#include "Player/Player.h"
#include "CardsDeckHand/Cards.h"
#include "Map/Map.h"
#include "OrdersList/Orders.h"
#include "GameEngine/GameEngine.h"

// Forward declarations for the test functions
void testLoadMaps();
void testPlayers();
void testOrdersList();
void testCards();
void testGameEngine();

int main() {
    // Test map loading and validation
    std::cout << "==========================" << std::endl;
    std::cout << "     MAP VALIDATION TEST   " << std::endl;
    std::cout << "==========================" << std::endl;
    testLoadMaps();
    std::cout << std::endl << std::endl;

    // Test cards system
    std::cout << "==========================" << std::endl;
    std::cout << "      CARDS TEST           " << std::endl;
    std::cout << "==========================" << std::endl;
    testCards();
    std::cout << std::endl << std::endl;

    // Test player functionality
    std::cout << "==========================" << std::endl;
    std::cout << "     PLAYERS TEST          " << std::endl;
    std::cout << "==========================" << std::endl;
    testPlayers();
    std::cout << std::endl << std::endl;

    // Test orders list functionality
    std::cout << "==========================" << std::endl;
    std::cout << "     ORDERS LIST TEST      " << std::endl;
    std::cout << "==========================" << std::endl;
    testOrdersList();
    std::cout << std::endl << std::endl;

    // Test game engine functionality
    std::cout << "==========================" << std::endl;
    std::cout << "    GAME ENGINE TEST       " << std::endl;
    std::cout << "==========================" << std::endl;
    testGameEngine();
    std::cout << std::endl << std::endl;

    return 0;
}
