#include "Cards.h"
#include <iostream>

void testCards() {
    // Create a deck of cards
    Deck deck;

    // Output the initial deck
    std::cout << "Initial deck: " << deck << std::endl;

    // Create two hands for two players
    Hand player1Hand;
    Hand player2Hand;

    // Player 1 draws 2 cards
    std::cout << "\nPlayer 1 draws 2 cards..." << std::endl;
    for (int i = 0; i < 2; ++i) {
        Card* card = deck.draw();
        if (card != nullptr) {
            player1Hand.addCard(card);
        }
    }

    // Output the deck and player 1's hand
    std::cout << "Deck after Player 1 draws: " << deck << std::endl;
    std::cout << "Player 1's hand: " << player1Hand << std::endl;

    // Player 2 draws 2 cards
    std::cout << "\nPlayer 2 draws 2 cards..." << std::endl;
    for (int i = 0; i < 2; ++i) {
        Card* card = deck.draw();
        if (card != nullptr) {
            player2Hand.addCard(card);
        }
    }

    // Output the deck and player 2's hand
    std::cout << "Deck after Player 2 draws: " << deck << std::endl;
    std::cout << "Player 2's hand: " << player2Hand << std::endl;

    // Player 1 plays all cards
    std::cout << "\nPlayer 1 plays their cards..." << std::endl;
    player1Hand.playAll(&deck);

    // Output the deck after Player 1 plays
    std::cout << "Deck after Player 1 plays: " << deck << std::endl;

    // Player 2 plays all cards
    std::cout << "\nPlayer 2 plays their cards..." << std::endl;
    player2Hand.playAll(&deck);

    // Output the deck after Player 2 plays
    std::cout << "Deck after Player 2 plays: " << deck << std::endl;
}

int main() {
    testCards();  // Run the test function
    return 0;
}
