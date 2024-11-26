#include <iostream>
#include <string>
#include <vector>
#include "Player/Player.h"
#include "CardsDeckHand/Cards.h"
#include "Map/Map.h"
#include "OrdersList/Orders.h"
#include "GameEngine/GameEngine.h"
#include "CommandProcessing/CommandProcessing.h"
#include "OrdersList/testOrders.h"
#include "PlayerStrategies/PlayerStrategies.h"

void testCommandProcessor();
void testStartupPhase();
void testOrderExecution();
void testLoggingObserver();
void testMainGameLoop();
void testPlayerStrategies();

// int main(int argc, char* argv[]) {
//     testOrderExecution();
//     testStartupPhase();
//     testMainGameLoop();
//     testLoggingObserver();
//     testCommandProcessor();

//     return 0;
  
// }

bool checkForEscape() {
    std::string userInput;
    std::cout << "Type 'escape' to skip to the next test or anything else to continue: ";
    std::getline(std::cin, userInput);
    return (userInput == "escape");
}

int main(int argc, char* argv[]) {
    
    bool runTests = false;
    if (argc > 1 && std::string(argv[1]) == "--test") {
        runTests = true;
    }

    if (runTests) {
         std::cout << "==========================" << std::endl;
        std::cout << "     Strategies test   " << std::endl;
        std::cout << "==========================" << std::endl;
        
        if (checkForEscape()) {
            std::cout << "Skipping strategies Test..." << std::endl;
        } else {
            testPlayerStrategies();
        }
        std::cout << std::endl << std::endl;
        std::cout << "==========================" << std::endl;
        std::cout << "     COMMAND PROCESSOR TEST   " << std::endl;
        std::cout << "==========================" << std::endl;
        
        if (checkForEscape()) {
            std::cout << "Skipping Command Processor Test..." << std::endl;
        } else {
            testCommandProcessor();
        }
        std::cout << std::endl << std::endl;

        std::cout << "==========================" << std::endl;
        std::cout << "      STARTUP PHASE TEST           " << std::endl;
        std::cout << "==========================" << std::endl;
        if (checkForEscape()) {
            std::cout << "Skipping Startup Phase Test..." << std::endl;
        } else {
            testStartupPhase();
        }
        std::cout << std::endl << std::endl;

        std::cout << "==========================" << std::endl;
        std::cout << "     ORDERS EXECUTION TEST      " << std::endl;
        std::cout << "==========================" << std::endl;
        if (checkForEscape()) {
            std::cout << "Skipping Orders Execution Test..." << std::endl;
        } else {
            testOrderExecution();
        }
        std::cout << std::endl << std::endl;

        std::cout << "==========================" << std::endl;
        std::cout << "  LOGGING OBSERVER TEST    " << std::endl;
        std::cout << "==========================" << std::endl;
        if (checkForEscape()) {
            std::cout << "Skipping Logging Observer Test..." << std::endl;
        } else {
            testLoggingObserver();
        }
        std::cout << std::endl << std::endl;

        std::cout << "==========================" << std::endl;
        std::cout << "     MAIN GAME LOOP TEST       " << std::endl;
        std::cout << "==========================" << std::endl;
        if (checkForEscape()) {
            std::cout << "Skipping Main Game Loop Test..." << std::endl;
        } else {
            testMainGameLoop();
        }
        std::cout << std::endl << std::endl;


    } else {
        std::cout << "Starting the game..." << std::endl;
        GameEngine gameEngine;
        gameEngine.startupPhase();
        gameEngine.mainGameLoop();
    }

    return 0;
}
