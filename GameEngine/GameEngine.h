#ifndef _GAMEENGINE_
#define _GAMEENGINE_

#include <ostream>

using namespace std;

class GameEngine
{
private:
    string* map = new string[8]{"start","map loaded","map validated",
                     "players added", "assign reinforcement",
                     "issue orders", "execute orders", "win"};
    
    string* commands = new string[11]{"loadmap","validatemap","addplayer",
                        "assigncountries","issueorder","endissueorders"
                        ,"execorder","endexecorders","win","play","end"};
   
    
public:
    GameEngine();

    ~GameEngine();

    GameEngine(GameEngine&);

    void navigate();

    // Assignment Operator
    GameEngine& operator=(const GameEngine& other);

    // Stream Insertion
    friend ostream& operator<<(ostream& os, const GameEngine& gameEngine);
};

#endif
