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
#include "GameEngine/GameEngine.h"

void testPlayerStrategies();
void testTournament();



bool checkForEscape() {
    std::string userInput;
    std::cout << "Type 'escape' to skip to the next test or anything else to continue: ";
    std::getline(std::cin, userInput);
    return (userInput == "escape");
}

int main(int argc, char* argv[]) {

    std::ofstream logFile("gamelog.txt", std::ios::trunc);
                
        if (logFile) {
            logFile << "New game session started.\n";  
        }
        logFile.close();

    
    bool runTests = false;
    if (argc > 1 && std::string(argv[1]) == "--test") {
        runTests = true;
    }
        // Testing strategies
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

        // Testing Tournaments
        std::cout << "==========================" << std::endl;
        std::cout << "     Tournament test   " << std::endl;
        std::cout << "==========================" << std::endl;

        if (checkForEscape()) {
            std::cout << "Skipping Tournament Test..." << std::endl;
        } else {
            testTournament();
        }
        std::cout << std::endl << std::endl;


    } else {

        // Testing the full game

        std::cout << "Starting the game..." << std::endl;
        GameEngine gameEngine;
        LogObserver logObserver;
        gameEngine.addObserver(&logObserver);
         gameEngine.startupPhase();
         gameEngine.mainGameLoop();
        
    }

    return 0;
}
