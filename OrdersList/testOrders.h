#ifndef _testOrders_
#define _testOrders_

#include <ostream>
#include <unordered_map>
#include <vector>

using namespace std;

class Map;
class Player;
class Territory;


class testOrders
{
private:
    string* map = new string[8]{"start","map loaded","map validated",
                     "players added", "assign reinforcement",
                     "issue orders", "execute orders", "win"};
    
    string* commands = new string[11]{"loadmap","validatemap","addplayer",
                        "assigncountries","issueorder","endissueorders"
                        ,"execorder","endexecorders","win","play","end"};
    Map* Cmap;

    vector<Player*> *playerList;
   
    
public:
    testOrders();

    ~testOrders();

    testOrders(testOrders&);

    void navigate();

    void testTheOrders();
    
    void DistributeTerritories(unordered_map<std::string, Territory*> m,vector<Player*> p)  ;

     void shufflePlayers();

     void assignArmyAmount(int amount);
     
     void DrawTwoCards();


    // Assignment Operator
    testOrders& operator=(const testOrders& other);

    // Stream Insertion
    friend ostream& operator<<(ostream& os, const testOrders& testOrders);

    void addplayer();

    void mainGameLoop();


    void reinforcementPhase();

    void issueOrdersPhase();

    void executeOrdersPhase();





};

#endif
