#include <iostream>
#include <string>
#include <vector>
#include "Player/Player.h"
#include "CardsDeckHand/Cards.h"
#include "Map/Map.h"
#include "OrdersList/Orders.h"
#include "GameEngine/GameEngine.h"

void testLoadMaps();
void testPlayers();
void testOrdersList();
void testCards();
void testGameEngine();

int main(){

    testLoadMaps();

    std::cout<<endl;
    std::cout<<endl;

    testCards();
    std::cout<<endl;
    std::cout<<endl;

    testPlayers();
    std::cout<<endl;
    std::cout<<endl;

    testOrdersList();
    std::cout<<endl;
    std::cout<<endl;

    testGameEngine();
    std::cout<<endl;
    std::cout<<endl;





    return 0;
}