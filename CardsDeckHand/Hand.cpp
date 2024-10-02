#include "Hand.h"
#include <iostream>

// Constructor
Hand::Hand() {}

// Destructor
Hand::~Hand() {
    for (Card* card : handCards) {
        delete card;
    }
}

// Add a card to the hand
void Hand::addCard(Card* card) {
    handCards.push_back(card);
}

// Play all cards in the hand
void Hand::playAll() {
    for (Card* card : handCards) {
        card->play();
        delete card;  // Remove the card after playing
    }
    handCards.clear();  // Clear the hand after playing all cards
}
