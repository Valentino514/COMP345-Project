#include <iostream>
#include "GameEngine.h"

using namespace std;

void testStartupPhase() {
    char userInput;
    do {
        GameEngine gameEngine;

        // Simulate user input and actions for game startup
        std::cout << "\n--- Game Startup Phase ---\n";

        // Step 1: Load Map
        gameEngine.startupPhase();  // Load map via the GameEngine's startup phase method

        // Ask if the user wants to start another game
        std::cout << "\n--- Game Over ---\n";
        std::cout << "Do you want to start another game? (Y/y to continue, N/n to exit): ";
        std::cin >> userInput;
        std::cin.ignore(std::numeric_limits<std::streamsize>::max(), '\n');  // Clear any remaining input

    } while (userInput == 'Y' || userInput == 'y');
}

void testMainGameLoop() {
    
    GameEngine gameEngine;

    // Simulate user input and actions for game startup
    std::cout << "\n--- Game Startup Phase ---\n";

    // Step 1: Load Map
    
    gameEngine.startupPhase();  // Load map via the GameEngine's startup phase method
    
}
