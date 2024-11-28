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
struct TournamentParams;


class GameEngine : public Subject
{
private:
    int* currentIndex = new int(0);

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
    void addObserver(Observer* observer);
    void notify();
    std::string getState() const;
    std::string stringToLog() const override;
    std::vector<Observer*> observers;

    void startupPhase();
    void testMainGame();
    void addPlayersToGameEngine(const std::vector<std::string>& strategies);
    void executeTournament(const TournamentParams& params);
    
    void DistributeTerritories(unordered_map<std::string, Territory*> m,vector<Player*> p)  ;
   Player* checkWinner(const std::unordered_map<std::string, Territory*>& allTerritories) const ;

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
