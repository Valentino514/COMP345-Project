#include "../Player/Player.h"
#include "../PlayerStrategies/PlayerStrategies.h"
#include <iostream>
#include <vector>
#include "../Map/Map.h"

using namespace std;

void testPlayerStrategies() {

    void testAggresiveBenevolent();

    testAggresiveBenevolent();

    // Create territories
    Territory* t1 = new Territory();
    Territory* t2 = new Territory();
    Territory* t3 = new Territory();
    Territory* t4 = new Territory();

    // Add adjacency for territories
    t1->addAdjacentTerritory(t2);
    t2->addAdjacentTerritory(t1);
    t3->addAdjacentTerritory(t1);
    t4->addAdjacentTerritory(t2);

    // Create players
    string neutralName = "Neutral Player";
    int neutralArmy = 10;
    Player* neutralPlayer = new Player(&neutralName, &neutralArmy);

    string enemyName = "Enemy Player";
    int enemyArmy = 15;
    Player* enemyPlayer = new Player(&enemyName, &enemyArmy);

    string cheaterName = "Cheater Player";
    int cheaterArmy = 20;
    Player* cheaterPlayer = new Player(&cheaterName, &cheaterArmy);

    string humanName = "Human Player";
    int humanArmy = 12;
    Player* humanPlayer = new Player(&humanName, &humanArmy);

    // Assign territories
    t1->setLandOccupier(neutralPlayer);
    t1->setArmyAmount(10);
    neutralPlayer->addTerritory(t1);

    t2->setLandOccupier(enemyPlayer);
    t2->setArmyAmount(15);
    enemyPlayer->addTerritory(t2);

    t3->setLandOccupier(cheaterPlayer);
    t3->setArmyAmount(20);
    cheaterPlayer->addTerritory(t3);

    t4->setLandOccupier(humanPlayer);
    t4->setArmyAmount(12);
    humanPlayer->addTerritory(t4);

    // Set strategies
    neutralPlayer->setStrategy(new NeutralPlayerStrategy());
    cheaterPlayer->setStrategy(new CheaterPlayerStrategy());
    humanPlayer->setStrategy(new HumanPlayerStrategy());

    // Test NeutralPlayerStrategy
    cout << "\n--- Testing NeutralPlayerStrategy ---" << endl;
    cout << "Neutral player issuing orders (should do nothing):" << endl;
    neutralPlayer->issueOrder({enemyPlayer});

    // Test CheaterPlayerStrategy
    cout << "\n--- Testing CheaterPlayerStrategy ---" << endl;
    cout << "Cheater player issuing orders (should conquer all adjacent territories):" << endl;
    cheaterPlayer->issueOrder({neutralPlayer, enemyPlayer, humanPlayer});

    // Test HumanPlayerStrategy
    cout << "\n--- Testing HumanPlayerStrategy ---" << endl;
    cout << "Human player issuing orders (requires user input):" << endl;
    humanPlayer->issueOrder({neutralPlayer, enemyPlayer, cheaterPlayer});

    // Cleanup
    delete t1;
    delete t2;
    delete t3;
    delete t4;
    delete neutralPlayer;
    delete enemyPlayer;
    delete cheaterPlayer;
    delete humanPlayer;


}

void testAggresiveBenevolent(){
    // Create territories
    Territory* t1 = new Territory();
    Territory* t2 = new Territory();
    Territory* t3 = new Territory();

    // Add adjacency for territories
    t1->addAdjacentTerritory(t2);
    t2->addAdjacentTerritory(t1);
    t1->addAdjacentTerritory(t3);
    t3->addAdjacentTerritory(t1);
    t2->addAdjacentTerritory(t3);
    t3->addAdjacentTerritory(t2);

    // Create players
    string aggressiveName = "Aggressive Player";
    int aggressiveArmy = 30;
    Player* aggressivePlayer = new Player(&aggressiveName, &aggressiveArmy);

    string benevolentName = "Benevolent Player";
    int benevolentArmy = 25;
    Player* benevolentPlayer = new Player(&benevolentName, &benevolentArmy);

    // Assign territories
    t1->setLandOccupier(aggressivePlayer);
    t1->setArmyAmount(30);
    aggressivePlayer->addTerritory(t1);

    t2->setLandOccupier(benevolentPlayer);
    t2->setArmyAmount(12);
    benevolentPlayer->addTerritory(t2);

    t3->setLandOccupier(benevolentPlayer);
    t3->setArmyAmount(2);
    benevolentPlayer->addTerritory(t3);

    // Set strategies
    aggressivePlayer->setStrategy(new AggresivePlayerStrategy());
    benevolentPlayer->setStrategy(new BenevolentPlayerStrategy());

    // Add some cards to the aggressive and benevolent players
    aggressivePlayer->addCard(new Card(new Card::CardType(Card::Bomb)));
    aggressivePlayer->addCard(new Card(new Card::CardType(Card::Airlift)));
    benevolentPlayer->addCard(new Card(new Card::CardType(Card::Diplomacy)));

    // Create player list
    vector<Player*> playerList = { aggressivePlayer, benevolentPlayer};

    // Test AggressivePlayerStrategy
    cout << "\n--- Testing AggressivePlayerStrategy ---" << endl;
    cout << "Aggressive player issuing orders:" << endl;
    aggressivePlayer->issueOrder(playerList);

    // Test BenevolentPlayerStrategy
    cout << "\n\n--- Testing BenevolentPlayerStrategy ---" << endl;
    cout << "Benevolent player issuing orders:" << endl;
    benevolentPlayer->issueOrder(playerList);

    // Cleanup
    delete t1;
    delete t2;
    delete t3;
    delete aggressivePlayer;
    delete benevolentPlayer;
}