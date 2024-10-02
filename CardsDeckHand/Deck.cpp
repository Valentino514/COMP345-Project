#include "Deck.h"
#include <iostream>
#include <ctime>
#include <cstdlib>

// Constructor
Deck::Deck() {
    initializeDeck();  // Fill the deck with cards
}

// Destructor
Deck::~Deck() {
    for (Card* card : cards) {
        delete card;
    }
}

// Initialize the deck with card types
void Deck::initializeDeck() {
    srand(time(0));  // Seed for randomness
    for (int i = 0; i < 10; ++i) {  // Adding 10 cards of each type for simplicity
        cards.push_back(new Card(Card::Bomb));
        cards.push_back(new Card(Card::Reinforcement));
        cards.push_back(new Card(Card::Blockade));
        cards.push_back(new Card(Card::Airlift));
        cards.push_back(new Card(Card::Diplomacy));
    }
}

// Draw a card from the deck
Card* Deck::draw() {
    if (cards.empty()) {
        std::cout << "Deck is empty!" << std::endl;
        return nullptr;
    }
    int randomIndex = rand() % cards.size();  // Randomly select a card
    Card* card = cards[randomIndex];
    cards.erase(cards.begin() + randomIndex);  // Remove drawn card from deck
    return card;
}
