#ifndef HAND_H
#define HAND_H

#include <vector>
#include "Cards.h"

// Hand class
class Hand {
public:
    Hand();  // Constructor
    ~Hand();  // Destructor

    void addCard(Card* card);  // Add a card to the hand
    void playAll();  // Play all cards in the hand

private:
    std::vector<Card*> handCards;  // Cards in the hand
};

#endif
