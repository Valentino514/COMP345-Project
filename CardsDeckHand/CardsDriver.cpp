#include "Cards.h"
#include <iostream>

void testCards() {
    // Create a deck of cards
    Deck deck;

    // Create two hands for two players
    Hand player1Hand;
    Hand player2Hand;

    // Player 1 draws 2 cards
    std::cout << "Player 1 draws 2 cards..." << std::endl;
    for (int i = 0; i < 2; ++i) {
        Card* card = deck.draw();
        if (card != nullptr) {
            player1Hand.addCard(card);
        }
    }

    // Player 2 draws 2 cards
    std::cout << "Player 2 draws 2 cards..." << std::endl;
    for (int i = 0; i < 2; ++i) {
        Card* card = deck.draw();
        if (card != nullptr) {
            player2Hand.addCard(card);
        }
    }

    // Player 1 plays all cards
    std::cout << "Player 1 plays their cards..." << std::endl;
    player1Hand.playAll(&deck);

    // Player 2 plays all cards
    std::cout << "Player 2 plays their cards..." << std::endl;
    player2Hand.playAll(&deck);
}

