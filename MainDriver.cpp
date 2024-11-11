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

void testCommandProcessor();
void testStartupPhase();
void testOrderExecution();
void testLoggingObserver();
void testMainGameLoop();

bool checkForEscape() {
    std::string userInput;
    std::cout << "Type 'escape' to skip to the next test or anything else to continue: ";
    std::getline(std::cin, userInput);
    return (userInput == "escape");
}

int main(int argc, char* argv[]) {
    testOrderExecution();
    testStartupPhase();
    testMainGameLoop();
    testCommandProcessor();
    testLoggingObserver();
    return 0;
  
}
