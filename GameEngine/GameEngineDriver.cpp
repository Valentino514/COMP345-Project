#include <iostream>
#include "GameEngine.h"

using namespace std;

void testStartupPhase() {
    GameEngine gameEngine;

    // Simulate user input and actions for game startup
    std::cout << "\n--- Game Startup Phase ---\n";

    // Step 1: Load Map
    std::cout << "Command: loadmap\n";
    gameEngine.startupPhase();  // Load map via the GameEngine's startup phase method
}
