#include <iostream>
#include "GameEngine.h"

using namespace std;

void testGameEngine(){

GameEngine* engine = new GameEngine();

engine->navigate();

delete engine;

}
