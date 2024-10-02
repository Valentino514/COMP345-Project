#ifndef _GAMEENGINE_
#define _GAMEENGINE_

#include <ostream>

using namespace std;

class GameEngine
{
private:
    string map[8] = {"start","map loaded","map validated",
                     "players added", "assign reinforcement",
                     "issue orders", "execute orders", "win"};
    
    string commands[9] ={"loadmap","validatemap","addplayer",
                        "assigncountries","issueorder","endissueorders"
                        ,"execorder","win","play"};
   int currentIndex = 0;
    
public:
    GameEngine();

    ~GameEngine();

    GameEngine(const GameEngine& other);

    void GenerateMap();

    void navigate();

    // Assignment Operator
    GameEngine& operator=(const GameEngine& other);

    // Stream Insertion
    friend ostream& operator<<(ostream& os, const GameEngine& gameEngine);
};

#endif
