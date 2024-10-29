#ifndef _GAMEENGINE_
#define _GAMEENGINE_

#include <ostream>


using namespace std;

class Map;
class Player;
class Territory;


class GameEngine
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
    GameEngine();

    ~GameEngine();

    GameEngine(GameEngine&);

    void navigate();

    void startupPhase();
    
    void DistributeTerritories(unordered_map<std::string, Territory*> m,vector<Player*> p)  ;

     void shufflePlayers();

     void assignArmyAmount(int amount);
     
     void DrawTwoCards();


    // Assignment Operator
    GameEngine& operator=(const GameEngine& other);

    // Stream Insertion
    friend ostream& operator<<(ostream& os, const GameEngine& gameEngine);

    void addplayer();

    void mainGameLoop();


    void reinforcementPhase();

    void issueOrdersPhase();

    void executeOrdersPhase();





};

#endif
