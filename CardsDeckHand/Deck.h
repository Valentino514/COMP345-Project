#ifndef DECK_H
#define DECK_H

#include <vector>
#include "Cards.h"

// Deck class
class Deck {
public:
    Deck();  // Constructor
    ~Deck();  // Destructor

    Card* draw();  // Draw a card from the deck

private:
    std::vector<Card*> cards;  // Cards in the deck
    void initializeDeck();  // Initialize the deck with cards
};

#endif
