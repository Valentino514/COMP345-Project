#include <iostream>
#include "Player.h"
#include "../OrdersList/Orders.h"
#include <string>

//use "g++ Player.cpp PlayerDriver.cpp ../OrdersList/Orders.cpp" to test it
using namespace std;

void testPlayers(){
        
    Player* player1 = new Player(new string("player1"));

    Order* order = new Bomb();


    cout<<"issuing order:"<<endl;
    player1->issueOrder(order);

    delete player1;

}


int main(){

testPlayers();

return 0;

}