#include "Cards.h"
#include <cstdlib>  // For rand()
#include <ctime>    // For time()

// Card class implementation

// Constructor to initialize a card with a specific type
Card::Card(CardType t) : type(t) {}

// Play method: Outputs the card type when the card is played
void Card::play() {
    std::cout << "Playing card: " << getCardTypeName() << std::endl;
}

// Get the type of the card
Card::CardType Card::getType() const {
    return type;
}

// Return the name of the card type as a string
std::string Card::getCardTypeName() const {
    switch (type) {
        case Bomb: return "Bomb";
        case Reinforcement: return "Reinforcement";
        case Blockade: return "Blockade";
        case Airlift: return "Airlift";
        case Diplomacy: return "Diplomacy";
        default: return "Unknown";
    }
}

// Deck class implementation

// Constructor: Initialize the deck with cards
Deck::Deck() {
    initializeDeck();  // Fill the deck with cards
    srand(time(0));  // Seed for random card drawing
}

// Initialize the deck with several cards of each type
void Deck::initializeDeck() {
    for (int i = 0; i < 5; ++i) {  // Add 5 cards of each type
        cards.push_back(new Card(Card::Bomb));
        cards.push_back(new Card(Card::Reinforcement));
        cards.push_back(new Card(Card::Blockade));
        cards.push_back(new Card(Card::Airlift));
        cards.push_back(new Card(Card::Diplomacy));
    }
}

// Draw a card from the deck at random
Card* Deck::draw() {
    if (cards.empty()) {
        std::cout << "The deck is empty!" << std::endl;
        return nullptr;
    }
    int randomIndex = rand() % cards.size();  // Pick a random card
    Card* drawnCard = cards[randomIndex];     // Get the card
    cards.erase(cards.begin() + randomIndex); // Remove the card from the deck
    return drawnCard;  // Return the drawn card
}

// Return a played card to the deck
void Deck::returnCard(Card* card) {
    cards.push_back(card);  // Put the card back into the deck
}

// Hand class implementation

// Constructor: Initialize an empty hand
Hand::Hand() {}

// Add a card to the hand
void Hand::addCard(Card* card) {
    handCards.push_back(card);
}

// Play all cards in the hand and return them to the deck
void Hand::playAll(Deck* deck) {
    for (Card* card : handCards) {  // Play each card in the hand
        card->play();
        deck->returnCard(card);  // Return the card to the deck after playing
    }
    handCards.clear();  // Clear the hand after playing all cards
}
