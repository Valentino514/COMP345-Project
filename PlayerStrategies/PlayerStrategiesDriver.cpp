#include "../Player/Player.h"
#include "../PlayerStrategies/PlayerStrategies.h"
#include <iostream>
#include <vector>
#include "../Map/Map.h"

void testPlayerStrategies() {
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
    std::cout << "\n--- Testing NeutralPlayerStrategy ---" << std::endl;
    std::cout << "Neutral player issuing orders (should do nothing):" << std::endl;
    neutralPlayer->issueOrder({enemyPlayer});

    // Test CheaterPlayerStrategy
    std::cout << "\n--- Testing CheaterPlayerStrategy ---" << std::endl;
    std::cout << "Cheater player issuing orders (should conquer all adjacent territories):" << std::endl;
    cheaterPlayer->issueOrder({neutralPlayer, enemyPlayer, humanPlayer});

    // Test HumanPlayerStrategy
    std::cout << "\n--- Testing HumanPlayerStrategy ---" << std::endl;
    std::cout << "Human player issuing orders (requires user input):" << std::endl;
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