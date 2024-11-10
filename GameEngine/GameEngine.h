#ifndef _GAMEENGINE_
#define _GAMEENGINE_

#include <ostream>
#include <unordered_map>
#include <vector>

#include "../Log/LoggingObserver.h"

using namespace std;

class Map;
class Player;
class Territory;


class GameEngine : public Subject
{
private:
    int* currentIndex;
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
    std::string stringToLog() const override;

    void startupPhase();
    void testMainGame();
    
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
